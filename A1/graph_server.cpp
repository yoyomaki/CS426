#include "./graph/graph.h"
#include "./library/mongoose.h"
#include <cstdlib>

using namespace std;

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;

static graph my_graph;

static void handle_add_node_call(struct mg_connection *nc, struct http_message *hm){
    /*parse from body*/
    uint64_t id;
    sscanf(hm->body.p, "{\"node_id\":%llu", &id);
    /*compute return value*/
    uint64_t res = my_graph.add_node(id);
    
    /*send header*/
    mg_printf(nc, "%s", ("HTTP/1.1 " + to_string(res) + " OK\r\n").c_str());
    
    /*send result as JSON*/
    if(res == 200){
        mg_printf(nc, "%s", ("Content-Length: " + to_string(hm->body.len + 10) + "\r\n").c_str());
        mg_printf(nc, "%s", "Content-Type: application/json\r\n");
        mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
        mg_printf_http_chunk(nc, "{\r\n\"node_id\":%llu\r\n}\r\n", id);
    }
    
    /* Send empty chunk, the end of response */
    mg_send_http_chunk(nc, "", 0);
}

static void handle_add_edge_call(struct mg_connection *nc, struct http_message *hm){
    uint64_t node_a_id;
    uint64_t node_b_id;
    int i;
    for(i = 0; i < strlen(hm->body.p); ++i){
        if(hm->body.p[i] == ',') break;
    }
    /*parse from body*/
    sscanf(hm->body.p, "{\"node_a_id\":%llu", &node_a_id);
    sscanf(hm->body.p+i+1, "\"node_b_id\":%llu", &node_b_id);
    
    /*compute return value*/
    uint64_t res = my_graph.add_edge(node_a_id, node_b_id);
    
    string echo = (res == 200) || (res == 204) ? "OK" : "Bad Request";
    /*send header*/
    mg_printf(nc, "%s", ("HTTP/1.1 " + to_string(res) + " " + echo + "\r\n").c_str());
    
    int len = res == 200 ? hm->body.len + 10 : 0;
    
    /*send result as JSON*/
    mg_printf(nc, "%s", ("Content-Length: " + to_string(len) + "\r\n").c_str());
    mg_printf(nc, "%s", "Content-Type: application/json\r\n");
    mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
    if(res == 200) mg_printf_http_chunk(nc, "{\r\n\"node_a_id\":%llu,\r\n\"node_b_id\":%llu\r\n}\r\n", node_a_id, node_b_id);
    
    /* Send empty chunk, the end of response */
    mg_send_http_chunk(nc, "", 0);
}

static void handle_remove_node_call(struct mg_connection *nc, struct http_message *hm){
    uint64_t node_id;
    /*parse from body*/
    sscanf(hm->body.p, "{\"node_id\":%llu", &node_id);
    
    /*compute return value*/
    uint64_t res = my_graph.remove_node(node_id);
    
    string echo = res == 200 ? "OK" : "Bad Request";
    /*send header*/
    mg_printf(nc, "%s", ("HTTP/1.1 " + to_string(res) + " " + echo + "\r\n").c_str());
    
    int len = res == 200 ? hm->body.len + 10 : 0;
    /*send result as JSON*/
    mg_printf(nc, "%s", ("Content-Length: " + to_string(len) + "\r\n").c_str());
    mg_printf(nc, "%s", "Content-Type: application/json\r\n");
    mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
    if(res == 200) mg_printf_http_chunk(nc, "{\r\n\"node_id\":%llu\r\n}\r\n", node_id);
    
    /* Send empty chunk, the end of response */
    mg_send_http_chunk(nc, "", 0);
}

static void handle_remove_edge_call(struct mg_connection *nc, struct http_message *hm){
    uint64_t node_a_id;
    uint64_t node_b_id;
    int i;
    for(i = 0; i < strlen(hm->body.p); ++i){
        if(hm->body.p[i] == ',') break;
    }
    /*parse from body*/
    sscanf(hm->body.p, "{\"node_a_id\":%llu", &node_a_id);
    sscanf(hm->body.p+i+1, "\"node_b_id\":%llu", &node_b_id);
    
    /*compute return value*/
    uint64_t res = my_graph.remove_edge(node_a_id, node_b_id);
    
    string echo = res == 200 ? "OK" : "Bad Request";
    /*send header*/
    mg_printf(nc, "%s", ("HTTP/1.1 " + to_string(res) + " " + echo + "\r\n").c_str());
    
    int len = res == 200 ? hm->body.len + 10 : 0;
    /*send result as JSON*/
    mg_printf(nc, "%s", ("Content-Length: " + to_string(len) + "\r\n").c_str());
    mg_printf(nc, "%s", "Content-Type: application/json\r\n");
    mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
    if(res == 200) mg_printf_http_chunk(nc, "{\r\n\"node_a_id\":%llu,\r\n\"node_b_id\":%llu\r\n}\r\n", node_a_id, node_b_id);
    
    /* Send empty chunk, the end of response */
    mg_send_http_chunk(nc, "", 0);
}

