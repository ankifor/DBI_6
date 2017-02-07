#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <assert.h>
#include "code_generation.h"
#include "Help.hpp"

using namespace std;


//==============================================================================
// Helpers
//==============================================================================
extern string type(const Schema::Relation::Attribute& attr);
extern string ReplaceString(string subject, const string& search, const string& replace);
extern void ReplaceStringInPlace(string& subject, const string& search, const string& replace);

struct AttrPredicate {
	string name;
	AttrPredicate(string name) : name(name) {}
	bool operator()(const Schema::Relation::Attribute& t) const { return t.name == name; }
};

struct RelPredicate {
	string name;
	RelPredicate(string name) : name(name) {}
	bool operator()(const Schema::Relation& r) const { return r.name == name; }
};

extern Name_Generator name_generator;

static bool subsetOf(size_t left, size_t right) {
	size_t tmp = ~left | right;
	const size_t all = ~size_t(0);
	return tmp == all;
}

static bool bitSet(size_t src, size_t bit) {
	size_t res = (src >> bit) & 1;
	return res == 1;
}

//void remove_not_required_fields(vector<Field_Unit>& produced, const vector<Field_Unit>& required) {
//	for (auto it = produced.begin(); it != produced.end(); ) {
//		auto it_req = find(required.begin(), required.end(), *it);
//		if (it_req == required.end()) {
//			// not found => not required
//			it = produced.erase(it);
//		} else {
//			++it;
//		}
//	}
//}
//
//void check_produced_for_required_fields(const vector<Field_Unit>& produced, const vector<Field_Unit>& required) {
//	for (auto it = required.begin(); it != required.end(); ++it) {
//		auto it_prod = find(produced.begin(), produced.end(), *it);
//		assert(it_prod != produced.end());
//	}
//}


//==============================================================================
// Context
//==============================================================================

const Schema::Relation* Context::getTabDef(string tabname) const {
	auto it = find_if(schema.relations.begin(), schema.relations.end(), RelPredicate(tabname));
	if (it == schema.relations.end()) {
		return nullptr;
	}
	return &(*it);
}

const Schema::Relation::Attribute* Context::getAttr(const Field_Unit& fu) const {
	auto rel = getTabDef(fu.tab_name);
	if (rel == nullptr) {
		return nullptr;
	}
	auto it = find_if(rel->attributes.begin(), rel->attributes.end(), AttrPredicate(fu.field_name));
	if (it == rel->attributes.end()) {
		return nullptr;
	}
	return &(*it);
}

//==============================================================================
// OperatorScan
//==============================================================================
OperatorScan::OperatorScan(const Context* context, stringstream& out, const string& tabname, const string& db_name) 
	: OperatorUnary(context,out)
	, tabname(tabname) 
	, db_name(db_name)
{
	tid_name = name_generator.request_name("tid", true);
}
		
void OperatorScan::computeProduced() {
	
	auto def = *context->getTabDef(tabname);
	
	for (auto it = fields.begin(); it != fields.end(); ) {
		auto it_tab = find_if(def.attributes.begin(), def.attributes.end(), AttrPredicate(it->field_name));
		if (it_tab == def.attributes.end() || (!it->tab_name.empty() && it->tab_name != tabname)) {
			// not present in table
			it = fields.erase(it);
		} else {
			it->tab_name = tabname;
			it->type_name = type(*it_tab);
			it->token = db_name + "." + it->tab_name + "." + it->field_name + "[" + tid_name + "]";
			++it;
		}
	}
	assert(fields.size() > 0);
}

void OperatorScan::produce() {
	const string tmplt = "for (Tid &tid; = 0;&tid; < &tab;.size(); ++&tid;)";
	string tmp = ReplaceString(tmplt,"&tid;",tid_name);
	ReplaceStringInPlace(tmp, "&tab;", db_name + "." + tabname);
	out << tmp << "{";
	consumer->consume(this);
	out << "}";
}

//==============================================================================
// OperatorPrint
//==============================================================================

void OperatorPrint::consume(const Operator* caller) {
	auto produced = input->getProduced();	
	out << "cout<<";
	string delim = "";
	for (auto& f : produced) {
		out << delim << f.token << "<<";
		delim = "\",\"<<";
	}
	out << "endl;";
}

