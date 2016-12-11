#include "Types.hpp"
#include "schema_1.hpp"
#include <iostream>
#include <unordered_map>
using namespace std;

//is generated 
template<bool f1,bool f2,bool f3,typename IndexType>
void my_print_hash(IndexType& index) {
	constexpr int n1 = 0;
	constexpr int n2 = n1 + (f1&&f2?1:0);
	constexpr int n3 = n2 + (f2&&f3?1:0);

	for (auto it = index.begin(); it != index.end(); ++it) {
		if (f1) cout << get<n1>(it->first); else cout << "null";
		cout << ";" ;
		if (f2) cout << get<n2>(it->first); else cout << "null";
		cout << ";" ;
		if (f3) cout << get<n3>(it->first); else cout << "null";
		cout << ";" ;
		cout << get<0>(it->second);
		cout << endl; 
	}
}

template<>
void my_print_hash<false,false,false, tuple<Numeric<2,0>>>(tuple<Numeric<2,0>>& val) {
	cout << "null"
		 << ";" << "null"
		 << ";" << "null"
		 << ";" << get<0>(val)
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
		update_val(it->second, val);
	}
}










//bool predicat0(const Varchar<16> &c_last) { return c_last == "BARBARBAR"; }

extern "C" void run_query0(const Database &db) {
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
	
	my_print_hash<true,true,true>(hash_111);
	my_print_hash<true,true,false>(hash_110);
	my_print_hash<true,false,true>(hash_101);
	my_print_hash<false,true,true>(hash_011);
	my_print_hash<true,false,false>(hash_100);
	my_print_hash<false,true,false>(hash_010);
	my_print_hash<false,false,true>(hash_001);
	my_print_hash<false,false,false>(val_000);
}



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
	
	my_print_hash<true,true,true>(hash_111);

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
	
	my_print_hash<true,true,false>(hash_110);
	
	//construct hash_100 from hash_110
	for (auto it = hash_110.begin(); it != hash_110.end(); ++it) {
		my_update_hash(hash_100, make_tuple(get<0>(it->first)), it->second);
	}
	
	//construct hash_010 from hash_110
	for (auto it = hash_110.begin(); it != hash_110.end(); ++it) {
		my_update_hash(hash_010, make_tuple(get<1>(it->first)), it->second);
	}
	hash_110.clear();

	my_print_hash<true,false,true>(hash_101);
	
	//construct hash_001 from hash_101
	for (auto it = hash_101.begin(); it != hash_101.end(); ++it) {
		my_update_hash(hash_001, make_tuple(get<1>(it->first)), it->second);
	}
	
	hash_101.clear();
	my_print_hash<false,true,true>(hash_011);
	hash_011.clear();
	my_print_hash<true,false,false>(hash_100);
	//construct val_000 from hash_100
	for (auto it = hash_100.begin(); it != hash_100.end(); ++it) {
		update_val(val_000, it->second);
	}
	hash_100.clear();

	my_print_hash<false,true,false>(hash_010);
	hash_010.clear();
	my_print_hash<false,false,true>(hash_001);
	hash_001.clear();
	my_print_hash<false,false,false>(val_000);
}



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
	my_print_hash<true,true,true>(hash_111);
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
	my_print_hash<true,true,false>(hash_110);
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
	my_print_hash<true,false,true>(hash_101);
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
	my_print_hash<false,true,true>(hash_011);
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
	my_print_hash<true,false,false>(hash_100);
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
	my_print_hash<false,true,false>(hash_010);
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
	my_print_hash<false,false,true>(hash_001);
	hash_001.clear();
	//construct val_000
	for (Tid tid0 = 0; tid0 < db.order.size(); ++tid0) 
		{
			get<0>(val_000) += db.order.o_ol_cnt[tid0];
		}
	my_print_hash<false,false,false>(val_000);
	
}