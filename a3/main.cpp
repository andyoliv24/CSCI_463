/**
 * main.cpp
 * 
 * CSCI 463 Assignment 3
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 2/27/2020
 *
 *This program reads in a word, converts it into binary and
 * shows how it is being stored showing memory segments.
 */  


#include "hex.h"
#include "memory.h"
using namespace std;


/***************************************************************
Function: usage
Type: void
Use: tells user how to use the program
Parameters: none
Returns: nothing
***************************************************************/
void usage(){
	cout << "Usage: program [DecimalNumber] [file]" << endl;
	exit(0);
}



int main (int argc, char ** argv){
	
	
	if (argc!= 3){
		usage();
	}

	memory mem(stoul(argv[1],0,16));
	mem.dump();

	if (!mem.load_file( argv [2])){
		usage ();
	}
	
	mem.dump();

	cout << mem.get_size()<< endl ;
	cout << hex32(mem.get8(0)) <<  endl ;
	cout << hex32 (mem.get16(0)) << endl ;
	cout << hex32(mem.get32 (0)) << endl ;
	cout << hex0x32(mem.get8(0)) << endl ;
	cout << hex0x32(mem.get16(0)) << endl ;
	cout << hex0x32(mem.get32(0)) << endl ;
	cout << hex8(mem.get8(0)) << endl ;
	cout << hex8(mem.get16(0)) << endl ;
	cout << hex8(mem.get32(0)) << endl ;

	cout << hex0x32(mem.get32(0x1000)) << endl ;

	mem.set8(0x10, 0x12);
	mem.set16(0x14, 0x1234);
	mem.set32(0x18, 0x87654321);
	mem.dump ();
 
	return 0;
}
