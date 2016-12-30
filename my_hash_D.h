#pragma once
#include <iostream>
#include <tuple>
#include <vector>
#include <memory>
#include "block_storage.h"

using namespace std;
template<typename _Key,typename _Tp,bool _Unique>
class My_Hash_D {
public:
	//public types
	using Entry = tuple<_Key, _Tp, void*>;
	using Upd = void (*)(_Tp& trg, const _Tp& src);
	using Eq = bool (*) (const _Key& left, const _Key& right);
	using Hash = size_t (*) (const _Key& k);
	using Storage = Block_Storage<Entry, 1024*400>;
	using Storage_Ptr = shared_ptr<Storage>;

private:
	//constant members
	static const size_t _Key_N = 0;
	static const size_t _Tp_N = 1;
	static const size_t _Next_N = 2;
	constexpr double _target_load_factor() {return 0.75;}
	const size_t _min_log2_n_buckets = 10;
	//members
	size_t _log2_n_buckets = _min_log2_n_buckets;
	size_t _size_threshold;
	size_t _size = 0;

	vector<Entry*> _hash_table;

	Storage_Ptr _storage = nullptr;
	Upd _upd = nullptr;
	Eq _eq = nullptr;
	Hash _h = nullptr;

	//private methods
	void calculate_next_size_threshold() {
		_size_threshold = size_t(_target_load_factor() * double(_hash_table.size()));
	}

	void rehash_if_full() {
		if (_size < _size_threshold) return;
		size_t new_log2_size = _log2_n_buckets + 1;
		rehash<false>(new_log2_size);
	}
	size_t get_bucket(_Key key) {
		return _h(key) & ((1ull << _log2_n_buckets)-1);
	}

public:
	//public methods
	class Iterator;

	My_Hash_D& set_upd    (Upd upd)       {_upd = upd  ; return *this;}
	My_Hash_D& set_eq     (Eq eq)         {_eq = eq    ; return *this;}
	My_Hash_D& set_hash   (Hash h)        {_h = h      ; return *this;}
	My_Hash_D& set_storage(Storage_Ptr s) {_storage = s; return *this;}

	Storage_Ptr get_storage() {return _storage;}

	My_Hash_D()	{
		clear();
		_log2_n_buckets = _min_log2_n_buckets;
		_hash_table.resize(1 << _log2_n_buckets, 0);
		calculate_next_size_threshold();
	}

	~My_Hash_D() { clear(); }

	void clear() {
		if (_storage.unique() && _storage != nullptr) {
			_storage->clear();
		}
		_hash_table.clear();
		_size = 0;
		_log2_n_buckets = -1;
		_upd = nullptr;
		_eq = nullptr;
		_h = nullptr;
	}

	size_t size() const { return _size;}

	Iterator insert(_Key key, _Tp val) {
		rehash_if_full();

		size_t bucket = get_bucket(key);
		Entry** current = &_hash_table[bucket];
		//if unique, then insert to the beginning of the bucket
		//else insert before the first item with the same key
		if (!_Unique) {
			while (*current != nullptr) {
				if (_eq(key, get<_Key_N>(**current))) break;
				current = reinterpret_cast<Entry**>(&get<_Next_N>(**current));
			}
		}
		*current = _storage->insert(make_tuple(
			key, val, reinterpret_cast<void*>(*current)));
		++_size;

		return Iterator(*this, bucket, *current);
	}

	Iterator modify(_Key key, _Tp val) {
		rehash_if_full();

		size_t bucket = get_bucket(key);
		Entry** current = &_hash_table[bucket];
		//search for key
		while (*current != nullptr && !_eq(key, get<_Key_N>(**current))) {
			current = reinterpret_cast<Entry**>(&get<_Next_N>(**current));
		}
		if (!_Unique || *current == nullptr) {
			//insert
			*current = _storage->insert(make_tuple(key, val, (void*) *current));
			++_size;
		} else {
			//update
			_upd(get<_Tp_N>(**current),val);
		}
		return Iterator(*this, bucket, *current);
	}