//==============================================================================
// OperatorProjection
//==============================================================================
void OperatorProjection::computeProduced() {
	auto produced = input->getProduced();
	for (auto it = fields.begin(); it != fields.end(); ) {
		auto it_prod = find(produced.begin(), produced.end(), *it);
		if (it_prod == produced.end()) {
			assert(false);//should not happen
		} else {
			*it = *it_prod;
			++it;
		}
	}
}


//==============================================================================
// OperatorSelect
//==============================================================================
void OperatorSelect::computeRequired() {
	fields = consumer->getRequired();
	//add fields from conditions, if needed
	for (auto& f : condition.fields) {
		auto it = find(fields.begin(), fields.end(), f);
		if (it == fields.end()) {
			fields.push_back(f);
		}
	}
}

void OperatorSelect::computeProduced() {
	auto produced = input->getProduced();
	for (auto it = fields.begin(); it != fields.end(); ) {
		auto it_prod = find(produced.begin(), produced.end(), *it);
		if (it_prod == produced.end()) {
			it = fields.erase(it);//not produced
		} else {
			*it = *it_prod;
			++it;
		}
	}
}

void OperatorSelect::consume(const Operator* caller) {
	auto produced = getProduced();
	out << "if (" << condition.predicat << "(";
	string delim = "";
	for (auto& f : condition.fields) {
		auto it = find(produced.begin(), produced.end(), f);
		assert(it != produced.end());
		out << delim << it->token;
		delim = ",";
	}
	out << ")){";
	consumer->consume(this);
	out << "}";
}

//==============================================================================
// OperatorGroupingSet
//==============================================================================
void OperatorGroupingSet::computeRequired() {
	for (const auto& f : key_fields) {
		auto it = find(fields.begin(), fields.end(), f);
		if (it == fields.end()) {
			fields.push_back(f);
		}
	}
	for (const auto& f : val_fields) {
		auto it = find(fields.begin(), fields.end(), f);
		if (it == fields.end()) {
			fields.push_back(f);
		} else {
			assert(false);
		}

	}
}

void OperatorGroupingSet::produce() {
	computeGroupsGraph();
	getProduced();
	//reserve names
	hash_table_group_names.clear();
	for (size_t group : key_groups) {
		hash_table_group_names.push_back(name_generator.request_name("hash_table_" +  to_string(group),false));
	}

	defineTypeKey();
	defineTypeVal();
	defineTypeUpdate();
	defineHashTable(root_group_index, "nullptr");

	input->produce();


	struct Node {
		size_t group_ind_stack;
		bool last_child;
		size_t parent_ind;
	};
	vector<Node> traversal;//for depth-first traversal
	//let n = key_fields.size();
	//depth-first traversal requires simultaneous storing of [n/2]+1 hash_tables, whereas
	//breadth-first traversal >= n-2.

	traversal.push_back({root_group_index, true, 0});
	vector<size_t> children;

	while (!traversal.empty()) {
		Node current = traversal.back();
		size_t group_ind = current.group_ind_stack;
		size_t group = key_groups[group_ind];
		traversal.pop_back();
		//process node
		if (group_ind == root_group_index) {
			if (print_root) {
				printHashTable(group_ind);
			}
		} else {
			bool memory_reusage = group != 0 && current.last_child;

			string storage;
			if (!memory_reusage) {
				storage = "nullptr";
			} else {
				storage = hash_table_group_names[current.parent_ind] + "._storage";
			}
			defineHashTable(group_ind, storage);

			if (memory_reusage) {
				//last child frees the parent's memory immediately after sharing the storage
				clearHashTable(current.parent_ind);
			}

			if (!memory_reusage) {
				out << "for (const auto& it: *" << hash_table_group_names[current.parent_ind] << "._storage){";
				if (group != 0) {
					//hash_4.modify(get<0>(it), get<1>(it));
					out << hash_table_group_names[group_ind] << ".modify(get<0>(it), get<1>(it));";
				} else {
					//upd(hash_0, get<1>(it));
					out << update_val_instance << "(" << hash_table_group_names[group_ind] << ",get<1>(it));";
				}
				out << "}";
				if (group == 0 && current.last_child) {
					clearHashTable(current.parent_ind);
				}


			} else {
				//hash_6.build_from_storage<true>();
				out << hash_table_group_names[group_ind] << ".build_from_storage<true>();";
			}

			printHashTable(group_ind);
		}
		//add children to traversal
		children.clear();
		for (auto it = groupsGraph.equal_range(group_ind); it.first != it.second; ++it.first) {
			children.push_back(it.first->second);
		}
		//no children => we should free memory
		if (children.size() == 0) {
			clearHashTable(group_ind);
		} else {
			sort(children.begin(), children.end());//[](size_t left, size_t right){return left > right;}
			traversal.push_back({children[0],true,group_ind});
			for (size_t i = 1; i < children.size(); ++i) {
				traversal.push_back({children[i],false,group_ind});
			}

		}
	}

}

