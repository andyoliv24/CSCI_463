/**
 * registerfile.cpp
 * 
 * CSCI 463 Assignment 5
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 4/16/2020
 *
 * registerfile.cpp class definitions
 */  

#include "registerfile.h"



/***************************************************************
Function: Constructor
Type: register File
Use: initializes vector 0xf0f0f0f0 and sets reg 0 to 0x0
Parameters: none
Returns: nothing
***************************************************************/
registerfile::registerfile(){
	//registers.resize(32, 0xf0f0f0f0);
	for(uint32_t i =0; i<registers.size(); i++){
		registers[i] = 0xf0f0f0f0;
	}
	registers[0] = 0x0;
}


/***************************************************************
Function: set
Type: void
Use: sets register r to val
Parameters: none
Returns: nothing
***************************************************************/
void registerfile::set(uint32_t r, int32_t val){
	if( r != 0){
        registers[r] = val;
	}
}


/***************************************************************
Function: get
Type: int32_t
Use: gets register r
Parameters: none
Returns: nothing
***************************************************************/
int32_t registerfile::get(uint32_t r) const{
	//if register is equal to 0 return 0
	if(r == 0){
		return 0;
	}
	return registers[r];
}

/***************************************************************
Function: Constructor
Type: void
Use: dumps regs in vector
Parameters: none
Returns: nothing
***************************************************************/
void registerfile::dump() const{
	int32_t counter = 0;
	string space2 = " ";
	for(uint32_t i=0; i<registers.size(); ++i){
		if(i > 9) {
			space2 = "";
		}
		if(counter == 8){
			cout << endl;
			counter = 0;
		}
		if((i% 8) == 0){
			cout << space2 << "x" << i << " ";
		}
		counter++;
		if(counter % 8 == 0){
			cout  << hex32(registers[i]);
		}else{
			cout  << hex32(registers[i]) << " ";
		}
	}
	cout << endl;
}
