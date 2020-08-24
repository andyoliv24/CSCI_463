/**
 * rv32i.cpp
 * 
 * CSCI 463 Assignment 4
 *
 * Author: Andy Olivares
 * z-ID: z1860934
 * Date: 3/30/2020
 *
 * rv32i.cpp class definitions
 */  

#include "rv32i.h"
#include "hex.h"
#include "memory.h"

/***************************************************************
Function: Constructor
Type: rv32i
Use: creates a pointer to a memory object
Parameters: 1. memory *m: memory pointer
Returns: nothing
***************************************************************/
rv32i::rv32i(memory *m){
	mem = m;
}

/***************************************************************
Function: run
Type: void
Use: loops through memory 4 by 4 bits
Parameters: none
Returns: nothing
***************************************************************/
void rv32i::run(void){
	for(pc =0; pc<mem->get_size();){	//loops through memory
		cout << hex32(pc)<< ": " << decode(mem->get32(pc)) << endl;
		pc +=4 ;
	}
}

/***************************************************************
Function: get_opcode
Type: uint32_t 
Use: extracts opcode (6 to 0 bits)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: opcode (uint32_t)
***************************************************************/
uint32_t rv32i::get_opcode(uint32_t insn){
	//extracts 6 - 0 bits
	return insn & 0x7f;
}

/***************************************************************
Function: get_rd
Type: uint32_t 
Use: extracts rd (11 to 7 bits)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: rd (uint32_t)
***************************************************************/    
uint32_t rv32i::get_rd(uint32_t insn){
	return (insn & 0xf80) >> (7-0); //extracts 11 - 7 bits and shifts 7
}

/***************************************************************
Function: get_funct3
Type: uint32_t 
Use: extracts funct3 (14 to 12 bits)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: funct3 (uint32_t)
***************************************************************/
uint32_t rv32i::get_funct3(uint32_t insn){
	return (insn & 0x7000) >> (12); // extract & shift bits 14 -12
}

/***************************************************************
Function: get_rs1
Type: uint32_t 
Use: extracts rs1 (19 to 15 bits)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: rs1 (uint32_t)
***************************************************************/	
uint32_t rv32i::get_rs1(uint32_t insn){
	return (insn & 0xf8000) >> (15); // extract & shift bits  19-15
}

/***************************************************************
Function: get_rs2
Type: uint32_t 
Use: extracts rs2 (24 to 20 bits)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: rs2 (uint32_t)
***************************************************************/
uint32_t rv32i::get_rs2(uint32_t insn){
	return (insn & 0x1f00000) >> (20); // extract & shift bits 24 -20
}

/***************************************************************
Function: get_funct7
Type: uint32_t 
Use: extracts funct7 (31 to 25 bits)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: funct7 (uint32_t)
***************************************************************/		
uint32_t rv32i::get_funct7(uint32_t insn){
	return (insn & 0xfe000000) >> (25); // extract & shift bits 31-25
}

/***************************************************************
Function: get_imm_i
Type: int32_t 
Use: extracts imm_i (31 to 20 bits) then placed in bits 11:0
Parameters: 1. uint32_t insn: instruction to get information from
Returns: imm_i (int32_t)
***************************************************************/
int32_t rv32i::get_imm_i(uint32_t insn){
	int32_t imm_i = (insn & 0xfff00000) >> (20); // extract & shift bits 31 -20
	if(insn & 0x80000000){ // sign - extend
		imm_i |= 0xfffff000 ;
	}      
	 return imm_i;
}

/***************************************************************
Function: get_imm_u
Type: int32_t 
Use: extracts imm_u (31 to 12 bits) then placed in bits 31:12
Parameters: 1. uint32_t insn: instruction to get information from
Returns: imm_u (int32_t)
***************************************************************/		
int32_t rv32i::get_imm_u(uint32_t insn){
	return insn;
}


/***************************************************************
Function: get_imm_b
Type: int32_t 
Use: extracts imm_b (31 to 25, 11 to 7 bits) then placed
in bits 12(a), 10:5(bcdefg), 4:1(uvwx), 11(y) 
Parameters: 1. uint32_t insn: instruction to get information from
Returns: imm_b (int32_t)
***************************************************************/
int32_t rv32i::get_imm_b(uint32_t insn){
	int32_t imm_b = (insn & 0x80 ) << (4); // extract & shift bit 11
	imm_b |= (insn & 0xf00) >> (7); // extract & shift bits 4 -1
	imm_b |= (insn & 0x7e000000) >> (20); // extract & shift bits 10-5
	imm_b |= (insn & 0x80000000) >> (19); // // extract & shift bit 12
	
	if(insn & 0x80000000){ // sign - extend
		imm_b |= 0xffffe000 ;
	} 
      
	return imm_b;
}

