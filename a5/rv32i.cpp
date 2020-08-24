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

/***************************************************************
Function: Constructor
Type: rv32i
Use: creates a pointer to a memory object
Parameters: 1. memory *m: memory pointer
Returns: nothing
***************************************************************/
rv32i::rv32i(memory *m){
	mem = m;
	pc = 0;
}

/***************************************************************
Function: run
Type: void
Use: loops through memory 4 by 4 bits
Parameters: none
Returns: nothing
***************************************************************/
void rv32i::run(uint32_t limit){
	regs.set(2, mem->get_size());
	while(insn_counter < limit && !halt){
		++insn_counter; 
		dump();
		uint32_t insn = mem->get32(pc);
		cout << hex32(pc) << ": ";
		cout << setw(35) << setfill(' ') << left << decode(insn) << " // ";
		cout << exec(insn) << endl;
	}
	cout << insn_counter << " instructions executed." << endl;
	dump();

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
	if(imm_i&0x800){ // sign - extend
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
	int32_t imm_u = insn &0xfffff000;
	return imm_u;
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
	uint32_t imm_i = get_imm_i(insn);
	std::ostringstream os;
	string pred;
	string succ;
	os << hex32(insn) << " " ; // the instruction hex value

	switch(get_opcode(insn)){
		 case 0b0110111: // LUI
		 os << " lui    x" << std::dec << get_rd(insn) << ",0x" << std::hex
		 << ((get_imm_u(insn) >> 12) &0xfffff) ;
			break;
			
		case 0b0010111: // AUIPC
		 os << " auipc  x" << std::dec << get_rd(insn) << ",0x" << std::hex
		 << ((get_imm_u(insn) >>12) &0xfffff) ;
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
			 
			 
		case 0b0010011: // I-type case 2
		
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

					imm_i = imm_i % XLEN;
						if(get_funct7(insn) == 0b0100000){
							os << " srai   ";
						}
						else{
							os << " srli   ";
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


/***************************************************************
Function: exec
Type: string
Use: returns an operation in the form of a string
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std :: string rv32i :: exec(uint32_t insn){
	uint32_t opcode = get_opcode ( insn );
	switch (opcode){

		case 0b0110111: return exec_lui(insn); //LUI
		case 0b0010111: return exec_auipc(insn); //AUIPC
		case 0b1101111: return exec_jal(insn); // JAL
		case 0b1100111: return exec_jalr(insn); // JALR
		
		case 0b1100011: // B-type
			//checks get_funct3 value
			switch(get_funct3(insn)){
				case 0b000: return exec_beq(insn); 	//BEQ
				case 0b001:	return exec_bne(insn);	//BNE
				case 0b100: return exec_blt(insn);	//BLT
				case 0b101: return exec_bge(insn);	//BGE
				case 0b110:	return exec_bltu(insn);	//BLTU
				case 0b111: return exec_bgeu(insn);	//BGEU
			}
			 break ;
			 
		case 0b0000011: // I-type
		//checks get_funct3 value
			switch(get_funct3(insn)){
				case 0b000: return exec_lb(insn);	//LB
				case 0b001: return exec_lh(insn);	//LH
				case 0b010: return exec_lw(insn);	//LW
				case 0b100: return exec_lbu(insn);	//LBU
				case 0b101: return exec_lhu(insn);	//LHU
			}
			 break ;
		
		
					 
		case 0b0010011: // I-type (2)
		//checks get_funct3 value
			switch(get_funct3(insn)){
				case 0b000: return exec_addi(insn);		//ADDI
				case 0b010: return exec_slti(insn);		//SLTI
				case 0b011: return exec_sltiu(insn);	//SLTIU
				case 0b100: return exec_xori(insn);		//XORI
				case 0b110: return exec_ori(insn);		//ORI
				case 0b111: return exec_andi(insn);		//ANDI
				case 0b001: return exec_slli(insn);		//SLLI
				case 0b101: 

                 switch(get_funct7(insn)){
                        case 0b0000000: return exec_srli(insn);		//SRLI
                        case 0b0100000: return exec_srai(insn);		//SRAI
                    }
				break;
			}
			break;
			
			
		case 0b0100011: // S - type store instructions
			 //checks get_funct3 value
			switch(get_funct3(insn)){
				case 0b000: return exec_sb(insn);	//SB	
				case 0b001: return exec_sh(insn);	//SH
				case 0b010: return exec_sw(insn);	//SW
			}
			break;
		
		case 0b0110011: // R-type
        //checks get_funct3 value
         switch(get_funct3(insn)){
            case 0b000:
            //checks get_funct7 value
                switch(get_funct7(insn)){
                    case 0b0000000: return exec_add(insn);	//ADD
                    case 0b0100000: return exec_sub(insn);	//SUB
                 }
            break;

            case 0b001: return exec_sll(insn);		//SLL
            case 0b010: return exec_slt(insn);		//SLT
            case 0b011: return exec_sltu(insn);		//SLTU
            case 0b100: return exec_xor(insn);		//XOR
            
			case 0b101:
            //checks get_funct7 value
                switch(get_funct7(insn)){
                    case 0b0000000: return exec_srl(insn);	//SRL
                    case 0b0100000: return exec_sra(insn);	//SRA
                }
                break;
            case 0b110: return exec_or(insn);	//OR
            case 0b111: return exec_and(insn);	//AND
        }
        break;

		case 0b0001111: return exec_fence(insn);
		
		case 0b1110011 : // ECALL / EBREAK
		// treat both ECALL and EBREAK the same way and just halt the simulator
		if(insn &0x00100000)
			return exec_ebreak(insn);
		else
			return exec_ebreak(insn);
		break;
	}
	pc += 4;		//INCREMENTS INSTRUCTION
	halt = true;	//SETS HALT TO TRUE
	return "ERRROR: UNIMPLEMENTED INSTRUCTION";
}


/**********************************************************= 0x0000016c, *****
Function: exec_ebreak
Type: string
Use: calls ebreak
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_ebreak(uint32_t insn){
	(void)insn; // shut up unused argument compiler warning
	halt = true; // stop the simulator
	return "HALT"; // the operation comment
}

/***************************************************************
Function: exec_lui
Type: string
Use: calls exec_lui
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_lui(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd ( insn ); // get the rd register number
	int32_t imm_u = get_imm_u ( insn ); // get the imm_u value
	regs.set(rd, imm_u); // store the imm_u value into rd
	pc += 4; // advance the pc past the end of this instruction

	// Return a string indicating the operation that has taken place
	os << std::dec << "x" << rd << " = " << hex0x32(imm_u);
	return os.str() ;
}

/***************************************************************
Function: exec_auipc
Type: string
Use: calls exec_auipc
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_auipc(uint32_t insn){
	std :: ostringstream os ;
	int32_t imm_u = get_imm_u(insn) + pc; 
	uint32_t rd = get_rd( insn );
	regs.set(rd, imm_u); 
	os << std::dec << "x" << rd << " = " << hex0x32(pc)<< " + " 
	<< hex0x32(get_imm_u(insn)) <<" = " << hex0x32(imm_u) ;
	pc += 4; 
	return os.str() ;
}

/***************************************************************
Function: exec_jal
Type: string
Use: calls exec_jal
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_jal(uint32_t insn)
{
    std :: ostringstream os ;
    uint32_t rd = get_rd(insn);			//rd 
    int32_t imm_j = get_imm_j(insn); 	//imm_j

    regs.set(rd, pc + 4); 
    pc += imm_j; 

    os << std::dec << "x" << rd << " = "<< hex0x32(regs.get(rd)) << ", ";
    os << " pc" << " = " << hex0x32(pc - imm_j);
    os <<" + " << hex0x32(imm_j);
    os << " = "<< hex0x32(pc);

    return os.str() ;
}

/***************************************************************
Function: exec_jalr
Type: string
Use: calls exec_jalr
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_jalr(uint32_t insn){
    std :: ostringstream os ;
	int32_t imm_i = get_imm_i(insn);		//imm_i
    uint32_t rs1 = regs.get(get_rs1(insn));	//rs1
    uint32_t rd = get_rd(insn);				//rd
    regs.set(rd, pc + 4);
    os << std::dec << "x" << rd << " = " << hex0x32(pc + 4) << ",  pc = ("  << hex0x32(imm_i) << " + "; 
    os << hex0x32(rs1) << ") & " << hex0x32(~1) << " = " << hex0x32(rs1 + imm_i);
    pc = (rs1 + imm_i);
    return os.str() ;
}

/***************************************************************
Function: exec_beq
Type: string
Use: calls exec_beq
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_beq(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	int32_t imm_b = get_imm_b(insn);		//imm_b
	if(rs1 == rs2){
		pc += imm_b;
	}else{
		pc +=4;
	}
	os << std::dec << "pc += (" << hex0x32(rs1) << " == " << hex0x32(rs2) << " ? " <<
	hex0x32(imm_b) << " : 4) = " << hex0x32(pc) ; 
	return os.str();
}

/***************************************************************
Function: exec_bne
Type: string
Use: calls exec_bne
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_bne(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	int32_t imm_b = get_imm_b(insn);		//imm_b
	if(rs1 != rs2){
		pc += imm_b;
	}else{
		pc += 4;
	}
	os << std::dec << "pc += (" << hex0x32(rs1)  << " != " << hex0x32(rs2) << " ? "
	<< hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

/***************************************************************
Function: exec_blt
Type: string
Use: calls exec_blt
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_blt(uint32_t insn){
	std :: ostringstream os ;				
	int32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	int32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	int32_t imm_b = get_imm_b(insn);		//imm_b
	if(rs1 < rs2){
		pc += imm_b;
	}else{
		pc += 4;
	}
	os << std::dec << "pc += (" << hex0x32(rs1) << " < " 
	<< hex0x32(rs2) << " ? " <<
	hex0x32(imm_b) << " : 4) = " << hex0x32(pc);
	return os.str();
}

/***************************************************************
Function: exec_bge
Type: string
Use: calls exec_bge
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_bge(uint32_t insn){
	std :: ostringstream os ;
	int32_t rs1 = regs.get(get_rs1(insn));	//rs1
	int32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	int32_t imm_b = get_imm_b(insn);		//imm_b
	
	if(rs1 >=rs2){
		pc += imm_b;
	}else{
		pc += 4;
	}
	os << std::dec << "pc += (" << hex0x32(rs1) << " >= " << hex0x32(rs2) << " ? " <<
	hex0x32(imm_b) << " : 4) = " << hex0x32(pc) ; 
	return os.str();
}

/***************************************************************
Function: exec_bltu
Type: string
Use: calls exec_bltu
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_bltu(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	int32_t imm_b = get_imm_b(insn);			//imm_b
	if(rs1 < rs2){
		pc += imm_b;
	}else{
		pc += 4;
	}	
	os << std::dec << "pc += (" << hex0x32(rs1) << " <U " << hex0x32(rs2) << " ? " <<
	hex0x32(imm_b) << " : 4) = " << hex0x32(pc) ; 
	return os.str();
}

/***************************************************************
Function: exec_bgeu
Type: string
Use: calls exec_bgue
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_bgeu(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	int32_t imm_b = get_imm_b(insn);			//imm_b
	if(rs1>= rs2){
		pc += imm_b;
	}else{
		pc += 4;
	}
	os << std::dec << "pc += (" << hex0x32(rs1) << " >=U " << hex0x32(rs2) << " ? " <<
	hex0x32(imm_b) << " : 4) = " << hex0x32(pc) ; 
	return os.str();
}

/***************************************************************
Function: exec_lb
Type: string
Use: calls exec_lb
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_lb(uint32_t insn)
{
    std :: ostringstream os ;
    int32_t rd = get_rd(insn);					//rd	
    int32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
    int32_t imm_i = get_imm_i(insn);			//imm_i
    int32_t Address = mem->get8(rs1 + imm_i);	//memory address
	string msbLB = std::bitset<8>(Address).to_string();
    
    //checks most significant bit
    if(msbLB[0]=='1'){
        Address = mem->get8(rs1 + imm_i) | 0xFFFFFF00;
	}

    regs.set(rd, Address);
    pc += 4;

    os << "x" << rd << " = sx(m8(" << hex0x32(rs1) << " + " << hex0x32(imm_i)
    << ")) = " << hex0x32(regs.get(rd));

    return os.str();
}

/***************************************************************
Function: exec_lh
Type: string
Use: calls exec_lh
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_lh(uint32_t insn)
{
    std :: ostringstream os ;
    uint32_t rd = get_rd(insn);				//rd
    uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
    uint32_t imm_i = get_imm_i(insn); 		//imm_i
    uint32_t address = (mem->get16(rs1+imm_i));
	string msb = std::bitset<8>(address).to_string();
    if(msb[0]=='1'){
        address = mem->get16(rs1 + imm_i) | 0xFFFF0000;
	}
    regs.set(rd, address);
    pc += 4;
    os << "x" << rd;
    os << " = sx(m16(" << hex0x32(rs1);
    os << " + " << hex0x32(imm_i);
    os << ")) = " << hex0x32(regs.get(rd));

    return os.str();
}


/***************************************************************
Function: exec_lw
Type: string
Use: calls exec_lw
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_lw(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	regs.set(rd, mem->get32(rs1+imm_i));
	pc += 4;
	os << "x" << rd << " = sx(m32(" << hex0x32(rs1) << " + " 
	<< hex0x32(imm_i) << ")) = " << hex0x32(regs.get(rd));
	return os.str();
}		

/***************************************************************
Function: exec_lbu
Type: string
Use: calls exec_lbu
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_lbu(uint32_t insn){
std :: ostringstream os ;
    uint32_t rd = get_rd(insn);					//rd
    uint32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
    uint32_t imm_i = get_imm_i(insn);			//imm_i

    regs.set(rd, mem->get8((rs1 + imm_i)));
    rd = mem->get8((rs1 + imm_i));
    pc += 4;

    os << "x" << get_rd(insn);
    os << " = zx(m8(" << hex0x32(rs1);
    os << " + " << hex0x32(imm_i);
    os << ")) = " << hex0x32(rd);

    return os.str();
}

/***************************************************************
Function: exec_lhu
Type: string
Use: calls exec_lhu
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_lhu(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	regs.set(rd, mem->get16((rs1 + imm_i)));
	os << "x" << rd << " = zx(m16(" << hex0x32(rs1) << " + " 
	<< hex0x32(imm_i) << ")) = " << hex0x32( mem->get16(rs1 + imm_i));
	pc += 4;
	return os.str();
}


/***************************************************************
Function: exec_sb
Type: string
Use: calls exec_sb
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sb(uint32_t insn){
	std :: ostringstream os ;
    uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
    uint32_t rs2 = regs.get(get_rs2(insn));	//rs2
    uint32_t imm_s = get_imm_s(insn); 		//imm_s
    mem->set8(rs1+imm_s, rs2 & 0x000000FF);
    pc += 4;

    os << "m8"<< "(" << hex0x32(rs1);
    os << " + " << hex0x32(imm_s);
    os << ") = " << hex0x32(rs2 & 0x000000FF);

    return os.str();
}

/***************************************************************
Function: exec_sh
Type: string
Use: calls exec_sh
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sh(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	uint32_t imm_s = get_imm_s(insn); 		//imm_s
	regs.set((rs1+imm_s), rs2 & 0xFFFF);
	pc += 4;
	os << "m16"<< "(" << hex0x32(rs1) << " + " << hex0x32(imm_s)
	<< ") = " << hex0x32(rs2 & 0xFFFF); 
	return os.str();
}

/***************************************************************
Function: exec_sw
Type: string
Use: calls exec_sw
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sw(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	uint32_t imm_s = get_imm_s(insn); 			//imm_s
	regs.set((rs1+imm_s), rs2 & 0xFFFFFFFF);
	pc += 4;
	os << "m32"<< "(" << hex0x32(rs1) << " + " << hex0x32(imm_s)
	<< ") = " << hex0x32(rs2 &0xFFFFFFFF); 
	return os.str();
}

/***************************************************************
Function: exec_addi
Type: string
Use: calls exec_addi
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_addi(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);					//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	uint32_t imm_i = get_imm_i(insn); 			//imm_i
	regs.set(rd,(rs1 + imm_i));
	os << "x" << rd << " = " << hex0x32(rs1) << " + " << hex0x32(imm_i)
	<< " = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_slti
Type: string
Use: calls exec_slti
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_slti(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	int32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	int32_t imm_i = get_imm_i(insn);		//imm_i
	if(rs1 < imm_i){
		regs.set(rd, 1);
	}
	else{
		regs.set(rd, 0);
	}
	os << "x" << rd << " = (" << hex0x32(rs1) << " < " << (imm_i)
	<< ") ? 1 : 0 = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_sltiu
Type: string
Use: calls exec_sltiu
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sltiu(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	if(rs1 < imm_i){
		regs.set(rd, 1);
	}
	else{
		regs.set(rd, 0);
	}
	os << "x" << rd << " = (" << hex0x32(rs1) << " <U " << imm_i
	<< ") ? 1 : 0 = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_xori
Type: string
Use: calls exec_xori
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_xori(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	regs.set(rd, (rs1 ^ imm_i));
	os << "x" << rd << " = " << hex0x32(rs1) << " ^ " << hex0x32(imm_i)
	<< " = "  << hex0x32(rs1 ^ imm_i); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_ori
Type: string
Use: calls exec_ori
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_ori(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	regs.set(rd, (rs1 | imm_i));
	os << "x" << rd << " = " << hex0x32(rs1) << " | " << hex0x32(imm_i)
	<< " = "  << hex0x32(rs1 | imm_i); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_andi
Type: string
Use: calls exec_andi
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_andi(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	regs.set(rd,(rs1 & imm_i));
	os << "x" << rd << " = " << hex0x32(rs1) << " & " << hex0x32(imm_i)
	<< " = " << hex0x32(rs1 & imm_i); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_slli
Type: string
Use: calls exec_slli
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_slli(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	regs.set(rd, rs1 << imm_i);
	os << "x" << rd << " = " << hex0x32(rs1) << " << " << imm_i
	<< " = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_srli
Type: string
Use: calls exec_srli
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/	
std::string rv32i::exec_srli(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t imm_i = get_imm_i(insn); 		//imm_i
	regs.set(rd, rs1 >> imm_i);
	os << "x" << rd << " = " << hex0x32(rs1) << " >> " << imm_i
	<< " = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
 }

/***************************************************************
Function: exec_srai
Type: string
Use: calls exec_srai
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_srai(uint32_t insn)
{	std :: ostringstream os ;
    int32_t rd = get_rd(insn);				//rd
    int32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
    int32_t imm_i = get_imm_i(insn); 		//imm_i
    regs.set(rd, rs1 >> imm_i);
    os << "x" << rd << " = " << hex0x32(rs1) << " >> " << (imm_i & 0x1f)
    << " = " << hex0x32(regs.get(rd)); 
    pc += 4;
    return os.str();

}

/***************************************************************
Function: exec_add
Type: string
Use: calls exec_add
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_add(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn));	//rs2
	regs.set(rd, (rs1 + rs2));
	os << "x" << rd << " = " << hex0x32(rs1) << " + " << hex0x32(rs2)
	<< " = " << hex0x32(rs1 + rs2); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_sub
Type: string
Use: calls exec_sub
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sub(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	regs.set(rd, (rs1 - rs2));
	os << "x" << rd << " = " << hex0x32(rs1) << " - " << hex0x32(rs2)
	<< " = " << hex0x32(rs1 - rs2); 
	pc += 4;
	return os.str();
}


/***************************************************************
Function: exec_sll
Type: string
Use: calls exec_sll
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sll(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	regs.set(rd, rs1 << (rs2 % XLEN));
	os << "x" << rd << " = " << hex0x32(rs1) << " << " << (rs2 % XLEN)
	<< " = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}


/***************************************************************
Function: exec_slt
Type: string
Use: calls exec_slt
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_slt(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	if(rs1 < rs2){
		regs.set(rd, 1);
	}
	else{
		regs.set(rd, 0);
	}
	os << "x" << rd << " = (" << hex0x32(rs1) << " < " << hex0x32(rs2) 
	<< ") ? 1 : 0 = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_sltu
Type: string
Use: calls exec_sltu
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sltu(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);					//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	if(rs1 < rs2){
		regs.set(rd, 1);
	}
	else{
		regs.set(rd, 0);
	}
	os << "x" << rd << " = (" << hex0x32(rs1) << " <U " << hex0x32(rs2) 
	<< ") ? 1 : 0 = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_xor
Type: string
Use: calls exec_xor
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_xor(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	regs.set(rd, (rs1 ^ rs2));
	os << "x" << rd << " = " << hex0x32(rs1) << " ^ " << hex0x32(rs2)
	<< " = "  << hex0x32(rs1 ^ rs2); 
	pc += 4;
	return os.str();
}


/***************************************************************
Function: exec_srl
Type: string
Use: calls exec_srl
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_srl(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);					//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	regs.set(rd, rs1 >> rs2);
	os << "x" << rd << " = " << hex0x32(rs1) << " >> " << (rs2%XLEN) 
	<< " = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_sra
Type: string
Use: calls exec_sra
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_sra(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	int32_t rs1 = regs.get(get_rs1(insn)); 	//rs1
	int32_t rs2 = regs.get(get_rs2(insn)); 	//rs2
	
	regs.set(rd, rs1 >> rs2);
	os << "x" << rd << " = " << hex0x32(rs1) << " >> " << (rs2%XLEN) 
	<< " = " << hex0x32(regs.get(rd)); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_or
Type: string
Use: calls exec_or
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_or(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	regs.set(rd, (rs1 | rs2));
	os << "x" << rd << " = " << hex0x32(rs1) << " | " << hex0x32(rs2)
	<< " = " << hex0x32(rs1 | rs2); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_and
Type: string
Use: calls exec_and
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_and(uint32_t insn){
	std :: ostringstream os ;
	uint32_t rd = get_rd(insn);				//rd
	uint32_t rs1 = regs.get(get_rs1(insn)); //rs1
	uint32_t rs2 = regs.get(get_rs2(insn)); //rs2
	regs.set(rd, (rs1 & rs2));
	os << "x" << rd << " = " << hex0x32(rs1) << " & " << hex0x32(rs2)
	<< " = " << hex0x32(rs1 & rs2); 
	pc += 4;
	return os.str();
}

/***************************************************************
Function: exec_fene
Type: string
Use: calls exec_fence
Parameters: 1. insn: instruction to be executed
Returns: string
***************************************************************/
std::string rv32i::exec_fence(uint32_t insn){
	(void) insn;
	std :: ostringstream os ;
	os << "fence";
	pc += 4;
	return os.str();
}

/***************************************************************
Function: dump
Type: void
Use: calls regs.dump
Parameters: none
Returns: nothing
***************************************************************/
void rv32i::dump() const{
	regs.dump();
	cout << " pc " << hex32(pc);
	cout << endl;
}



























