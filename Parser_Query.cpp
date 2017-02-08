#include "Parser_Query.h"

#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <regex>
#include <assert.h>
#include "Help.hpp"

#include "code_generation.h"

using namespace std;

extern string type(const Schema::Relation::Attribute& attr);
extern Name_Generator name_generator;

namespace keyword {
	const string Select  = "select"  ;
	const string From    = "from"    ;
	const string And    = "and"    ;
	const string Where   = "where"   ;
	const string Group   = "group"   ;
	const string By   = "by"   ;
	const string Grouping   = "grouping"   ;
	const string Sets   = "sets"   ;
	const string Rollup   = "rollup"   ;
	const string Cube   = "cube"   ;
}

namespace literal {
	const char Comma  = ','  ;
	const char Semicolon  = ';'  ;
	const char EqualSign  = '='  ;
	const char Quote  = '\''  ;
	const char Screen  = '\\'  ;
	const char LBracket  = '('  ;
	const char RBracket = ')'  ;
}

template<typename T>
struct NamePredicate {
	const string& name;
	NamePredicate(const string& name) : name(name) {}
	bool operator()(const T& t) const { return t.name == name; }
};

static bool is_identifier(const string& s) {
	if (s == keyword::Select || s == keyword::From || s == keyword::Where) return false;
	return regex_match(s, regex("[A-Za-z_](\\w|_)+"));
}

static bool is_numeric(const string& s) {
	return regex_match(s, regex("[+-]?\\d*(.\\d+)?"));
}

struct FUPredicate {
	string field_name;
	FUPredicate(string field_name) : field_name(field_name) {}
	bool operator()(const Field_Unit& f) const { return f.field_name == field_name; }
};

//static bool is_quoted_text(const string& s) {
//	return s.length() >= 2 && s[0] == '"' && s.back() == '"';
//}


struct Read_Token {
	Read_Token() {
		reserved_literals = ",;='\\()";
	}

	void push_reserved_literals(const string& src) {
		reserved_literals_stack.push_back(reserved_literals);
		reserved_literals = src;
	}
	void pop_reserved_literals() {
		reserved_literals = reserved_literals_stack.back();
		reserved_literals_stack.pop_back();
	}

	string operator()(istringstream& in, char* delim = nullptr) {
		assert(!in.eof());
		string tmp = "";
		bool any = false;
		char ch = 0;
		if (delim != nullptr) *delim = 0;
		while (in.get(ch)) {
			if (reserved_literals.find(ch) !=std::string::npos) {
				if (!any) {
					tmp += ch;
					any = true;
				} else {
					if (delim != nullptr) *delim = ch;
					in.unget();
				}
				break;
			} else if (whitespace.find(ch) !=std::string::npos) {
				if (any) {
					if (delim != nullptr) *delim = ch;
					break;
				}
			} else {
				tmp += ch;
				any = true;
			}
		}
		return tmp;
	}
private:

	string reserved_literals;
	const string whitespace = " \r\n\t";
	vector<string> reserved_literals_stack;

} read_token;


static string read_quoted_text(istringstream& in) {
	enum class State : unsigned { 
		General, Screen, Finished
	} state;
	state = State::General;
	
	string res = "";
	string tmp;
	size_t start = in.tellg();
	
	while (!in.eof() && state != State::Finished) {
		tmp = read_token(in);
		switch (state) {
			case State::General:
				if (tmp == string(1,literal::Screen)) {
					state = State::Screen;
				} else if (tmp == string(1,literal::Quote)) {
					state = State::Finished;
					break;
				}
				break;
			case State::Screen:
				state = State::General;
				break;
			case State::Finished:
				break;
		}
	}
	
	size_t end = (size_t)in.tellg() - 1;
	if (end == string::npos) end = in.str().length() - 1;
	return "\"" + in.str().substr(start, end-start) + "\"";
}

