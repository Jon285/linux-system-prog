#include <unistd.h>

#include <stddef.h>
#include <stdbool.h>

//Extremely basic memory allocated
//TODO: calloc and realloc

//metadata needed to manage the allocated memory
struct blk_header {
    size_t size;
    bool is_free;
    struct blk_header* next;
};

//head and tail of the list of allocated blocks
static struct blk_header *head = NULL, *tail = NULL;

struct blk_header* get_free_block(size_t);

void* _malloc(size_t size) {
    if(!size)
        return NULL;

    struct blk_header *new_block_header = get_free_block(size);

    if(new_block_header) {
        new_block_header->is_free = false;
        return (void*)(new_block_header + 1);
    }
    
    size_t total_size = size + sizeof(struct blk_header);
    void* block = sbrk(total_size);

    if(block == (void*) -1)
        return NULL;

    new_block_header = block;
    new_block_header->size = size;
    new_block_header->is_free = false;
    new_block_header->next = NULL;

    if(!head)
        head = new_block_header;

    if(tail)
        tail->next = new_block_header;

    tail = new_block_header;
    
    return (void*)(new_block_header + 1);
}

void _free(void* block) {
    if(!block)
        return;

    struct blk_header* rem_header = (struct blk_header*) block - 1;

    void* program_brk = sbrk(0);

    //if it's the last block on the heap
    if((char*) block + rem_header->size == program_brk) {
        //update the list
        if(head == tail) {
            head = tail = NULL;
        } else {
            struct blk_header* temp = head;

            while(temp) {
                if(temp->next == tail) {
                    temp->next = NULL;
                    tail = temp;
                }
                temp = temp->next;
            }
        }
        //and free the block
        sbrk(0 - sizeof(struct blk_header) - rem_header->size);
        return;
    }

    //otherwise just mark as free to reuse
    rem_header->is_free = true;
    return;
}

struct blk_header* get_free_block(size_t size) {
    struct blk_header* current = head;

    while(current) {
        if(current->is_free && current->size >= size)
            return current;

        current = current->next;
    }
    return NULL;
}
