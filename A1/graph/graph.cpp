uint64_t graph::add_node(uint64_t node_id){
    unordered_map<uint64_t, node*>::iterator it = this->nodes.find(node_id);
    //not finding node_id
    if(it == this->nodes.end()){
        node* new_node = new node(node_id);
        this->nodes.insert({node_id, new_node});
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
    unordered_set<node*> a_neighbors = ita->second->neighbors;
    unordered_set<node*> b_neighbors = itb->second->neighbors;
    unordered_set<node*>::iterator ita_neighbors = a_neighbors.find(node_b_id);
    if(ita_neighbors != a_neighbors.end()){
        return 204;
    }
    a_neighbors.insert(itb->second);
    b_neighbors.insert(ita->second);
    return 200;
}

uint64_t graph::remove_node(uint64_t node_id){

}
uint64_t graph::remove_edge(uint64_t node_a_id, uint64_t node_b_id){

}
uint64_t graph::get_node(uint64_t node_id){

}
uint64_t graph::get_edge(uint64_t node_a_id, uint64_t node_b_id){

}
uint64_t graph::get_neighbors(uint64_t node_id){

}
uint64_t graph::shortest_path(uint64_t node_a_id, uint64_t node_b_id){

}