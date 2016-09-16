#include "./graph/graph.h"
#include "./library/mongoose.h"
//#include <json.h>

using namespace std;

static const char *s_http_port = "8000";
static struct mg_serve_http_opts s_http_server_opts;
static graph my_graph;

//  200 and a boolean JSON field in_graph indicating whether the node is in the graph
//    HTTP/1.1 200 OK
//    Content-Length: <length>
//    Content-Type: application/json
//
//    {
//        "in_graph": TRUE/FALSE
//    }
//  return true if node exists, otherwise false
//  bool get_node(uint64_t node_id);
//
//static void handle_sum_call(struct mg_connection *nc, struct http_message *hm) {
//    char n1[100], n2[100];
//    double result;
//    
//    /* Get form variables */
//    mg_get_http_var(&hm->body, "n1", n1, sizeof(n1));
//    mg_get_http_var(&hm->body, "n2", n2, sizeof(n2));
//    
//    /* Send headers */
//    mg_printf(nc, "%s", "HTTP/1.1 200 OK\r\nTransfer-Encoding: chunked\r\n\r\n");
//    
//    /* Compute the result and send it back as a JSON object */
//    result = strtod(n1, NULL) + strtod(n2, NULL);
//    mg_printf_http_chunk(nc, "{ \"result\": %lf }", result);
//    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
//}
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

    cout << len << endl;
    cout << "o<-<" << endl;
    
//    mg_printf_http_chunk(nc, "HTTP/1.1 200 OK \n");
//    mg_printf_http_chunk(nc, "Content-Length: <%d> \n", len);
//    mg_printf_http_chunk(nc, "Content-Type: application/json \n");
    mg_printf_http_chunk(nc, "{ \n \"in_graph\": %s \n} \n", sresult.c_str());
    mg_send_http_chunk(nc, "", 0); /* Send empty chunk, the end of response */
}

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
    struct http_message *hm = (struct http_message *) ev_data;
    
    switch (ev) {
        case MG_EV_HTTP_REQUEST:
            if (mg_vcmp(&hm->uri, "/api/v1/get_node") == 0) {
                handle_get_node_call(nc, hm); /* Handle RESTful call */
            } else if (mg_vcmp(&hm->uri, "/printcontent") == 0) {
                char buf[100] = {0};
                memcpy(buf, hm->body.p,
                       sizeof(buf) - 1 < hm->body.len ? sizeof(buf) - 1 : hm->body.len);
                printf("%s\n", buf);
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
    for (;;) {
        mg_mgr_poll(&mgr, 1000);
    }
    mg_mgr_free(&mgr);
    
    return 0;
}
