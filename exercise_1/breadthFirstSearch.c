/*Your program shall read in a graph in adjacency list format and perform a breadth first search(BFS) on the graph,
starting at the node first read. Each node's name is a single letter 'A'-'Z', so you can safely assume less than 27 nodes for your graph.
The format of each line to be read in is specified as
<nodeName>-<string of neighboring node names>\n(see example below).
The requested output  of your program is one line of node names followed by '\n',
in which the order of names represents one possible exploration following breadth first search.
Note(1) : for most inputs several different outputs are possible.Here we only ask you to find one of them!
Note(2) : all graphs here are undirected, i.e. if a connection A - ...B... exists you will also find a connection B - ...A...
*/
#define _NO_DEBUG_HEAP 1 //http://stackoverflow.com/a/21641421

#include<stdio.h>
#include <stdlib.h>

#define ADD_CAPACITY 10
#define NOT_FOUND -1

typedef struct Node {
	char name; //e.g. 'X'
	struct Node** neighbors;
	int neighborCount; //actual number of neighbors
	int neighborCapacity; //capacity of neighbors array
} Node;

typedef struct Graph {
	Node* nodes;
	int nodeCount;
	int nodeCapacity;
} Graph;


void initNode(Node* node) {
	node->name = '.';
	//allocate type stored in array (Node*), but cast to array type (Node**)!!
	node->neighbors = (Node**)malloc(ADD_CAPACITY*sizeof(Node*));
	node->neighborCapacity = ADD_CAPACITY;
	node->neighborCount = 0;
}
void initGraph(Graph* graph) {
	graph->nodes = (Node*)malloc(ADD_CAPACITY*sizeof(Node));
	graph->nodeCapacity = ADD_CAPACITY;
	graph->nodeCount = 0;
}

void freeNode(Node* node) {
	free(node->neighbors);
	node->neighborCount = 0;
	node->neighborCapacity = 0;
	node->name = '.';
}
void freeGraph(Graph* graph) {
	free(graph->nodes);
	graph->nodeCount = 0;
	graph->nodeCapacity = 0;
}

void addNeighborCapacityIfFull(Node* node) {
	if (node->neighborCount >= node->neighborCapacity) {
		node->neighborCapacity += ADD_CAPACITY;
		Node** temp = (Node**)realloc(node->neighbors, node->neighborCapacity*sizeof(Node*));
		if (temp == NULL) {
			fprintf(stderr, "Reallocation failed --> %s\n", strerror(errno));
			free(node->neighbors);
			exit(EXIT_FAILURE);
		}
		else {
			node->neighbors = temp;
		}
	}
}
void addNodeCapacityIfFull(Graph* graph) {
	if (graph->nodeCount >= graph->nodeCapacity) {
		graph->nodeCapacity += ADD_CAPACITY;
		Node* temp = (Node*)realloc(graph->nodes, graph->nodeCapacity*sizeof(Node));
		if (temp == NULL) {
			fprintf(stderr, "Reallocation failed --> %s\n", strerror(errno));
			free(graph->nodes);
			exit(EXIT_FAILURE);
		}
		else {
			graph->nodes = temp;
		}
	}
}

void addNode(Graph* graph, char name) {
	addNodeCapacityIfFull(graph);
	Node node;
	initNode(&node);
	node.name = name;
	graph->nodes[graph->nodeCount++] = node;
}
void addNeighbor(Node* node, const Node* neighbor) {
	addNeighborCapacityIfFull(node);
	node->neighbors[node->neighborCount++] = neighbor;
}

//search a node in a Graph - return: position or NOT_FOUND
int searchNode(Graph* graph, char name) {
	for (int pos = 0; pos < graph->nodeCount; pos++) {
		if (graph->nodes[pos].name == name)
			return pos; //found
	}
	return NOT_FOUND; //not found
}
//search a neighbor in a Node - return: position or NOT_FOUND
int searchNeighbor(Node* node, char name) {
	for (int pos = 0; pos < node->neighborCount; pos++) {
		if (node->neighbors[pos]->name == name)
			return pos; //found
	}
	return NOT_FOUND; //not found
}

//process the input from system, e.g. X-AB
void processInput(char* input, Graph* graph) {
	char name = '.';
	for (int i = 0; name != '\n'; i++) {
		name = input[i];
		//check if node already exists
		if (searchNode(graph, name) == NOT_FOUND) {
			addNode(graph, name);
		}
		/*
		lalala
		*/
	}
}

/*
build adjacency list
input: x-nnnnn

parse input: 1.node 2.skip 3.to\n neighbors

-search graph for node with name x
--if not exists, create node x
---add node x to graph

for each neighbor n
-search graph for node with name n
--if not exists, create node n
---add node n to graph
-search neighbors of x for n
--if not listed, add node n to neighbors of x
-search neighbors of n for x
--if not listed, add node x to neighbors of n
*/

int main() {
	//test Graph and Node
	Graph graph;
	initGraph(&graph);

	addNode(&graph, 'X');
	addNode(&graph, 'A');
	addNode(&graph, 'B');
	addNode(&graph, 'C');
	addNode(&graph, 'D');
	addNode(&graph, 'E');
	addNode(&graph, 'F');
	addNode(&graph, 'G');
	addNode(&graph, 'H');
	addNode(&graph, 'I');
	addNode(&graph, 'J');
	addNode(&graph, 'K');

	int res = searchNode(&graph, 'J');
	res = searchNode(&graph, 'V');

	freeGraph(&graph);

	return(0);
}