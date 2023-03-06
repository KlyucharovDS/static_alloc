#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "../lib/alloc_static.h"
#include "../test/test.h"
#define PROC_RESULT(curr_result,glob_result) \
            glob_result=(curr_result & 1);\
            glob_result=glob_result<<1;

static uint8_t eq_data(size_t address);
static uint8_t test_clean(size_t address);
//--------------------------SUPPORTING FUNCTIONS-------------------------
/*
    The procedure for writing the inverted low byte of the address of each byte in the memory block, reading it and checking
    Parameters:
        * address - address of the first byte of the memory block.
    return:
        1-error
        0-success
*/
static uint8_t eq_data(size_t address)
{
    uint8_t* pa=(uint8_t*)address;
    for(size_t i=0;i<BLOCK_SIZE;i++)
        pa[i]=(uint8_t)((~(address+i)) & 0xFF);
    for(size_t i=0;i<BLOCK_SIZE;i++)
        if(pa[i]!=(uint8_t)((~(address+i)) & 0xFF))
            return 1;
    return 0;
}
/*
   The procedure for checking the clearing of the memory block.
    Parameters:
        * address - address of the first byte of the memory block.
    return:
        0 success
        1-error
*/
static uint8_t test_clean(size_t address)
{
    uint8_t* p=(uint8_t*)address;
    for(size_t i=0;i<BLOCK_SIZE;i++)
        if(p[i]!=0)
            return 1;
    return 0;
}

/*
    This procedure prints the text of the test, the test result, and aligns the test result.
    Parameters:
        * text- string with output text
        * result - test result flag, which is written in the least significant bit
            0-OK
            1-FAIL
*/
void print_result(char* text, uint8_t result)
{
    int count=0;
    printf("\n%s%n",text,&count);
    char spaces[50]={'\0'};
    for(uint8_t i=0;i<50-count;i++)
        strcat(spaces,".");
    if(text[0]!='\t')
    {
        strcat(spaces,".......");
    }
    if(text[0]=='\n')
    {
        strcat(spaces,".");
    }
    if(text[1]=='\n')
    {
        strcat(spaces,".");
    }
    printf("%s%s",spaces,(result & 1)?"FAIL":"OK");
}

//---------------MAIN TESTS------------------------
/*
    Pool initialization procedure. Initialization occurs once, the first time the procedure is called.
    Subsequent calls to the procedure have no effect.
    Return:
        1-error
        0-success
*/
uint8_t init_test()
{
     uint8_t error=0;
    //инициализация с проверкой очистки памяти;
    init_pool();
    for(size_t i=0;i<PULL_SIZE;i++)
        error|=test_clean((size_t)pool[i].value);
    print_result("\tinit pool",error);
    return error;
}
/*
    TEST 1
    Algorithm:
        * initialization with memory cleaning check;
        * Allocation of 1 block of memory with verification of the data recorded in the memory;
        * cleaning 1 block of memory with memory cleaning check;
        * allocation of all memory blocks with verification of data written to the memory;
        * release of all memory with data cleaning check;
    Return:
        0-success
        1-error
*/
uint8_t test1()
{
    printf("\n\n-----------------------------TEST 1---------------------------");
    uint8_t error=0;
    //initialization with memory cleaning check
    PROC_RESULT(init_test(),error)
    //Allocation of 1 block of memory with verification of the data recorded in the memory;
    size_t block_mem=alloc_block();
    PROC_RESULT(block_mem?0:1,error)
    print_result("\tallocate 1 block",error);
    PROC_RESULT(eq_data(block_mem),error)
    print_result("\tequal data in 1 block",error);
    //cleaning 1 block of memory with memory cleaning check;
    free_block(block_mem);
    PROC_RESULT(test_clean(block_mem),error)
    print_result("\ttest clean block",error);
    //allocation of all memory blocks with verification of data written to the memory;
    size_t arr_block_mem[PULL_SIZE];
    memset(arr_block_mem,(size_t)0,PULL_SIZE*sizeof(size_t));
    for(size_t i=0;i<PULL_SIZE;i++)
    {
        arr_block_mem[i]=alloc_block();
        error|=block_mem?0:1;
        error|=eq_data(arr_block_mem[i]);
        //release of all memory with data cleaning check;
        free_block(arr_block_mem[i]);
        error|=test_clean(arr_block_mem[i]);
    }
    print_result("\tallocate and free memory pool",error);
    print_result("TEST 1",error?1:0);
    return error;
}
/*
    TEST 2
    Algorithm:
        * allocating more memory blocks than pool have and checking to issue pointers only to available memory
    Return:
        0-success
        1-error
*/
uint8_t test2()
{
    printf("\n\n-----------------------------TEST 2---------------------------");
    uint8_t error=0;
    PROC_RESULT(init_test(),error)
    const size_t error_size=PULL_SIZE+3;
    size_t arr_block_mem[error_size];
    memset(arr_block_mem,(size_t)0,error_size*sizeof(size_t));
    for(size_t i=0;i<error_size;i++)
    {
        arr_block_mem[i]=alloc_block();
        if((i<PULL_SIZE && arr_block_mem[i]==(size_t)NULL) ||
           (i>=PULL_SIZE && arr_block_mem[i]!=(size_t)NULL))
            error|=1;
    }
    print_result("\tallocate more then available",error?1:0);
    print_result("TEST 2",error?1:0);
    return error;
}
/*
    TEST 3
    * clearing an unallocated block of memory with a check for changes in memory
    Return:
        0-success
        1-error
*/
uint8_t test3()
{
    uint8_t error=0;
    printf("\n\n-----------------------------TEST 3---------------------------");
    PROC_RESULT(init_test(),error)
    uint8_t temp[BLOCK_SIZE];
    memset(temp,(uint8_t)0xAA,BLOCK_SIZE);
    free_block((size_t)temp);
    for(size_t i=0;i<BLOCK_SIZE;i++)
        if(temp[i]!=0xAA)
        {
            error|=1;
            break;
        }
    print_result("\ttest for lack of memory clearing.",error);
    print_result("TEST 3",error?1:0);
    return error;
}


