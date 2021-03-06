#pragma once
#include <vector>
#include <assert.h>

using namespace std;

template<typename T, size_t block_bytes>
class Block_Storage {
public:

	class Iterator {
	public:
		friend Block_Storage;

		Iterator(Block_Storage<T,block_bytes>& storage, size_t pos)
			: _storage(storage), _block(0), _pos_in_block(0), _pos(pos)
		{
			_block = pos / _storage._block_size;
			_pos_in_block = pos - _block * _storage._block_size;
		}

		~Iterator() {}

		bool operator==(const Iterator& it) const { return _pos == it._pos; }
		bool operator!=(const Iterator& it) const { return _pos != it._pos; }

		Iterator& operator++() { //prefix increment
			if (_pos_in_block < _storage._block_size - 1) {
				++_pos_in_block;
			} else {
				_pos_in_block = 0;
				++_block;
			}
			++_pos;
			return *this;
		}

		Iterator& operator=(const Iterator& src) {
			_block        = src._block;
			_pos_in_block = src._pos_in_block;
			_pos          = src._pos;
			return *this;
		}

		T& operator*() { return _storage._blocks[_block][_pos_in_block]; }

		T* get_data() { return &_storage._blocks[_block][_pos_in_block]; }
	private:
		Block_Storage<T,block_bytes>& _storage;
		size_t _block;
		size_t _pos_in_block;
		size_t _pos;
	};

	Iterator begin() { return Iterator(*this,0); }
	Iterator end() { return Iterator(*this,_size); }

private:
	const size_t _block_size = block_bytes / sizeof(T);
	vector<T*> _blocks;
	size_t _size;
	size_t _last_block_size;
	size_t _last_block;

public:
	Block_Storage()
	{
		static_assert(block_bytes >= sizeof(T), "_block_size should be positive");
		reset();
//		allocate_block();
	}

	~Block_Storage() { clear();}

	void clear() {
		for (size_t i = 0; i < _blocks.size(); ++i) {
			if (_blocks[i] != nullptr) {
				delete [] _blocks[i];
			}
			_blocks[i] = nullptr;
		}
		reset();
		_blocks.clear();
	}


	T* insert(T t) {
		check_new_block();
		_blocks[_last_block][_last_block_size] = t;
		++_size;
		return &_blocks[_last_block][_last_block_size++];
	}

	void reset() {
		_last_block = 0;
		_last_block_size = 0;
		_size = 0;
	}

	void swap(Iterator& left, Iterator& right) {
		std::swap(*left,*right);
	}

	void shrink(Iterator& from) {
		assert(from._pos <= _size);
		size_t delete_from_block = from._block + (from._pos_in_block==0? 0: 1);
		assert(delete_from_block > 0);
		for (size_t i = delete_from_block; i < _blocks.size(); ++i) {
			delete [] _blocks[i];
			_blocks[i] = nullptr;
		}
		_blocks.resize(delete_from_block);

		_last_block = (_last_block < delete_from_block? _last_block : delete_from_block-1);
		_size = from._pos;
		_last_block_size = _size - _last_block * _block_size;
	}

	size_t size() const {return _size;}



	//size_t block_size() const {return _block_size;}
private:
	void allocate_block() {
		_blocks.push_back(new T[_block_size]);
	}

	void check_new_block() {
		if (_last_block_size == _block_size) {
			++_last_block;
			if (_last_block == _blocks.size()) {
				allocate_block();
			}
			_last_block_size = 0;
		} else if (_blocks.size() == 0) {
			allocate_block();
			_last_block = 0;
		}

	}
};
