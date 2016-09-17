#pragma once
#include "../library/mongoose.h"
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <queue>
#include <list>
#include <string>
#include <iostream>
#include <algorithm>

using namespace std;

struct node{
	uint64_t id;
    unordered_map<uint64_t, node*> neighbors;
	node(uint64_t i){id=i;}
    /*
    bool operator==(node *another) const{
        return id == another->id;
    }
    bool operator==(uint64_t another_id) const{
        return id == another_id;
    }
     */
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
    
    //return 200 on success, 400 if the node does not exist
	uint64_t remove_node(uint64_t node_id);
    
    //return 200 on success, 400 if the edge does not exist
	uint64_t remove_edge(uint64_t node_a_id, uint64_t node_b_id);
    
    //return true if node exists, otherwise false
	bool get_node(uint64_t node_id);
    
    /*
     First boolean indicates if the edge exists in the graph.
     Second boolean returns true if both nodes exist in the graph;
     false if any of them doesn't exist in the graph.
     */
	pair<bool, bool> get_edge(uint64_t node_a_id, uint64_t node_b_id);
    
    /**
     Boolean indicates whether the node is in the graph.
     The returned map returns the neighbors of the node.
     */
	pair<bool,unordered_map<uint64_t, node*> > get_neighbors(uint64_t node_id);
    
    /*
     int = shortest path length, if returned -1, then there is no path between node a and b.
     Boolean = true if both nodes exist in the graph;
               false if any of them doesn't exist in the graph.
    */
	pair<uint64_t, bool> shortest_path(uint64_t node_a_id, uint64_t node_b_id);
};
