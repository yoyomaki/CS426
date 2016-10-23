//
//  virtual_memory.hpp
//  A1
//
//  Created by yo on 16/10/22
//

#ifndef virtual_memory_hpp
#define virtual_memory_hpp

#include <stdio.h>
#include <../graph/graph.h>
using namespace std;
/*
 Log: 2GB
  -----------
 |super block|
  -----------
 | log block |
 |...........|
 |...........|
 |log entries|
  -----------
 | log block |
 |...........|
 |...........|
 |log entries|
  -----------
*/


struct super_block{
    uint32_t cur_generation;
    uint64_t check_sum;
    uint32_t cur_block;
    uint32_t end_block = 250000;
    bool check_log_full(void);
    void write_add_node(uint64_t id, int fd);
    void write_remove_node(uint64_t id, int fd);
}

struct log_block{
    uint32_t generation;
    uint32_t num_entry;
    uint64_t check_sum;
};


//4-byte + 8-byte + 8-byte = 20-byte
struct log_entry{
    uint32_t opcode;
    uint64_t node_a;
    uint64_t node_b; //only read node_b if op is 2 or 3
};




/*
 Graph: 8GB
  -----------------
 |   check point   |
  -----------------
 |  node a, node b |
 |.................|
 |.................|
 |  node a, node b |
  -----------------
*/

struct check_point{
    uint64_t size;  // number of graph_data, 8GB first line
};

struct graph_data{
    uint64_t node_a;
    uint64_t node_b;
};

super_block read_super_block_from_vm(int fd);
check_point read_checkpoint_from_vm(int fd);
    
#endif /* virtual_memory_hpp */
