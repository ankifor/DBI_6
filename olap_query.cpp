#include "Types.hpp"
#include "schema_1.hpp"
#include <iostream>
#include <unordered_map>
#include <tuple>

#include "my_hash.h"
using namespace std;

#define My_Hash_U(_Key, _Tp) My_Hash<_Key, _Tp, hash_types::hash<_Key>, equal_to<_Key>, true, UpdateVal>

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

static void hash_combine1(size_t& seed, size_t val) {
	seed ^= val + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

template<bool f1,bool f2,bool f3>
struct myHash {
	size_t operator()(tuple<Integer,Integer,Integer>& key) {
		size_t seed = 0;
		if (f1) {
			seed = get<0>(key).hash();
			if (f2) hash_combine1(seed,get<1>(key).hash());
			if (f3) hash_combine1(seed,get<2>(key).hash());
		} else if (f2) {
			seed = get<1>(key).hash();
			if (f3) hash_combine1(seed,get<2>(key).hash());
		} else if (f3) {
			seed = get<2>(key).hash();
		}
		return seed;
	}
};

//4 with same key
extern "C" void run_query5(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;
	using type_key = tuple<Integer,Integer,Integer>;


	My_Hash<type_key, type_val, myHash<true,true,true>  , myEq<true,true,true>  , true, UpdateVal> hash_111(nullptr);
	My_Hash<type_key, type_val, myHash<true,true,false> , myEq<true,true,false> , true, UpdateVal> hash_110(nullptr);
	My_Hash<type_key, type_val, myHash<true,false,true> , myEq<true,false,true> , true, UpdateVal> hash_101(nullptr);
	My_Hash<type_key, type_val, myHash<false,true,true> , myEq<false,true,true> , true, UpdateVal> hash_011(nullptr);
	My_Hash<type_key, type_val, myHash<true,false,false>, myEq<true,false,false>, true, UpdateVal> hash_100(nullptr);
	My_Hash<type_key, type_val, myHash<false,true,false>, myEq<false,true,false>, true, UpdateVal> hash_010(nullptr);
	My_Hash<type_key, type_val, myHash<false,false,true>, myEq<false,false,true>, true, UpdateVal> hash_001(nullptr);

	type_val val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}

	my_print_hash1<true,true,true>(hash_111._storage.get());

	//construct hash_110 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_110.modify(get<0>(*it), get<1>(*it));
	}

	//construct hash_101 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_101.modify(get<0>(*it), get<1>(*it));
	}

	//construct hash_011 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_011.modify(get<0>(*it), get<1>(*it));
	}
	hash_111.clear();

	my_print_hash1<true,true,false>(hash_110._storage.get());

	//construct hash_100 from hash_110
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_100.modify(get<0>(*it), get<1>(*it));
	}

	//construct hash_010 from hash_110
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_010.modify(get<0>(*it), get<1>(*it));
	}
	hash_110.clear();

	my_print_hash1<true,false,true>(hash_101._storage.get());

	//construct hash_001 from hash_101
	for (auto it = hash_101._storage->begin(); it != hash_101._storage->end(); ++it) {
		hash_001.modify(get<0>(*it), get<1>(*it));
	}

	hash_101.clear();
	my_print_hash1<false,true,true>(hash_011._storage.get());
	hash_011.clear();
	my_print_hash1<true,false,false>(hash_100._storage.get());
	//construct val_000 from hash_100
	for (auto it = hash_100._storage->begin(); it != hash_100._storage->end(); ++it) {
		update_val(val_000, get<1>(*it));
	}
	hash_100.clear();

	my_print_hash1<false,true,false>(hash_010._storage.get());
	hash_010.clear();
	my_print_hash1<false,false,true>(hash_001._storage.get());
	hash_001.clear();
	my_print_hash<false,false,false>(&val_000);
}

