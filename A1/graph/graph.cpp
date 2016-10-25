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
    int check_point_size = my_checkpoint.size;
      cout << "starting to set graph from vm, checkpoint size is " << check_point_size<< endl;
    uint64_t offset = OFFSET + (1 << 12);
//    cout << "a" << offset << endl;
    //read from check point
    int remainder = check_point_size % 256;
    int num_pages = check_point_size / 256 + (remainder == 0 ? 0 : 1);
    int index = 0;
    for (int i = 0; i < num_pages; i++) {
        long long tmp = offset + i * 4096;
        graph_page* page = (graph_page*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, tmp);
        for (int j = 0; j < 256; j++) {
            if (index == check_point_size) {
                break;
            }
            index++;
            // + j = + j's graph_data size
            graph_data edge = page->edges[j];
            node* node_a = NULL;
            node* node_b = NULL;
            // add node
            if (edge.node_a == edge.node_b) {
                if (nodes.find(edge.node_a) == nodes.end()) {
                    node_a = new node(edge.node_a);
                    nodes[edge.node_a] = node_a;
                }
                // add edge
            } else {
                if (nodes.find(edge.node_a) == nodes.end()) {
                    node_a = new node(edge.node_a);
                    nodes[edge.node_a] = node_a;
                } else {
                    node_a = nodes[edge.node_a];
                }
                if (nodes.find(edge.node_b) == nodes.end()) {
                    node_b = new node(edge.node_b);
                } else {
                    node_b = nodes[edge.node_b];
                }
                node_a->neighbors[edge.node_b] = node_b;
                node_b->neighbors[edge.node_a] = node_a;
            }
        }
    }
    
    //read from log
    cout << "reading log , cur_block is " << my_super_block.cur_block << endl;
    for(uint32_t i = 1; i <= my_super_block.cur_block; ++i){
        log_block* log_page = (log_block*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, i * 4096);
        uint64_t cur_cs = log_page->check_sum;
        if (cur_cs != (log_page->generation ^ log_page->num_entry)) {
//            cout << "gen is " << log_page->generation << " entry is " << log_page->num_entry << endl;
//            cout << "cs is " << cur_cs << ", should be " << (log_page->generation ^ log_page->num_entry) << endl;
//            cout << "CHECKSUM NOT EQUAL BREAKING" << endl;
            break;
        }
//        cout << "logpage generation is " << log_page->generation << endl;
//        cout << "super block generation is " << my_super_block.cur_generation << endl;
        // checkpoint || format gen++
        if(log_page->generation != my_super_block.cur_generation) break;
        cout << "start reading log for setting graph, number of entry of this page is " << log_page->num_entry << endl;
        for(uint32_t j = 0; j < log_page->num_entry; ++j){
            log_entry single_log = log_page->logs[j];
            cout << single_log.opcode << "  " << single_log.node_a << " " << single_log.node_b << endl;
            if(single_log.opcode == 0){
                this->add_node(single_log.node_a);
            }else if(single_log.opcode == 1){
                this->add_edge(single_log.node_a, single_log.node_b);
            }else if(single_log.opcode == 2){
                this->remove_node(single_log.node_a);
            }else{
                this->remove_edge(single_log.node_a, single_log.node_b);
            }
        }
    }
}


void graph::generate_edge_pairs(vector<pair<uint64_t, uint64_t>>& unique_pairs){
    for(auto& a : nodes){
        if(a.second->neighbors.size() == 0){
            unique_pairs.push_back(make_pair(a.first, a.first));
        }else{
            for(auto& b : a.second->neighbors){
                if(find(unique_pairs.begin(), unique_pairs.end(), make_pair(b.first, a.first)) == unique_pairs.end()){
                    unique_pairs.push_back(make_pair(a.first, b.first));
                }
            }
        }
    }
}

int graph::write_graph_to_vm(check_point& my_checkpoint, int fd){
    cout << "starting checkpoint" << endl;
    vector<pair<uint64_t, uint64_t>> edge_pairs;
    this->generate_edge_pairs(edge_pairs);
    //long long offset = 2 * 1024 * 1024 * 1024 + 4096;
    long long offset = OFFSET + (1 << 12);
    long long total_page_av = (OFFSET*5 - offset) / 4096;
    int total_page = edge_pairs.size() / 256;
    int remainder = edge_pairs.size() % 256;
    if (total_page > total_page_av || (total_page == total_page_av && remainder > 0)) {
        return 507;
    }
    int index = 0;
    int page_no = 0;
    graph_page* page = (graph_page*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset + page_no * 4096);
    for (auto& edge : edge_pairs) {
        page->edges[index].node_a = edge.first;
        page->edges[index].node_b = edge.second;
        index += 1;
        if (index == 256) {
            page_no += 1;
            page = (graph_page*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset + page_no * 4096);
            index = 0;
        }
    }
    clear_superblock_after_checkpoint(fd);
    my_checkpoint.size = edge_pairs.size();
    check_point* written_check_point = (check_point*)mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, OFFSET);
    written_check_point -> size =  my_checkpoint.size;
    cout << "after checkpoint, size should be " << my_checkpoint.size << endl;
    return 200;
}

void graph::print(){
    for(auto m : nodes){
        cout << m.first << endl;
    }
}