static void handle_get_node_call(struct mg_connection *nc, struct http_message *hm) {
    uint64_t node_id;
    bool result;
    string sresult;
    int len;
    
    /* Get form variables */
    sscanf(hm->body.p, "{\"node_id\":%llu", &node_id);
    
    /* Compute the result and send it back as a JSON object */
    result = my_graph.get_node(node_id);
    sresult = result ? "TRUE" : "FALSE";
    len = sresult.length() + 12;
    
    /* Send headers */
    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\n");
    mg_printf(nc, "%s", ("Content-Length: " + to_string(len) + "\r\n").c_str());
    mg_printf(nc, "%s", "Content-Type: application/json\r\n");
    mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");

    mg_printf_http_chunk(nc, "{ \n \"in_graph\": %s \n} \n", sresult.c_str());
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
    
    cout << "o<-<" << endl;
}

static void handle_get_edge_call(struct mg_connection *nc, struct http_message *hm) {
    uint64_t node_a_id;
    uint64_t node_b_id;
    pair<bool, bool> result;
    string sresult;
    int len;
    int i;
    for (i = 0; i < strlen(hm->body.p); i++) {
        if (hm->body.p[i] == ',') {
            break;
        }
    }
    sscanf(hm->body.p, "{\n\r\"node_a_id\":%llu", &node_a_id);
    sscanf(hm->body.p + i + 1, "\n\r\"node_b_id\":%llu", &node_b_id);
    
    /* Compute the result and send it back as a JSON object */
    result = my_graph.get_edge(node_a_id, node_b_id);
    
    // result.second == true if both nodes are in the graph,
    //               == false if at least one of the vertices does not exist
    if (result.second) {
        sresult = result.first ? "TRUE" : "FALSE";
        cout << sresult << endl;
        len = sresult.length() + 12;
        /* Send headers */
        mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\n");
        mg_printf(nc, "%s", ("Content-Length: " + to_string(len) + "\r\n").c_str());
        mg_printf(nc, "%s", "Content-Type: application/json\r\n");
        mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
        mg_printf_http_chunk(nc, "{ \n \"in_graph\": %s \n} \n", sresult.c_str());
    } else {
        mg_printf(nc, "%s", "HTTP/1.1 400 \r\n");
        mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
        cout << "node not in graph" << endl;
    }
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */

    cout << "o<-<" << endl;
}

static void handle_get_neighbors_call(struct mg_connection *nc, struct http_message *hm) {
    uint64_t node_id;
    pair<bool,unordered_map<uint64_t, node*> > result;
    string sresult;
    int len;
    
    /* Get form variables */
    sscanf(hm->body.p, "{\"node_id\":%llu", &node_id);
    
    /* Compute the result and send it back as a JSON object */
    result = my_graph.get_neighbors(node_id);
    len = result.second.size();
    unordered_map<uint64_t, node*>::iterator n_it;
    for (n_it = result.second.begin(); n_it != result.second.end(); ++n_it) {
        sresult += to_string(n_it->first) + " ";
    }
    // result.first indicates if the node is in the graph
    if (result.first) {
        /* Send headers */
        mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\n");
        mg_printf(nc, "%s", ("Content-Length: " + to_string(len) + "\r\n").c_str());
        mg_printf(nc, "%s", "Content-Type: application/json\r\n");
        mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
        
        mg_printf_http_chunk(nc, "{ \n \"node_id\": %llu \n", node_id);
        mg_printf_http_chunk(nc, " \"neighbors\": %s \n}\r\n", sresult.c_str());
    } else {
        mg_printf(nc, "%s", "HTTP/1.1 400 \r\n");
        mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
        cout << "node not in graph" << endl;
    }
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
    
    cout << "o<-<" << endl;
}


//u64 and a field distance containing the shortest path between the two nodes or
//204 if there is no path
//400 if either node does not exist
/*
 int = shortest path length, if returned -1, then there is no path between node a and b.
 Boolean = true if both nodes exist in the graph;
 false if any of them doesn't exist in the graph.
 */
