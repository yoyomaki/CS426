//
//  virtual_memory.cpp
//  A1
//
//  Created by yo on 16/10/22.
//

#include "virtual_memory.hpp"

bool super_block::check_log_full(int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
    return cur_block == end_block && cur_log_page->num_entry == 204;
}

void super_block::write_add_node(uint64_t id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
    cur_log_page->generation = cur_generation;
    if(cur_log_page->num_entry == 170){
//        cout << "CHANGING PAGE!!!" << endl;
        super_block *sb = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sb->cur_block += 1;
        cur_block += 1;
    
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
        cur_log_page->num_entry = 1;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
//        cout << "! updating checksum to be " << cur_log_page->check_sum << endl;
        cur_log_page->logs[0].opcode = 0;
        cur_log_page->logs[0].node_a = id;
        cur_log_page->logs[0].node_b = 0;
    }else{
        cout << "adding node " << id <<  " without change page" << endl;
        cur_log_page->logs[cur_log_page->num_entry].opcode = 0;
        cur_log_page->logs[cur_log_page->num_entry].node_a = id;
        cur_log_page->logs[cur_log_page->num_entry].node_b = 0;
        cur_log_page->num_entry += 1;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
//        cout << "! updating checksum to be " << cur_log_page->check_sum << endl;
    }
//    cout << "gen is " << cur_log_page->generation << endl;
//    cout << "num entry on this page is " << cur_log_page->num_entry << endl;
}

void super_block::write_remove_node(uint64_t id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
     cur_log_page->generation = cur_generation;
    if(cur_log_page->num_entry == 204){
//        cout << "CHANGING PAGE!!!" << endl;
        super_block *sb = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sb->cur_block += 1;
        cur_block += 1;
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
        cur_log_page->num_entry = 1;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
        cur_log_page->logs[0].opcode = 2;
        cur_log_page->logs[0].node_a = id;
        cur_log_page->logs[0].node_b = 0;
    }else{
        cout << "removing node " << id << "without change page" << endl;
        cur_log_page->logs[cur_log_page->num_entry].opcode = 2;
        cur_log_page->logs[cur_log_page->num_entry].node_a = id;
        cur_log_page->logs[cur_log_page->num_entry].node_b = 0;
        cur_log_page->num_entry += 1;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
    }
//    cout << "gen is " << cur_log_page->generation << endl;
//    cout << "num entry on this page is " << cur_log_page->num_entry << endl;
}

void super_block::write_add_edge(uint64_t node_a_id, uint64_t node_b_id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
//    cur_log_page->generation = cur_generation;
    if(cur_log_page->num_entry == 170){
        cout << "CHANGING PAGE!!!" << endl;
        super_block *sb = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sb->cur_block += 1;
        cur_block += 1;
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
        cur_log_page->num_entry = 1;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
//        cout << "! updating checksum to be " << cur_log_page->check_sum << endl;
        cur_log_page->logs[0].opcode = 1;
        cur_log_page->logs[0].node_a = node_a_id;
        cur_log_page->logs[0].node_b = node_b_id;
    }else{
        cout << "adding edge" << node_a_id << " " << node_b_id << "without change page" << endl;
        cur_log_page->logs[cur_log_page->num_entry].opcode = 1;
        cur_log_page->logs[cur_log_page->num_entry].node_a = node_a_id;
        cur_log_page->logs[cur_log_page->num_entry].node_b = node_b_id;
        cur_log_page->num_entry += 1;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
//        cout << "! updating checksum to be " << cur_log_page->check_sum << endl;
    }
//    cout << "gen is " << cur_log_page->generation << endl;
//    cout << "num entry on this page is " << cur_log_page->num_entry << " !!" <<cur_generation<< endl;
}

