#include "Types.hpp"
#include "schema_1.hpp"
#include "Help.hpp"
#include <iostream>
#include <unordered_map>
#include <tuple>

#include "my_hash.h"
//#include "my_hash_D.h"
using namespace std;

#define My_Hash_U(_Key, _Tp) My_Hash<_Key, _Tp, hash_types::hash<_Key>, equal_to<_Key>, true, UpdateVal>

template<bool f1,bool f2,bool f3>
struct myEq {
	bool operator()(tuple<Integer,Integer,Integer>& left, tuple<Integer,Integer,Integer>& right) {
		return
			   (!f1 || get<0>(left)==get<0>(right))
			&& (!f2 || get<1>(left)==get<1>(right))
			&& (!f3 || get<2>(left)==get<2>(right))
		;
	}
};

//is generated
template<bool f1,bool f2,bool f3,typename IndexType>
void my_print_hash(IndexType* index) {
	const int n1 = 0;
	const int n2 = n1 + (f1&&f2?1:0);
	const int n3 = n2 + (f2&&f3?1:0);

	for (auto it = index->begin(); it != index->end(); ++it) {
		if (f1) cout << get<n1>(get<0>(*it)); else cout << "null";
		cout << ";" ;
		if (f2) cout << get<n2>(get<0>(*it)); else cout << "null";
		cout << ";" ;
		if (f3) cout << get<n3>(get<0>(*it)); else cout << "null";
		cout << ";" ;
		cout << get<0>(get<1>(*it));
		cout << endl;
	}
}

template<bool f1,bool f2,bool f3,typename IndexType>
void my_print_hash1(IndexType* index) {
	for (auto it = index->begin(); it != index->end(); ++it) {
		if (f1) cout << get<0>(get<0>(*it)); else cout << "null";
		cout << ";" ;
		if (f2) cout << get<1>(get<0>(*it)); else cout << "null";
		cout << ";" ;
		if (f3) cout << get<2>(get<0>(*it)); else cout << "null";
		cout << ";" ;
		cout << get<0>(get<1>(*it));
		cout << endl;
	}
}

template<>
void my_print_hash<false,false,false, tuple<Numeric<2,0>>>(tuple<Numeric<2,0>>* val) {
	cout << "null"
		 << ";" << "null"
		 << ";" << "null"
		 << ";" << get<0>(*val)
		 << endl;
}

void update_val(tuple<Numeric<2,0>>& left, const tuple<Numeric<2,0>>&  right) {
	get<0>(left) += get<0>(right);
}

template<typename IndexType, typename key_type, typename mapped_type>
void my_update_hash(IndexType& index, key_type&& key,  mapped_type&& val) {
	auto it = index.find(key);
	if (it == index.end()) {
		index.insert(make_pair(key, val));
	} else {
		update_val(get<1>(*it), val);
	}
}


struct UpdateVal {
	void operator()(tuple<Numeric<2,0>>& left, const tuple<Numeric<2,0>>&  right) {
		get<0>(left) += get<0>(right);
	}
};

//void UpdateVal1(tuple<Numeric<2,0>>& left, const tuple<Numeric<2,0>>&  right) {
//	get<0>(left) += get<0>(right);
//}

//bool predicat0(const Varchar<16> &c_last) { return c_last == "BARBARBAR"; }

