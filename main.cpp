#include <iostream>
#include <memory>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <dlfcn.h>
#include <regex>
#include <vector>
#include <chrono>
#include "Schema.hpp"
#include "Parser_Schema.hpp"
#include "Parser_Query.h"
#include "code_generation.h"
#include "schema_1.hpp"

using namespace std;


using query_fun = void (*)(const Database &db);

static void run_query_dummy(const Database &db) {}
extern "C" void run_query0(const Database &db);
extern "C" void run_query1(const Database &db);
extern "C" void run_query2(const Database &db);
extern "C" void run_query3(const Database &db);
extern "C" void run_query4(const Database &db);
extern "C" void run_query5(const Database &db);
extern "C" void run_query6(const Database &db);
extern "C" void run_query_rollup_1(const Database &db);
extern "C" void run_query_rollup_2(const Database &db);




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

//	in.open(path + "tpcc_warehouse.tbl");
//	db.warehouse.read_from_file(in);
//	in.close();

//	in.open(path + "tpcc_district.tbl");
//	db.district.read_from_file(in);
//	in.close();
//
//	in.open(path + "tpcc_customer.tbl");
//	db.customer.read_from_file(in);
//	in.close();
//
//	in.open(path + "tpcc_history.tbl");
//	db.history.read_from_file(in);
//	in.close();

	in.open(path + "tpcc_order_1.tbl");
	db.order.read_from_file(in);
	in.close();

//	in.open(path + "tpcc_neworder.tbl");
//	db.neworder.read_from_file(in);
//	in.close();
//
//	in.open(path + "tpcc_orderline.tbl");
//	db.orderline.read_from_file(in);
//	in.close();
//
//	in.open(path + "tpcc_item.tbl");
//	db.item.read_from_file(in);
//	in.close();
//
//	in.open(path + "tpcc_stock.tbl");
//	db.stock.read_from_file(in);
//	in.close();
}

int main(int argc, char* argv[]) {
	if (argc != 5) {
		cerr << "usage: " << argv[0]
			 << " <table size> <max field 1> <max field 2> <max field 3>"
		     << endl
		     << argc << endl;
		return -1;
	}

	string path_data = string(argv[1],strlen(argv[1])) + "/";

//	cout << "reading db from '" << path_data << "'..." << endl;
//	read_data(path_data);
	cout << "populating db..." << endl;
	populate_order(stol(argv[1]), stol(argv[2]), stol(argv[3]), stol(argv[4]));

	cerr << "db.order: " << db.order.size() << endl;

	ofstream out;

	vector<query_fun> run_queries;
//	run_queries.push_back(run_query_dummy);
//	run_queries.push_back(run_query_dummy);
//	run_queries.push_back(run_query2);
//	run_queries.push_back(run_query_dummy);
//	run_queries.push_back(run_query4);
//	run_queries.push_back(run_query5);
//	run_queries.push_back(run_query6);
	run_queries.push_back(run_query_rollup_1);
	run_queries.push_back(run_query_rollup_2);

	vector<chrono::milliseconds> elapsed;
	elapsed.resize(run_queries.size());

	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;

	for (auto& x : elapsed) {
		x = chrono::milliseconds::zero();
	}

	int n = 10;
	ofstream devnull;
	devnull.open("/dev/null");
	{int i; cout << "..."; cin >> i; cout << endl;}
	try {
		if (n<=0) throw runtime_error("negative number of runs");
		auto coutbuf = cout.rdbuf(devnull.rdbuf());
		for (int i=0;i<n;++i) {
			for (size_t cnt = 0; cnt < run_queries.size(); ++cnt) {
				if (run_queries[cnt] != run_query_dummy) {
					start = chrono::high_resolution_clock::now();
					run_queries[cnt](db);
					end = chrono::high_resolution_clock::now();
					elapsed[cnt] += chrono::duration_cast<chrono::milliseconds>(end - start);
				}
			}
		}
		cout.rdbuf(coutbuf);

		for (size_t cnt = 0; cnt < run_queries.size(); ++cnt) {
			if (run_queries[cnt] != run_query_dummy) {
				cerr << cnt << ": " << elapsed[cnt].count() / n << "ms" << endl;
			}
		}

	} catch (runtime_error& e) {
		cerr << e.what() << endl;
		exit(1);
	}
	{int i; cout << "..."; cin >> i; cout << endl;}

	devnull.close();
	return 0;
}