static string read_conditions_list(istringstream& in, vector<pair<string,string>>& conditions) {
	enum class State : unsigned { 
		Init, Left, Equal, Right, And, Finished
	} state;
	state = State::Init;
	
	string tmp;
	while (!in.eof() && state != State::Finished) {
		tmp = read_token(in);
		switch (state) {
			case State::And:
			case State::Init:
				if (is_identifier(tmp) || is_numeric(tmp)) {
					conditions.push_back(make_pair("",""));
					conditions.back().first = tmp;
					state = State::Left;
				} else if (tmp == string(1,literal::Quote)) {
					conditions.push_back(make_pair("",""));
					conditions.back().first = read_quoted_text(in);
					state = State::Left;
				} else {
					throw ParserQueryError("Identifier, Number or Quoted Text expected",in.tellg());
				}
				break;
			case State::Equal:
				if (is_identifier(tmp) || is_numeric(tmp)) {
					conditions.back().second = tmp;
					state = State::Right;
				} else if (tmp == string(1,literal::Quote)) {
					conditions.back().second = read_quoted_text(in);
					state = State::Right;
				} else {
					throw ParserQueryError("Identifier, Number or Quoted Text expected",in.tellg());
				}
				break;
			case State::Left:
				if (tmp == string(1,literal::EqualSign)) {
					state = State::Equal;
				} else {
					throw ParserQueryError("'=' Expected",in.tellg());
				}
				break;
			case State::Right:
				if (tmp == string(1,literal::Semicolon)) {
					state = State::Finished;
				} else if (tmp == keyword::And) {
					state = State::And;
				} else {
					throw ParserQueryError("';' or 'and' expected",in.tellg());
				}
				break;
			case State::Finished:
				break;
		}
	}
	if (state != State::Finished) {
		throw ParserQueryError("Input ended too early",in.tellg());
	}
	if (in.eof()) tmp = "";
	return tmp;
}


static string read_delimited_list(
		istringstream& in
		, const string& stop_str
		, const string& delimiter
		, const string& ignore_start, const string& ignore_end
		, vector<string>& ident_list) {
	enum class State : unsigned { 
		Init, Item, Comma, Finished, Ignore_First, Ignore
	} state;
	state = State::Init;
	
	string tmp;
	char delim = 0;
//	char prev_delim = 0;
	string current_item;
	while (!in.eof() && state != State::Finished) {
		tmp = read_token(in, &delim);
		switch (state) {
			case State::Comma:
			case State::Init:
				if (tmp == stop_str) {
					state = State::Finished;
				} else if (tmp == string(1,literal::Semicolon)) {
					state = State::Finished;
				} else if (tmp == ignore_start) {
//					prev_delim = delim;
					current_item = tmp;
					state = State::Ignore_First;
				} else {
					state = State::Item;
					current_item = tmp;
					ident_list.push_back(current_item);
					current_item.clear();
				}
				break;
			case State::Item:
				if (tmp == delimiter) {
					state = State::Comma;
				} else if (tmp == string(1,literal::Semicolon)) {
					state = State::Finished;
				} else {
					state = State::Finished;
				}
				break;
			case State::Ignore_First:
				if (tmp == ignore_end) {
					current_item += tmp;
					ident_list.push_back(current_item);
					current_item.clear();
					state = State::Item;
				} else {
					current_item += tmp;
					state = State::Ignore;
				}
				break;
			case State::Ignore:
				if (tmp == ignore_end) {
					state = State::Item;
					current_item += tmp;
					ident_list.push_back(current_item);
					current_item.clear();
				} else {
					current_item += tmp;
				}
				break;
			case State::Finished:
				break;
		}
	}
	if (state != State::Finished && state != State::Item) {
		throw ParserQueryError("Identifier Expected",in.tellg());
	}
	if (in.eof()) tmp = "";
	return tmp;
}