void OperatorGroupingSet::defineTypeKey() {
	type_key = name_generator.request_name("type_key", false);

	out << "using " << type_key << "=tuple<";
	string delim = "";
	for (const auto& f : key_fields) {
		auto it = find(fields.begin(),fields.end(),f);
		out << delim << it->type_name;
		delim = ",";
	}
	out << ">;";

}

void OperatorGroupingSet::defineTypeVal() {
	type_val = name_generator.request_name("type_val", false);

	out << "using " << type_val << "=tuple<";
	string delim = "";
	for (const auto& f : val_fields) {
		auto it = find(fields.begin(),fields.end(),f);
		out << delim << it->type_name;
		delim = ",";
	}
	out << ">;";
}

void OperatorGroupingSet::defineTypeUpdate() {
	update_val = name_generator.request_name("Update_Val", true);
	update_val_instance = name_generator.request_name("update_val", true);

	out << "struct " << update_val
		<< "{"
		<< "void operator()(" << type_val << "& left" << ", const " << type_val << "& right) {";
	//only addition
	for (size_t i = 0; i < val_fields.size(); ++i) {
		out << "get<" << i << ">(left) += get<" << i << ">(right);";
	}
	out	<< "}"
		<< "} " << update_val_instance << ";";


}

void OperatorGroupingSet::defineHashTable(size_t group_index, const string& storage) {
	//we need to define: struct for equality of two key tuples
	//define hash
	//define hash_table
	size_t group = key_groups[group_index];

	if (group != 0) {
		//equality struct
		//	struct Eq_7 {
		//		bool operator()(type_key& left, type_key& right) {
		//          bool res = true;
		//          res &&= (get<0>(left)==get<0>(right));
		//          res &&= (get<1>(left)==get<1>(right));
		//          res &&= (get<2>(left)==get<2>(right));
		//			return res;
		//		}
		//	};
		string struct_equal = name_generator.request_name("Eq_" + to_string(group), false);
		out << "struct " << struct_equal
			<< "{"
			<< "bool operator()(" << type_key << "& left" << ", " << type_key << "& right) {return ";

		if (group == 0) {
			out << "true;";
		} else {
			string delim = "";
			for (size_t i = 0; i < key_fields.size(); ++i) {
				if (bitSet(group,i)) {
					out << delim << "(get<" << i << ">(left) == get<" << i << ">(right))";
					delim = " && ";
				}
			}
		}
		out	<< ";}"
			<< "};";

		//hash
		//using h_111 = hash_types_1::hash<type_key, 7>;
		string hash_type = name_generator.request_name("Hash_" + to_string(group), false);
		out << "using " << hash_type << "=hash_types_1::hash<" << type_key << "," << group << ">;";

		//define hash_table
		//My_Hash<type_key, type_val, h_111, myEq_7, true, UpdateVal1> hash_7(nullptr);
		out << "My_Hash<"
				<< type_key
				<< "," << type_val
				<< "," << hash_type
				<< "," << struct_equal
				<< "," << "true"
				<< "," << update_val
				<< "> " << hash_table_group_names[group_index]
				<< "(" <<  storage << ");";
	} else {
		//in this case we don't need hash_table
		//type_val hash_0;
		out << type_val << " " << hash_table_group_names[group_index] << ";";
	}
}


