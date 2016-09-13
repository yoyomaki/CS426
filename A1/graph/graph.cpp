uint64_t graph::add_node(uint64_t node_id){
    unordered_map<uint64_t, node*>::iterator it = this->nodes.find(node_id);
    //not find
    if(it == this->nodes.end()){
        node* new_node = new node(node_id);
        this->nodes.insert({node_id,})
    }else{
        
    }
}


uint64_t graph::add_edge(uint64_t node_a_id, uint64_t node_b_id){

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