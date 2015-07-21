// CO3402 Assignment.cpp : Defines the entry point for the console application.
//

///////////////////////////////////////
// INCLUDES
///////////////////////////////////////

#include "stdafx.h"

#include <stdint.h>   //fixed definition variables
#include <iostream>	  //cout
#include <string>     //strings
#include <map>        //STL map for comparison
#include <stdexcept>  //Exceptions
#include <iomanip>    //set precision
#include <Windows.h>

//#include "CTimer.h"  // Timer class

#include "VectorMap.h"
#include "VectorMap.cpp"

///////////////////////////////////////
// CONSTS
///////////////////////////////////////

const uint32_t NUM_INSERTS = 1000000;
const uint32_t NUM_LOOKUPS = 1000;
const uint32_t NUM_REMOVE = 1000;
const uint32_t NUM_SORTS = 10000;

const uint32_t NUM_TESTS = 3;

///////////////////////////////////////
// MAPS
///////////////////////////////////////

CVectorMap<int32_t, int32_t> myMapIntInt;
CVectorMap<std::string*, std::string*> myMapStringString;
CVectorMap<int32_t*, int32_t*> myMapIntpIntp;


///////////////////////////////////////
// MAIN ENTRY POINT
///////////////////////////////////////
int _tmain(int argc, _TCHAR* argv[])
{
	///////////////////////////////////////
	// MAP ONE
	///////////////////////////////////////
	std::cout << "New map created of type int:int, starting capacity: " << myMapIntInt.GetCapacity() << std::endl;

	
	myMapIntInt.InsertData(29,101);
	myMapIntInt.InsertData(19,102);
	myMapIntInt.InsertData(3,103);
	myMapIntInt.InsertData(7,104);
	myMapIntInt.InsertData(11,105);
	myMapIntInt.InsertData(37,106);
	myMapIntInt.InsertData(5,107);
	myMapIntInt.InsertData(17,108);
	myMapIntInt.InsertData(1,109);
	myMapIntInt.InsertData(23,110);
	myMapIntInt.InsertData(13,111);
	myMapIntInt.InsertData(31,112);
	myMapIntInt.InsertData(2,113);

	std::cout << "Map now contains " <<  myMapIntInt.GetCount() << " items and has a capacity of " << myMapIntInt.GetCapacity() << "." << std::endl;

	myMapIntInt.DisplayAll();

	std::cout << "Sorting the array" << std::endl;

	myMapIntInt.Sort();

	myMapIntInt.DisplayAll();

	std::cout << "Inserting 20, 114 as sorted data. Array should still be sorted afterwards" << std::endl;

	myMapIntInt.InsertSortedData(20, 114);

	myMapIntInt.DisplayAll();

	std::cout << "Deleting index 10." << std::endl;

	myMapIntInt.DeleteIndex(10);

	myMapIntInt.DisplayAll();

	//std::cout << "Data with key 109 is: " << myMapIntInt[109] << std::endl;

	int32_t data;
	uint32_t index;

	if( myMapIntInt.FindData(11, &data, &index) )
	{
		std::cout << "The key '11' holds the data " << data << " and is at index " << index << "." << std::endl;
	}
	else
	{
		std::cout << "Failed to find the data associated with key '11' " << std::endl;
	}

	system("pause");

	///////////////////////////////////////
	// MAP TWO
	///////////////////////////////////////

	std::cout << "New map created of type int*:int* with a starting capacity of " << myMapIntpIntp.GetCapacity() << std::endl;

	myMapIntpIntp.InsertData( new int(1), new int(1) );
	myMapIntpIntp.InsertData( new int(2), new int(2) );
	myMapIntpIntp.InsertData( new int(3), new int(3) );
	myMapIntpIntp.InsertData( new int(5), new int(5) );
	myMapIntpIntp.InsertData( new int(7), new int(7) );
	myMapIntpIntp.InsertData( new int(11), new int(11) );


	myMapIntpIntp.DisplayAll();
	

	int32_t** data2 = new int32_t*;
	int32_t key = 7;
	uint32_t index2;

	if( myMapIntpIntp.FindData( &key, data2, &index2 ) )
	{
		std::cout << "The key '7' holds the data " << **data2 << " and is at index " << index2 << "." << std::endl;
	}
	else
	{
		std::cout << "Failed to find the data associated with key '7' " << std::endl;
	}

	std::cout << "Is the map empty? " << myMapIntpIntp.IsEmpty() << std::endl;

	std::cout << "Clearing the map..." << std::endl;

	myMapIntpIntp.Clear();

	std::cout << "Is the map empty? " << myMapIntpIntp.IsEmpty() << std::endl;

	system("pause");

	///////////////////////////////////////
	// MAP THREE
	///////////////////////////////////////

	std::cout << "New map created of type string:string, starting capacity: " << myMapStringString.GetCapacity() << std::endl;

	myMapStringString.InsertData(new std::string("Emergency"), new std::string("0118 999 881 999 119 725        3") );
	myMapStringString.InsertData(new std::string("23 Fake Street"), new std::string("555-1234"));

	myMapStringString.DisplayAll();

	std::string* search = new std::string("Emergency");
	std::string* output = new std::string;
	if( myMapStringString.FindData( search, &output, &index) )
	{
		std::cout << "The key 'Emergency' holds the data " << *output << " and is at index " << index << "." << std::endl;
	}
	else
	{
		std::cout << "Could not retreive the emergency number. Oh no!" << std::endl;
	}

	system("pause");

	std::cout << "***********************************************" << std::endl;
	std::cout << "** Beginging Performance Tests               **" << std::endl;
	std::cout << "***********************************************" << std::endl;

	CVectorMap<int32_t, int32_t> myMapIntInt2;

	std::map<int32_t, int32_t> stlMapIntInt;
	std::pair<std::string, std::string> stlPair;

	uint32_t startTime, endTime;

	std::cout << "Performance test inserting into my map (ETA 30s)" << std::endl;
	startTime = GetTickCount();
	for(uint32_t i = 0; i < NUM_TESTS; i++ )
	{
		myMapIntInt.Clear();
		for(uint32_t j = 0; j < NUM_INSERTS; j++)
		{
			int32_t key = rand();
			int32_t data = rand();
			if(!myMapIntInt2.InsertData(key, data) )
			{
				std::cout << "Insert Failure!";
				break;
			}
		}
	}
	endTime = GetTickCount();
	std::cout << "Test ";
	std::cout << ": ";
	std::cout << (endTime - startTime);
	std::cout << " milliseconds";
	std::cout << std::endl;


	std::cout << "Performance test inserting into STL map (ETA 10s)" << std::endl;
	startTime = GetTickCount();
	for(uint32_t i = 0; i < NUM_TESTS; i++ )
	{
		stlMapIntInt.clear();
		for(uint32_t j = 0; j < NUM_INSERTS; j++)
		{
			int32_t key = rand();
			int32_t data = rand();
			 stlMapIntInt.insert( std::pair<int32_t, int32_t>(key, data) );
		}
		
	}
	endTime = GetTickCount();
	std::cout << "Test ";
	std::cout << ": ";
	std::cout << (endTime - startTime);
	std::cout << " milliseconds";
	std::cout << std::endl;

// tempramental for some reason, works 50-50. 
/*	std::cout << "Performance test sorting my map" << std::endl;
	startTime = GetTickCount();
	myMapIntInt2.Sort();
	endTime = GetTickCount();
	std::cout << "Test ";
	std::cout << ": ";
	std::cout << (endTime - startTime);
	std::cout << " milliseconds";
	std::cout << std::endl;
	*/
	system("pause");
	return 0;
}