	Iterator find(_Key x) {
		size_t bucket = get_bucket(x);
		Entry* current = _hash_table[bucket];
		while (current != nullptr && !_eq(x, get<_Key_N>(*current))) {
			current = reinterpret_cast<Entry*>(get<_Next_N>(*current));
		}

		return Iterator(*this, bucket, current);
	}

	pair<Iterator,Iterator> equal_range(_Key&& x) {
		size_t bucket = get_bucket(x);
		Entry* left = _hash_table[bucket];
		while (left != nullptr && !_eq(x, get<_Key_N>(*left))) {
			left = reinterpret_cast<Entry*>(get<_Next_N>(*left));
		}

		Entry* right = left;

		while (right != nullptr && _eq(x, get<_Key_N>(*right))) {
			right = reinterpret_cast<Entry*>(get<_Next_N>(*right));
		}

		return make_pair(
			 Iterator(*this, bucket, left)
			,Iterator(*this, bucket, right));
	}

	template<bool with_update>
	void build_from_storage() {
		static_assert(_Unique || !with_update, "_Unique=false and with_update=true are incompatible");
		size_t sz = size_t(double(_storage->size()) / _target_load_factor() );
		size_t tmp = size_t(log2(sz));

		rehash<with_update>(tmp);
	}

	template<bool with_update>
	void rehash(size_t new_log2_n_buckets) {
		new_log2_n_buckets = max(new_log2_n_buckets, _min_log2_n_buckets);

		_hash_table.resize(1ull << new_log2_n_buckets);
		_log2_n_buckets = new_log2_n_buckets;
		calculate_next_size_threshold();
		memset(_hash_table.data(), 0, _hash_table.size()*sizeof(Entry*));
		_size = 0;

		size_t bucket = 0;
		Entry** current = nullptr;

		auto it_empty = _storage->end();

		for (auto it = _storage->begin(); it != _storage->end(); ++it) {
			bucket = get_bucket(get<_Key_N>(*it));
			current = &_hash_table[bucket];
			//search for key
			if (!_Unique || with_update) {
				while (*current != nullptr) {
					if (_eq(get<_Key_N>(*it), get<_Key_N>(**current))) break;
					current = reinterpret_cast<Entry**>(&get<_Next_N>(**current));
				}
			}

			if (!_Unique || !with_update || *current == nullptr) {
				//insert
				get<_Next_N>(*it) = *current;
				if (it_empty != _storage->end()) {
					//_storage->swap(it_empty,it);
					*it_empty = *it;
					*current = it_empty.get_data();
					++it_empty;
				} else {
					*current = it.get_data();
				}
				++_size;
			} else {
				//update
				_upd(get<_Tp_N>(**current),get<_Tp_N>(*it));
				if (it_empty == _storage->end()) {
					it_empty = it;
				}
			}
		}
		if (it_empty != _storage->end()) {
			_storage->shrink(it_empty);
		}

	}


//Iterator description
public:

	class Iterator {
	public:
		Iterator(My_Hash_D& my_hash, size_t bucket, My_Hash_D::Entry* entry)
			: _hash(my_hash), _bucket(bucket), _entry(entry) {}

		~Iterator() {}

		bool operator==(const Iterator& it) const { return _entry==it._entry; }
		bool operator!=(const Iterator& it) const { return _entry!=it._entry; }

		Iterator& operator++() { //prefix increment
			if (get<_Next_N>(*_entry) != nullptr) {
				_entry = reinterpret_cast<Entry*>(get<_Next_N>(*_entry));
			} else {
				_entry = nullptr;
				while (++_bucket < _hash._hash_table.size()) {
					_entry = _hash._hash_table[_bucket];
					if (_entry != nullptr) break;
				}
			}
			return *this;
		}

		Entry& operator*() const { return *_entry; }
	private:
		My_Hash_D& _hash;
		size_t _bucket;
		Entry* _entry;
	};

	Iterator begin() {
		size_t i = 0;
		for (i = 0; i < _hash_table.size() && _hash_table[i] == nullptr; ++i) {;}
		return Iterator(*this,i, (i == _hash_table.size()?nullptr:_hash_table[i]));
	}
	Iterator end() { return Iterator(*this,_hash_table.size(),nullptr); }
};



