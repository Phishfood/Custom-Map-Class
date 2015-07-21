///////////////////////////////////////
// INCLUDES
///////////////////////////////////////

#include "stdafx.h"
#include "VectorMap.h"

#include <stdexcept>

//#define ___VERBOSE
///////////////////////////////////////
// CONSTRUCTORS AND DESTRUCTOR
///////////////////////////////////////

//Default constructor
template <class KeyType, class DataType>
CVectorMap<KeyType, DataType>::CVectorMap(void)
{
	if( is_string<KeyType>::value || is_string<DataType>::value )
	{
		throw std::invalid_argument("I can't handle strings yet!");
	}
	//store the capacity
	mi_capacity = START_SIZE;

	//create the array
	mv_keyValue = ( Pair<KeyType, DataType>* ) malloc(sizeof( Pair<KeyType, DataType> ) * mi_capacity );
	mi_count = 0;
	mb_sorted = false;
}


//Destructor
template <class KeyType, class DataType>
CVectorMap<KeyType, DataType>::~CVectorMap(void)
{
	free(mv_keyValue);
}

///////////////////////////////////////
// PUBLIC
///////////////////////////////////////

//Puts data at the end of the array, returns true on success
template <class KeyType, class DataType>
bool CVectorMap<KeyType, DataType>::InsertData (KeyType newKey, DataType newData)
{

	uint32_t index;
	DataType data;
	//check the key exists (pointers cause issue?)
	if( !is_pointer<KeyType>::value && FindData(newKey, &data, &index) )
	{
		//overwrite existing value
		mv_keyValue[index].data = newData;
		return true;
	}

	//if the array is full
	if(mi_count == mi_capacity)
	{
		//double the size
		if( !IncreaseCapacity() )
		{
			//failed to allocate new memory, exception time
			throw std::exception("Unable to allocate memory when increasing array size");
			return false;
		}
	}
	// array is no longer sorted after calling this function
	mb_sorted = false;
	//insert the new value at the end
	mv_keyValue[mi_count].key = newKey;
	mv_keyValue[mi_count].data = newData;
	mi_count++;
	return true;
}

//Puts data in the correct position in a sorted array, returns true on success
template <class KeyType, class DataType>
bool CVectorMap<KeyType, DataType>::InsertSortedData (KeyType newKey, DataType newData)
{
	uint32_t index;
	DataType data;
	//check the key exists
	if(  !is_pointer<KeyType>::value && FindData(newKey, &data, &index) )
	{
		//overwrite existing value
		mv_keyValue[index].data = newData;
		return true;
	}

	//if the array isn't sorted just bung it at the end
	if( !mb_sorted )
	{
		return InsertData ( newKey, newData );
	}

	//if the array is full
	if(mi_count == mi_capacity)
	{
		//double the size
		if( !IncreaseCapacity() )
		{
			//failed to allocate new memory, exception time
			throw std::exception("Unable to allocate memory when increasing array size");
		}
	}


	uint32_t i = 0;
	//scroll through the array until we hit the end or we find a space
	while( (i <= mi_count) && (newKey > mv_keyValue[i].key) )
	{
		i++;
	}

	//if we didn't hit the end, shuffle everything along. This will be expensive
	//but less expensive than calling quick sort on a nearly sorted array.
	if( i < mi_count )
	{
		//from the far end, shuffle everything along one.
		//by doing it from the end we can avoid using a place holder
		for(uint32_t j = mi_count; j > i; j--)
		{
			mv_keyValue[j].key  = mv_keyValue[j-1].key;
			mv_keyValue[j].data = mv_keyValue[j-1].data;
		}
	}

	//finally, insert the new data
	mv_keyValue[i].key  = newKey;
	mv_keyValue[i].data = newData;
	//and keep the count up to date
	mi_count++;

	return true;
}

//Gets the data and optionally the index associated with the given key.
//returns true on success
template <class KeyType, class DataType>
bool CVectorMap<KeyType, DataType>::FindData (KeyType searchKey, DataType* output, uint32_t* index)
{
	
	if(mi_count == 0)
	{
		return false;
	}

	//linear search
	for( uint32_t i = 0; i < mi_count; i++ )
	{
		//string pointers are different
		if( is_string_pointer< KeyType >::value )
		{
			std::string* myKey = reinterpret_cast<std::string*>(mv_keyValue[i].key);
			std::string* mySearchKey = reinterpret_cast<std::string*>(searchKey);
			if(  *myKey == *mySearchKey )
			{
				*output = mv_keyValue[i].data;
				if( index != NULL )
				{
					*index = i;
				}
				return true;
			}
		}
		//pointers need dereferencing
		else if( is_pointer< KeyType >::value )
		{
			KeyType* myKey = reinterpret_cast<KeyType*>(mv_keyValue[i].key);
			KeyType* mySearchKey = reinterpret_cast<KeyType*>(searchKey);
			if(  *myKey == *mySearchKey )
			{
				*output = mv_keyValue[i].data;
				if( index != NULL )
				{
					*index = i;
				}
				return true;
			}
		}
		//objects
		else
		{
			if(  mv_keyValue[i].key == searchKey )
			{
				*output = mv_keyValue[i].data;
				if( index != NULL )
				{
					*index = i;
				}
				return true;
			}
		}
	}

	return false;

}

