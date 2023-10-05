#include <stdio.h>	/* fprintf(), printf() */
#include <stdlib.h>	/* atoi() */
#include <stdint.h>	/* uint32_t */

#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"

int main(int argc, char * argv[]) {

	/*
	 * This variable will store the maximum
	 * number of instructions to run before
	 * forcibly terminating the program. It
	 * is set via a command line argument.
	 */
	uint32_t MaxInstructions;

	/*
	 * This variable will store the address
	 * of the next instruction to be fetched
	 * from the instruction memory.
	 */
	uint32_t ProgramCounter;

	/*
	 * This variable will store the instruction
	 * once it is fetched from instruction memory.
	 */
	uint32_t CurrentInstruction;

	//IF THE USER HAS NOT SPECIFIED ENOUGH COMMAND LINE ARUGMENTS
	if(argc < 3){

		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Input argument missing!\n");
		fprintf(stderr, "Expected: file-name, max-instructions\n");
		return -1;

	}

     	//CONVERT MAX INSTRUCTIONS FROM STRING TO INTEGER	
	MaxInstructions = atoi(argv[2]);	

	//Open file pointers & initialize Heap & Regsiters
	initHeap();
	initFDT();
	initRegFile(0);

	//LOAD ELF FILE INTO MEMORY AND STORE EXIT STATUS
	int status = LoadOSMemory(argv[1]);

	//IF LOADING FILE RETURNED NEGATIVE EXIT STATUS
	if(status < 0){ 
		
		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Unable to open file at %s!\n", argv[1]);
		return status; 
	
	}

	printf("\n ----- BOOT Sequence ----- \n");
	printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", exec.GSP, exec.GP, exec.GPC_START);

	RegFile[28] = exec.GP;
	RegFile[29] = exec.GSP;
	RegFile[31] = exec.GPC_START;

	printRegFile();

	printf("\n ----- Execute Program ----- \n");
	printf("Max Instruction to run = %d \n",MaxInstructions);
	fflush(stdout);
	ProgramCounter = exec.GPC_START;
	
	/***************************/
	/* ADD YOUR VARIABLES HERE */
	/***************************/
	const int32_t R_type = 0; //(31:26 bits = 0)
	const int32_t sll = 0; //(5:0 bits = 0)
	const int32_t srl = 0x00000002; //(5:0 bits = 000010)
	const int32_t sra = 0x00000003; //(5:0 bits = 000011)
	const int32_t sllv = 0x00000004; //(5:0 bits = 000100)
	const int32_t srlv = 0x00000006; //(5:0 bits = 000110)
	const int32_t srav = 0x00000007; //(5:0 bits = 000111)
	const int32_t jump_register = 0x00000008; //(5:0 bits = 001000)
	const int32_t jalr = 0x00000009; //(5:0 bits = 001001)
	const int32_t syscall = 0x0000000C; //(5:0 bits = 001100)
	const int32_t break_ = 0x0000000D; //(5:0 bits = 001101)
	const int32_t mfhi = 0x00000010; //(5:0 bits = 010000)
	const int32_t mthi = 0x00000011; //(5:0 bits = 010001)
	const int32_t mflo = 0x00000012; //(5:0 bits = 010010)
	const int32_t mtlo = 0x00000013; //(5:0 bits = 010011)
	const int32_t mult = 0x00000018; //(5:0 bits = 011000)
	const int32_t multu = 0x00000019; //(5:0 bits = 011001)
	const int32_t div = 0x000000001A; //(5:0 bits = 011010)
	const int32_t divu = 0x000000001B; //(5:0 bits = 011011)
	const int32_t add = 0x00000020; //(5:0 bits = 100000)
	const int32_t addu = 0x00000021; //(5:0 bits = 100001)
	const int32_t sub = 0x00000022; //(5:0 bit = 100010)
	const int32_t subu = 0x00000023; //(5:0 bits = 100011)
	const int32_t and = 0x00000024; //(5:0 bits = 100100)
	const int32_t or = 0x00000025; //(5:0 bits = 100101)
	const int32_t xor = 0x00000026; //(5:0 bits = 100110)
	const int32_t nor = 0x00000027; //(5:0 bits = 100111)
	const int32_t set_less_than = 0x0000002A; //(5:0 bits 101010)
	const int32_t set_less_thanu = 0x0000002B; // (5:0 bits 101011)

	const int32_t bltz_gez = 0x04000000; //(31:26 bits 000001)
	const int32_t jump = 0x08000000; //(31:26 bits 000010)
	const int32_t jump_link = 0xC000000; //(31:26 bits 000011)
	const int32_t branch_equal = 0x10000000; //(31: 26 bits 000100)
	const int32_t branch_not_equal = 0x14000000; //(31:26 bits 000101)
	const int32_t blez = 0x18000000; //(31:26 bits 000110)
	const int32_t bgtz = 0x1C000000; //(31:26 bits 000111)
	const int32_t addi = 0x20000000; //(31:26 bits 001000)
	const int32_t addiu = 0x24000000; //(31:26 bits 001001)
	const int32_t set_less_than_i = 0x28000000; //(31:26 bits 001010)
	const int32_t set_less_than_iu = 0x2C000000; //(31:26 bits 001011)
	const int32_t andi = 0x30000000; //(31:26 bits 001100)
	const int32_t ori = 0x34000000; //(31:26 bits 001101)
	const int32_t xori = 0x38000000; //(31:26 bits 001110)
	const int32_t lui = 0x3C000000; //(31:26 bits 001111)
	const int32_t load_byte = 0x80000000; //(31:26 bits 100000)
	const int32_t load_half = 0x84000000; //(31:26 bits 100001)
	const int32_t lwl = 0x88000000; //(31:26 bits 100010)
	const int32_t load_word = 0x8C000000; //(31:26 bits 100011)
	const int32_t load_byteu = 0x90000000; //(31:26 bits 100100)
	const int32_t load_halfu = 0x94000000; //(31:26 bits 100101)
	const int32_t lwr = 0x98000000; //(31:26 bits 100110)
	const int32_t store_byte = 0xA0000000; //(31:26 bits 101000)
	const int32_t store_half = 0xA4000000; //(31:26 bits 101001)
	const int32_t swl = 0xA8000000; //(31:26 bits 101010)
	const int32_t store_word = 0xAC000000; //(31:26 bits 101011)
	const int32_t swr = 0xB8000000; //(31:26 bits 101110)
	const int32_t load_linked_word = 0xC0000000; //(31:26 bits 110000)
	const int32_t lwc1 = 0xC4000000; //(31:26 bits 110001)
	const int32_t store_cond_word = 0xE0000000; //(31:26 bits 111000)
	const int32_t swc1 = 0xE4000000; //(31:26 bits 111001)
	//Masks used to extract specific individual information from the currentinstruction.
	const int32_t opcode_mask =  0xFC000000;
	const int32_t rs_mask = 0x03E00000;
	const int32_t rt_mask = 0x001F0000;
	const int32_t rd_mask = 0x0000F800;
	const int32_t shamt_mask = 0x000007C0;
	const int32_t address_jump_mask = 0x03FFFFFF;
	const int32_t immediate_mask = 0x0000FFFF;
	const int32_t function_mask = 0x0000003F;
	//variables used to extract individual information from the currentinstruction.
	int32_t opcode;
	int32_t function;
	int32_t rs;
	int32_t rd;
	int32_t rt;
	int32_t shamt;
	int32_t immediate;
	int32_t address_jump;
	bool check_jump = false;
	int i;
	for(i = 0; i < MaxInstructions; i++) {

		//FETCH THE INSTRUCTION AT 'ProgramCounter'	
		check_jump = false;
		RegFile[0] = 0;
		CurrentInstruction = readWord(ProgramCounter,false);
		if(CurrentInstruction == 0) //NOP
		{
			ProgramCounter +=4;
			continue;
		}	
		opcode = CurrentInstruction & opcode_mask;
		rs = CurrentInstruction & rs_mask;
		rs = rs >> 21; //to get the real register number in decimal and use rs as RegFile array index.
		rt = CurrentInstruction & rt_mask;
		rt = rt >> 16; //to get the real register number in decimal and use rt as RegFile array index.
		rd = CurrentInstruction & rd_mask;
		rd = rd >> 11; //to get the real register number in decimal and use rd as RegFile array index.
		shamt = CurrentInstruction & shamt_mask;
		shamt = shamt >> 6;
		function = CurrentInstruction & function_mask;
		immediate = CurrentInstruction & immediate_mask;
		if(immediate & 0x8000)
		{
			immediate |= 0xFFFF0000;
		}
		address_jump = CurrentInstruction & address_jump_mask;
		if(opcode == R_type)
		{
			if(function == add)
			{
				RegFile[rd] = RegFile[rs] + RegFile[rt];
			}
			else if(function == addu)
			{
				uint32_t temp4 = (uint32_t)RegFile[rs] + (uint32_t)RegFile[rt];
				RegFile[rd] = (int32_t) temp4;
			}
			else if(function == sub)
			{
				RegFile[rd] = RegFile[rs] - RegFile[rt];
			}
			else if(function == and)
			{
				RegFile[rd] = RegFile[rs] & RegFile[rt];
			}
			else if(function == or)
			{
				RegFile[rd] = RegFile[rs] | RegFile[rt];
			}
			else if(function == xor)
			{
				RegFile[rd] = RegFile[rs] ^ RegFile[rt];
			}
			else if(function == nor)
			{
				RegFile[rd] = ~(RegFile[rs] | RegFile[rt]);
			}
			else if(function == subu)
			{
				uint32_t temp6 = (uint32_t)RegFile[rs] - (uint32_t)RegFile[rs];
				RegFile[rd] = (int32_t)temp6;
			}
			else if(function == sll)
			{
				RegFile[rd] = RegFile[rt] << shamt;
			}
			else if(function == srl)
			{
				RegFile[rd] = RegFile[rt] >> shamt;
			}
			else if(function == sra)
			{
				int32_t sra_mask = ~0;
				if(RegFile[rt] < 0)
				{
					sra_mask <<= shamt;
				}
				else
				{
					sra_mask >>= shamt;
					sra_mask <<= 32 - shamt;
				}
				RegFile[rd] = (RegFile[rt] >> shamt) | sra_mask;
			}
			else if(function == sllv)
			{
				RegFile[rd] = RegFile[rt] << RegFile[rs];
			}
			else if(function == srlv)
			{
				RegFile[rd] = RegFile[rt] >> RegFile[rs];
			}
			else if(function == srlv)
			{
				int32_t sra_mask = ~0;
				if(RegFile[rt] < 0)
				{
					sra_mask <<= RegFile[rs];
				}
				else
				{
					sra_mask >>= RegFile[rs];
					sra_mask <<= 32 - RegFile[rs];
				}
				RegFile[rd] = (RegFile[rt] >> RegFile[rs]) | sra_mask;
			}
			else if (function == jump_register)
			{
				ProgramCounter = RegFile[rs];
				check_jump = true;
			}
			else if (function == jalr)
			{
				if(rd == 0)
				{
					RegFile[31] = ProgramCounter + 4;
				}
				else
				{
					RegFile[rd] = ProgramCounter + 4;
				}
				ProgramCounter = RegFile[rs];
				check_jump = true;
			}
			else if(function == syscall)
			{
				SyscallExe(RegFile[2]);
			}
			else if(function == mfhi)
			{
				RegFile[rd] = RegFile[32];
			}
			else if(function == mthi)
			{
				RegFile[32] = RegFile[rs];
			}
			else if(function == mflo)
			{
				RegFile[rd] = RegFile[33];
			}
			else if(function == mtlo)
			{
				RegFile[33] = RegFile[rs];
			}
			else if(function == mult)
			{
				int32_t high_mask = 0xFFFF0000;
				int32_t low_mask = 0x0000FFFF;
				int32_t temp3 = RegFile[rs] * RegFile[rt];
				RegFile[32] = temp3 & high_mask;
				RegFile[33] = temp3 & low_mask;
			}
			else if(function == multu)
			{
				int32_t high_mask = 0xFFFF0000;
				int32_t low_mask = 0x0000FFFF;
				uint32_t temp = (uint32_t)RegFile[rs] * (uint32_t)RegFile[rt];
				RegFile[32] = (int32_t)temp & high_mask;
				RegFile[33] = (int32_t)temp & low_mask;
			}
			else if(function == div)
			{
				RegFile[33] = RegFile[rs] / RegFile[rt];
				RegFile[32] = RegFile[rs] % RegFile[rt];
			}
			else if(function == divu)
			{
				uint32_t temp1 = (uint32_t)RegFile[rs] / (uint32_t)RegFile[rt];
				uint32_t temp2 = (uint32_t)RegFile[rs] % (uint32_t)RegFile[rt];
				RegFile[33] = (int32_t) temp1;
				RegFile[32] = (int32_t) temp2;
			}
			else if(function == set_less_than)
			{
				if (RegFile[rs] < RegFile[rt])
				{
					RegFile[rd] = 1;
				}
				else
				{
					RegFile[rd] = 0;
				}
			}
			else if(function == set_less_thanu)
			{
				if ((uint32_t)RegFile[rs] < (uint32_t)RegFile[rt])
				{
					RegFile[rd] = 1;
				}
				else
				{
					RegFile[rd] = 0;
				}
			}
		}
		else if(opcode == bltz_gez && rt == 0)
		{
			if(RegFile[rs] < 0)
			{
				ProgramCounter = ProgramCounter + 4 + (immediate << 2); 
				check_jump = true;
			}
		}
		else if(opcode == bltz_gez && rt == 1) //BGEZ
		{
			if(RegFile[rs] >= 0)
			{
			ProgramCounter = ProgramCounter + 4 + (immediate << 2); 
			check_jump = true;	
			}
		}
		else if(opcode == bltz_gez && rt == 16) //BLTZAL
		{
			if(RegFile[rs] < 0)
			{
				RegFile[31] = ProgramCounter + 4;
				ProgramCounter = ProgramCounter + 4 + (immediate << 2); 
				check_jump = true;
			}
		}
		else if(opcode == bltz_gez && rt == 17) //BGEZAL
		{
			if(RegFile[rs] >= 0)
			{
				RegFile[31] = ProgramCounter + 4;
				ProgramCounter = ProgramCounter + 4 + (immediate << 2); 
				check_jump = true;
			}
		}
		else if(opcode == blez)
		{
			if(RegFile[rs] <= 0)
			{
				ProgramCounter = ProgramCounter + 4 + (immediate << 2);  
				check_jump = true;
			}
		}
		else if(opcode == bgtz)
		{
			if(RegFile[rs] > 0)
			{
				ProgramCounter = ProgramCounter + 4 + (immediate << 2); 
				check_jump = true;
			}
		}
		else if(opcode == jump)
		{
			ProgramCounter = ProgramCounter + 4;
			ProgramCounter = ProgramCounter & 0xf0000000;
			address_jump = address_jump << 2;
			ProgramCounter = ProgramCounter | address_jump;
			check_jump = true;
		}
		else if(opcode == jump_link)
		{
			RegFile[31] = ProgramCounter + 4;
			ProgramCounter = ProgramCounter + 4;
			ProgramCounter = ProgramCounter & 0xf0000000;
			address_jump = address_jump << 2;
			ProgramCounter = ProgramCounter | address_jump; 
			check_jump = true;
		}
		else if(opcode == branch_equal)
		{
			if(RegFile[rs] == RegFile[rt])
			{
				ProgramCounter = ProgramCounter + 4 + (immediate << 2); 
				check_jump = true;
			}
		}
		else if(opcode == branch_not_equal)
		{
			if(RegFile[rs] != RegFile[rt])
			{
				ProgramCounter = ProgramCounter + 4 + (immediate << 2);
				check_jump = true;
			}
		}
		else if(opcode == addi)
		{
			RegFile[rt] = RegFile[rs] + immediate;
		}
		else if(opcode == addiu)
		{
			uint32_t temp5 = (uint32_t)RegFile[rs] + (uint32_t)immediate;
			RegFile[rt] = (int32_t)temp5;
		}
		else if(opcode == set_less_than_i)
		{
			if(RegFile[rs] < immediate)
			{
				RegFile[rt] = 1;
			}
			else
			{
				RegFile[rt] = 0;
			}
		}
		else if(opcode == set_less_than_iu)
		{
			if(RegFile[rs] < (uint32_t)immediate)
			{
				RegFile[rt] = 1;
			}
			else
			{
				RegFile[rt] = 0;
			}
		}
		else if(opcode == andi)
		{
			immediate = immediate & 0x0000ffff;
			RegFile[rt] = RegFile[rs] & immediate;
		}
		else if(opcode == ori)
		{
			immediate = immediate & 0x0000ffff;
			RegFile[rt] = RegFile[rs] | immediate;
		}
		else if(opcode == xori)
		{
			immediate = immediate & 0x0000ffff;
			RegFile[rt] = RegFile[rs] ^ immediate;
		}
		else if(opcode == lui)
		{
			RegFile[rt] = immediate << 16;
		}
		else if(opcode == load_byte)
		{
			RegFile[rt] = readByte(RegFile[rs] + immediate,false);
		}
		else if(opcode == load_half)
		{
			RegFile[rt] = readByte(RegFile[rs] + immediate, false);
			RegFile[rt] = RegFile[rt] | readByte(RegFile[rs] + immediate + 4,false);
		}
		else if(opcode == load_word)
		{
			RegFile[rt] = readWord(RegFile[rs] + immediate, false);
		}
		else if(opcode == load_byteu)
		{
			RegFile[rt] = readByte(RegFile[rs] + (uint32_t)immediate, false);
		}
		else if(opcode == load_halfu)
		{
			RegFile[rt] = readByte(RegFile[rs] + (uint32_t)immediate, false);
			RegFile[rt] = RegFile[rt] | readByte(RegFile[rs] + (uint32_t)immediate + 4,false);
		}
		else if(opcode == store_byte)
		{
			writeByte(RegFile[rs] + immediate, RegFile[rt], false);
		}
		else if(opcode == store_half)
		{
			writeByte(RegFile[rs] + immediate, RegFile[rt], false);
			writeByte(RegFile[rs] + immediate + 4, RegFile[rt], false);
		}
		else if(opcode == store_word)
		{
			writeWord(RegFile[rs] + immediate, RegFile[rt],false);
		}
		if(check_jump == false)
		{
			ProgramCounter += 4;
		}
		RegFile[0] = 0;
		printRegFile();
	}  
	//Close file pointers & free allocated Memory
	closeFDT();
	CleanUp();

	return 0;

}
