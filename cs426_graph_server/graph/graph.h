#pragma once
#include "../libraray/mongoose.h"
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

struct node{
	uint64_t id;
	vector<node*> neighbors;
	node(uint64_t i){this->id=i;}
};

class graph{
private:
	unordered_map<uint64_t, node*> nodes;
public:
	uint64_t add_node(uint64_t node_id);
	uint64_t add_edge(uint64_t node_a_id, uint64_t node_b_id);
	uint64_t remove_node(uint64_t node_id);
	uint64_t remove_edge(uint64_t node_a_id, uint64_t node_b_id);
	uint64_t get_node(uint64_t node_id);
	uint64_t get_edge(uint64_t node_a_id, uint64_t node_b_id);
	uint64_t get_neighbors(uint64_t node_id);
	uint64_t shortest_path(uint64_t node_a_id, uint64_t node_b_id);
};
