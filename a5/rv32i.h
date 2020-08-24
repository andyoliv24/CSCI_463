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
#include "registerfile.h"
#include "hex.h"


class rv32i{
	
	public:
		rv32i(memory *m);
		void run(uint32_t limit);
		void dump() const;
		std::string exec(uint32_t insn);
		std::string exec_lui(uint32_t insn);
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
		
		std::string exec_ebreak(uint32_t insn);
		std::string exec_auipc(uint32_t insn);
		std::string exec_jal(uint32_t insn);
		std::string exec_jalr(uint32_t insn);
        std::string exec_beq(uint32_t insn);
        std::string exec_bne(uint32_t insn);
        std::string exec_blt(uint32_t insn);
        std::string exec_bge(uint32_t insn);
        std::string exec_bltu(uint32_t insn);
        std::string exec_bgeu(uint32_t insn);
        std::string exec_lb(uint32_t insn);
        std::string exec_lh(uint32_t insn);
        std::string exec_lw(uint32_t insn);
        std::string exec_lbu(uint32_t insn);
        std::string exec_lhu(uint32_t insn);
        std::string exec_sb(uint32_t insn);
        std::string exec_sh(uint32_t insn);
        std::string exec_sw(uint32_t insn);
        std::string exec_addi(uint32_t insn);
        std::string exec_slti(uint32_t insn);
        std::string exec_sltiu(uint32_t insn);
        std::string exec_xori(uint32_t insn);
        std::string exec_ori(uint32_t insn);
        std::string exec_andi(uint32_t insn);
        std::string exec_slli(uint32_t insn);
        std::string exec_srli(uint32_t insn);
        std::string exec_srai(uint32_t insn);
        std::string exec_add(uint32_t insn);
        std::string exec_sub(uint32_t insn);
        std::string exec_sll(uint32_t insn);
        std::string exec_slt(uint32_t insn);
        std::string exec_sltu(uint32_t insn);
        std::string exec_xor(uint32_t insn);
        std::string exec_srl(uint32_t insn);
        std::string exec_sra(uint32_t insn);
        std::string exec_or(uint32_t insn);
        std::string exec_and(uint32_t insn);
        std::string exec_fence(uint32_t insn);

	private:
		memory * mem;	//pointer to memory
		uint32_t pc = {0};	// will hold memory address
		static constexpr uint32_t XLEN = 32;	//buffer
		registerfile regs;
		bool halt = { false };
		uint64_t insn_counter = { 0 };

};

#endif
