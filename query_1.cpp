#include "Types.hpp"
#include "schema_1.hpp"
#include <iostream>
#include <unordered_map>
using namespace std;
bool predicat0(const Varchar<16>& c_last){return c_last=="BARBARBAR";}extern "C" void run_query(const Database& db) {
for (Tid tid0 = 0;tid0 < db.customer.size(); ++tid0){if (predicat0(db.customer.c_last[tid0])){cout<<db.customer.c_id[tid0]<<","<<db.customer.c_first[tid0]<<","<<db.customer.c_middle[tid0]<<","<<db.customer.c_last[tid0]<<endl;}}}