//sorts the array by key
template <class KeyType, class DataType>
void CVectorMap<KeyType, DataType>::Sort()
{
	if( is_pointer< KeyType >::value )
	{
		//I can't sort pointers yet
		throw std::bad_cast("I can't sort pointers yet!");
	}

	if( mb_sorted || mi_count < 2 )
	{
		mb_sorted = true;
		return;
	}


	QuickSort( 0, mi_count-1 );

	mb_sorted = true;
}

//Displays everything in the array. Will choose the correct function depending on type
//workers are marked private to prevent direct calling
template <class KeyType, class DataType>
void CVectorMap<KeyType, DataType>::DisplayAll()
{
	if( is_pointer< KeyType >::value )
	{
		DisplayAllPP();
	}
	else
	{
		DisplayAllOO();
	}
}

//removes all elements from the array and resets the counter to 0
template <class KeyType, class DataType>
void CVectorMap<KeyType, DataType>::Clear()
{
	mi_count = 0;
	
	if( is_pointer<KeyType>::value )
	{
		for( uint32_t i = 0; i < mi_count; i++ )
		{
			delete reinterpret_cast<void*>(mv_keyValue[i].key);
		}
	}

	if( is_pointer<DataType>::value )
	{
		for( uint32_t i = 0; i < mi_count; i++ )
		{
			delete reinterpret_cast<void*>(mv_keyValue[i].data);
		}
	}
	
}

//returns true when the map is empty
template <class KeyType, class DataType>
bool CVectorMap<KeyType, DataType>::IsEmpty()
{
	return (mi_count == 0);
}

//swaps the position of the elements at the given indexes
template <class KeyType, class DataType>
void CVectorMap<KeyType, DataType>::SwapIndex(uint32_t a, uint32_t b)
{
	if( a > (mi_count-1) || b > (mi_count-1) )
	{
		//Index out of bounds
		throw std::out_of_range("Indices are out of range");
		return;
	}

	Pair<KeyType,DataType> temp;
	temp.key = mv_keyValue[a].key;
	temp.data = mv_keyValue[a].data;

	mv_keyValue[a].key = mv_keyValue[b].key;
	mv_keyValue[a].data = mv_keyValue[b].data;

	mv_keyValue[b].key = temp.key;
	mv_keyValue[b].data = temp.data;

}

//deletes the element at the given index
template <class KeyType, class DataType>
void CVectorMap<KeyType, DataType>::DeleteIndex(uint32_t index)
{
	if( index >= mi_count )
	{
		//index out of bounds
		throw std::out_of_range("Index larger than array size");
	}
	//if the container is holding pointers
	//free the memory
	if( is_pointer< KeyType >::value )
	{
		delete reinterpret_cast<void*>( mv_keyValue[index].key);
	}

	if( is_pointer< DataType >::value )
	{
		delete reinterpret_cast<void*>(mv_keyValue[index].data);
	}

	//if it's the last element in the array just clear it
	if( mi_count == 1 )
	{
		mv_keyValue[index].key = NULL;
		mv_keyValue[index].data = NULL;
	}

	for(uint32_t i = index; i < mi_count-1; i++)
	{
		mv_keyValue[i].key  = mv_keyValue[i+1].key;
		mv_keyValue[i].data = mv_keyValue[i+1].data;
	}

	mi_count--;

}

//deletes the element with the given key
template <class KeyType, class DataType>
bool CVectorMap<KeyType, DataType>::DeleteKey(KeyType key)
{
	uint32_t index;
	DataType data;
	//check the key exists
	if( !FindData(key, &data, &index) )
	{
		return false;
	}

	DeleteIndex(index);
	return true;
}

template <class KeyType, class DataType>
DataType CVectorMap<KeyType, DataType>::operator[] (KeyType &key)
{
	uint32_t index;
	DataType data;
	//check the key exists
	if( FindData(newKey, &data, &index) )
	{
		return data;
	}
	return NULL;
}

