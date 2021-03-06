#include <iostream>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <dlfcn.h>
#include <stdio_ext.h>
#include <regex>
#include <vector>
#include <chrono>
#include "Schema.hpp"
#include "Parser_Schema.hpp"
#include "Parser_Query.h"
#include "code_generation.h"
#include "schema_1.hpp"

//-DASAN_OPTIONS=symbolize=1
//-fsanitize=address -fno-omit-frame-pointer
//-lasan

using namespace std;


using query_fun = void (*)(const Database &db);

//extern "C" void run_query(const Database &db);

void run_query_dummy(const Database &db) {}
extern "C" void run_query0(const Database &db);
extern "C" void run_query1(const Database &db);
extern "C" void run_query2(const Database &db);
extern "C" void run_query3(const Database &db);
extern "C" void run_query4(const Database &db);
extern "C" void run_query5(const Database &db);
extern "C" void run_query6(const Database &db);
extern "C" void run_query7(const Database &db);
extern "C" void run_query8(const Database &db);
extern "C" void run_query_rollup_1(const Database &db);
extern "C" void run_query_rollup_2(const Database &db);
extern "C" void run_query_rollup_3(const Database &db);
extern "C" void run_query_rollup_4(const Database &db);
extern "C" void olap_query_001(const Database &db);



Name_Generator name_generator;

struct Database db;

static int32_t urand(int32_t min,int32_t max) {
   return (rand()%(max-min+1))+min;
}

static void populate_order(size_t n, int32_t N_o_d_id, int32_t N_o_w_id, int32_t N_o_carrier_id) {
	Integer in_o_d_id;
	Integer in_o_w_id;
	Integer in_o_carrier_id;
	Numeric<2,0> in_o_ol_cnt;
	for (size_t cnt = 0; cnt < n; ++cnt) {
		in_o_d_id = urand(1, N_o_d_id);
		in_o_w_id = urand(1, N_o_w_id);
		in_o_carrier_id = urand(1, N_o_carrier_id);
		db.order.insert(cnt,in_o_d_id,in_o_w_id,0,0,in_o_carrier_id,1,0);
	}
}

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


string create_query(Schema* schema) {
	Context context(*schema);
	stringstream out;

	out << "#include \"Types.hpp\""   << endl;
	out << "#include \"schema_1.hpp\""   << endl;
	out << "#include \"my_hash.h\""   << endl;
	out << "#include <iostream>"      << endl;
	out << "#include <unordered_map>" << endl;
	out << "#include <tuple>" << endl;
	out << "using namespace std;"     << endl;

	out << "extern \"C\" void olap_query_001(const Database& db) {" << endl;

	OperatorScan scanOrder(&context, out, "order", "db");

	OperatorGroupingSet groupingSet(&context, out
		, {{"o_d_id"},{"o_w_id"},{"o_carrier_id"},{"o_ol_cnt"}}
		, {0,1,2}
		, {3}
		, {0,1,6}
	);

	OperatorProjection projOrder(&context, out, {{"o_d_id"},{"o_w_id"},{"o_carrier_id"},{"o_ol_cnt"}});


	groupingSet.setInput(&projOrder);
	projOrder.setInput(&scanOrder);
	groupingSet.produce();
	out << "}" << endl;
	return out.str();
}

int main1(int argc, char* argv[]) {
//	if (argc != 3) {
//		cerr << "usage: " << argv[0]
//			 << " <schema path> <output path>"
//		     << endl
//		     << argc << endl;
//		return -1;
//	}
//	string path_schema = string(argv[1],strlen(argv[1]));
//	string path_output = string(argv[2],strlen(argv[2]));

	string path_schema = "/home/ankifor/Documents/CPP/DB_data/schema1.sql";
	string path_output = "/home/ankifor/Documents/CPP/DB_data/q.cpp";


	Parser_Schema p(path_schema);
	try {
		unique_ptr<Schema> schema = p.parse();

		ofstream out;
		out.open(path_output);
		out << create_query(schema.get());
		out.close();

	} catch (ParserError& e) {
		cerr << e.what() << endl;
	}
	return 0;
}

