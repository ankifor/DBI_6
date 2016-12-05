#include "Parser_Query.h"

#include <sstream>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <cstdlib>
#include <cctype>
#include <regex>
#include <assert.h>

#include "code_generation.h"

using namespace std;

extern string type(const Schema::Relation::Attribute& attr);
extern Name_Generator name_generator;

namespace keyword {
	const string Select  = "select"  ;
	const string From    = "from"    ;
	const string And    = "and"    ;
	const string Where   = "where"   ;
}

namespace literal {
	const char Comma  = ','  ;
	const char Semicolon  = ';'  ;
	const char EqualSign  = '='  ;
	const char Quote  = '\''  ;
	const char Screen  = '\\'  ;
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

//static bool is_quoted_text(const string& s) {
//	return s.length() >= 2 && s[0] == '"' && s.back() == '"';
//}

static string read_token(istringstream& in) {
	assert(!in.eof());
	string tmp = "";
	bool any = false;
	char ch = 0;
	while (in.get(ch)) {
		if (
			ch == literal::Comma
			|| ch == literal::EqualSign
			|| ch == literal::Quote
			|| ch == literal::Screen
			|| ch == literal::Semicolon
		)
		{
			if (!any) {
				tmp += ch;
				any = true;
			} else {
				in.unget();
			}
			break;
		} else if (ch==' '||ch == '\r'|| ch=='\n'||ch=='\t') {
			if (any) {
				break;
			}
		} else {
			tmp += ch;
			any = true;
		}
	}
	return tmp;
}


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


static string read_comma_delimited_list(istringstream& in, const string& stop_str, vector<string>& ident_list) {
	enum class State : unsigned { 
		Init, Identifier, Comma, Finished
	} state;
	state = State::Init;
	
	string tmp;
	while (!in.eof() && state != State::Finished) {
		tmp = read_token(in);
		switch (state) {
			case State::Comma:
			case State::Init:
				if (is_identifier(tmp)) {
					state = State::Identifier;
					ident_list.push_back(tmp);
				} else {
					throw ParserQueryError("Identifier Expected",in.tellg());
				}
				break;
			case State::Identifier:
				if (tmp == string(1,literal::Comma)) {
					state = State::Comma;
				} else if (tmp == string(1,literal::Semicolon)) {
					state = State::Finished;
					break;
				} else if (tmp == stop_str) {
					state = State::Finished;
					break;
				} else {
					throw ParserQueryError("Comma Expected",in.tellg());
				}
				break;
			case State::Finished:
				break;
		}
	}
	if (state != State::Finished && state != State::Identifier) {
		throw ParserQueryError("Identifier Expected",in.tellg());
	}
	if (in.eof()) tmp = "";
	return tmp;
}




void Parser_Query::parse() {
	string tmp;
	tmp = read_token(in);
	if (tmp != keyword::Select) {
		throw ParserQueryError("Select Expected",in.tellg());
	}
	
	vector<string> fields_tmp;
	if (read_comma_delimited_list(in, keyword::From, fields_tmp) != keyword::From) {
		throw ParserQueryError("From Expected",in.tellg());
	}
	for (auto& f : fields_tmp) {
		fields.push_back({f});
	}
	
	
	
	if (read_comma_delimited_list(in, keyword::Where, tables) == keyword::Where) {
		if (read_conditions_list(in, conditions) != string(1,literal::Semicolon)) {
			assert(false);
		}
	}
//	for (auto& x : fields) cout << x.field_name << ",";
//	cout << endl;
//	
//	for (auto& x : tables) cout << x << ",";
//	cout << endl;
//	
//	for (auto& x : conditions) cout << x.first << "=" << x.second << ",";
//	cout << endl;
	
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


