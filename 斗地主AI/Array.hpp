#ifndef __ARRAY_HPP__
#define  __ARRAY_HPP__
#include <stdint.h>
#include <iostream>
#include "config.h"
namespace zero {
	

	template<typename T>
	class Array {
	public:
		class ArrayValue {

		private:
			Array<T> *data;	//数据记录
			uint32_t index;	//索引号记录

		public:

			ArrayValue(Array<T>* arr, uint32_t index) :data(arr), index(index) {}
			ArrayValue() :data(nullptr), index(0) {}

			ArrayValue(const ArrayValue& value) {
				this->data = value.data;
				this->index = value.index;
			}
			//= 运算符重载
			void operator =(const T &value) {
				this->data->_data[index] = value;
			}

			//强制转换重载
			operator T() {
				return ((T)(this->data->_data[index]));
			}
			bool operator ==(const T &value) {
				return this->data[this->index]==value;
			}
		};


	public:
		class Crossing {};	//越界异常

	private:
		T* _data;
		uint32_t _limit;

	public:
		Array() :_limit(0), _data(nullptr) {}
		Array(uint32_t limit):_limit(limit) {
			
			_data = new T[limit];
		}
		Array(T* data, uint32_t limit):_data(data),_limit(limit) {}
		Array(const zero::Array<T> &value) :_limit(value._limit),_data(value._data) {

			_data = new T[_limit];
		}

		Array<T>& setDimension(uint32_t limit) {
			if (this->_data == nullptr) {
				this->_data = new T[limit];
				this->_limit = limit;
			}
			return *this;
		}

		ArrayValue operator[](const uint32_t &index) {
			if (index < (this->_limit)) {
				return zero::Array<T>::ArrayValue(this,index);
			}
			else {
				throw Array<T>::Crossing();
			}
		}
		~Array() {
			delete[] this->_data;
		}
		//初始化
		Array<T>& InitValue(T value) {
			for (uint32_t i = 0; i < this->_limit; ++i) {
				this->_data[i] = value;
			}
			return *this;
		}
		

		uint32_t GetSize() {
			return this->_limit;
		}
		uint32_t GetBytesSize() {
			return this->_limit*sizeof(T);
		}
		T* GetPointer() {
			return this->_data;
		}
		uint32_t GetLimit() {
			return this->_limit;
		}

		template<typename U>
		friend std::ostream & operator<<(std::ostream &,  Array<U> &);

		template<typename U>
		friend std::ostream & operator<<(std::ostream &, Array<U> *);

	
	};
	
#define CLASS(TYPE) template<>class Array<TYPE> {\
	public:\
		class ArrayValue {\
		private:\
			Array<TYPE> *data;\
			uint32_t index;	\
		public:\
			ArrayValue(Array<TYPE>* arr, uint32_t index) :data(arr), index(index) {}\
			ArrayValue() :data(nullptr), index(0) {}\
			ArrayValue(const ArrayValue& value) {\
				this->data = value.data;\
				this->index = value.index;\
			}\
			void operator =(const TYPE &value) {\
				this->data->_data[index] = value;\
			}\
			operator TYPE() {\
				return ((TYPE)(this->data->_data[index]));\
			}\
			bool operator ==(const TYPE &value) {\
				return this->data->_data[this->index] == value;\
			}\
			ArrayValue& operator ++( int) {\
				this->data->_data[this->index]=this->data->_data[this->index]+1;\
				return *this;\
			}\
			ArrayValue& operator ++() {\
					this->data->_data[this->index]=this->data->_data[this->index]+1;\
					return *this;\
			}\
			ArrayValue& operator --(int) {\
				this->data->_data[this->index]=this->data->_data[this->index]-1;\
					return *this;\
			}\
			ArrayValue& operator --() {\
				this->data->_data[this->index]=this->data->_data[this->index]-1;\
					return *this;\
			}\
		};\
	public:\
		class Crossing {};\
	private:\
		TYPE * _data;\
		uint32_t _limit;\
	public:\
		Array() :_limit(0), _data(nullptr) {}\
		Array(uint32_t limit) :_limit(limit) {\
			_data = new TYPE[limit];\
		}\
		Array(TYPE* data, uint32_t limit) :_data(data), _limit(limit) {}\
		Array(const zero::Array<TYPE> &value) :_limit(value._limit), _data(value._data) {\
			_data = new TYPE[_limit];\
		}\
		Array<TYPE>& setDimension(uint32_t limit) {\
			if (this->_data == nullptr) {\
				this->_data = new TYPE[limit];\
				this->_limit = limit;\
			}\
			return *this;\
		}\
		zero::Array<TYPE>::ArrayValue operator[](const uint32_t &index) {\
			if (index < (this->_limit)) {\
				return zero::Array<TYPE>::ArrayValue(this, index);\
			}\
			else {\
				throw Array<TYPE>::Crossing();\
			}\
		}\
		~Array() {\
			delete[] this->_data;\
		}\
		Array<TYPE>& InitValue(TYPE value) {\
			for (uint32_t i = 0; i < this->_limit; ++i) {\
				this->_data[i] = value;\
			}\
			return *this;\
		}\
		uint32_t GetSize() {\
			return this->_limit;\
		}\
		uint32_t GetBytesSize() {\
			return this->_limit * sizeof(TYPE);\
		}\
		TYPE* GetPointer() {\
			return this->_data;\
		}\
	uint32_t GetLimit() {\
	return this->_limit;\
	}\
	};
	
	CLASS(int32_t);
	CLASS(int8_t);
	CLASS(int16_t);
	CLASS(int64_t);
	CLASS(uint32_t);
	CLASS(uint8_t);
	CLASS(uint16_t);
	CLASS(uint64_t);
	CLASS(float);
	CLASS(double);

