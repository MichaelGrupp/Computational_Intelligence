/*Your program shall read in a graph in adjacency list format and perform a breadth first search(BFS) on the graph,
starting at the node first read. Each node's name is a single letter 'A'-'Z', so you can safely assume less than 27 nodes for your graph.
The format of each line to be read in is specified as
<nodeName>-<string of neighboring node names>\n(see example below).
The requested output  of your program is one line of node names followed by '\n',
in which the order of names represents one possible exploration following breadth first search.
Note(1) : for most inputs several different outputs are possible.Here we only ask you to find one of them!
Note(2) : all graphs here are undirected, i.e. if a connection A - ...B... exists you will also find a connection B - ...A...
*/


#include<stdio.h>
#include <stdlib.h>

#define ADD_CAPACITY 10

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
	node->neighbors = (Node*)malloc(ADD_CAPACITY*sizeof(Node*));
	node->neighborCapacity = ADD_CAPACITY;
	node->neighborCount = 0;
}
void initGraph(Graph* graph) {
	graph->nodes = (Node**)malloc(ADD_CAPACITY*sizeof(Node**));
	graph->nodeCapacity = ADD_CAPACITY;
	graph->nodeCount = 0;
}

void addNeighborCapacityIfFull(Node* node) {
	if (node->neighborCount >= node->neighborCapacity) {
		node->neighborCapacity += ADD_CAPACITY;
		node->neighbors = (Node*)realloc(node->neighbors, node->neighborCapacity*sizeof(Node*));
	}
}
void addNodeCapacityIfFull(Graph* graph) {
	if (graph->nodeCount >= graph->nodeCapacity) {
		graph->nodeCapacity += ADD_CAPACITY;
		graph->nodes = (Node**)realloc(graph->nodes, graph->nodeCapacity*sizeof(Node**));
	}
}

void addNode(Graph* graph, Node* node) {
	addNodeCapacityIfFull(graph);
	graph->nodes[graph->nodeCount++] = *node;
}
void addNeighbor(Node* node, const Node* neighbor) {
	addNeighborCapacityIfFull(node);
	node->neighbors[node->neighborCount++] = neighbor;
}

//search a node in a Graph - return: position or -1 if not found
int searchNode(Graph* graph, char name) {
	for (int pos = 0; pos < graph->nodeCount; pos++) {
		if (graph->nodes[pos].name == name)
			return pos; //found
	}
	return -1; //not found
}
//search a neighbor in a Node - return: position or -1 if not found
int searchNeighbor(Node* node, char name) {
	for (int pos = 0; pos < node->neighborCount; pos++) {
		if (node->neighbors[pos]->name == name)
			return pos; //found
	}
	return -1; //not found
}

//process the input from system, e.g. X-AB
void processInput(char* input, int inputSize, Graph theGraph) {

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

//empty dummy structs for easier initialization
Node* emptyNodePtr() {
	Node* emptyNode = (Node*)malloc(sizeof(Node));
	emptyNode->neighborCapacity = 0;
	return emptyNode;
}
Graph emptyGraph() {
	Graph* emptyGraph = (Graph*)malloc(sizeof(Graph));
	emptyGraph->nodeCount = 0;
	emptyGraph->nodes = emptyNodePtr();
	return *emptyGraph;
}
//static const Node emptyNode = { '.', &emptyNode, 0 };
//static const Graph emptyGraph = { &emptyNode, 0 };

int main() {
	Graph graph = emptyGraph();

	//test Graph and Node
	addNode(graph.nodes, &graph.nodeCount, 'X');
	addNode(graph.nodes, &graph.nodeCount, 'A');
	int index1 = searchNode(graph.nodes, graph.nodeCount, 'X');
	int index2 = searchNode(graph.nodes, graph.nodeCount, 'A');
	addNodePtr(graph.nodes[index1].neighbors, &graph.nodes[index1].neighborCapacity, &graph.nodes[index2]);
	addNode(graph.nodes, &graph.nodeCount, 'C');

	return(0);
}