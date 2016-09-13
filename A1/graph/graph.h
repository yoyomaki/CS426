#pragma once
#include "../libraray/mongoose.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>

using namespace std;

struct node{
	uint64_t id;
	unordered_set<node*> neighbors;
	node(uint64_t i){this->id=i;}
    bool operator==(node *another) const{
        return id == another->id;
    }
    bool operator==(uint64_t another_id) const{
        return this->id == another_id;
    }
    
};

class graph{
private:
	unordered_map<uint64_t, node*> nodes;
public:
    //return 200 on success, 204 if the node already exists
	uint64_t add_node(uint64_t node_id);
    
    //return 200 on suceess, 204 if the edge exists
    //400 if either node does not exists, or if node_a_id = node_b_id
	uint64_t add_edge(uint64_t node_a_id, uint64_t node_b_id);
    
	uint64_t remove_node(uint64_t node_id);
	uint64_t remove_edge(uint64_t node_a_id, uint64_t node_b_id);
	uint64_t get_node(uint64_t node_id);
	uint64_t get_edge(uint64_t node_a_id, uint64_t node_b_id);
	uint64_t get_neighbors(uint64_t node_id);
	uint64_t shortest_path(uint64_t node_a_id, uint64_t node_b_id);
};
