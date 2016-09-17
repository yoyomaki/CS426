#include "graph.h"

uint64_t graph::add_node(uint64_t node_id){
    unordered_map<uint64_t, node*>::iterator it = this->nodes.find(node_id);
    if(it == this->nodes.end()){
        node* new_node = new node(node_id);
        nodes[node_id] = new_node;
        return 200;
    }else{
        return 204;
    }
}

uint64_t graph::add_edge(uint64_t node_a_id, uint64_t node_b_id){
    if(node_a_id == node_b_id){
        return 400;
    }
    if(nodes.find(node_a_id) == nodes.end() || nodes.find(node_b_id) == nodes.end()){
        return 400;
    }
    unordered_map<uint64_t, node*> a_neighbors = nodes[node_a_id]->neighbors;
    unordered_map<uint64_t, node*> b_neighbors = nodes[node_b_id]->neighbors;
    if(a_neighbors.find(node_b_id) != a_neighbors.end()){
        return 204;
    }
    nodes[node_a_id]->neighbors[node_b_id] = nodes[node_b_id];
    nodes[node_b_id]->neighbors[node_a_id] = nodes[node_a_id];
    return 200;
}

uint64_t graph::remove_node(uint64_t node_id){
    if(nodes.find(node_id) == nodes.end()){
        return 400;
    }else{
        unordered_map<uint64_t, node*> nbs = nodes[node_id]->neighbors;
        unordered_map<uint64_t, node*>::iterator nit;
        for(nit = nbs.begin(); nit != nbs.end(); ++nit){
            nit->second->neighbors.erase(node_id);
        }
        nodes.erase(node_id);
        return 200;
    }
}

uint64_t graph::remove_edge(uint64_t node_a_id, uint64_t node_b_id){
    if(nodes.find(node_a_id) == this->nodes.end()
       || nodes.find(node_b_id) == this->nodes.end()){
        return 400;
    }
    if(nodes[node_a_id]->neighbors.find(node_b_id) ==
       nodes[node_a_id]->neighbors.end()){
        return 400;
    }
    nodes[node_a_id]->neighbors.erase(node_b_id);
    nodes[node_b_id]->neighbors.erase(node_a_id);
    return 200;
}

bool graph::get_node(uint64_t node_id){
    return nodes.find(node_id) != nodes.end();
}

pair<bool,bool> graph::get_edge(uint64_t node_a_id, uint64_t node_b_id){
    bool edge_exist = false;
    bool both_node_exist = true;
    if (!get_node(node_a_id) || !get_node(node_b_id)) {
        both_node_exist = false;
    }
    if (get_node(node_a_id) && get_node(node_b_id)) {
        unordered_map<uint64_t, node*> neighbors_a = nodes[node_a_id]->neighbors;
        if(neighbors_a.find(node_b_id) != neighbors_a.end()) {
            edge_exist = true;
        }
    }
    return make_pair(edge_exist, both_node_exist);
}

pair<bool, unordered_map<uint64_t, node*> > graph::get_neighbors(uint64_t node_id){
    if (this->get_node(node_id)) {
        return make_pair(true, nodes[node_id]->neighbors);
    } else {
        unordered_map<uint64_t, node*> no_exist;
        return make_pair(false, no_exist);
    }
}

pair<uint64_t, bool> graph::shortest_path(uint64_t node_a_id, uint64_t node_b_id){
    bool both_node_exist = true;
    uint64_t path = -1;
    uint64_t s;
    //    uint64_t temp_path = 0;
    if (!get_node(node_a_id) || !get_node(node_b_id)) {
        both_node_exist = false;
    }
    // initializing
    unordered_map<uint64_t, node*>::iterator node_i;
    unordered_map<uint64_t, pair<bool, int> > imp;
    for(node_i = nodes.begin(); node_i != nodes.end(); ++node_i) {
        imp[node_i->first] = make_pair(false, -1);
    }
    //    cout << imp.size() << endl;
    
    // Create a queue for BFS
    list<uint64_t> queue;
    
    // Mark the current node as visited and enqueue it
    
    imp[node_a_id] = make_pair(true, 0);
    queue.push_back(node_a_id);
    
    // 'i' will be used to get all adjacent vertices of a vertex
    unordered_map<uint64_t, node*>::iterator i;
    
    while(!queue.empty()) {
        s = queue.front();
        // if queue contians node_b_id
        if (std::find(queue.begin(), queue.end(), node_b_id) != queue.end()) {
            
            path = imp[node_b_id].second;
            break;
        }
        
        queue.pop_front();
        
        // Get all adjacent vertices of the dequeued vertex s.
        // If a adjacent has not been visited, then mark it visited
        // and enqueue it
        unordered_map<uint64_t, node*> s_neighbors = nodes[s]->neighbors;
        for(i = s_neighbors.begin(); i != s_neighbors.end(); ++i) {
            if (!imp[i->first].first) {
                imp[i->first].first = true;
                imp[i->first].second = imp[s].second + 1;
                queue.push_back(i->first);
            }
        }
    }
    return make_pair(path, both_node_exist);
}
