# Assignment 1: Simple HTTP API for an in-memory graph #

----

## Introduction ##

In this class you will be building a service which responds to graph requests.
Storing and performing queries over graphs is important in fields such as pathfinding, image processing,
and social networks, and the ideal storage strategy is still often an open research question.  

In this lab you will start by implementing a simple single-node in-memory version of your server, which will store undirected graphs.
Next lab you will use secondary storage to add durability and increase the supported graph size,
and in later labs you will add features such as failure-atomicity, scalability, and transactions.  

**NOTE:** Since in later labs you will add features to your early servers, the quality of your implementation
will have a major effect on how difficult you find later labs.

In this lab you'll be using the Zoo machines; for later labs we'll switch to the Google cloud.

## Changelog ##

```
```

## Submission Instructions ##

For this assignment you will submit your implementation using [Git](www.git-scm.com). More information about Git can be found in the [tutorial](http://www.kernel.org/pub/software/scm/git/docs/gittutorial.html), [user's manual](http://www.kernel.org/pub/software/scm/git/docs/user-manual.html), or by googling your specific problem.

To obtain your initial repository for this course, `ssh` to your cs zoo account (turn to [this](http://zoo.cs.yale.edu/newzoo/) for detailed zoo usage instruction), `cd` to the directory you wish to keep your repository and run.
```
$ /c/cs426/getrepo.sh
Cloning into bare repository /c/cs426/SUBMIT/netid-digits...
done.
Cloning into cs426_graph_server...
done.
$ cd cs426_graph_server
$
```
**NOTE:** You should run this script _only once_, at the beginning of this course. Running it multiple times will likely result in your work getting lost.

To submit your work, commit your changes, label your commit `lab1` with `git tag lab1` and perform a `git push && git push --tags`.
Your submission should be such that the following script
```sh
make && ./cs426_graph_server <port>
```
runs your server listening on port `port`.

## Interfaces ##

We're using a simple HTTP based system, which only uses the `HTTP` and `Content-Length` headers. All requests will be in the form:
```HTTP
POST <function_name> HTTP/1.0
<other-headers?>
Content-Length: <length>
<other-headers?>

<length bytes of JSON encoded content>
```

The arguments will be in [JSON](http://www.json.org/). You can use the [Mongoose Library](https://github.com/cesanta/mongoose) in C or C++ for both request handling and JSON decoding if you so desire to. Seach for JSON and HTTP in the header file for more information.


In this lab you will implement the following functions for your undirected-graph store:

   Function    | Method |    Arguments     | Return
-------------- | ------ | ---------------- | ------
 `add_node`    | `POST` | `u64 node_id`    |  `200` on success<br/> `204`if the node already exists
 `add_edge`    | `POST` | `u64 node_a_id`, `u64 node_b_id`  |  `200` on success<br/>`204` if the edge already exists<br /> `400` if either node doesn't exist, or if `node_a_id` is the same as `node_b_id`
 `remove_node` | `POST` | `u64 node_id` | `200` on success<br/> `400`if the node does not exist
 `remove_edge` | `POST` | `u64 node_a_id`, `u64 node_b_id`  |  `200` on success<br/>`400` if the edge does not exist
 `get_node`    | `POST` | `u64 node_id` | `200` and a boolean JSON field `in_graph` indicating whether the node is in the graph
 `get_edge`    | `POST` | `u64 node_a_id`, `u64 node_b_id`  |  `200` and a boolean JSON field `in_graph` indicating whether the node is in the graph<br/>`400` of at least one of the vertices does not exist
 `get_neighbors`   | `POST`  | `u64 node_id` | `200` and a list of neighbors[*](#get_neighbors_description) or<br/> `400` if the node does not exist
 `shortest_path`   | `POST`  | `u64 node_a_id`, `uint node_b_id` | `u64` and a field `distance` containing the shortest path between the two nodes or<br/>`204` if there is no path <br/>`400` if either node does not exist

Any
 In the current server version, the endpoints must be prefixed with `/api/v1/`, see the [examples](#examples) for more detail.

<a name="get_neighbors_description">*</a> Specifically the contents returned from `get_neighbors` must be in the form
```JSON
{
  "node_id": <node_id>,
  "neighbors": [<neighbors>]
}
```

### Example Requests and Responses ###
<a name="examples"></a>
#### add_node ####
Sample Request:
```HTTP
POST www.example.com:8000/api/v1/add_node HTTP/1.1
Content-Length: <length>
Content-Type: application/json

{
  "node_id": 123
}
```
Sample Response:
When request resulted in a newly created node:
```HTTP
HTTP/1.1 200 OK
Content-Length: <length>
Content-Type: application/json

{
  "node_id": 123
}
```
When requested node_id is already in the graph (status code 204, which MUST NOT contain any payload):
```HTTP
HTTP/1.1 204 OK
<other-headers>
```

#### add_edge ####
Sample Request:
```HTTP
POST www.example.com:8000/api/v1/add_edge HTTP/1.1
Content-Length: <length>
Content-Type: application/json

{
  "node_a_id": 1,
  "node_b_id": 2
}
```
Sample Response:
When request resulted in a newly created edge:
```HTTP
HTTP/1.1 200 OK
Content-Length: <length>
Content-Type: application/json

{
  "node_a_id": 1,
  "node_b_id": 2
}
```
When specified edge is already in the graph (status code 204, which MUST NOT contain any payload):
```HTTP
HTTP/1.1 204 OK
<other-headers>
```
When one or both of the specified nodes are not in the graph, respond with status code 400 Bad Request:
```HTTP
HTTP/1.1 400 Bad Request
<other-headers>

<optional error message payload>
```

## Test Script ##

To assist with debugging, a test script containing some of the tests we will use to grade the assignment can be found at `/c/cs426/scripts/lab1test.sh` it should be invoked
```sh
/c/cs426/scripts/lab1test.sh http://<host>:<port>
```
There is also a simple mode which can be invoked:
```sh
/c/cs426/scripts/lab1test.sh -s http://<host>:<port>
```

You can also use `curl` to test each individual request by yourself. For example, if you want to add a node with node_id 1 to your local graph server, which is listening on port 8000. You can simply issuing a HTTP request by
```
curl -H "Content-Type: application/json" -X POST -d '{"node_id":1}' http://127.0.0.1:8000/api/v1/add_node
```
