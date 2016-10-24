//
//  virtual_memory.cpp
//  A1
//
//  Created by yo on 16/10/22.
//

#include "virtual_memory.hpp"

bool super_block::check_log_full(void){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
    return cur_block == end_block && cur_log_page->num_entry == 204;
}

// mmap(NULL, BLOCK_SIZE_BYTE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset)
void super_block::write_add_node(uint64_t id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096 + (cur_block - 1) * 4096);
    if(cur_log_page->num_entry == 204){
        cur_block += 1;
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096 + (cur_block - 1) * 4096);
        cur_log_page->num_entry = 0;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = 22222;
        // bypass the log_block data
        cur_log_page += 1;
        log_entry* cur_log_entry = (log_entry*)cur_log_page;
//        log_entry *cur_log_entry = (log_entry*)mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 0;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }else{
        cur_log_page += 1;
        log_entry *cur_log_entry = (log_entry*)cur_log_page + cur_log_page->num_entry;
        //        log_entry *cur_log_entry = mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 0;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }
}

void super_block::write_remove_node(uint64_t id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096 + (cur_block - 1) * 4096);
    if(cur_log_page->num_entry == 204){
        cur_block += 1;
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096 + (cur_block - 1) * 4096);
        cur_log_page->num_entry = 0;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = 22222;
        cur_log_page += 1;
        log_entry* cur_log_entry = (log_entry*)cur_log_page;
//        log_entry *cur_log_entry = mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 1;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }else{
        cur_log_page += 1;
        log_entry *cur_log_entry = (log_entry*)cur_log_page + cur_log_page->num_entry;
//        log_entry *cur_log_entry = mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 1;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }
}

void super_block::write_add_edge(uint64_t node_a_id, uint64_t node_b_id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096 + (cur_block - 1) * 4096);
    if (cur_log_page->num_entry == 204) {
        cur_block += 1;
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096 + (cur_block - 1) * 4096);
        cur_log_page->num_entry = 0;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = 22222;
        cur_log_page += 1;
        log_entry* cur_log_entry = (log_entry*)cur_log_page;
//        log_entry *cur_log_entry = mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 2;
        cur_log_entry->node_a = node_a_id;
        cur_log_entry->node_b = node_b_id;
    } else {
        cur_log_page += 1;
        log_entry *cur_log_entry = (log_entry*)cur_log_page + cur_log_page->num_entry;
//        log_entry *cur_log_entry = mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 2;
        cur_log_entry->node_a = node_a_id;
        cur_log_entry->node_b = node_b_id;
    }
}

void super_block::write_remove_edge(uint64_t node_a_id, uint64_t node_b_id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, size(super_block) + (cur_block - 1) * 4096);
    if (cur_log_page->num_entry == 204) {
        cur_block += 1;
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, size(super_block) + (cur_block - 1) * 4096);
        cur_log_page->num_entry = 0;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = 22222;
        cur_log_page += 1;
        log_entry* cur_log_entry = (log_entry*)cur_log_page;
//        log_entry *cur_log_entry = mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 3;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    } else {
        cur_log_page += 1;
        log_entry *cur_log_entry = (log_entry*)cur_log_page + cur_log_page->num_entry;
//        log_entry *cur_log_entry = mmap(NULL, 20, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 3;
        cur_log_entry->node_a = node_a_id;
        cur_log_entry->node_b = node_b_id;
    }
}

super_block read_super_block_from_vm(int fd){
    super_block my_super_block;
    super_block* super_block_in_vm = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    my_super_block.cur_generation = super_block_in_vm->cur_generation;
    my_super_block.check_sum = super_block_in_vm->check_sum;
    my_super_block.cur_block = super_block_in_vm->cur_block;
    my_super_block.end_block = super_block_in_vm->end_block;
    return my_super_block;
}

check_point read_checkpoint_from_vm(int fd){
    check_point my_checkpoint;
    check_point* checkpoint_in_vm = (check_point*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 2 * 1024 * 1024 * 1024);
    my_checkpoint.size = checkpoint_in_vm->size;
    return my_checkpoint;
}


