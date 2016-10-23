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
    if (!get_node(node_a_id) || !get_node(node_b_id)) {
        both_node_exist = false;
    }
    // initializing
    unordered_map<uint64_t, node*>::iterator node_i;
    unordered_map<uint64_t, pair<bool, int> > imp;
    for(node_i = nodes.begin(); node_i != nodes.end(); ++node_i) {
        imp[node_i->first] = make_pair(false, -1);
    }
    
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

void graph::set_graph_from_vm(check_point& my_checkpoint, super_block& my_super_block, int fd){
    int check_point_size = my_check_point.size;
    long offset = 2048000000 + sizeof(check_point);
    //read from check point
    for(int i = 0; i < check_point_size; ++i){
        graph_data* edge = mmap(NULL, sizeof(graph_data), fd, offset + i * sizeof(graph_data));
        node* node_a, node_b;
        if(edge->node_a == edge->node_b){
            if(nodes.find(node_a) == nodes.end()){
                node_a = new node(egde->node_a);
                nodes[egde->node_a] = node_a;
            }
        }else{
            if(nodes.find(node_a) == nodes.end()){
                node_a = new node(egde->node_a);
                nodes[egde->node_a] = node_a;
            }else{
                node_a = nodes[edge->node_a];
            }
            if(nodes.find(node_b) == nodes.end()){
                node_b = new node(egde->node_b);
            }else{
                node_b = nodes[edge->node_b];
            }
            node_a->neighbors[edge->node_b] = node_b;
            node_b->neighbors[edge->node_a] = node_a;
        }
    }
    //read from log
    for(int i = 1; i <= my_super_block.cur_block; ++i){
        log_block* log_page = mmap(NULL, sizeof(log_block), fd, i * 4096);
        if(log_page->generation != my_super_block.cur_generation) break;
        for(int j = 0; j < log_page->num_entry; ++j){
            long log_entry_offset = i * 4096 + sizeof(log_block);
            log_entry* single_log = mmap(NULL, sizeof(log_entry), fd, log_entry_offset + j * sizeof(log_entry));
            if(single_log->opcode == 0){
                this->add_node(single_log->node_a);
            }else if(single_log->opcode == 1){
                this->add_edge(single_log->node_a, single_log->node_b);
            }else if(single_log->opcode == 2){
                this->remove_node(single_log->node_a);
            }else{
                this->remove_edge(single_log->node_a, single_log->node_b);
            }
        }
    }
}

void graph::generate_edge_pairs(unordered_set<pair<uint64_t, uint64_t>>& unique_pairs){
    for(auto& a : nodes){
        if(a->neighbors.size() == 0){
            unique_pairs.insert({a->id, a->id});
        }else{
            for(auto& b : a->neighbors){
                if(unique_pairs.find({b->id, a->id}) == unique_pairs.end()){
                    unique_pairs.insert({a->id, b->id});
                }
            }
        }
    }
}

int graph::write_graph_to_vm(check_point& my_checkpoint, int fd){
    unordered_set<pair<uint64_t, uint64_t>> edge_pairs;
    this->generate_edge_pairs(edge_pairs);
    long offset = 2048000000 + sizeof(check_point);
    int i = 0;
    for(auto& edge : edge_pairs){
        graph_data* single_edge = mmap(NULL, sizeof(graph_data), fd, offset + i * sizeof(graph_data));
        if(single_edge == NULL){
            my_checkpoint.size = i;
            return 507;
        }
        single_edge->node_a = edge.first;
        single_edge->node_b = edge.second;
        i += 1;
    }
    my_checkpoint.size = edge_pairs.size();
    return 200;
}
