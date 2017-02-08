#pragma once

#pragma once

#include <exception>
#include <string>
#include <vector>
#include <utility>
#include <memory>
#include <sstream>
#include "Schema.hpp"
#include "code_generation.h"

using namespace std;

class ParserQueryError : exception {
   string msg;
   int pos;
public:
   ParserQueryError(const string& m,int pos) : msg(m), pos(pos) {}
   ~ParserQueryError() throw() {}
   const char* what() const throw() {
      return msg.c_str();
   }
};

struct Parser_Query {
	istream& in;
	Schema schema;
	string db_name;

	Schema::Relation* rel;

	Parser_Query(istream& in, const Schema* pschema, const string& db_name)
		: in(in), schema(*pschema), db_name(db_name), rel(nullptr) {}

	~Parser_Query() {};
	bool parse();
	string generate();
private:
	void nextToken(unsigned line, const string& token, Schema& s);
	
	vector<Field_Unit> fields;
	vector<size_t> aggregated_fields_ind;
	vector<size_t> key_field_groups;
	vector<size_t> key_fields_ind;
	vector<string> tables;

	vector<pair<string,string>> conditions;
};
