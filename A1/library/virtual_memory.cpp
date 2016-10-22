//
//  virtual_memory.cpp
//  A1
//
//  Created by yo on 16/10/22.
//

#include "virtual_memory.hpp"

bool super_block::check_log_full(void){
    log_block *cur_log_page = mmap(NULL, 4096, fd, cur_block * 4096);
    return cur_block == end_block && cur_log_page->num_entry == 22222;
}

void super_block::write_add_node(uint64_t id, int fd){
    log_block *cur_log_page = mmap(NULL, 4096, fd, cur_block * 4096);
    if(cur_log_page->num_entry == 22222){
        cur_block += 1;
        log_block *cur_log_page = mmap(NULL, 4096, fd, cur_block * 4096);
        cur_log_page->num_entry = 0;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = 22222;
        log_entry *cur_log_entry = mmap(NULL, 20, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 0;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }else{
        log_entry *cur_log_entry = mmap(NULL, 20, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 0;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }
}