#undef CLASS


	//二维数组
	template<typename T>
	class MostArray {
	private:
		T ** _data;
		uint32_t _level1Limit;
		uint32_t _level2Limit;

	public:
		MostArray() :_level1Limit(0), _level2Limit(0), _data(nullptr) {}
		void setDimension(uint32_t limit1, uint32_t limit2) {
			this->_data = new T*[limit1];
			for (uint32_t i = 0; i < limit2; ++i) {
				this->_data[i] = new T[limit2];
			}
			this->_level1Limit = limit1;
			this->_level2Limit = limit2;
		}
		MostArray(uint32_t level1Limit, uint32_t level2Limit):_level1Limit(level1Limit),_level2Limit(level2Limit) {
			this->_data=new T*[level1Limit];
			for (uint32_t i = 0; i < level2Limit; ++i) {
				this->_data[i] = new T[level2Limit];
			}
		}
		MostArray(const MostArray &value):_data(value._data),_level1Limit(value._level1Limit),_level2Limit(value._level2Limit) {
			
		}
		Array<T> operator[](const uint32_t &index) {
			if (index < (this->_level1Limit)) {
				
				return zero::Array<T>(this->_data[index], _level1Limit);
			}
			else {
				throw Array<T>::Crossing();
			}
		}

		//初始化
		void InitValue(T value) {
			for (uint32_t i = 0; i < this->_level1Limit; ++i) {
				let item = this->_data[i];
				for (uint32_t j = 0; j < this->_level2Limit; ++j) {
					item[j] = value;
				}
			}
		}

		~MostArray() {
			for (uint32_t i = 0; i < this->_level2Limit; ++i) {
				delete this->_data[i];
			}
			delete[] this->_data;
		}

		template<typename U>
		friend std::ostream & operator<<(std::ostream &out, zero::MostArray<U> &d);

		template<typename U>
		friend std::ostream & operator<<(std::ostream &out, zero::MostArray<U> *d);
	};
	


	template<typename U>
	bool operator==(const U& obj, typename zero::Array<U>::ArrayValue &value)
	{
		return value->data[value->index] == obj;
	}

	template<typename U>
	std::ostream & operator<<(std::ostream &out,  zero::Array<U> &d) {
		for (uint32_t i = 0, len = d.GetLimit(); i < len; i++) {
			out << (U)d[i] << ',';
		}
			
		return out;
	}

	
	template<typename U>
	std::ostream & operator<<(std::ostream &out, Array<U> *d) {
		for (uint32_t i = 0, len = d._limit; i < len; i++)
			out << (U)((*d)[i]) << ',';
		return out;
	}


	template<typename U>
	inline std::ostream & operator<<(std::ostream & out, zero::MostArray<U>& d)
	{
		for (uint32_t i = 0; i < d._level1Limit; ++i) {
			let item = d._data[i];
			for (uint32_t j = 0; j < d._level2Limit; ++j) {
				item[j] = value;

				out << (U)d[i] << ',';
			}
			out << endl;
		}
		return out;
	}

	template<typename U>
	inline std::ostream & operator<<(std::ostream & out, zero::MostArray<U>* d)
	{
		for (uint32_t i = 0; i < d->_level1Limit; ++i) {
			let item = d->_data[i];
			for (uint32_t j = 0; j < d->_level2Limit; ++j) {
				item[j] = value;
				out << (U)d[i] << ',';
			}
			out << endl;
		}
		return out;
	}








}
#endif