void Parser_Query::parse() {
	string tmp;

	//select-clause
	{
		tmp = read_token(in);
		if (tmp != keyword::Select) {
			throw ParserQueryError("Select Expected",in.tellg());
		}
		vector<string> fields_tmp;
		read_token.push_reserved_literals(",;");
		tmp = read_delimited_list(in, keyword::From, string(1,literal::Comma)
				, string(1,literal::LBracket),  string(1,literal::RBracket), fields_tmp);
		read_token.pop_reserved_literals();

		if (tmp != keyword::From) {
			throw ParserQueryError("From Expected",in.tellg());
		}
		if (fields_tmp.empty()) {
			throw ParserQueryError("Empty Select Section",in.tellg());
		}
		for (auto& f : fields_tmp) {
			smatch m;
			regex_match(f, m, regex("^sum\\((.*)\\)$"));

			if (!m.empty()) {
				assert(m.size() > 1);
				f = m[1].str();
				if (!is_identifier(f)) {
					throw ParserQueryError("'" + f + "' is not an identified in Select Section",in.tellg());
				}
				fields.push_back({f});
				aggregated_fields_ind.push_back(fields.size()-1);
			} else if (is_identifier(f)) {
				fields.push_back({f});
			} else {
				throw ParserQueryError("'" + f + "' is not an identified in Select Section",in.tellg());
			}
		}

		fields_tmp.clear();
		//check fields for duplicates
		for (const auto& f : fields) {
			fields_tmp.push_back(f.field_name);
		}
		sort(fields_tmp.begin(), fields_tmp.end());
		for (size_t i = 0; i < fields_tmp.size()-1; ++i) {
			if (fields_tmp[i] == fields_tmp[i+1]) {
				throw ParserQueryError("Field '" + fields_tmp[i] + "' appears several times in Select-Clause",in.tellg());
			}
		}
	}

	//from-clause
	{
		tmp = read_delimited_list(in, "", string(1,literal::Comma), "",  "", tables);
		if (tmp == keyword::Where) {
			//conditions-clause
			tmp = read_conditions_list(in, conditions);
		}
	}

	//grouping-clause
	bool group_by_clause = (tmp == keyword::Group);
	if (group_by_clause) {
		tmp = read_token(in);
		assert(tmp == keyword::By);
		tmp = read_token(in);

		if (fields.size() > sizeof(size_t)*8-1) {
			throw ParserQueryError("Select-Fields should contain <= 63 entries",in.tellg());
		}

		vector<vector<size_t>> groups_fields_ind;
		//groups_fields_ind[i] = {indices within 'fields' of group[i] }} (single redirection)
		//key_field_groups[i].bit(j) corresponds fields_ind[j] (double redirection)


		if (tmp == keyword::Rollup || tmp == keyword::Cube) {
			//build groups_fields_ind for rollup or cube
			bool cube = (tmp == keyword::Cube);
			vector<string> fields_tmp;

			tmp = read_token(in);
			if (tmp != "(") {
				throw ParserQueryError("Expected '(' after Rollup/Cube",in.tellg());
			}
			tmp = read_delimited_list(in, ")", string(1,literal::Comma), "",  "", fields_tmp);
			if (tmp != ")") {
				throw ParserQueryError("Expected ')' after fields enumeration in Rollup/Cube",in.tellg());
			}

			for (const auto& f : fields_tmp) {
				auto it = find_if(fields.begin(), fields.end(), FUPredicate(f));
				if (it == fields.end()) {
					throw ParserQueryError("All fields from grouping set should be defined in select-clase: '" + f + "'",in.tellg());
				} else {
					key_fields_ind.push_back(distance(fields.begin(), it));
				}
			}

			if (key_fields_ind.size() > sizeof(size_t)*8-1) {
				throw ParserQueryError("Grouping doesn't support more then 63 grouping fields",in.tellg());
			}

			if (cube) {
				size_t max_group = (size_t(1) << key_fields_ind.size()) - 1;
				groups_fields_ind.resize(max_group + 1);
				for (size_t group = 0; group <= max_group; ++group) {
					for (size_t bit = 0; bit < key_fields_ind.size(); ++ bit) {
						if (bitSetTest(group,bit)) {
							groups_fields_ind[group].push_back(key_fields_ind[bit]);
						}
					}

				}
			} else { //rollup
				groups_fields_ind.resize(key_fields_ind.size()+1);
				size_t group_count = 0;
				for (size_t i = 0; i < key_fields_ind.size()+1; ++i, ++group_count) {
					size_t group = (size_t(1) << i) - 1;//2^i-1
					for (size_t bit = 0; bit < key_fields_ind.size(); ++ bit) {
						if (bitSetTest(group,bit)) {
							groups_fields_ind[group_count].push_back(key_fields_ind[bit]);
						}
					}

				}
			}

		} else if (tmp == keyword::Grouping) {
			//build groups_fields_ind for grouping set
			tmp = read_token(in);
			if (tmp != keyword::Sets) {
				throw ParserQueryError("Expecting 'Sets' in 'Grouping Set'",in.tellg());
			}
			// get all groups
			tmp = read_token(in);
			if (tmp != "(") {
				throw ParserQueryError("Expected '(' after Grouping Set",in.tellg());
			}
			vector<string> groups_tmp;
			tmp = read_delimited_list(in, ")", string(1,literal::Comma), "(",  ")", groups_tmp);
			if (tmp != ")") {
				throw ParserQueryError("Expected ')' after groups enumeration in Grouping Set",in.tellg());
			}

			vector<string> fields_tmp;
			// parse each group
			groups_fields_ind.resize(groups_tmp.size());
			size_t count_group = 0;
			for (const auto& group_text : groups_tmp) {
				istringstream group_in(group_text);
				tmp = read_token(group_in);
				if (tmp != "(") {
					throw ParserQueryError("Expected '(' before Group in '" + group_text + "'",in.tellg());
				}
				tmp = read_delimited_list(group_in, ")", string(1,literal::Comma), "",  "", fields_tmp);
				if (tmp != ")") {
					throw ParserQueryError("Expected ')' after Group in '" + group_text + "'",in.tellg());
				}

				for (const auto& f : fields_tmp) {
					auto it = find_if(fields.begin(), fields.end(), FUPredicate(f));
					if (it == fields.end()) {
						throw ParserQueryError("All fields from grouping set should be defined in select-clase: '" + f + "'",in.tellg());
					} else {
						size_t ind = distance(fields.begin(), it);
						auto it1 = find(key_fields_ind.begin(), key_fields_ind.end(), ind);
						if (it1 == key_fields_ind.end()) {
							key_fields_ind.push_back(ind);
						}
						groups_fields_ind[count_group].push_back(ind);
					}
				}
				++count_group;
				fields_tmp.clear();
			}
			groups_tmp.clear();

		} else {
			throw ParserQueryError("Incorrect group by condition: expected grouping set, rollup or cube",in.tellg());
		}
		if (groups_fields_ind.empty()) {
			throw ParserQueryError("No grouping sets in group-by-clause specified!",in.tellg());
		}

		//now we need to convert groups_fields_ind to key_field_groups, but first sort key_fields_ind
		//
		//select key_field1, ..., key_fieldn, aggr_field1, ..., aggr_fieldn
		//from...
		//
		//NB: groups_fields_ind[i] = {indices within 'fields' of group[i] }} (single redirection)
		//key_field_groups[i].bit(j) corresponds fields_ind[j] (double redirection)
		sort(key_fields_ind.begin(), key_fields_ind.end());
		for (size_t i = 0; i < key_fields_ind.size()-1; ++i) {
			if (key_fields_ind[i] >= key_fields_ind[i+1]) {
				throw ParserQueryError("The order of select-fields and the order of grouping sets are in contradiction!",in.tellg());
			}
		}
		//check whether all remaining fields inside 'fields' are in 'aggregated_fields_ind'
		size_t current_ind = key_fields_ind.back();
		++current_ind;
		if (aggregated_fields_ind.empty()) {
			throw ParserQueryError("No aggregated fields for group-by-clause",in.tellg());
		}
		sort(aggregated_fields_ind.begin(),aggregated_fields_ind.end());
		for (size_t i = 0; i < aggregated_fields_ind.size(); ++i) {
			if (aggregated_fields_ind[i] < current_ind) {
				throw ParserQueryError("Aggregated field '" + fields[aggregated_fields_ind[i]].field_name
						+ "' is already used for grouping, while the grouping-clause is present",in.tellg());
			}
			if (aggregated_fields_ind[i] > current_ind) {
				throw ParserQueryError("Select-field '" + fields[current_ind].field_name
						+ "' is used neither for grouping nor for aggregation, while the grouping-clause is present",in.tellg());
			}
			++current_ind;
		}
		if (current_ind < fields.size()) {
			throw ParserQueryError("Select-field '" + fields[current_ind].field_name
					+ "' is used neither for grouping nor for aggregation, while the grouping-clause is present",in.tellg());
		}
		//finally, conversion...
		for (const auto& group_fields_ind : groups_fields_ind) {
			size_t group_mask = 0;
			for (const auto& field_ind : group_fields_ind) {
				//convert index of 'field' to index of 'key_fields_ind'
				auto it = find(key_fields_ind.begin(), key_fields_ind.end(), field_ind);
				size_t ind = distance(key_fields_ind.begin(), it);
				//save index in mask
				group_mask |= size_t(1) << ind;
			}
			key_field_groups.push_back(group_mask);
		}


		tmp = read_token(in);
	} else if (aggregated_fields_ind.size() > 0) {
		throw ParserQueryError("No grouping-clause, while aggregated fields are specified",in.tellg());
	}


	assert(tmp == string(1,literal::Semicolon));
}