template <class KeyType, class DataType>
bool CVectorMap<KeyType, DataType>::SetCapacity(uint32_t newCapacity)
{
	//check to make sure the new capacity is not smaller than
	//the current data set.
	if( newCapacity < mi_count )
	{
		throw std::out_of_range("New size is smaller than dataset");
		return false;
	}

	//Increase capacity doubles capacity, so divide desired capacity by 2 and add 1 to ensure
	// at least the desired capacity is allocated
	mi_capacity = (newCapacity/2)+1;
	IncreaseCapacity();
}

///////////////////////////////////////
// PRIVATE
///////////////////////////////////////

//Doubles the capacity of the array, returns true on success
template <class KeyType, class DataType>
bool CVectorMap<KeyType, DataType>::IncreaseCapacity()
{
	//double the capacity
	mi_capacity *= 2;
	//allocate new memory
	Pair<KeyType, DataType>* newArray = ( Pair<KeyType, DataType>* ) malloc(sizeof( Pair<KeyType, DataType> ) * mi_capacity );
	//check for success
	if( newArray == NULL )
	{
		throw std::exception("Unable to allocate new memory");
		return false;
	}
	//copy the old data into the new array
	for( uint32_t i = 0; i <= mi_count; i++ )
	{
		newArray[i].key  = mv_keyValue[i].key;
		newArray[i].data = mv_keyValue[i].data;
	}
	//free the old array. 
	free (mv_keyValue);
	//store the new array
	mv_keyValue = newArray;
	return true;
}

template <class KeyType, class DataType>
void CVectorMap<KeyType,DataType>::QuickSort(uint32_t start, uint32_t end)
{
//debug code
#ifdef ___VERBOSE 
	std::cout << "QuickSort called " << start << " - " << end << std::endl;
#endif
	uint32_t range = end - start;

	//if 2 or fewer elements in the passed segment
	if( range == 0 )
	{
		return;
	}

	//2 elements left
	if(range == 1) 
	{
		//check if the two elements are sorted
		if( mv_keyValue[start].key > mv_keyValue[end].key )
		{
//debug code
#ifdef ___VERBOSE
			std::cout << "Final Swap between " << start << " - " << mv_keyValue[start].key << " and "  << end << " - " << mv_keyValue[end].key << std::endl;
#endif
			SwapIndex(start, end);
		}
		return;
	}

	//more than 2 elements left
	//pull out the pivotValue to make life easer
	KeyType pivotValue = mv_keyValue[end].key;
	uint32_t storeIndex = start;

	//loop up until the pivot
	for( uint32_t i = start; i < (end-1); i++ )
	{
		//if less than pivot, move to the begining of the array
		//increment storeIndex to the next element
		if( mv_keyValue[i].key < pivotValue )
		{
#ifdef ___VERBOSE
			std::cout << "Intermediate Swap between " << i << " - " << mv_keyValue[i].key << " and "  << storeIndex << " - " << mv_keyValue[storeIndex].key << std::endl;
#endif
			SwapIndex( i, storeIndex );
			storeIndex++;
		}
	}

	//move the pivot to it's correct position
	SwapIndex( storeIndex, end );

	//recurse on the low end - split to either side of the pivot
	if( start < storeIndex-1)
		QuickSort(start, storeIndex-1);
	//recurse on the upper end
	if( storeIndex+1 < end )
		QuickSort(storeIndex+1, end);
}

//Displays everything in the array assuming keys and values are both objects
template <class KeyType, class DataType>
void CVectorMap<KeyType, DataType>::DisplayAllOO()
{
	for( uint32_t i = 0; i < mi_count; i++ )
	{
		std::cout << "item " << i << " Key: " << mv_keyValue[i].key << " Data: " << mv_keyValue[i].data << std::endl;
	}
}

//Displays everything in the array assuming keys and values are both pointers
template <class KeyType, class DataType>
void CVectorMap<KeyType, DataType>::DisplayAllPP()
{
	for( uint32_t i = 0; i < mi_count; i++ )
	{
		std::cout << "Pointer item " << i << " Key: ";
		if( is_string_pointer< KeyType >::value )
		{
			std::string* myKey = reinterpret_cast<std::string*>(mv_keyValue[i].key);
			std::cout << *myKey;
		}
		else
		{
			KeyType* myKey = reinterpret_cast<KeyType*>(mv_keyValue[i].key);
			std::cout << *myKey;
		}

		if( is_string_pointer< DataType >::value )
		{
			std::string* myData = reinterpret_cast<std::string*>(mv_keyValue[i].data);
			std::cout << " Data: " << *myData << std::endl;
		}
		else
		{
			DataType* myData = reinterpret_cast<DataType*>(mv_keyValue[i].data);
			std::cout << " Data: " << *myData << std::endl;
		}
		
	}
}