void super_block::write_remove_edge(uint64_t node_a_id, uint64_t node_b_id, int fd){
    log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
    cur_log_page->generation = cur_generation;
    if(cur_log_page->num_entry == 204){
//        cout << "CHANGING PAGE!!!" << endl;
        super_block *sb = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
        sb->cur_block += 1;
        cur_block += 1;
        log_block *cur_log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, cur_block * 4096);
        cur_log_page->num_entry = 1;
        cur_log_page->generation = cur_generation;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
        cur_log_page->logs[0].opcode = 3;
        cur_log_page->logs[0].node_a = node_a_id;
        cur_log_page->logs[0].node_b = node_b_id;
    }else{
        cout << "remove edge" << node_a_id << " " << node_b_id << "without change page" << endl;
        cur_log_page->logs[cur_log_page->num_entry].opcode = 3;
        cur_log_page->logs[cur_log_page->num_entry].node_a = node_a_id;
        cur_log_page->logs[cur_log_page->num_entry].node_b = node_b_id;
        cur_log_page->num_entry += 1;
        cur_log_page->check_sum = (cur_log_page->generation ^ cur_log_page->num_entry);
//        cur_log_page->generation = cur_generation;
        
    }
//    cout << "gen is " << cur_log_page->generation << endl;
//    cout << "num entry on this page is " << cur_log_page->num_entry << endl;
}

super_block read_super_block_from_vm(int fd){
    super_block my_super_block;
    super_block* super_block_in_vm = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    my_super_block.cur_generation = super_block_in_vm->cur_generation;
    my_super_block.check_sum = super_block_in_vm->check_sum;
    my_super_block.cur_block = super_block_in_vm->cur_block;
    my_super_block.end_block = super_block_in_vm->end_block;
    cout << "reading super block, cur_block is " << super_block_in_vm->cur_block << endl;
    return my_super_block;
}

check_point read_checkpoint_from_vm(int fd){
    check_point my_checkpoint;
    long long offset = OFFSET;
    check_point* checkpoint_in_vm = (check_point*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    my_checkpoint.size = checkpoint_in_vm->size;
    cout << "reading checkpoint, checkpoint size is " << checkpoint_in_vm->size << endl;
    return my_checkpoint;
}


void initialize_superblock(int fd){
    super_block *spblck = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    uint32_t cg = spblck->cur_generation;
    uint32_t cb = spblck->cur_block;
    uint32_t eb = spblck->end_block;
    uint64_t cs = spblck->check_sum;
    // cur_block changing, ignore for easier
    uint64_t tmp_check = (cg ^ eb);
    
    // check if superblock if valid
    if (tmp_check != cs) {
        cout << "invalid superblock--> gen no. to 0" << endl;
        spblck->cur_generation = 0;
        spblck->cur_block = 1;
        spblck->end_block = 250000;
        spblck->check_sum = 0 ^ 250000;
        log_block *log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
        log_page->num_entry = 0;
        log_page->generation = 0;
        log_page->check_sum = 0 ^ 0;
    } else {
    // superblock is valid
        spblck->cur_generation += 1;
        spblck->check_sum = tmp_check;
        cout << "valid superblock--> " << spblck->cur_generation << endl;
        spblck->cur_block = 1;
        //cur_block = 1;
        spblck->check_sum = (spblck->cur_generation ^ 250000);
        log_block *log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
        log_page->num_entry = 0;
//        log_page->generation = -1;
//        log_page->generation = spblck->cur_generation;
//        log_page->check_sum = 0 ^ 0;
    }
    cout << "initialized superblock" << endl;
}


void clear_superblock_after_checkpoint(int fd){
    super_block *spblck = (super_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    spblck->cur_generation += 1;
    spblck->cur_block = 1;
    spblck->end_block = 250000;
    spblck->check_sum = spblck->cur_generation ^ 250000;
    log_block *log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 4096);
    log_page->num_entry = 0;
    log_page->generation = spblck->cur_generation;
    log_page->check_sum = log_page->generation ^ log_page->num_entry;
    
}


void initialize_checkpoint(int fd){
    long long offset = OFFSET;
    cout << offset << endl;
    check_point *cp = (check_point*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
    if (cp == NULL) return ;
    cp->size = 0;
    cout << "initialized checkpoint" << endl;
}






