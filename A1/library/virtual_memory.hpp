//
//  virtual_memory.hpp
//  A1
//
//  Created by yo on 16/10/22
//

#ifndef virtual_memory_hpp
#define virtual_memory_hpp

#include <stdio.h>
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

struct log_block{
    uint32_t generation;
    uint32_t num_entry;
    uint64_t check_sum;
    vector<log_entry> log_data;
};


//4-byte + 8-byte + 8-byte = 20-byte
struct log_entry{
    uint32_t opcode; //0 for add_node, 1 for remove_node, 2 for remove_edge, 3 for remove_edge;
    uint64_t node_a;
    uint64_t node_b; //only read node_b if op is 2 or 3
};

struct super_block{
    uint32_t cur_generation;
    uint64_t check_sum;
    uint32_t cur_block;
    uint32_t end_block;
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

#endif /* virtual_memory_hpp */
