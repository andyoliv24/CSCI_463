/**
 * memory.cpp
 * 
 * CSCI 463 Assignment 3
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 2/27/2020
 *
 * hex class definitions
 */  

#include "hex.h"
using namespace std;


/***************************************************************
Function: hex8
Type: string
Use: returns string with hex values
Parameters: 1. uint8_t: value to be printed
Returns: value string
***************************************************************/
std::string hex8(uint8_t i){
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(2) << static_cast<uint16_t>(i);
	return os.str();
}


/***************************************************************
Function: hex32
Type: string
Use: returns string with hex values
Parameters: 1. uint32_t: value to be printed
Returns: value string
***************************************************************/
std::string hex32(uint32_t i){
	std::ostringstream os;
	os << std::hex << std::setfill('0') << std::setw(8) << static_cast<uint32_t>(i);
	return os.str();
}


/***************************************************************
Function: hex0x32
Type: string
Use: returns string with hex values
Parameters: 1. uint32_t: value to be printed
Returns: value string
***************************************************************/
std::string hex0x32(uint32_t i){
	return std::string("0x")+hex32(i);
}
