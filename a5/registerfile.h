/**
 * registerfile.h
 * 
 * CSCI 463 Assignment 5
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 4/16/2020
 *
 * Purpose: registerfile class definition
 */ 

#ifndef REGISTERFILE_H
#define REGISTERFILE_H
#include "memory.h"
#include "hex.h"
#include <iostream> 
#include <vector>
#include <string>

class registerfile{
	
	public:
		registerfile();						//register file constructor
		void set(uint32_t r, int32_t val);	//set prototype
		int32_t get(uint32_t r) const;		//get prototype
		void dump() const;					//dump prototype


	private:
		//holds registers
		vector<int32_t> registers = vector <int32_t>(32);

};

#endif