void OperatorGroupingSet::printHashTable(size_t group_index) {
	size_t group = key_groups[group_index];

	const string null_token = "\"null\"";
	const string comma_token = "\",\"";

	string key_token;
	string val_token;

	if (group != 0) {
		//	for (const auto& it : *hash_1._storage) {
		//		cout << "null" << "null" << get<2>(get<0>(it)) << get<0>(get<1>(it)) << endl;
		//	}
		key_token = "get<0>(it)";
		val_token = "get<1>(it)";

		out << "for (const auto& it: *" << hash_table_group_names[group_index] << "._storage)";

	} else {
		//	cout << "null" << "null" << "null" << get<0>(hash_0) << endl;
		key_token = "";
		val_token = hash_table_group_names[group_index];
	}

	out << "{cout<<";
	//print key
	string delim = "";
	for (size_t i = 0; i < key_fields.size(); ++i) {
		out << delim;
		if (bitSet(group, i)) {
			out << "get<" << i << ">(" << key_token << ")";
		} else {
			out << null_token;
		}
		delim = "<<" + comma_token + "<<";
	}
	//print value
	for (size_t i = 0; i < val_fields.size(); ++i) {
		out << delim  << "get<" << i << ">(" << val_token << ")";
		delim = "<<" + comma_token + "<<";
	}
	//print end line
	out << "<<endl;}";
}

void OperatorGroupingSet::clearHashTable(size_t group_index) {
	size_t group = key_groups[group_index];
	if (group != 0) {
		out << hash_table_group_names[group_index] << ".clear();";
	}
}

void OperatorGroupingSet::consume(const Operator* caller) {
//	hash_111.modify(
//		make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
//		, make_tuple(db.order.o_ol_cnt[tid0])
//	);
	string delim = "";
	auto& produced = input->getProduced();

	size_t group = key_groups[root_group_index];
	if (group != 0) {
		out << hash_table_group_names[root_group_index] << ".modify(";
		{
			out << "make_tuple(";
			delim = "";
			for (const auto& f : key_fields) {
				auto it = find(produced.begin(),produced.end(),f);
				out << delim << it->token;
				delim = ",";
			}
			out << ")";
		}
		out << ",";
	} else {
		out << update_val_instance << "(" << hash_table_group_names[root_group_index] << ",";
	}
	{
		out << "make_tuple(";
		delim = "";
		for (const auto& f : val_fields) {
			auto it = find(produced.begin(),produced.end(),f);
			out << delim << it->token;
			delim = ",";
		}
		out << ")";
	}
	out << ");";
}

void OperatorGroupingSet::computeGroupsGraph() {
	groupsGraph.clear();
	size_t root_group = 0;
	//check key_groups
	{
		size_t all_fields = 0;
		for (size_t mask : key_groups) {
			all_fields |= mask;
		}

		size_t set_bits = 0;
		for (size_t bit = 0; bit < sizeof(size_t)*8 && bit == set_bits; ++bit) {
			if (bitSet(all_fields, bit)) {
				++set_bits;
			}
		}
		assert(key_fields.size() == set_bits);
		root_group = all_fields;
	}
	//sort in ascending order like size_t (works for binary masks)
	sort(key_groups.begin(), key_groups.end());
	//add root if needed
	if (key_groups.back() != root_group) {
		key_groups.push_back(root_group);
		print_root = false;
	} else {
		print_root = true;
	}
	root_group_index = key_groups.size() - 1;
	// check duplicates
	{
		for (size_t i = 0; i < key_groups.size()-1; ++i) {
			assert(key_groups[i] != key_groups[i+1]);
		}
	}
	// populate groupsGraph
	{
		for (size_t i = 0; i < key_groups.size()-1; ++i) {
			size_t j = i + 1;
			//find first j, s.t. key_groups[i] is subset of key_groups[j]
			//binary: x subset of y <=> y | ~x
			while( j < key_groups.size() && !subsetOf(key_groups[i],key_groups[j])) {
				++j;
			}
			assert(j < key_groups.size());
			groupsGraph.insert(make_pair(j, i));
		}
	}

}

//==============================================================================
// OperatorHashJoin
//==============================================================================

OperatorHashJoin::OperatorHashJoin(
	const Context* context, stringstream& out
	, const vector<Field_Unit>& left_fields
	, const vector<Field_Unit>& right_fields
) : OperatorBinary(context, out), left_fields(left_fields), right_fields(right_fields)
{
	hash_name = name_generator.request_name("hash", true);
	iterator_name = name_generator.request_name("it", true);
}