extern "C" void run_query0(const Database &db) {
	using type_key_111 = tuple<Integer,Integer,Integer>;
	using type_val_111 = tuple<Numeric<2,0>>;
	unordered_map<type_key_111, type_val_111, hash_types::hash<type_key_111>> hash_111;

	using type_key_110 = tuple<Integer,Integer>;
	using type_val_110 = tuple<Numeric<2,0>>;

	std::unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_110;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_101;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_011;

	using type_key_100 = tuple<Integer>;
	using type_val_100 = tuple<Numeric<2,0>>;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_100;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_010;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_001;

	using type_val_000 = tuple<Numeric<2,0>>;
	type_val_000 val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		auto it = hash_111.find(make_tuple(
			 db.order.o_d_id[tid0]
			,db.order.o_w_id[tid0]
			,db.order.o_carrier_id[tid0]
		));
		//construct hash_111
		if (it == hash_111.end()) {
			hash_111.insert(make_pair(
				make_tuple(
					 db.order.o_d_id[tid0]
					,db.order.o_w_id[tid0]
					,db.order.o_carrier_id[tid0])
				,make_tuple(
					db.order.o_ol_cnt[tid0])
			));
		} else {
			update_val(it->second, make_tuple(db.order.o_ol_cnt[tid0]));
		}
	}

	//int dummy = 0;
	//read hash_111 & print & construct hash_110
	for (auto it_111 = hash_111.begin(); it_111 != hash_111.end(); ++it_111) {
		cout <<        get<0>(it_111->first)
			 << ";" << get<1>(it_111->first)
			 << ";" << get<2>(it_111->first)
			 << ";" << get<0>(it_111->second)
			 << endl;
		auto it = hash_110.find(make_tuple(
			 get<0>(it_111->first)
			,get<1>(it_111->first)
		));
		if (it == hash_110.end()) {
			hash_110.insert(make_pair(
				make_tuple(
					 get<0>(it_111->first)
					,get<1>(it_111->first))
				,make_tuple(
					get<0>(it_111->second))
			));
		} else {
			get<0>(it->second) += get<0>(it_111->second);
		}
	}

	//read hash_111 & construct hash_101
	for (auto it_111 = hash_111.begin(); it_111 != hash_111.end(); ++it_111) {
		auto it = hash_101.find(make_tuple(
			 get<0>(it_111->first)
			,get<2>(it_111->first)
		));
		if (it == hash_101.end()) {
			hash_101.insert(make_pair(
				make_tuple(
					 get<0>(it_111->first)
					,get<2>(it_111->first))
				,make_tuple(
					get<0>(it_111->second))
			));
		} else {
			get<0>(it->second) += get<0>(it_111->second);
		}
	}

	//read hash_111 & construct hash_011
	for (auto it_111 = hash_111.begin(); it_111 != hash_111.end(); ++it_111) {
		auto it = hash_011.find(make_tuple(
			 get<1>(it_111->first)
			,get<2>(it_111->first)
		));
		if (it == hash_011.end()) {
			hash_011.insert(make_pair(
				make_tuple(
					 get<1>(it_111->first)
					,get<2>(it_111->first))
				,make_tuple(
					get<0>(it_111->second))
			));
		} else {
			get<0>(it->second) += get<0>(it_111->second);
		}
	}

	//read hash_110 & print & construct hash_100
	for (auto it_110 = hash_110.begin(); it_110 != hash_110.end(); ++it_110) {
		cout << get<0>(it_110->first)
			 << ";" << get<1>(it_110->first)
			 << ";" << "null"
			 << ";" << get<0>(it_110->second)
			 << endl;

		auto it = hash_100.find(make_tuple(
			 get<0>(it_110->first)
		));
		if (it == hash_100.end()) {
			hash_100.insert(make_pair(
				make_tuple(
					 get<0>(it_110->first))
				,make_tuple(
					get<0>(it_110->second))
			));
		} else {
			get<0>(it->second) += get<0>(it_110->second);
		}
	}

	//read hash_110 & construct hash_010
	for (auto it_110 = hash_110.begin(); it_110 != hash_110.end(); ++it_110) {
		auto it = hash_010.find(make_tuple(
			 get<1>(it_110->first)
		));
		if (it == hash_010.end()) {
			hash_010.insert(make_pair(
				make_tuple(
					 get<1>(it_110->first))
				,make_tuple(
					get<0>(it_110->second))
			));
		} else {
			get<0>(it->second) += get<0>(it_110->second);
		}
	}

	//read hash_101 & print & construct hash_001
	for (auto it_101 = hash_101.begin(); it_101 != hash_101.end(); ++it_101) {
		cout << get<0>(it_101->first)
			 << ";" << "null"
			 << ";" << get<1>(it_101->first)
			 << ";" << get<0>(it_101->second)
			 << endl;

		auto it = hash_001.find(make_tuple(
			 get<1>(it_101->first)
		));
		if (it == hash_001.end()) {
			hash_001.insert(make_pair(
				make_tuple(
					 get<1>(it_101->first))
				,make_tuple(
					get<0>(it_101->second))
			));
		} else {
			get<0>(it->second) += get<0>(it_101->second);
		}
	}

	//read hash_011 & print
	for (auto it_011 = hash_011.begin(); it_011 != hash_011.end(); ++it_011) {
		cout << "null"
			 << ";" << get<0>(it_011->first)
			 << ";" << get<1>(it_011->first)
			 << ";" << get<0>(it_011->second)
			 << endl;
	}

	//read hash_100 & print & construct val_000
	for (auto it_100 = hash_100.begin(); it_100 != hash_100.end(); ++it_100) {
		cout << get<0>(it_100->first)
			 << ";" << "null"
			 << ";" << "null"
			 << ";" << get<0>(it_100->second)
			 << endl;

		get<0>(val_000) += get<0>(it_100->second);
	}

	//read hash_010 & print
	for (auto it_010 = hash_010.begin(); it_010 != hash_010.end(); ++it_010) {
		cout << "null"
			 << ";" << get<0>(it_010->first)
			 << ";" << "null"
			 << ";" << get<0>(it_010->second)
			 << endl;
	}

	//read hash_001 & print
	for (auto it_001 = hash_001.begin(); it_001 != hash_001.end(); ++it_001) {
		cout << "null"
			 << ";" << "null"
			 << ";" << get<0>(it_001->first)
			 << ";" << get<0>(it_001->second)
			 << endl;
	}

	//read val_000 & print
	cout << "null"
		 << ";" << "null"
		 << ";" << "null"
		 << ";" << get<0>(val_000)
		 << endl;
}


