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

extern "C" void run_query0(const Database &db);
extern "C" void run_query1(const Database &db);
extern "C" void run_query2(const Database &db);
extern "C" void run_query3(const Database &db);
extern "C" void run_query4(const Database &db);
extern "C" void run_query5(const Database &db);
extern "C" void run_query6(const Database &db);

Name_Generator name_generator;

struct Database db;

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
	if (argc != 2) {
		cerr << "usage: " << argv[0]
			 << " <data dir>"
		     << endl
		     << argc << endl;
		return -1;
	}

	string path_data = string(argv[1],strlen(argv[1])) + "/";

	cout << "reading db from '" << path_data << "'..." << endl;
	read_data(path_data);

	ofstream out;
	chrono::duration<int64_t, std::milli> elapsed0;
	chrono::duration<int64_t, std::milli> elapsed1;
	chrono::duration<int64_t, std::milli> elapsed2;
	chrono::duration<int64_t, std::milli> elapsed3;
	chrono::duration<int64_t, std::milli> elapsed4;
	chrono::duration<int64_t, std::milli> elapsed5;
	chrono::duration<int64_t, std::milli> elapsed6;

	chrono::high_resolution_clock::time_point start;
	chrono::high_resolution_clock::time_point end;

	elapsed0 = chrono::duration_cast<chrono::milliseconds>(end - end);
	elapsed1 = chrono::duration_cast<chrono::milliseconds>(end - end);
	elapsed2 = chrono::duration_cast<chrono::milliseconds>(end - end);
	elapsed3 = chrono::duration_cast<chrono::milliseconds>(end - end);
	elapsed4 = chrono::duration_cast<chrono::milliseconds>(end - end);
	elapsed5 = chrono::duration_cast<chrono::milliseconds>(end - end);
	elapsed6 = chrono::duration_cast<chrono::milliseconds>(end - end);

	int n = 10;
	ofstream devnull;
	devnull.open("/dev/null");
	{int i; cout << "..."; cin >> i; cout << endl;}
	try {
		//cin >> n;
		if (n<=0) throw runtime_error("negative number of runs");
		for (int i=0;i<n;++i) {
			auto coutbuf = cout.rdbuf(devnull.rdbuf());

			//start = chrono::high_resolution_clock::now();
			//run_query0(db);
			//end = chrono::high_resolution_clock::now();
			//elapsed0 += chrono::duration_cast<chrono::milliseconds>(end - start);
			//
			//start = chrono::high_resolution_clock::now();
			//run_query1(db);
			//end = chrono::high_resolution_clock::now();
			//elapsed1 += chrono::duration_cast<chrono::milliseconds>(end - start);

			start = chrono::high_resolution_clock::now();
			run_query2(db);
			end = chrono::high_resolution_clock::now();
			elapsed2 += chrono::duration_cast<chrono::milliseconds>(end - start);
			//
			//start = chrono::high_resolution_clock::now();
			//run_query3(db);
			//end = chrono::high_resolution_clock::now();
			//elapsed3 += chrono::duration_cast<chrono::milliseconds>(end - start);

			start = chrono::high_resolution_clock::now();
			run_query4(db);
			end = chrono::high_resolution_clock::now();
			elapsed4 += chrono::duration_cast<chrono::milliseconds>(end - start);

			start = chrono::high_resolution_clock::now();
			run_query5(db);
			end = chrono::high_resolution_clock::now();
			elapsed5 += chrono::duration_cast<chrono::milliseconds>(end - start);

			start = chrono::high_resolution_clock::now();
			run_query6(db);
			end = chrono::high_resolution_clock::now();
			elapsed6 += chrono::duration_cast<chrono::milliseconds>(end - start);

			cout.rdbuf(coutbuf);
			//cerr << elapsed.count() << "ms" << endl;
		}

		cerr << elapsed0.count() / n << "ms avg0" << endl;
		cerr << elapsed1.count() / n << "ms avg1" << endl;
		cerr << elapsed2.count() / n << "ms avg2" << endl;
		cerr << elapsed3.count() / n << "ms avg3" << endl;
		cerr << elapsed4.count() / n << "ms avg4" << endl;
		cerr << elapsed5.count() / n << "ms avg5" << endl;
		cerr << elapsed6.count() / n << "ms avg6" << endl;


	} catch (runtime_error& e) {
		cerr << e.what() << endl;
		exit(1);
	}
	{int i; cout << "..."; cin >> i; cout << endl;}

	devnull.close();
	return 0;
}