/***************************************************************
Function: get_imm_s
Type: int32_t 
Use: extracts imm_s (31 to 25, 4 to 0 bits) then placed in bits
11:5(abcdefg), 4:0(uvwxy)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: imm_s (int32_t)
***************************************************************/	
int32_t rv32i::get_imm_s(uint32_t insn){
	int32_t imm_s = (insn & 0xfe000000) >> (25 -5) ; // extract & shift bits 11-5
	imm_s |= (insn & 0x00000f80) >> (7 -0) ; // extract & shift bits 0 -4

 	if(insn & 0x80000000){ // sign - extend
		imm_s |= 0xfffff000 ;
	}                              
	return imm_s ;
}

/***************************************************************
Function: get_imm_j
Type: int32_t 
Use: extracts imm_j (31 to 12 bits) then placed in bits 20(a),
10:1(bcdefghijk), 11(l), 19:12(mnopqrst)
Parameters: 1. uint32_t insn: instruction to get information from
Returns: imm_j (int32_t)
***************************************************************/

int32_t rv32i::get_imm_j(uint32_t insn){
	int32_t imm_j = (insn & 0xff000); // extract & shift bits 19-12
	imm_j |= (insn & 0x80000000) >> (11); // extract & shift bits 20
	imm_j |= (insn & 0x100000) >> (9); // extract & shift bits 10-1
	imm_j |= (insn & 0x7fe00000) >> (20); // extract & shift bit 11
	
	if(insn & 0x80000000){ // sign - extend
		imm_j |= 0xffe00000 ;
	} 
	return imm_j;
}

