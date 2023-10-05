#include <stdio.h>	/* printf() */
#include <stdint.h>	/* int32_t */

#include "RegFile.h"

//32x32 Register File
int32_t RegFile[34];

void initRegFile(int32_t val) {
    
	int i;
    	
	for(i = 0; i < 34; i++){
    	
		RegFile[i] = val;
    	
	}

}

void printRegFile(){

    printf("\n ----- REG DUMP ----- \n");

    int j;
    for (j = 0; j < 32; j++){

	printf("REG[%2d]: 0x%08x (%d)",j,RegFile[j],RegFile[j]);
        if(j%2==0){
	
		printf("\t\t");
	
	}else{
		
		printf("\n");
	}
        
    }

    printf("\n");
    printf("Reg[32] (HI): 0x%08x (%d)\t\t", RegFile[32], RegFile[32]);
    printf("Reg[33] (LO): 0x%08x (%d)\n", RegFile[33], RegFile[33]);
    printf("\n");

}




