#pragma once

///////////////////////////////////////
// INCLUDES
///////////////////////////////////////

#include <stdint.h>
#include <iostream>
#include <stdexcept>


///////////////////////////////////////
// CONSTS
///////////////////////////////////////
const uint32_t START_SIZE = 10;

///////////////////////////////////////
// TRAITS
///////////////////////////////////////

//Pointers
template< typename T > 
struct is_pointer
{ 
  static const bool value = false; 
};


template< typename T > 
struct is_pointer< T* >
{ 
  static const bool value = true;
};

//Strings
template< typename T >
struct is_string
{
	static const bool value = false;
};

template<>
struct is_string<std::string>
{
	static const bool value = true;
};


template< typename T >
struct is_string_pointer
{
	static const bool value = false;
};

template<>
struct is_string_pointer<std::string*>
{
	static const bool value = true;
};

//struct to hold the pairings in the class
template <class KeyType, class DataType>
struct Pair {
	KeyType key;
	DataType data;
};

//
template <class KeyType, class DataType>
class CVectorMap
{
//variables
public:
private:
	Pair<KeyType, DataType>* mv_keyValue;
	uint32_t mi_capacity;
	uint32_t mi_count;
	bool mb_sorted;
//methods
public:
	CVectorMap(void);

	~CVectorMap(void);

	bool InsertData (KeyType newKey, DataType newData);
	bool InsertSortedData (KeyType newKey, DataType newData);

	bool FindData (KeyType searchKey, DataType* output, uint32_t* index = NULL);

	DataType GetDataIndex( uint32_t index ){ return mv_keyValue[index].data; };
	KeyType GetKeyIndex( uint32_t index ){return mv_keyValue[index].key; };

	void Sort();

	uint32_t GetCount() { return mi_count; }
	uint32_t GetCapacity() { return mi_capacity; }

	//selector - workers in private section
	void DisplayAll();

	void Clear();

	inline bool IsEmpty();

	bool IsSorted() { return mb_sorted; }

	void SwapIndex(uint32_t a, uint32_t b);

	void DeleteIndex(uint32_t index);

	bool DeleteKey(KeyType key);

	bool SetCapacity(uint32_t newCapacity);

	//opperators
	DataType operator[] (KeyType &key);

private:
	bool IncreaseCapacity();

	void QuickSort(uint32_t start, uint32_t end);

	//these do the work
	void DisplayAllOO();
	void DisplayAllPP();
};