/***************************************************************
Function: decode
Type: int32_t 
Use: determines how to decode a instruction based on insn
Parameters: 1. uint32_t insn: used to determine instruction
Returns: string decoded instruction (int32_t)
***************************************************************/
std :: string rv32i :: decode ( uint32_t insn ) const{
	std::ostringstream os;
	string pred;
	string succ;
	os << hex32(insn) << " " ; // the instruction hex value

	switch(get_opcode(insn)){
		 case 0b0110111: // LUI
		 os << " lui    x" << std::dec << get_rd(insn) << ",0x" << std::hex
		 << ((get_imm_u(insn) >> 12) &0x0fffff ) ;
			break;
			
		case 0b0010111: // AUIPC
		 os << " auipc  x" << std::dec << get_rd(insn) << ",0x" << std::hex
		 << ((get_imm_u(insn) >> 12) &0x0fffff ) ;
			break;
			
		case 0b1101111: // JAL
		 os << " jal    x" << std::dec << get_rd(insn) << ",0x" << std::hex
		 << ((get_imm_j(insn)  + pc )) ;
			break;
			
		case 0b1100111: // JALR
		 os << " jalr   x" << std::dec << get_rd(insn) << "," <<  get_imm_i(insn) 
		 << "(x" << get_rs1(insn) << ")";
			break;
		
		
		case 0b1100011: // B-type
			//checks get_funct3 value
			switch(get_funct3(insn)){
				case 0b000: os << " beq    "; break;
				case 0b001: os << " bne    "; break;
				case 0b100: os << " blt    "; break;
				case 0b101: os << " bge    "; break;
				case 0b110: os << " bltu   "; break;
				case 0b111: os << " bgeu   "; break;
				default :
					 os << " ERROR ";
				return os.str();
			}
			os << "x" << std::dec << get_rs1(insn) << ",x" << get_rs2(insn) << "," << "0x" <<std::hex << (get_imm_b(insn) + pc);
			 break ;
			 
		
		case 0b0000011: // I-type
		//checks get_funct3 value
			switch(get_funct3(insn)){
				case 0b000: os << " lb     "; break;
				case 0b001: os << " lh     "; break;
				case 0b010: os << " lw     "; break;
				case 0b100: os << " lbu    "; break;
				case 0b101: os << " lhu    "; break;
				default :
					 os << " ERROR ";
				return os.str();
			}
				os << "x" << std::dec << get_rd(insn) << "," <<  get_imm_i(insn) << "(x" << get_rs1(insn) <<")";
			 break ;
			 
			 
		case 0b0010011: // I-type
		//checks get_funct3 value
			switch(get_funct3(insn)){
				case 0b000: os << " addi   "; break;
				case 0b010: os << " slti   "; break;
				case 0b011: os << " sltiu  "; break;
				case 0b100: os << " xori   "; break;
				case 0b110: os << " ori    "; break;
				case 0b111: os << " andi   "; break;
				case 0b001: os << " slli   "; break;
				case 0b101:
				//checks get_funct7 value
					switch(get_funct7(insn)){
						case 0b0000000: os << " srli   "; break;
						case 0b0100000: os << " srai   "; break;
						default :
							os << " ERROR ";
						return os.str();
					}
				break;
				default :
					 os << " ERROR ";
				return os.str();
			}
			if(get_funct3(insn) == 0b101 || get_funct3(insn ) == 0b001){
				os << "x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << "," << ( (insn & 0x1f00000) >> 20);
				break;
			}
			os << "x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << "," << get_imm_i(insn);
			
		break;
		
		
		 case 0b0100011: // S - type store instructions
		 //checks get_funct3 value
			 switch(get_funct3(insn)){
				 case 0b000: os << " sb     "; break ;
				 case 0b001: os << " sh     "; break ;
				 case 0b010: os << " sw     "; break ;
				 default :
					 os << " ERROR ";
				 return os.str();
			 }
			 os << "x" << std::dec << get_rs2(insn) << "," << get_imm_s(insn) << "(x" << get_rs1 ( insn ) <<")";
		break;
			 
			 	 
		case 0b0110011: // R-type
		//checks get_funct3 value
		 switch(get_funct3(insn)){
			case 0b000:
			//checks get_funct7 value
				switch(get_funct7(insn)){
					case 0b0000000: os << " add    "; break;
					case 0b0100000: os << " sub    "; break;
					default :
					 os << " ERROR ";
					return os.str();
 				}
 			break;
 			case 0b001: os << " sll    "; break;
 			case 0b010: os << " slt    "; break;
 			case 0b011: os << " sltu   "; break;
			case 0b100: os << " xor    "; break;
			case 0b101:
			//checks get_funct7 value
				switch(get_funct7(insn)){
					case 0b0000000: os << " srl    "; break;
					case 0b0100000: os << " sra    "; break;
					default :
					 os << " ERROR ";
					return os.str();
				}
				break;
			case 0b110: os << " or     "; break;
			case 0b111: os << " and    "; break;
		}
				os << "x" << std::dec << get_rd(insn) << ",x" << get_rs1(insn) << ",x" << get_rs2 ( insn );
		break;
			

		case 0b0001111: //fence operation
			//checks if 1 bit of pred is on
			if((insn &(1<<27))!= 0){
				pred += 'i';
			}
			//checks if 2 bit of pred is on
			if((insn &(1<<26))!= 0){
				pred += 'o';
			}
			//checks if 3 bit of pred is on
			if((insn &(1<<25))!= 0){
				pred += 'r';
			}
			//checks if 4 bit of pred is on
			if((insn &(1<<24))!= 0){
				pred += 'w';
			}
			//checks if 1 bit of succesor is on
			if((insn &(1<<23))!= 0){
				succ += 'i';
			}
			//checks if 2 bit of succesor is on
			if((insn &(1<<22))!= 0){
				succ += 'o';
			}
			//checks if 3 bit of succesor is on
			if((insn &(1<<21))!= 0){
				succ += 'r';
			}
			//checks if 4 bit of succesor is on
			if((insn &(1<<20))!= 0){
				succ += 'w';
			}
			os << " fence  " << pred << "," << succ;
			//std::hex << ((insn & 0xf000000) >> 24) 
		break;
		

		case 0b1110011:
			switch(get_funct7(insn) + get_rs2(insn)){
				case 0b000000000000: os << " ecall    "; break;
				case 0b000000000001: os << " ebreak    "; break;
				default :
					 os << " ERROR";
				return os.str();
			}
		break;
		
		 default :
		 os << " ERROR" ;
	}
	return os.str();
}
