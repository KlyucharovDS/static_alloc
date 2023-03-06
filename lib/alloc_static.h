#ifndef ALLOC_STATIC_H_INCLUDED
#define ALLOC_STATIC_H_INCLUDED

// number BLOCK_SIZE in BLOCK_SIZE bytes
#define PULL_SIZE 1024
// BLOCK_SIZE in bytes
#define BLOCK_SIZE 8

typedef struct s_mem{
    uint8_t value[BLOCK_SIZE];
    uint8_t busy;
    size_t next_free;
} block_t;
block_t pool[PULL_SIZE];

//functions
void init_pool();
size_t alloc_block();
void free_block(size_t addr);


#endif // ALLOC_STATIC_H_INCLUDED