//with memory reusage
extern "C" void run_query6(const Database &db) {
	using type_val = tuple<Numeric<2,0>>;
	using type_key = tuple<Integer,Integer,Integer>;


	My_Hash<type_key, type_val, myHash<true,true,true>  , myEq<true,true,true>  , true, UpdateVal> hash_111(nullptr);
	My_Hash<type_key, type_val, myHash<true,true,false> , myEq<true,true,false> , true, UpdateVal> hash_110(hash_111._storage);
	My_Hash<type_key, type_val, myHash<true,false,true> , myEq<true,false,true> , true, UpdateVal> hash_101(nullptr);
	My_Hash<type_key, type_val, myHash<false,true,true> , myEq<false,true,true> , true, UpdateVal> hash_011(nullptr);
	My_Hash<type_key, type_val, myHash<true,false,false>, myEq<true,false,false>, true, UpdateVal> hash_100(hash_110._storage);
	My_Hash<type_key, type_val, myHash<false,true,false>, myEq<false,true,false>, true, UpdateVal> hash_010(nullptr);
	My_Hash<type_key, type_val, myHash<false,false,true>, myEq<false,false,true>, true, UpdateVal> hash_001(nullptr);

	type_val val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}

	my_print_hash1<true,true,true>(hash_111._storage.get());

	//construct hash_110 from hash_111
	hash_110.build_from_storage<true>();
//	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
//		hash_110.modify(get<0>(*it), get<1>(*it));
//	}

	//construct hash_101 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_101.modify(get<0>(*it), get<1>(*it));
	}

	//construct hash_011 from hash_111
	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
		hash_011.modify(get<0>(*it), get<1>(*it));
	}
	hash_111.clear();

	my_print_hash1<true,true,false>(hash_110._storage.get());

	//construct hash_100 from hash_110
	hash_100.build_from_storage<true>();
//	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
//		hash_100.modify(get<0>(*it), get<1>(*it));
//	}

	//construct hash_010 from hash_110
	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
		hash_010.modify(get<0>(*it), get<1>(*it));
	}
	hash_110.clear();

	my_print_hash1<true,false,true>(hash_101._storage.get());

	//construct hash_001 from hash_101
	for (auto it = hash_101._storage->begin(); it != hash_101._storage->end(); ++it) {
		hash_001.modify(get<0>(*it), get<1>(*it));
	}

	hash_101.clear();
	my_print_hash1<false,true,true>(hash_011._storage.get());
	hash_011.clear();
	my_print_hash1<true,false,false>(hash_100._storage.get());
	//construct val_000 from hash_100
	for (auto it = hash_100._storage->begin(); it != hash_100._storage->end(); ++it) {
		update_val(val_000, get<1>(*it));
	}
	hash_100.clear();

	my_print_hash1<false,true,false>(hash_010._storage.get());
	hash_010.clear();
	my_print_hash1<false,false,true>(hash_001._storage.get());
	hash_001.clear();
	my_print_hash<false,false,false>(&val_000);
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


	My_Hash<type_key, type_val, myHash<true,true,true>  , myEq<true,true,true>  , true, UpdateVal> hash_111(nullptr);
	My_Hash<type_key, type_val, myHash<true,true,false> , myEq<true,true,false> , true, UpdateVal> hash_110(hash_111._storage);//
	My_Hash<type_key, type_val, myHash<true,false,false>, myEq<true,false,false>, true, UpdateVal> hash_100(hash_110._storage);//

	type_val val_000;

	//scan
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) {
		hash_111.modify(
			make_tuple(db.order.o_d_id[tid0],db.order.o_w_id[tid0],db.order.o_carrier_id[tid0])
			, make_tuple(db.order.o_ol_cnt[tid0])
		);
	}

	my_print_hash1<true,true,true>(hash_111._storage.get());

	//construct hash_110 from hash_111
//	for (auto it = hash_111._storage->begin(); it != hash_111._storage->end(); ++it) {
//		hash_110.modify(get<0>(*it), get<1>(*it));
//	}
	hash_110.build_from_storage<true>();
	hash_111.clear();

	my_print_hash1<true,true,false>(hash_110._storage.get());

	//construct hash_100 from hash_110
//	for (auto it = hash_110._storage->begin(); it != hash_110._storage->end(); ++it) {
//		hash_100.modify(get<0>(*it), get<1>(*it));
//	}
	hash_100.build_from_storage<true>();
	my_print_hash1<true,false,false>(hash_100._storage.get());
	//construct val_000 from hash_100
	for (auto it = hash_100._storage->begin(); it != hash_100._storage->end(); ++it) {
		update_val(val_000, get<1>(*it));
	}
	hash_100.clear();

	my_print_hash<false,false,false>(&val_000);
}
