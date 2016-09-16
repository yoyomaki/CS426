#include "graph.h"

uint64_t graph::add_node(uint64_t node_id){
    unordered_map<uint64_t, node*>::iterator it = this->nodes.find(node_id);
    if(it == this->nodes.end()){
        node* new_node = new node(node_id);
        nodes.insert(make_pair(node_id, new_node));
        return 200;
    }else{
        return 204;
    }
}

uint64_t graph::add_edge(uint64_t node_a_id, uint64_t node_b_id){
    if(node_a_id == node_b_id){
        return 400;
    }
    unordered_map<uint64_t, node*>::iterator ita = this->nodes.find(node_a_id);
    unordered_map<uint64_t, node*>::iterator itb = this->nodes.find(node_b_id);
    if(ita == this->nodes.end() || itb == this->nodes.end()){
        return 400;
    }
    unordered_map<uint64_t, node*> a_neighbors = ita->second->neighbors;
    unordered_map<uint64_t, node*> b_neighbors = itb->second->neighbors;
    unordered_map<uint64_t, node*>::iterator ita_neighbors = a_neighbors.find(node_b_id);
    if(ita_neighbors != a_neighbors.end()){
        return 204;
    }
    a_neighbors.insert(make_pair(node_b_id,itb->second));
    b_neighbors.insert(make_pair(node_a_id,ita->second));
    return 200;
}

uint64_t graph::remove_node(uint64_t node_id){
    unordered_map<uint64_t, node*>::iterator it = this->nodes.find(node_id);
    if(it == this->nodes.end()){
        return 400;
    }else{
        unordered_map<uint64_t, node*> nbs = it->second->neighbors;
        unordered_map<uint64_t, node*>::iterator nit;
        for(nit = nbs.begin(); nit != nbs.end(); ++nit){
            node* n = nit->second;
            n->neighbors.erase(n->neighbors.find(node_id));
        }
        this->nodes.erase(it);
        return 200;
    }
}

uint64_t graph::remove_edge(uint64_t node_a_id, uint64_t node_b_id){
    unordered_map<uint64_t, node*>::iterator ita = this->nodes.find(node_a_id);
    unordered_map<uint64_t, node*>::iterator itb = this->nodes.find(node_b_id);
    if(ita == this->nodes.end() || itb == this->nodes.end()){
        return 400;
    }
    unordered_map<uint64_t, node*> a_neighbors = ita->second->neighbors;
    unordered_map<uint64_t, node*> b_neighbors = itb->second->neighbors;
    unordered_map<uint64_t, node*>::iterator ita_neighbors = a_neighbors.find(node_b_id);
    unordered_map<uint64_t, node*>::iterator itb_neighbors = b_neighbors.find(node_a_id);
    if(ita_neighbors == a_neighbors.end()){
        return 400;
    }
    a_neighbors.erase(ita_neighbors);
    b_neighbors.erase(itb_neighbors);
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
    uint64_t temp_path = 0;
    if (!get_node(node_a_id) || !get_node(node_b_id)) {
        both_node_exist = false;
    }
    if (get_edge(node_a_id, node_b_id).first) {
        // bool visited[nodes.size()];
        bool *visited = new bool[nodes.size()];
        for (int i = 0; i < nodes.size(); i++) {
            visited[i] = false;
        }
        // Create a queue for BFS
        list<uint64_t> queue;
        
        // Mark the current node as visited and enqueue it
        visited[node_a_id] = true;
        queue.push_back(node_a_id);
        
        // 'i' will be used to get all adjacent vertices of a vertex
        unordered_map<uint64_t, node*>::iterator i;
        
        while(!queue.empty()) {
            // Dequeue a vertex from queue and print it
            s = queue.front();
            if (s == node_b_id) {
                path = temp_path;
                break;
            }
            temp_path++;
            queue.pop_front();
            
            // Get all adjacent vertices of the dequeued vertex s.
            // If a adjacent has not been visited, then mark it visited
            // and enqueue it
            unordered_map<uint64_t, node*> a_neighbors = get_neighbors(node_a_id).second;
            for(i = a_neighbors.begin(); i != a_neighbors.end(); ++i) {
                if(!visited[i->first]) {
                    visited[i->first] = true;
                    queue.push_back(i->first);
                }
            }
        }
    }
    return make_pair(path, both_node_exist);
}