void OperatorHashJoin::computeRequired() {
	fields = consumer->getRequired();
	//add fields from left if needed
	for (const auto& f : left_fields) {
		auto it = find(fields.begin(), fields.end(), f);
		if (it == fields.end()) {
			fields.push_back(f);
		}
	}
	//add fields from right if needed
	for (const auto& f : right_fields) {
		auto it = find(fields.begin(), fields.end(), f);
		if (it == fields.end()) {
			fields.push_back(f);
		}
	}
}

void OperatorHashJoin::computeProduced() {
	size_t ind_val = 0;
	string it_key_name = iterator_name + ".first->first";
	string it_val_name = iterator_name + ".first->second";
	// refresh fields from left without change
	auto& produced_left = left->getProduced();
	for (auto& f : fields) {
		auto it = find(produced_left.begin(), produced_left.end(), f);
		if (it != produced_left.end()) {
			f = *it;
			auto it_key = find(left_fields.begin(), left_fields.end(), f);
			if (it_key != left_fields.end()) {
				f.token = "get<" + to_string(distance(left_fields.begin(), it_key)) + ">(" + it_key_name + ")";
			} else {
				f.token = "get<" + to_string(ind_val++) + ">(" + it_val_name + ")";
			}
		}
	}
	// refresh fields from right without change
	auto& produced_right = right->getProduced();
	for (auto& f : fields) {
		auto it = find(produced_right.begin(), produced_right.end(), f);
		if (it != produced_right.end()) {
			f = *it;
		}
	}
	// delete fileds without token
	for (auto it = fields.begin(); it != fields.end(); ) {
		if (it->token.empty()) {
			it = fields.erase(it);//not produced
		} else {
			++it;
		}
	}	
}

void OperatorHashJoin::produce(){
	string delim;
	auto& produced = left->getProduced();
	//key definition
	string type_key = name_generator.request_name("type_key", true);
	{
		out << "using " << type_key << "=tuple<";
		delim = "";
		for (const auto& f : left_fields) {
			auto it = find(produced.begin(),produced.end(),f);
			out << delim << it->type_name;
			delim = ",";
		}
		out << ">;";
	}
	//value definition
	string type_val = name_generator.request_name("type_val", true);
	{
		out << "using " << type_val << "=tuple<";
		delim = "";
		for (const auto& f : produced) {
			auto it = find(left_fields.begin(),left_fields.end(),f);
			if (it == left_fields.end()) {
				out << delim << f.type_name;
				delim = ",";
			}
		}
		out << ">;";
	}
	//hash definition
	out << "unordered_multimap<" 
		<< type_key 
		<< "," << type_val 
		<< "," << "hash_types::hash<" + type_key + ">> "
		<< hash_name << ";";
	//call produce of children
	left->produce();
	right->produce();
}

void OperatorHashJoin::consumeLeft() {
	string delim = "";
	auto& produced = left->getProduced();
	
	out << hash_name << ".insert(make_pair(";
	{
		out << "make_tuple(";
		delim = "";
		for (const auto& f : left_fields) {
			auto it = find(produced.begin(),produced.end(),f);
			out << delim << it->token;
			delim = ",";
		}
		out << ")";
	}
	out << ",";
	{
		out << "make_tuple(";
		delim = "";
		for (const auto& f : produced) {
			auto it = find(left_fields.begin(),left_fields.end(),f);
			if (it == left_fields.end()) {
				out << delim << f.token;
				delim = ",";
			}
		}		
		out << ")";
	}
	out << "));";
}

void OperatorHashJoin::consumeRight() {
	string delim = "";
	auto& produced = right->getProduced();
	out << "auto " << iterator_name << "="
		<< hash_name << ".equal_range(make_tuple(";
	{
		delim = "";
		for (const auto& f : right_fields) {
			auto it = find(produced.begin(),produced.end(),f);
			out << delim << it->token;
			delim = ",";
		}
	}
	out << "));";
	out << "while (" << iterator_name << ".first != " << iterator_name << ".second){";
	consumer->consume(this);
	out << "++" << iterator_name << ".first;}";
}