//pair<uint64_t, bool> shortest_path(uint64_t node_a_id, uint64_t node_b_id);
static void handle_shortest_path_call(struct mg_connection *nc, struct http_message *hm) {
    uint64_t node_a_id;
    uint64_t node_b_id;
    uint64_t path_length;
    pair<uint64_t, bool> result;

    int len = 20;
    int i;
    for (i = 0; i < strlen(hm->body.p); i++) {
        if (hm->body.p[i] == ',') {
            break;
        }
    }
    sscanf(hm->body.p, "{\n\r\"node_a_id\":%llu", &node_a_id);
    sscanf(hm->body.p + i + 1, "\n\r\"node_b_id\":%llu", &node_b_id);
    
    /* Compute the result and send it back as a JSON object */
    result = my_graph.shortest_path(node_a_id, node_b_id);
    
    // result.second == true if both nodes are in the graph,
    //               == false if at least one of the vertices does not exist
    if (result.second) {
        path_length = result.first;
        // which is -1 in signed int, the case when we return 204
        if (path_length == 18446744073709551615) {
            mg_printf(nc, "%s", "HTTP/1.1 204 \r\n");
            mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
            cout << "path not found" << endl;
        } else {
            /* Send headers */
            mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\n");
            mg_printf(nc, "%s", ("Content-Length: " + to_string(len) + "\r\n").c_str());
            mg_printf(nc, "%s", "Content-Type: application/json\r\n");
            mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
            mg_printf_http_chunk(nc, "{ \n \"distance\": %llu \n} \n", path_length);
        }
    } else {
        mg_printf(nc, "%s", "HTTP/1.1 400 \r\n");
        mg_printf(nc, "%s", "Transfer-Encoding: chunked\r\n\r\n");
        cout << "node not in graph" << endl;
    }
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
    
    cout << "o<-<" << endl;
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    
    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if(mg_vcmp(&hm->uri, "/api/v1/add_node") == 0){
                handle_add_node_call(nc, hm);
            }else if(mg_vcmp(&hm->uri, "/api/v1/add_edge") == 0){
                handle_add_edge_call(nc, hm);
            }else if(mg_vcmp(&hm->uri, "/api/v1/remove_node") == 0){
                handle_remove_node_call(nc, hm);
            }else if(mg_vcmp(&hm->uri, "/api/v1/remove_edge") == 0){
                handle_remove_edge_call(nc, hm);
            } else if(mg_vcmp(&hm->uri, "/api/v1/get_node") == 0) {
                handle_get_node_call(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/get_edge") == 0) {
                handle_get_edge_call(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/get_neighbors") == 0) {
                handle_get_neighbors_call(nc, hm);
            } else if (mg_vcmp(&hm->uri, "/api/v1/shortest_path") == 0) {
                handle_shortest_path_call(nc, hm);
            } else {
                mg_serve_http(nc, hm, s_http_server_opts); /* Serve static content */
            }
            break;
        default:
            break;
    }
}

int main(int argc, char *argv[]) {
    struct mg_mgr mgr;
    struct mg_connection *nc;
    struct mg_bind_opts bind_opts;
    int i;
    char *cp;
    const char *err_str;
#ifdef MG_ENABLE_SSL
    const char *ssl_cert = NULL;
#endif
    
    mg_mgr_init(&mgr, NULL);
    
    /* Use current binary directory as document root */
    if (argc > 0 && ((cp = strrchr(argv[0], DIRSEP)) != NULL)) {
        *cp = '\0';
        s_http_server_opts.document_root = argv[0];
    }
    
    /* Process command line options to customize HTTP server */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-D") == 0 && i + 1 < argc) {
            mgr.hexdump_file = argv[++i];
        } else if (strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            s_http_server_opts.document_root = argv[++i];
        } else if (strcmp(argv[i], "-p") == 0 && i + 1 < argc) {
            s_http_port = argv[++i];
        } else if (strcmp(argv[i], "-a") == 0 && i + 1 < argc) {
            s_http_server_opts.auth_domain = argv[++i];
#ifdef MG_ENABLE_JAVASCRIPT
        } else if (strcmp(argv[i], "-j") == 0 && i + 1 < argc) {
            const char *init_file = argv[++i];
            mg_enable_javascript(&mgr, v7_create(), init_file);
#endif
        } else if (strcmp(argv[i], "-P") == 0 && i + 1 < argc) {
            s_http_server_opts.global_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-A") == 0 && i + 1 < argc) {
            s_http_server_opts.per_directory_auth_file = argv[++i];
        } else if (strcmp(argv[i], "-r") == 0 && i + 1 < argc) {
            s_http_server_opts.url_rewrites = argv[++i];
#ifndef MG_DISABLE_CGI
        } else if (strcmp(argv[i], "-i") == 0 && i + 1 < argc) {
            s_http_server_opts.cgi_interpreter = argv[++i];
#endif
#ifdef MG_ENABLE_SSL
        } else if (strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            ssl_cert = argv[++i];
#endif
        } else {
            fprintf(stderr, "Unknown option: [%s]\n", argv[i]);
            exit(1);
        }
    }
    
    /* Set HTTP server options */
    memset(&bind_opts, 0, sizeof(bind_opts));
    bind_opts.error_string = &err_str;
#ifdef MG_ENABLE_SSL
    if (ssl_cert != NULL) {
        bind_opts.ssl_cert = ssl_cert;
    }
#endif
    nc = mg_bind_opt(&mgr, s_http_port, ev_handler, bind_opts);
    if (nc == NULL) {
        fprintf(stderr, "Error starting server on port %s: %s\n", s_http_port,
                *bind_opts.error_string);
        exit(1);
    }
    
    mg_set_protocol_http_websocket(nc);
    s_http_server_opts.enable_directory_listing = "yes";
    
    printf("Starting RESTful server on port %s, serving %s\n", s_http_port,
           s_http_server_opts.document_root);
    
    //
    my_graph.add_node(1);
    my_graph.add_node(2);
    my_graph.add_node(3);
    my_graph.add_node(4);
    my_graph.add_edge(1, 2);
    my_graph.add_edge(1, 3);
    my_graph.add_edge(2, 4);
    
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
    
    return 0;
}
