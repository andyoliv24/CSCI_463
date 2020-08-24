/**
 * hex.h
 * 
 * CSCI 463 Assignment 3
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 2/27/2020
 *
 * hex class definition
 */  

#ifndef HEX_H
#define HEX_H

#include <string>
#include <stdint.h>
#include <iostream>
#include <iomanip>
#include <sstream>

std::string hex8(uint8_t i);		//hex8 prototype
std::string hex32(uint32_t i);		//hex32 prototype
std::string hex0x32(uint32_t i);	//hex0x32 prototype

#endif
