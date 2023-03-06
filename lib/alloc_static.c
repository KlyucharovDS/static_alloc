/*
I decided to develop a memory allocation system based on an array of structures (s_mem) with a PULL_SIZE size.
The structure (s_mem) consists of a static array (value) with a size of BLOCK_SIZE bytes and a busy indicator (busy) and
pointer to the next free block.
The static array of bytes in the structure (s_mem) is 1 block of memory allocated to the user.
Initially, an array of structures must be initialized, which is done in the init_pool() procedure.
The allocater gives the user of the library an address to the first element of the memory block.
The library is designed using locking with a mutex, which is one way synchronization in a multi-threaded environment.
*/

#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <pthread.h>
#include "alloc_static.h"

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static size_t next_free_index=0;
/*
This function initializes the memory pool. It is called only once.
This function clears the entire pool memory, resets the busy flag in each block,
sets the next_free value to the next unallocated block from the pool, sets the next_free_index to 0.
*/
void init_pool()
{
    static uint8_t init=0;
    if(!init)
    {
        for(size_t i=0;i<PULL_SIZE;i++)
        {
            pool[i].busy=0;
            memset(pool[i].value,(uint8_t)0,BLOCK_SIZE);
            pool[i].next_free=i+1;
        }
        next_free_index=0;
        init=1;
    }
}
/*
    This function allocates 1 block of BLOCK_SIZE bytes from a pool of memory of PULL_SIZE blocks
    If the static memory array is not initialized, then initialization is performed.
    Return:
        * 0-error allocate
        * Address of the allocated beginning of the memory block
*/
size_t alloc_block()
{
    pthread_mutex_lock(&mutex);
    init_pool();
    if(next_free_index<PULL_SIZE && !pool[next_free_index].busy)
    {
        pool[next_free_index].busy=1;
        size_t address=(size_t)pool[next_free_index].value;
        next_free_index=pool[next_free_index].next_free;
        return address;
    }
    pthread_mutex_unlock(&mutex);
    return 0;
}
/*
    The function of freeing and clearing the contents of the allocated memory.
    addr is the address of the beginning of the allocated memory block.
    If addr is not an address from a static memory pool, then no cleanup is performed.
*/
void free_block(size_t addr)
{
    pthread_mutex_lock(&mutex);
    if(!addr)
        return;
    size_t index = (size_t)(addr-(size_t)pool)/sizeof(block_t);
    if(index<PULL_SIZE && addr == (size_t)pool[index].value)
    {
        if(pool[index].busy)
        {
            memset(pool[index].value,(uint8_t)0,BLOCK_SIZE);
            pool[index].busy=0;
            pool[index].next_free=next_free_index;
            next_free_index=index;
        }
    }
    pthread_mutex_unlock(&mutex);
}