int main(int argc, char* argv[]) {
	if (argc < 7) {
		cerr << "usage: " << argv[0]
			 << " <table size> <max field 1> <max field 2> <max field 3> <times> <method1> <method2> ... <methodk>"
		     << endl
		     << argc << endl;
		return -1;
	}

	string path_data = string(argv[1],strlen(argv[1])) + "/";

//	cout << "reading db from '" << path_data << "'..." << endl;
//	read_data(path_data);
	long db_count = stol(argv[1]);
	long db_field1 = stol(argv[2]);
	long db_field2 = stol(argv[3]);
	long db_field3 = stol(argv[4]);
	int n = stol(argv[5]);

	vector<string> methods;
	for (int i = 6; i < argc; ++i) {
		methods.push_back(string(argv[i],strlen(argv[i])));
	}


	populate_order(db_count, db_field1, db_field2, db_field3);
	string run_info;

	{
		stringstream tmp;
		tmp << db_count << "," << db_field1 << "," << db_field2 << "," << db_field3;
		run_info = tmp.str();
	}

	cerr << "input: " << run_info << endl;

	ofstream out;

	vector<pair<query_fun,string>> run_queries;
	run_queries.push_back({run_query_dummy,"none"});
	run_queries.push_back({run_query0,""});
	run_queries.push_back({run_query1,""});
	run_queries.push_back({run_query2,"cube_unordMap"});
	run_queries.push_back({run_query3,"cube_unordMap_union"});
	run_queries.push_back({run_query4,"cube_myHash"});
	run_queries.push_back({run_query5,"cube_myHash_sameKey"});
	run_queries.push_back({run_query6,"cube_myHash_reusage"});
	run_queries.push_back({run_query7,"cube_myHash_reusage_depth"});
	run_queries.push_back({run_query8,"cube_final"});
	run_queries.push_back({run_query_rollup_1,"rollup_myHash"});
	run_queries.push_back({run_query_rollup_2,"rollup_myHash_reusage"});
	run_queries.push_back({run_query_rollup_3,"rollup_union"});
	run_queries.push_back({run_query_rollup_4,"rollup_final"});
	run_queries.push_back({olap_query_001,"olap_query_001"});

	vector<chrono::milliseconds> elapsed;
	elapsed.resize(run_queries.size());

	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;

	for (auto& x : elapsed) {
		x = chrono::milliseconds::zero();
	}


	ofstream devnull;
	devnull.open("/dev/null");


	vector<size_t> methods_ind;
	for (const auto& method : methods) {
		for (size_t i = 0; i < run_queries.size(); ++i) {
			if (run_queries[i].second == method) {
				methods_ind.push_back(i);
				break;
			}
		}
	}


//	{int i; cout << "..."; cin >> i; cout << endl;}
	try {
		if (n<=0) throw runtime_error("negative number of runs");
		auto coutbuf = cout.rdbuf(devnull.rdbuf());
		for (int i=0;i<n;++i) {
			//for (size_t cnt = 0; cnt < run_queries.size(); ++cnt)
			for (auto cnt : methods_ind)
			{
				start = chrono::high_resolution_clock::now();
				run_queries[cnt].first(db);
				end = chrono::high_resolution_clock::now();
				cout.flush();
				if (elapsed[cnt] != chrono::milliseconds::zero()) {
					elapsed[cnt] = min(elapsed[cnt] ,chrono::duration_cast<chrono::milliseconds>(end - start));
				} else {
					elapsed[cnt] = chrono::duration_cast<chrono::milliseconds>(end - start);
				}
			}
		}
		cout.rdbuf(coutbuf);

//		for (size_t cnt = 0; cnt < run_queries.size(); ++cnt)
		for (auto cnt : methods_ind)
		{
			cout
				<< run_info << ","
				<< run_queries[cnt].second << ","
				<< elapsed[cnt].count() << endl;
		}

	} catch (runtime_error& e) {
		cerr << e.what() << endl;
		exit(1);
	}
//	{int i; cout << "..."; cin >> i; cout << endl;}

	devnull.close();
	return 0;
}


int main3(int argc, char* argv[]) {
//	if (argc != 5) {
//		cerr << "usage: " << argv[0]
//		     << " <schema file>"
//		     << " <source folder>"
//		     << " <object folder>"
//			 << " <data dir>"
//		     << endl
//		     << argc << endl;
//		return -1;
//	}
//
//	string schema_path = string(argv[1],strlen(argv[1]));
//	string source_path = string(argv[2],strlen(argv[2])) + "/";
//	string object_path = string(argv[3],strlen(argv[3])) + "/";
//	string path_data = string(argv[4],strlen(argv[4])) + "/";

	string schema_path = "/home/ankifor/Documents/CPP/DB_data/schema1.sql";
	string source_path = "/home/ankifor/workspace/DBI-Project/runtime/";
	string object_path = "/home/ankifor/workspace/DBI-Project/runtime/";
	string path_data = "/home/ankifor/Documents/CPP/DB_data/";


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
	string o_types = object_path + "Types.o";
	string o_schema = object_path + "schema_1.o";
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
			fflush(stderr);
			cout << "#";
			//parse query
			Parser_Query q(cin, schema.get(), "db");
			if (!q.parse()) break;
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
			__fpurge(stdin);
			cerr << e.what() << endl;
			continue;
		} catch (runtime_error& e) {
			cerr << e.what() << endl;
			continue;
		}

	}
	return 0;
}

int main4(int argc, char* argv[]) {
	read_data("/home/ankifor/Documents/CPP/DB_data/");
	//run_query(db);
	return 0;
}