string Parser_Query::generate() {
	
	Context context(schema);
	stringstream out;
	out << "#include \"Types.hpp\""   << endl;
	out << "#include \"schema_1.hpp\""   << endl;
	out << "#include <iostream>"      << endl;
	out << "#include <unordered_map>" << endl;
	out << "using namespace std;"     << endl;
	//out << "bool pred(const Varchar<16>& s) {return s.len > 0 && s.value[0]=='B';}";
	
	
	//tables
	vector<const Schema::Relation*> rels;
	vector<OperatorScan> scans;
	for (auto& t : tables) {
		const auto* rel = context.getTabDef(t);
		if (rel == nullptr) {
			throw runtime_error("No table called '"+t+"'");
		}
		rels.push_back(rel);
		scans.emplace_back(&context, out, t, db_name);
	}	
	//fields
	for (auto& f : fields) {
		if (f.tab_name.empty()) {
			for (auto* rel : rels) {
				auto attr = context.getAttr({f.field_name, rel->name});
				if (attr != nullptr) {
					f.tab_name = rel->name;
					f.type_name = type(*attr);
					break;
				}
			}
		}
	}
	
	// joins
	vector<OperatorHashJoin> joins;
	joins.reserve(tables.size()-1);
	
	vector<Field_Unit> f_left;
	vector<Field_Unit> f_right;
	for (size_t i = 0; i < tables.size() -1; ++i) {
		f_left.clear();
		f_right.clear();
		auto t_left = rels[i];
		auto t_right = rels[i+1];
		
		for (auto it = conditions.begin(); it != conditions.end(); ) {
			if (context.getAttr({it->first, t_left->name}) != nullptr
				&& context.getAttr({it->second, t_right->name}) != nullptr
			) 
			{
				f_left.push_back({it->first, t_left->name});
				f_right.push_back({it->second, t_right->name});
				it = conditions.erase(it);
			} 
			else if (context.getAttr({it->first, t_right->name}) != nullptr
				&& context.getAttr({it->second, t_left->name}) != nullptr
			) 
			{
				f_left.push_back({it->second, t_left->name});
				f_right.push_back({it->first, t_right->name});
				it = conditions.erase(it);
			} 
			else 
			{
				++it;
			}
		}
		assert(f_left.size() == f_right.size());
		if (f_left.empty()) {
			throw runtime_error("No join conditions for '" + t_left->name + "' and '" + t_right->name + "'");
		}
		joins.emplace_back(&context, out,f_left,f_right);
		if (i > 0) {
			joins[i].setLeft(&joins[i-1]);
		}
	}
	
	OperatorProjection proj(&context, out, fields);
	if (joins.size() > 0) {
		proj.setInput(&joins.back());
	}
	//print
	OperatorPrint print(&context, out);
	print.setInput(&proj);
	//conditions & selects
	vector<OperatorSelect> selects;
	
	struct Condition {
		const Schema::Relation::Attribute* attr;
		string const_val;
		size_t index;
	};
	vector<Condition> conditions2;
	const Schema::Relation::Attribute* attr = nullptr;
	for (size_t i = 0; i < conditions.size(); ++i) {
		for (size_t n = 0; n < rels.size(); ++n) {
			if ((attr=context.getAttr({conditions[i].first, rels[n]->name})) != nullptr) {
				if (is_identifier(conditions[i].second)) 
					throw runtime_error("Condition '" + conditions[i].first + "=" + conditions[i].second + "': problem");
				conditions2.push_back({attr, conditions[i].second,n});
				break;
			}
			if ((attr=context.getAttr({conditions[i].second, rels[n]->name})) != nullptr) {
				if (is_identifier(conditions[i].first)) 
					throw runtime_error("Condition '" + conditions[i].first + "=" + conditions[i].second + "': problem");
				conditions2.push_back({attr, conditions[i].first,n});
				break;
			}
		}
		if (attr == nullptr) {
			throw runtime_error("Attributes '" + conditions[i].first + "' and '" + conditions[i].second + "' not found in schema");
		}
	}
	
	Field_Comparison fc;
	string pred_header;
	string pred_body;
	for (size_t i = 0; i < rels.size(); ++i) {
		bool first = true;
		pred_header.clear();
		pred_body.clear();
		for (auto it = conditions2.begin(); it != conditions2.end(); ) {
			if (it->index != i) {
				++it;
				continue;
			} 
			if (first) {
				fc.predicat = name_generator.request_name("predicat",true);
				pred_header += "bool " + fc.predicat + "(";
				pred_body += "return ";
				pred_header += "const " + type(*it->attr) + "& " + it->attr->name;
				pred_body += it->attr->name + "==" + it->const_val;
				fc.fields.push_back({it->attr->name, rels[i]->name});
				first = false;
			} else {
				pred_header += ",const " + type(*it->attr) + "& " + it->attr->name;
				pred_body += " && " + it->attr->name + "==" + it->const_val;
				fc.fields.push_back({it->attr->name, rels[i]->name});
			}
			it = conditions2.erase(it);
		}
		if (!first) {
			pred_header += ")";
			pred_body += ";";
			out << pred_header << "{" << pred_body << "}";
			selects.emplace_back(&context, out, fc);
			selects.back().setInput(&scans[i]);
			if (i==0) {
				if (rels.size() == 1) {
					proj.setInput(&selects.back());
				} else {
					joins[0].setLeft(&selects.back());
				}
			} else {
				joins[i-1].setRight(&selects.back());
			}
		} else {
			if (i==0) {
				if (rels.size() == 1) {
					proj.setInput(&scans[0]);
				} else {
					joins[0].setLeft(&scans[0]);
				}
			} else {
				joins[i-1].setRight(&scans[i]);
			}
		}
	}
	assert(conditions2.empty());
	
	out << "extern \"C\" void run_query(const Database& " + db_name + ") {" << endl;
	print.produce();
	out << "}";
	
	return out.str();
}