//bucket
extern "C" void run_query1(const Database &db) {
	using type_key_111 = tuple<Integer,Integer,Integer>;
	using type_val_111 = tuple<Numeric<2,0>>;
	unordered_map<type_key_111, type_val_111, hash_types::hash<type_key_111>> hash_111;

	using type_key_110 = tuple<Integer,Integer>;
	using type_val_110 = tuple<Numeric<2,0>>;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_110;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_101;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_011;

	using type_key_100 = tuple<Integer>;
	using type_val_100 = tuple<Numeric<2,0>>;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_100;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_010;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_001;

	using type_val_000 = tuple<Numeric<2,0>>;
	type_val_000 val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		//construct hash_111
		{
			auto it = hash_111.find(make_tuple(
				 db.order.o_d_id[tid0]
				,db.order.o_w_id[tid0]
				,db.order.o_carrier_id[tid0]
			));
			if (it == hash_111.end()) {
				hash_111.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0]
						,db.order.o_w_id[tid0]
						,db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
		//construct hash_110
		{
			auto it = hash_110.find(make_tuple(
				 db.order.o_d_id[tid0]
				,db.order.o_w_id[tid0]
			));
			if (it == hash_110.end()) {
				hash_110.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0]
						,db.order.o_w_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
		//construct hash_101
		{
			auto it = hash_101.find(make_tuple(
				 db.order.o_d_id[tid0]
				,db.order.o_carrier_id[tid0]
			));
			if (it == hash_101.end()) {
				hash_101.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0]
						,db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
		//construct hash_011
		{
			auto it = hash_011.find(make_tuple(
				 db.order.o_w_id[tid0]
				,db.order.o_carrier_id[tid0]
			));
			if (it == hash_011.end()) {
				hash_011.insert(make_pair(
					make_tuple(
						 db.order.o_w_id[tid0]
						,db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
		//construct hash_100
		{
			auto it = hash_100.find(make_tuple(
				 db.order.o_d_id[tid0]
			));
			if (it == hash_100.end()) {
				hash_100.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
		//construct hash_010
		{
			auto it = hash_010.find(make_tuple(
				db.order.o_w_id[tid0]
			));
			if (it == hash_010.end()) {
				hash_010.insert(make_pair(
					make_tuple(db.order.o_w_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
		//construct hash_001
		{
			auto it = hash_001.find(make_tuple(
				db.order.o_carrier_id[tid0]
			));
			if (it == hash_001.end()) {
				hash_001.insert(make_pair(
					make_tuple(
						db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
		//construct val_000
		{
			get<0>(val_000) += db.order.o_ol_cnt[tid0];
		}
	}

	my_print_hash<true,true,true>(&hash_111);
	my_print_hash<true,true,false>(&hash_110);
	my_print_hash<true,false,true>(&hash_101);
	my_print_hash<false,true,true>(&hash_011);
	my_print_hash<true,false,false>(&hash_100);
	my_print_hash<false,true,false>(&hash_010);
	my_print_hash<false,false,true>(&hash_001);
	my_print_hash<false,false,false>(&val_000);
}


//normal
extern "C" void run_query2(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;

	using type_key_111 = tuple<Integer,Integer,Integer>;
	unordered_map<type_key_111, type_val, hash_types::hash<type_key_111>> hash_111;

	using type_key_110 = tuple<Integer,Integer>;
	unordered_map<type_key_110, type_val, hash_types::hash<type_key_110>> hash_110;
	unordered_map<type_key_110, type_val, hash_types::hash<type_key_110>> hash_101;
	unordered_map<type_key_110, type_val, hash_types::hash<type_key_110>> hash_011;

	using type_key_100 = tuple<Integer>;
	unordered_map<type_key_100, type_val, hash_types::hash<type_key_100>> hash_100;
	unordered_map<type_key_100, type_val, hash_types::hash<type_key_100>> hash_010;
	unordered_map<type_key_100, type_val, hash_types::hash<type_key_100>> hash_001;

	type_val val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		my_update_hash(hash_111
			, make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}

	my_print_hash<true,true,true>(&hash_111);

	//construct hash_110 from hash_111
	for (auto it = hash_111.begin(); it != hash_111.end(); ++it) {
		my_update_hash(hash_110
			, make_tuple(get<0>(it->first),get<1>(it->first))
			, it->second
		);
	}

	//construct hash_101 from hash_111
	for (auto it = hash_111.begin(); it != hash_111.end(); ++it) {
		my_update_hash(hash_101, make_tuple(get<0>(it->first),get<2>(it->first)), it->second);
	}

	//construct hash_011 from hash_111
	for (auto it = hash_111.begin(); it != hash_111.end(); ++it) {
		my_update_hash(hash_011, make_tuple(get<1>(it->first),get<2>(it->first)), it->second);
	}
	hash_111.clear();

	my_print_hash<true,true,false>(&hash_110);

	//construct hash_100 from hash_110
	for (auto it = hash_110.begin(); it != hash_110.end(); ++it) {
		my_update_hash(hash_100, make_tuple(get<0>(it->first)), it->second);
	}

	//construct hash_010 from hash_110
	for (auto it = hash_110.begin(); it != hash_110.end(); ++it) {
		my_update_hash(hash_010, make_tuple(get<1>(it->first)), it->second);
	}
	hash_110.clear();

	my_print_hash<true,false,true>(&hash_101);

	//construct hash_001 from hash_101
	for (auto it = hash_101.begin(); it != hash_101.end(); ++it) {
		my_update_hash(hash_001, make_tuple(get<1>(it->first)), it->second);
	}

	hash_101.clear();
	my_print_hash<false,true,true>(&hash_011);
	hash_011.clear();
	my_print_hash<true,false,false>(&hash_100);
	//construct val_000 from hash_100
	for (auto it = hash_100.begin(); it != hash_100.end(); ++it) {
		update_val(val_000, it->second);
	}
	hash_100.clear();

	my_print_hash<false,true,false>(&hash_010);
	hash_010.clear();
	my_print_hash<false,false,true>(&hash_001);
	hash_001.clear();
	my_print_hash<false,false,false>(&val_000);
}


//union
extern "C" void run_query3(const Database &db) {
	using type_key_111 = tuple<Integer,Integer,Integer>;
	using type_val_111 = tuple<Numeric<2,0>>;
	unordered_map<type_key_111, type_val_111, hash_types::hash<type_key_111>> hash_111;

	using type_key_110 = tuple<Integer,Integer>;
	using type_val_110 = tuple<Numeric<2,0>>;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_110;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_101;
	unordered_map<type_key_110, type_val_110, hash_types::hash<type_key_110>> hash_011;

	using type_key_100 = tuple<Integer>;
	using type_val_100 = tuple<Numeric<2,0>>;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_100;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_010;
	unordered_map<type_key_100, type_val_100, hash_types::hash<type_key_100>> hash_001;

	using type_val_000 = tuple<Numeric<2,0>>;
	type_val_000 val_000;

	//scan
	//construct hash_111
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			auto it = hash_111.find(make_tuple(
				 db.order.o_d_id[tid0]
				,db.order.o_w_id[tid0]
				,db.order.o_carrier_id[tid0]
			));
			if (it == hash_111.end()) {
				hash_111.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0]
						,db.order.o_w_id[tid0]
						,db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
	my_print_hash<true,true,true>(&hash_111);
	hash_111.clear();

	//construct hash_110
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			auto it = hash_110.find(make_tuple(
				 db.order.o_d_id[tid0]
				,db.order.o_w_id[tid0]
			));
			if (it == hash_110.end()) {
				hash_110.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0]
						,db.order.o_w_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
	my_print_hash<true,true,false>(&hash_110);
	hash_110.clear();

	//construct hash_101
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			auto it = hash_101.find(make_tuple(
				 db.order.o_d_id[tid0]
				,db.order.o_carrier_id[tid0]
			));
			if (it == hash_101.end()) {
				hash_101.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0]
						,db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
	my_print_hash<true,false,true>(&hash_101);
	hash_101.clear();
	//construct hash_011
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			auto it = hash_011.find(make_tuple(
				 db.order.o_w_id[tid0]
				,db.order.o_carrier_id[tid0]
			));
			if (it == hash_011.end()) {
				hash_011.insert(make_pair(
					make_tuple(
						 db.order.o_w_id[tid0]
						,db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
	my_print_hash<false,true,true>(&hash_011);
	hash_011.clear();
	//construct hash_100
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			auto it = hash_100.find(make_tuple(
				 db.order.o_d_id[tid0]
			));
			if (it == hash_100.end()) {
				hash_100.insert(make_pair(
					make_tuple(
						 db.order.o_d_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
	my_print_hash<true,false,false>(&hash_100);
	hash_100.clear();
	//construct hash_010
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			auto it = hash_010.find(make_tuple(
				db.order.o_w_id[tid0]
			));
			if (it == hash_010.end()) {
				hash_010.insert(make_pair(
					make_tuple(db.order.o_w_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
	my_print_hash<false,true,false>(&hash_010);
	hash_010.clear();
	//construct hash_001
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			auto it = hash_001.find(make_tuple(
				db.order.o_carrier_id[tid0]
			));
			if (it == hash_001.end()) {
				hash_001.insert(make_pair(
					make_tuple(
						db.order.o_carrier_id[tid0])
					,make_tuple(
						db.order.o_ol_cnt[tid0])
				));
			} else {
				get<0>(it->second) += db.order.o_ol_cnt[tid0];
			}
		}
	my_print_hash<false,false,true>(&hash_001);
	hash_001.clear();
	//construct val_000
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
		{
			get<0>(val_000) += db.order.o_ol_cnt[tid0];
		}
	my_print_hash<false,false,false>(&val_000);

}

//2 with new hash
extern "C" void run_query4(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;

	using type_key_111 = tuple<Integer,Integer,Integer>;
	My_Hash_U(type_key_111, type_val) hash_111(nullptr);


	using type_key_110 = tuple<Integer,Integer>;
	My_Hash_U(type_key_110, type_val) hash_110(nullptr);
	My_Hash_U(type_key_110, type_val) hash_101(nullptr);
	My_Hash_U(type_key_110, type_val) hash_011(nullptr);

	using type_key_100 = tuple<Integer>;
	My_Hash_U(type_key_100, type_val) hash_100(nullptr);
	My_Hash_U(type_key_100, type_val) hash_010(nullptr);
	My_Hash_U(type_key_100, type_val) hash_001(nullptr);

	type_val val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}

	my_print_hash<true,true,true>(hash_111._storage.get());

	//construct hash_110 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_110.modify(
			make_tuple(get<0>(get<0>(*it)),get<1>(get<0>(*it)))
			, get<1>(*it)
		);
	}

	//construct hash_101 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_101.modify(
			make_tuple(get<0>(get<0>(*it)),get<2>(get<0>(*it)))
			, get<1>(*it)
		);
	}

	//construct hash_011 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_011.modify(
			make_tuple(get<1>(get<0>(*it)),get<2>(get<0>(*it)))
			, get<1>(*it)
		);
	}
	hash_111.clear();

	my_print_hash<true,true,false>(hash_110._storage.get());

	//construct hash_100 from hash_110
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_100.modify(
			make_tuple(get<0>(get<0>(*it)))
			, get<1>(*it)
		);
	}

	//construct hash_010 from hash_110
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_010.modify(
			make_tuple(get<1>(get<0>(*it)))
			, get<1>(*it)
		);
	}
	hash_110.clear();

	my_print_hash<true,false,true>(hash_101._storage.get());

	//construct hash_001 from hash_101
	for (auto it = hash_101._storage->begin(); it != hash_101._storage->end(); ++it) {
		hash_001.modify(
			make_tuple(get<1>(get<0>(*it)))
			, get<1>(*it)
		);
	}

	hash_101.clear();
	my_print_hash<false,true,true>(hash_011._storage.get());
	hash_011.clear();
	my_print_hash<true,false,false>(hash_100._storage.get());
	//construct val_000 from hash_100
	for (auto it = hash_100._storage->begin(); it != hash_100._storage->end(); ++it) {
		update_val(val_000, get<1>(*it));
	}
	hash_100.clear();

	my_print_hash<false,true,false>(hash_010._storage.get());
	hash_010.clear();
	my_print_hash<false,false,true>(hash_001._storage.get());
	hash_001.clear();
	my_print_hash<false,false,false>(&val_000);
}

//4 with same key
extern "C" void run_query5(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;
	using type_key = tuple<Integer,Integer,Integer>;
	using h_111 = hash_types_1::hash<type_key, 7>;
	using h_110 = hash_types_1::hash<type_key, 3>;
	using h_101 = hash_types_1::hash<type_key, 5>;
	using h_011 = hash_types_1::hash<type_key, 6>;
	using h_100 = hash_types_1::hash<type_key, 1>;
	using h_010 = hash_types_1::hash<type_key, 2>;
	using h_001 = hash_types_1::hash<type_key, 4>;

	My_Hash<type_key, type_val, h_111, myEq<true,true,true>  , true, UpdateVal> hash_111(nullptr);
	My_Hash<type_key, type_val, h_110, myEq<true,true,false> , true, UpdateVal> hash_110(nullptr);
	My_Hash<type_key, type_val, h_101, myEq<true,false,true> , true, UpdateVal> hash_101(nullptr);
	My_Hash<type_key, type_val, h_011, myEq<false,true,true> , true, UpdateVal> hash_011(nullptr);
	My_Hash<type_key, type_val, h_100, myEq<true,false,false>, true, UpdateVal> hash_100(nullptr);
	My_Hash<type_key, type_val, h_010, myEq<false,true,false>, true, UpdateVal> hash_010(nullptr);
	My_Hash<type_key, type_val, h_001, myEq<false,false,true>, true, UpdateVal> hash_001(nullptr);

	type_val val_000;

	//111
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}
	my_print_hash1<true,true,true>(hash_111._storage.get());
	//110
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_110.modify(get<0>(*it), get<1>(*it));
	}
	my_print_hash1<true,true,false>(hash_110._storage.get());
	//101
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_101.modify(get<0>(*it), get<1>(*it));
	}
	my_print_hash1<true,false,true>(hash_101._storage.get());
	//011
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_011.modify(get<0>(*it), get<1>(*it));
	}
	my_print_hash1<false,true,true>(hash_011._storage.get());
	hash_011.clear();
	hash_111.clear();

	//100
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_100.modify(get<0>(*it), get<1>(*it));
	}
	my_print_hash1<true,false,false>(hash_100._storage.get());
	//010
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_010.modify(get<0>(*it), get<1>(*it));
	}
	my_print_hash1<false,true,false>(hash_010._storage.get());
	hash_010.clear();
	hash_110.clear();
	//001
	for (auto it = hash_101._storage->begin(); it != hash_101._storage->end(); ++it) {
		hash_001.modify(get<0>(*it), get<1>(*it));
	}
	my_print_hash1<false,false,true>(hash_001._storage.get());
	hash_001.clear();
	hash_101.clear();
	//construct val_000 from hash_100
	for (auto it = hash_100._storage->begin(); it != hash_100._storage->end(); ++it) {
		update_val(val_000, get<1>(*it));
	}
	my_print_hash<false,false,false>(&val_000);
	hash_100.clear();
}

//with memory reusage
extern "C" void run_query6(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;
	using type_key = tuple<Integer,Integer,Integer>;
	struct UpdateVal1 {
		void operator()(type_val& left, const type_val&  right) {
			get<0>(left) += get<0>(right);
		}
	} upd;

	//111
	struct myEq_7 {
		bool operator()(type_key& left, type_key& right) {
			return
				   (get<0>(left)==get<0>(right))
				&& (get<1>(left)==get<1>(right))
				&& (get<2>(left)==get<2>(right))
			;
		}
	};
	using h_111 = hash_types_1::hash<type_key, 7>;
	My_Hash<type_key, type_val, h_111, myEq_7, true, UpdateVal1> hash_111(nullptr);
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}
	my_print_hash1<true,true,true>(hash_111._storage.get());
	//101
	using h_101 = hash_types_1::hash<type_key, 5>;
	My_Hash<type_key, type_val, h_101, myEq<true,false,true> , true, UpdateVal1> hash_101(nullptr);
	for (const auto& it: *hash_111._storage) {
		hash_101.modify(get<0>(it), get<1>(it));
	}

	my_print_hash1<true,false,true>(hash_101._storage.get());
	//011
	using h_011 = hash_types_1::hash<type_key, 6>;
	My_Hash<type_key, type_val, h_011, myEq<false,true,true> , true, UpdateVal1> hash_011(nullptr);
	for (const auto& it: *hash_111._storage) {
		hash_011.modify(get<0>(it), get<1>(it));
	}
	my_print_hash1<false,true,true>(hash_011._storage.get());
	hash_011.clear();
	//110
	using h_110 = hash_types_1::hash<type_key, 3>;
	My_Hash<type_key, type_val, h_110, myEq<true,true,false> , true, UpdateVal1> hash_110(hash_111._storage);
	hash_110.build_from_storage<true>();
	my_print_hash1<true,true,false>(hash_110._storage.get());
	hash_111.clear();
	//001
	using h_001 = hash_types_1::hash<type_key, 4>;
	My_Hash<type_key, type_val, h_001, myEq<false,false,true>, true, UpdateVal1> hash_001(nullptr);
	for (const auto& it: *hash_101._storage) {
		hash_001.modify(get<0>(it), get<1>(it));
	}
	my_print_hash1<false,false,true>(hash_001._storage.get());
	hash_001.clear();
	hash_101.clear();
	//010
	using h_010 = hash_types_1::hash<type_key, 2>;
	My_Hash<type_key, type_val, h_010, myEq<false,true,false>, true, UpdateVal1> hash_010(nullptr);
	for (const auto& it: *hash_110._storage) {
		hash_010.modify(get<0>(it), get<1>(it));
	}
	my_print_hash1<false,true,false>(hash_010._storage.get());
	hash_010.clear();
	hash_110.clear();
	//100
	using h_100 = hash_types_1::hash<type_key, 1>;
	My_Hash<type_key, type_val, h_100, myEq<true,false,false>, true, UpdateVal1> hash_100(hash_110._storage);
	hash_100.build_from_storage<true>();
	my_print_hash1<true,false,false>(hash_100._storage.get());
	//construct val_000 from hash_100
	type_val val_000;
	for (const auto& it: *hash_100._storage) {
		upd(val_000, get<1>(it));
	}
	hash_100.clear();
	my_print_hash<false,false,false>(&val_000);
}

//with memory reusage in-depth
extern "C" void run_query7(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;
	using type_key = tuple<Integer,Integer,Integer>;
	struct UpdateVal1 {
		void operator()(type_val& left, const type_val&  right) {
			get<0>(left) += get<0>(right);
		}
	} upd;

	//7
	struct myEq_7 {
		bool operator()(type_key& left, type_key& right) {
			return
				   (get<0>(left)==get<0>(right))
				&& (get<1>(left)==get<1>(right))
				&& (get<2>(left)==get<2>(right))
			;
		}
	};
	using h_7 = hash_types_1::hash<type_key, 7>;
	My_Hash<type_key, type_val, h_7, myEq_7, true, UpdateVal1> hash_7(nullptr);
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_7.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}
	my_print_hash1<true,true,true>(hash_7._storage.get());
	//6
	using h_6 = hash_types_1::hash<type_key, 6>;
	My_Hash<type_key, type_val, h_6, myEq<false,true,true> , true, UpdateVal1> hash_6(nullptr);
	for (const auto& it: *hash_7._storage) {
		hash_6.modify(get<0>(it), get<1>(it));
	}
	my_print_hash1<false,true,true>(hash_6._storage.get());
	hash_6.clear();
	//5
	using h_5 = hash_types_1::hash<type_key, 5>;
	My_Hash<type_key, type_val, h_5, myEq<true,false,true> , true, UpdateVal1> hash_5(nullptr);
	for (const auto& it: *hash_7._storage) {
		hash_5.modify(get<0>(it), get<1>(it));
	}
	my_print_hash1<true,false,true>(hash_5._storage.get());
	//4
	using h_4 = hash_types_1::hash<type_key, 1>;
	My_Hash<type_key, type_val, h_4, myEq<false,false,true>, true, UpdateVal1> hash_4(hash_5._storage);
	hash_5.clear();

	hash_4.build_from_storage<true>();
	my_print_hash1<false,false,true>(hash_4._storage.get());
	hash_4.clear();
	//3
	using h_3 = hash_types_1::hash<type_key, 3>;
	My_Hash<type_key, type_val, h_3, myEq<true,true,false> , true, UpdateVal1> hash_3(hash_7._storage);
	hash_7.clear();
	hash_3.build_from_storage<true>();
	my_print_hash1<true,true,false>(hash_3._storage.get());
	//2
	using h_2 = hash_types_1::hash<type_key, 2>;
	My_Hash<type_key, type_val, h_2, myEq<false,true,false>, true, UpdateVal1> hash_2(nullptr);
	for (const auto& it: *hash_3._storage) {
		hash_2.modify(get<0>(it), get<1>(it));
	}
	my_print_hash1<false,true,false>(hash_2._storage.get());
	hash_2.clear();
	//1
	using h_1 = hash_types_1::hash<type_key, 1>;
	My_Hash<type_key, type_val, h_1, myEq<true,false,false>, true, UpdateVal1> hash_1(hash_3._storage);
	hash_3.clear();
	hash_1.build_from_storage<true>();
	my_print_hash1<true,false,false>(hash_1._storage.get());
	//0
	type_val hash_0;
	for (const auto& it: *hash_1._storage) {
		upd(hash_0, get<1>(it));
	}
	hash_1.clear();
	my_print_hash<false,false,false>(&hash_0);
}

extern "C" void run_query8(const Database &db) {
  using type_key = tuple<Integer, Integer, Integer>;
  using type_val = tuple<Numeric<2, 0>>;
  struct Update_Val0 {
    void operator()(type_val &left, const type_val &right) {
      get<0>(left) += get<0>(right);
    }
  } update_val0;
  struct Eq_7 {
    bool operator()(type_key &left, type_key &right) {
      return (get<0>(left) == get<0>(right)) &&
             (get<1>(left) == get<1>(right)) && (get<2>(left) == get<2>(right));
    }
  };
  using Hash_7 = hash_types_1::hash<type_key, 7>;
  My_Hash<type_key, type_val, Hash_7, Eq_7, true, Update_Val0> hash_table_7(
      nullptr);
  for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
    hash_table_7.modify(make_tuple(db.order.o_d_id[tid0], db.order.o_w_id[tid0],
                                   db.order.o_carrier_id[tid0]),
                        make_tuple(db.order.o_ol_cnt[tid0]));
  }
  for (const auto &it : *hash_table_7._storage) {
    cout << get<0>(get<0>(it)) << "," << get<1>(get<0>(it)) << ","
         << get<2>(get<0>(it)) << "," << get<0>(get<1>(it)) << endl;
  }
  struct Eq_6 {
    bool operator()(type_key &left, type_key &right) {
      return (get<1>(left) == get<1>(right)) && (get<2>(left) == get<2>(right));
    }
  };
  using Hash_6 = hash_types_1::hash<type_key, 6>;
  My_Hash<type_key, type_val, Hash_6, Eq_6, true, Update_Val0> hash_table_6(
      nullptr);
  for (const auto &it : *hash_table_7._storage) {
    hash_table_6.modify(get<0>(it), get<1>(it));
  }
  for (const auto &it : *hash_table_6._storage) {
    cout << "null"
         << "," << get<1>(get<0>(it)) << "," << get<2>(get<0>(it)) << ","
         << get<0>(get<1>(it)) << endl;
  }
  hash_table_6.clear();
  struct Eq_5 {
    bool operator()(type_key &left, type_key &right) {
      return (get<0>(left) == get<0>(right)) && (get<2>(left) == get<2>(right));
    }
  };
  using Hash_5 = hash_types_1::hash<type_key, 5>;
  My_Hash<type_key, type_val, Hash_5, Eq_5, true, Update_Val0> hash_table_5(
      nullptr);
  for (const auto &it : *hash_table_7._storage) {
    hash_table_5.modify(get<0>(it), get<1>(it));
  }
  for (const auto &it : *hash_table_5._storage) {
    cout << get<0>(get<0>(it)) << ","
         << "null"
         << "," << get<2>(get<0>(it)) << "," << get<0>(get<1>(it)) << endl;
  }
  struct Eq_4 {
    bool operator()(type_key &left, type_key &right) {
      return (get<2>(left) == get<2>(right));
    }
  };
  using Hash_4 = hash_types_1::hash<type_key, 4>;
  My_Hash<type_key, type_val, Hash_4, Eq_4, true, Update_Val0> hash_table_4(
      hash_table_5._storage);
  hash_table_5.clear();
  hash_table_4.build_from_storage<true>();
  for (const auto &it : *hash_table_4._storage) {
    cout << "null"
         << ","
         << "null"
         << "," << get<2>(get<0>(it)) << "," << get<0>(get<1>(it)) << endl;
  }
  hash_table_4.clear();
  struct Eq_3 {
    bool operator()(type_key &left, type_key &right) {
      return (get<0>(left) == get<0>(right)) && (get<1>(left) == get<1>(right));
    }
  };
  using Hash_3 = hash_types_1::hash<type_key, 3>;
  My_Hash<type_key, type_val, Hash_3, Eq_3, true, Update_Val0> hash_table_3(
      hash_table_7._storage);
  hash_table_7.clear();
  hash_table_3.build_from_storage<true>();
  for (const auto &it : *hash_table_3._storage) {
    cout << get<0>(get<0>(it)) << "," << get<1>(get<0>(it)) << ","
         << "null"
         << "," << get<0>(get<1>(it)) << endl;
  }
  struct Eq_2 {
    bool operator()(type_key &left, type_key &right) {
      return (get<1>(left) == get<1>(right));
    }
  };
  using Hash_2 = hash_types_1::hash<type_key, 2>;
  My_Hash<type_key, type_val, Hash_2, Eq_2, true, Update_Val0> hash_table_2(
      nullptr);
  for (const auto &it : *hash_table_3._storage) {
    hash_table_2.modify(get<0>(it), get<1>(it));
  }
  for (const auto &it : *hash_table_2._storage) {
    cout << "null"
         << "," << get<1>(get<0>(it)) << ","
         << "null"
         << "," << get<0>(get<1>(it)) << endl;
  }
  hash_table_2.clear();
  struct Eq_1 {
    bool operator()(type_key &left, type_key &right) {
      return (get<0>(left) == get<0>(right));
    }
  };
  using Hash_1 = hash_types_1::hash<type_key, 1>;
  My_Hash<type_key, type_val, Hash_1, Eq_1, true, Update_Val0> hash_table_1(
      hash_table_3._storage);
  hash_table_3.clear();
  hash_table_1.build_from_storage<true>();
  for (const auto &it : *hash_table_1._storage) {
    cout << get<0>(get<0>(it)) << ","
         << "null"
         << ","
         << "null"
         << "," << get<0>(get<1>(it)) << endl;
  }
  type_val hash_table_0;
  for (const auto &it : *hash_table_1._storage) {
    update_val0(hash_table_0, get<1>(it));
  }
  hash_table_1.clear();
  {
    cout << "null"
         << ","
         << "null"
         << ","
         << "null"
         << "," << get<0>(hash_table_0) << endl;
  }
}

//rollup with my_hash
extern "C" void run_query_rollup_1(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;

	using type_key_111 = tuple<Integer,Integer,Integer>;
	My_Hash_U(type_key_111, type_val) hash_111(nullptr);


	using type_key_110 = tuple<Integer,Integer>;
	My_Hash_U(type_key_110, type_val) hash_110(nullptr);

	using type_key_100 = tuple<Integer>;
	My_Hash_U(type_key_100, type_val) hash_100(nullptr);

	type_val val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}

	my_print_hash<true,true,true>(hash_111._storage.get());

	//construct hash_110 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_110.modify(
			make_tuple(get<0>(get<0>(*it)),get<1>(get<0>(*it)))
			, get<1>(*it)
		);
	}

	hash_111.clear();

	my_print_hash<true,true,false>(hash_110._storage.get());

	//construct hash_100 from hash_110
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_100.modify(
			make_tuple(get<0>(get<0>(*it)))
			, get<1>(*it)
		);
	}

	hash_110.clear();

	my_print_hash<true,false,false>(hash_100._storage.get());
	//construct val_000 from hash_100
	for (auto it = hash_100._storage->begin(); it != hash_100._storage->end(); ++it) {
		update_val(val_000, get<1>(*it));
	}
	hash_100.clear();

	my_print_hash<false,false,false>(&val_000);
}

extern "C" void run_query_rollup_2(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;
	using type_key = tuple<Integer,Integer,Integer>;

	using h_111 = hash_types_1::hash<type_key, 7>;
	using h_110 = hash_types_1::hash<type_key, 3>;
	using h_100 = hash_types_1::hash<type_key, 1>;

	My_Hash<type_key, type_val, h_111 , myEq<true,true,true>  , true, UpdateVal> hash_111(nullptr);
	My_Hash<type_key, type_val, h_110 , myEq<true,true,false> , true, UpdateVal> hash_110(hash_111._storage);
	My_Hash<type_key, type_val, h_100 , myEq<true,false,false>, true, UpdateVal> hash_100(hash_110._storage);

	type_val val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}

	my_print_hash1<true,true,true>(hash_111._storage.get());

	hash_110.build_from_storage<true>();
	hash_111.clear();

	my_print_hash1<true,true,false>(hash_110._storage.get());

	hash_100.build_from_storage<true>();
	my_print_hash1<true,false,false>(hash_100._storage.get());
	//construct val_000 from hash_100
	for (auto it = hash_100._storage->begin(); it != hash_100._storage->end(); ++it) {
		update_val(val_000, get<1>(*it));
	}
	hash_100.clear();

	my_print_hash<false,false,false>(&val_000);
}

extern "C" void run_query_rollup_3(const Database &db) {
  using type_key = tuple<Integer, Integer, Integer>;

  using type_key_7 = tuple<Integer, Integer, Integer>;
  using type_key_3 = tuple<Integer, Integer>;
  using type_key_1 = tuple<Integer>;

  using type_val = tuple<Numeric<2, 0>>;
  struct Update_Val0 {
    void operator()(type_val &left, const type_val &right) {
      get<0>(left) += get<0>(right);
    }
  } update_val0;


  unordered_map<type_key_7, type_val, hash_types::hash<type_key_7>> hash_table_7;
  unordered_map<type_key_3, type_val, hash_types::hash<type_key_3>> hash_table_3;
  unordered_map<type_key_1, type_val, hash_types::hash<type_key_1>> hash_table_1;
  type_val hash_table_0;

	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
	{
		auto it = hash_table_7.find(make_tuple(
			 db.order.o_d_id[tid0]
			,db.order.o_w_id[tid0]
			,db.order.o_carrier_id[tid0]
		));
		if (it == hash_table_7.end()) {
			hash_table_7.insert(make_pair(
				make_tuple(
					 db.order.o_d_id[tid0]
					,db.order.o_w_id[tid0]
					,db.order.o_carrier_id[tid0])
				,make_tuple(db.order.o_ol_cnt[tid0])
			));
		} else {
			update_val0(it->second,make_tuple(db.order.o_ol_cnt[tid0]));
		}
	}
	for (const auto &it : hash_table_7) {
		cout
			<< get<0>(get<0>(it)) << "," << get<1>(get<0>(it)) << ","
			<< get<2>(get<0>(it)) << "," << get<0>(get<1>(it)) << endl;
	}
	hash_table_7.clear();

	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
	{
		auto it = hash_table_3.find(make_tuple(
			 db.order.o_d_id[tid0]
			,db.order.o_w_id[tid0]
		));
		if (it == hash_table_3.end()) {
			hash_table_3.insert(make_pair(
				make_tuple(
					 db.order.o_d_id[tid0]
					,db.order.o_w_id[tid0])
				,make_tuple(db.order.o_ol_cnt[tid0])
			));
		} else {
			update_val0(it->second,make_tuple(db.order.o_ol_cnt[tid0]));
		}
	}
	for (const auto &it : hash_table_3) {
		cout
			<< get<0>(get<0>(it)) << "," << get<1>(get<0>(it)) << ","
			<< "null" << "," << get<0>(get<1>(it)) << endl;
	}
	hash_table_3.clear();


	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)	{
		auto it = hash_table_1.find(make_tuple(
			 db.order.o_d_id[tid0]
		));
		if (it == hash_table_1.end()) {
			hash_table_1.insert(make_pair(
				make_tuple(
					 db.order.o_d_id[tid0])
				,make_tuple(db.order.o_ol_cnt[tid0])
			));
		} else {
			update_val0(it->second,make_tuple(db.order.o_ol_cnt[tid0]));
		}
	}
	for (const auto &it : hash_table_1) {
		cout
			<< get<0>(get<0>(it)) << "," << "null" << ","
			<< "null" << "," << get<0>(get<1>(it)) << endl;
	}
	hash_table_1.clear();

	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0)
	{
			update_val0(hash_table_0,make_tuple(db.order.o_ol_cnt[tid0]));
	}
	{
	cout << "null"
		 << "," << "null"
		 << "," << "null"
		 << "," << get<0>(hash_table_0) << endl;
	}
}


extern "C" void run_query_rollup_4(const Database &db) {
  using type_key = tuple<Integer, Integer, Integer>;
  using type_val = tuple<Numeric<2, 0>>;
  struct Update_Val0 {
    void operator()(type_val &left, const type_val &right) {
      get<0>(left) += get<0>(right);
    }
  } update_val0;
  struct Eq_7 {
    bool operator()(type_key &left, type_key &right) {
      return (get<0>(left) == get<0>(right)) &&
             (get<1>(left) == get<1>(right)) && (get<2>(left) == get<2>(right));
    }
  };
  using Hash_7 = hash_types_1::hash<type_key, 7>;
  My_Hash<type_key, type_val, Hash_7, Eq_7, true, Update_Val0> hash_table_7(
      nullptr);
  for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
    hash_table_7.modify(make_tuple(db.order.o_d_id[tid0], db.order.o_w_id[tid0],
                                   db.order.o_carrier_id[tid0]),
                        make_tuple(db.order.o_ol_cnt[tid0]));
  }
  for (const auto &it : *hash_table_7._storage) {
    cout << get<0>(get<0>(it)) << "," << get<1>(get<0>(it)) << ","
         << get<2>(get<0>(it)) << "," << get<0>(get<1>(it)) << endl;
  }
  struct Eq_3 {
    bool operator()(type_key &left, type_key &right) {
      return (get<0>(left) == get<0>(right)) && (get<1>(left) == get<1>(right));
    }
  };
  using Hash_3 = hash_types_1::hash<type_key, 3>;
  My_Hash<type_key, type_val, Hash_3, Eq_3, true, Update_Val0> hash_table_3(
      hash_table_7._storage);
  hash_table_7.clear();
  hash_table_3.build_from_storage<true>();
  for (const auto &it : *hash_table_3._storage) {
    cout << get<0>(get<0>(it)) << "," << get<1>(get<0>(it)) << ","
         << "null"
         << "," << get<0>(get<1>(it)) << endl;
  }
  struct Eq_1 {
    bool operator()(type_key &left, type_key &right) {
      return (get<0>(left) == get<0>(right));
    }
  };
  using Hash_1 = hash_types_1::hash<type_key, 1>;
  My_Hash<type_key, type_val, Hash_1, Eq_1, true, Update_Val0> hash_table_1(
      hash_table_3._storage);
  hash_table_3.clear();
  hash_table_1.build_from_storage<true>();
  for (const auto &it : *hash_table_1._storage) {
    cout << get<0>(get<0>(it)) << ","
         << "null"
         << ","
         << "null"
         << "," << get<0>(get<1>(it)) << endl;
  }
  type_val hash_table_0;
  for (const auto &it : *hash_table_1._storage) {
    update_val0(hash_table_0, get<1>(it));
  }
  hash_table_1.clear();
  {
    cout << "null"
         << ","
         << "null"
         << ","
         << "null"
         << "," << get<0>(hash_table_0) << endl;
  }
}

extern "C" void olap_query_001(const Database &db) {

}
