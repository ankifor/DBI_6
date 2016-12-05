#include <iostream>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <dlfcn.h>
#include <regex>
#include <chrono>
#include "Schema.hpp"
#include "Parser_Schema.hpp"
#include "Parser_Query.h"
#include "code_generation.h"
#include "schema_1.hpp"

using namespace std;


Name_Generator name_generator;

struct Database db;

static void read_data(const string& path)
{
	ifstream in;

	in.open(path + "tpcc_warehouse.tbl");
	db.warehouse.read_from_file(in);
	in.close();

	in.open(path + "tpcc_district.tbl");
	db.district.read_from_file(in);
	in.close();

	in.open(path + "tpcc_customer.tbl");
	db.customer.read_from_file(in);
	in.close();

	in.open(path + "tpcc_history.tbl");
	db.history.read_from_file(in);
	in.close();

	in.open(path + "tpcc_order.tbl");
	db.order.read_from_file(in);
	in.close();

	in.open(path + "tpcc_neworder.tbl");
	db.neworder.read_from_file(in);
	in.close();

	in.open(path + "tpcc_orderline.tbl");
	db.orderline.read_from_file(in);
	in.close();

	in.open(path + "tpcc_item.tbl");
	db.item.read_from_file(in);
	in.close();

	in.open(path + "tpcc_stock.tbl");
	db.stock.read_from_file(in);
	in.close();
}

int main(int argc, char* argv[]) {
	if (argc != 5) {
		cerr << "usage: " << argv[0] 
		     << " <schema file>" 
		     << " <source folder>"
		     << " <object folder>"
			 << " <data dir>"
		     << endl
		     << argc << endl;
		return -1;
	}
	
	string schema_path = string(argv[1],strlen(argv[1]));
	string source_path = string(argv[2],strlen(argv[2])) + "/";
	string object_path = string(argv[3],strlen(argv[3])) + "/";
	string path_data = string(argv[4],strlen(argv[4])) + "/";
	
	cout << "reading db from '" << path_data << "'..." << endl;
	read_data(path_data);
	
	cout << "parsing schema from'" << schema_path << "'..." << endl;
	Parser_Schema p(schema_path);
	unique_ptr<Schema> schema;
	try {
		schema = p.parse();
	} catch (ParserError& e) {
		cerr << e.what() << endl;
		exit(1);
	}
	
	string input_string = "";
	ofstream out;
	string cpp_query = source_path + "query_1.cpp";
	string o_types = object_path + "Types.cpp.o";
	string o_schema = object_path + "schema_1.cpp.o";
	string so_query = object_path + "query_1.so";
	string compilation_command = "g++ "
		+ cpp_query + " " + o_types + " " + o_schema + " "
		+ " -std=c++11 -O3 -fPIC -shared -rdynamic -o "
		+ so_query
	;
	
	chrono::duration<int64_t, std::milli> elapsed_compilation;
	chrono::duration<int64_t, std::milli> elapsed_execution;
	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;
	while (true) {
		try {
			cout << "#";
			getline(cin, input_string);
			//transform(input_string.begin(), input_string.end(), input_string.begin(), ::tolower);
			if (input_string == "quit" || input_string == "exit") break;
			if (input_string.empty()) continue;
			//parse query
			Parser_Query q(input_string, schema.get(), "db");
			q.parse();
			//create temporary file
			out.open(cpp_query, ios_base::out);
			out << q.generate();
			out.close();
			//compiling
			start = chrono::high_resolution_clock::now();
			int res  = system(compilation_command.c_str());
			end = chrono::high_resolution_clock::now();
			elapsed_compilation = chrono::duration_cast<chrono::milliseconds>(end - start);
			if (res != 0) throw runtime_error("g++ exited with error " + to_string(res));
			//load lib
			void* handle=dlopen(so_query.c_str(),RTLD_NOW);
			if (!handle) throw runtime_error(dlerror());
			//get function handler
			auto fn=reinterpret_cast<void (*)(const Database&)>(dlsym(handle, "run_query"));
			if (!fn) throw runtime_error(dlerror());
			//run function
			start = chrono::high_resolution_clock::now();
			fn(db);
			end = chrono::high_resolution_clock::now();
			elapsed_execution = chrono::duration_cast<chrono::milliseconds>(end - start);
			//close lib
			if (dlclose(handle)) throw runtime_error(dlerror());
			cout << "compilation: " << elapsed_compilation.count() << " ms" << endl
				 << "execution:   " << elapsed_execution.count()   << " ms" << endl;
			
		} catch (ParserQueryError& e) {
			cerr << e.what() << endl;
			continue;
		} catch (runtime_error& e) {
			cerr << e.what() << endl;
			continue;
		}
		
	}
	return 0;
}
