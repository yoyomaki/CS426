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

void super_block::write_remove_node(uint64_t id, int fd){
    log_block *cur_log_page = mmap(NULL, 4096, fd, cur_block * 4096);
    if(cur_log_page->num_entry == 22222){
        cur_block += 1;
        log_block *cur_log_page = mmap(NULL, 4096, fd, cur_block * 4096);
        cur_log_page->num_entry = 0;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = 22222;
        log_entry *cur_log_entry = mmap(NULL, 20, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 1;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }else{
        log_entry *cur_log_entry = mmap(NULL, 20, fd, cur_log_page + sizeof(log_block)+ cur_log_page->num_entry * sizeof(log_entry));
        cur_log_page->num_entry += 1;
        cur_log_entry->opcode = 1;
        cur_log_entry->node_a = id;
        cur_log_entry->node_b = 0;
    }
}

super_block read_super_block_from_vm(int fd){
    super_block my_super_block;
    super_block* super_block_in_vm = mmap(NULL, 4096, fd, 0);
    my_super_block.cur_generation = super_block_in_vm->cur_generation;
    my_super_block.check_sum = super_block_in_vm->check_sum;
    my_super_block.cur_block = super_block_in_vm->cur_block;
    my_super_block.end_block = super_block_in_vm->end_block;
    return my_super_block;
}

check_point read_checkpoint_from_vm(int fd){
    check_point my_checkpoint;
    check_point* checkpoint_in_vm = mmap(NULL, sizeof(check_point), fd, 2048000000);
    my_checkpoint.size = checkpoint_in_vm->size;
    return my_checkpoint;
}






