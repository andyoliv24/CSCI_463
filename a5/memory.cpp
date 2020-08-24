/**
 * memory.cpp
 * 
 * CSCI 463 Assignment 3
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 2/27/2020
 *
 * memory class definitions
 */  

#include "memory.h"
#include "hex.h"


/***************************************************************
Function: Constructor
Type: memory
Use: initializes vector to 0xa5
Parameters: 1. uint32_t siz: used to determined vectors size
Returns: nothing
***************************************************************/
memory::memory(uint32_t siz){
	size = (siz+15)&0xfffffff0;
	
	//resizes vector to size value and sets every element to 0xa5
	mem.resize(size, 0xa5);
}

/***************************************************************
Function: Destructor
Type: memory
Use: calls destructor for vector
Parameters: none
Returns: nothing
***************************************************************/
memory::~ memory(){
	//destructor
	mem.clear();
}


/***************************************************************
Function: check_address
Type: bool
Use: checks if address is valid
Parameters: 1. uint32_t i: address to be checked
Returns: true/false
***************************************************************/
bool memory::check_address(uint32_t i) const{
	//checks if i is less or equal to size
	if(i < size){
		return true;
	}
	//print warning if address out of range
	else{
		cout << "WARNING: Address out of range: " << hex0x32(i) << endl;
		return 0;
	}
}	
	
	
/***************************************************************
Function: get_size
Type: uint32_t
Use: returns size
Parameters: none
Returns: size(uint32_t)
***************************************************************/
uint32_t memory::get_size() const{
	return size;
}


/***************************************************************
Function: get8
Type: uint8_t
Use: returns value in a given address
Parameters: 1. uint32_t: used for getting value in a certain address
Returns: value in index(address) uint8_t
***************************************************************/
uint8_t memory::get8(uint32_t addr) const{
	//checks if check address is true
	if(check_address(addr)){
		return mem[addr];
	}
	else{
		return 0;
	}
}
	

/***************************************************************
Function: get16
Type: uint16_t
Use: returns value in a given address (little endiend format)
Parameters: 1. uint16_t: used for getting value in a certain address
Returns: value in little endiend format
***************************************************************/
uint16_t memory::get16(uint32_t addr) const{
	uint16_t sum;
	//sets sum to the addresses in little endiend format
	sum = (get8(addr) | get8(addr+1) << 8);
	return sum;
}


/***************************************************************
Function: get32
Type: uint32_t
Use: returns value in a given address (little endiend format)
Parameters: 1. uint32_t: used for getting value in a certain address
Returns: value in little endiend format
***************************************************************/
uint32_t memory::get32(uint32_t addr) const{
	uint32_t sum;
	//sets sum to the addresses in little endiend format
	sum = get16(addr) | get16(addr+2) << 16;
	return sum;
}


/***************************************************************
Function: set8
Type: void
Use: sets val to the given address
Parameters: 1. uint32_t: used to determined the address
* 			2. uint8_t: value to be stored
Returns: nothing
***************************************************************/
void memory::set8(uint32_t addr, uint8_t val){
	//checks if address is valid
	if(check_address(addr)){
		mem[addr] = val;
	}
}


/***************************************************************
Function: set16
Type: void
Use: sets val to the given address
Parameters: 1. uint32_t: used to determined the address
* 			2. uint16_t: value to be stored (little endiend)
Returns: nothing
***************************************************************/
void memory::set16(uint32_t addr, uint16_t val){
	//shifts value 8 bits address and increments address
	set8(addr+1, val>>8);
	set8(addr, val);
	mem[addr] = val;
}


/***************************************************************
Function: set32
Type: void
Use: sets val to the given address
Parameters: 1. uint32_t: used to determined the address
* 			2. unint32_t: value to be stored (little endiend)
Returns: nothing
***************************************************************/
void memory::set32(uint32_t addr, uint32_t val){
	//shifts value 8 bits, 16 bits and increments address
	set16(addr+1, val>>8);
	set16(addr+2, val>>16);
	mem[addr] = val;
}


/***************************************************************
Function: dump
Type: void
Use: prints out every single bit, address, warnings
Parameters: none
Returns: nothing
***************************************************************/
void memory::dump() const{

	char ascii[17];
	ascii [16] = 0;
	for(uint32_t i=0; i<size; i++){
		if(i%16 == 0){
			if(i !=0 )
				cout << " *" << ascii << "*" << endl;
			cout << hex32(i) << ":";
		}
		uint8_t ch = get8(i);
		cout << (i% 16 == 8?"  ":" ")<< hex8(ch);
		ascii[i%16] = isprint(ch)?ch:'.';
	}
	cout << " *" << ascii << "*" << endl;
}


/***************************************************************
Function: load_file
Type: bool
Use: reads in file
Parameters: 1. const string& fname: file to be read
Returns: nothing
***************************************************************/
bool memory::load_file(const string& fname){
	char temp;
	int counter=0;
	ifstream infile(fname, ios::in|ios::binary);
	//checks if file exists or can be opened
	if(!infile){
		cout << "Can't open file " << fname << " for reading" << endl;
		return false;
	}
	//reads in bit by bit
	while(infile.get(temp)){
		//checks if address is ok
		if(check_address(counter)){
			//sets value of temp into vector index (counter)
			mem[counter] = temp;
		}
		else{
			cerr << "Program too big." <<  endl;
			return false;
		}
		counter++;
	
	}
	return true;
}



