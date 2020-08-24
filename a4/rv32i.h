/**
 * rv32i.h
 * 
 * CSCI 463 Assignment 4
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 3/30/2020
 *
 * Purpose: rv32i class definition
 */ 

#ifndef RV32I_H
#define RV32I_H

#include "memory.h"

class rv32i{
	
	public:
		rv32i(memory *m);
		void run(void);
		static uint32_t get_opcode(uint32_t insn);  //get_opcode prototype
		static uint32_t get_rd(uint32_t insn);		//ger_rd prototype
		static uint32_t get_funct3(uint32_t insn);	//get_funct3 prototype
		static uint32_t get_rs1(uint32_t insn);		//get_rs1 prototype
		static uint32_t get_rs2(uint32_t insn);		//get_rs2 prototype
		static uint32_t get_funct7(uint32_t insn);	//get_funct7 prototype
		static int32_t get_imm_i(uint32_t insn);	//get_imm_i prototype
		static int32_t get_imm_u(uint32_t insn);	//get_imm_u prototype
		static int32_t get_imm_b(uint32_t insn);	//get_imm_b prototype
		static int32_t get_imm_s(uint32_t insn);	//get_imm_s prototype
		static int32_t get_imm_j(uint32_t insn);	//get imm_j prototype
		std::string decode(uint32_t insn) const;	//decode prototype


	private:
		memory * mem;	//pointer to memory
		uint32_t pc;	// will hold memory address
		static constexpr uint32_t XLEN = 32;	//buffer
};

#endif
