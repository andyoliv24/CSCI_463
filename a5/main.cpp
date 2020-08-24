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
#include "rv32i.h"
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



int main ( int argc , char ** argv ){
	if(argc != 4)
		usage();

	memory mem(stoul(argv[1], 0, 16));

	if(!mem.load_file(argv[3]))
		usage();

	rv32i sim(&mem);
	sim.run(stoul(argv[2]));
	mem.dump();

	return 0;
}
