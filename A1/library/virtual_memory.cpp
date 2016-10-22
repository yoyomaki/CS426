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




