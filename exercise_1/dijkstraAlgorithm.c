/*Your program shall read in a graph with associated costs for traversing edges, 
and find optimal paths from a starting node to all other nodes in this graph.
This is the same problem as finding shortest distances on highways that connect cities!
Each node's name is a single letter 'A'-'Z', so you can safely assume less than 27 nodes for your graph. 
Each connected pair of nodes will be provided as an independent input line in the following format:
<nodeNameA>-<nodeNameB>-<cost>\n(see example below).
You can assume the cost of traveling between neighboring nodes to be a positive integer number below 100 for each single edge : 0 < edge - cost < 100.

Your program needs to traverse the graph following Dijkstra's algorithm to find the shortest routes from a given starting node S to all other 
nodes in the graph. S is the one node that was mentioned first in the first line of input. When done, for each node N that exists in the graph, 
your program shall provide the backtracking path from N to S with the final (minimal) associated cost for traveling between N and S. 
Print exactly one such line (path and cost) for each node that exists in the system (see example below); the ordering of your lines does not matter.
	
Note(1) : the starting node S is also a node in the system, with traversal cost of 0.
Note(2) : all graphs here are undirected, i.e. if an edge A - B - cost exists you will also find an edge B - A - cost.
*/

#include<stdio.h>
#include<stdlib.h>

#define ADD_CAPACITY 10
#define NOT_FOUND -1

typedef struct Node {
	char name; //e.g. 'X'
	struct Node** neighbors;
	int* neighborCosts; //edge costs to neighbors - same order as neighbors array
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
	node->neighborCosts = (int*)malloc(ADD_CAPACITY*sizeof(int));
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

//memory handling
void addNeighborCapacityIfFull(Node* node) {
	if (node->neighborCount >= node->neighborCapacity) {
		node->neighborCapacity += ADD_CAPACITY;
		Node** temp = (Node**)realloc(node->neighbors, node->neighborCapacity*sizeof(Node*));
		int* temp2 = (int*)realloc(node->neighborCosts, node->neighborCapacity*sizeof(int));
		if (temp == NULL || temp2 == NULL) {
			//fprintf(stderr, "Reallocation failed --> %s\n", strerror(errno));
			free(node->neighbors);
			exit(EXIT_FAILURE);
		}
		else {
			node->neighbors = temp;
			node->neighborCosts = temp2;
		}
	}
}
//memory handling
void addNodeCapacityIfFull(Graph* graph) {
	if (graph->nodeCount >= graph->nodeCapacity) {
		graph->nodeCapacity += ADD_CAPACITY;
		Node* temp = (Node*)realloc(graph->nodes, graph->nodeCapacity*sizeof(Node));
		if (temp == NULL) {
			//fprintf(stderr, "Reallocation failed --> %s\n", strerror(errno));
			free(graph->nodes);
			exit(EXIT_FAILURE);
		}
		else {
			graph->nodes = temp;
		}
	}
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

void addNode(Graph* graph, char name) {
	addNodeCapacityIfFull(graph);
	Node node;
	initNode(&node);
	node.name = name;
	graph->nodes[graph->nodeCount++] = node;
}
void addNeighbor(Node* node, const Node* neighbor, int cost) {
	addNeighborCapacityIfFull(node);
	node->neighbors[node->neighborCount] = neighbor;
	node->neighborCosts[node->neighborCount++] = cost;
	//adding a neighbor is a mutual operation --> add node and cost also to neighbor
	if (searchNeighbor(neighbor, node->name) == NOT_FOUND) {
		addNeighbor(neighbor, node, cost);
	}
}

void printNode(Node node) {
	printf("%c", node.name);
	if (node.neighborCount >= 1) {
		printf("-");
		for (int i = 0; i < node.neighborCount; i++)
			printf("-(%d)%c", node.neighborCosts[i], node.neighbors[i]->name);
	}
	printf("\n");
}
void printGraph(Graph graph) {
	for (int i = 0; i < graph.nodeCount; i++)
		printNode(graph.nodes[i]);
}

//process the input from system, e.g. X-Y-3
void buildGraphFromInput(char* input, Graph* graph) {
	char name = input[0];
	if (name == '\n') return; //catch empty case
	//check if node already exists
	if (searchNode(graph, name) == NOT_FOUND) {
		addNode(graph, name);
	}
	int nodeIndex = searchNode(graph, name);
	//add neighbor with its edge cost after '-'
	if (input[1] == '\n') return; //catch one letter case
	name = input[2];
	int cost = input[4]-48; //'0' is 48 in ASCII
	if (searchNode(graph, name) == NOT_FOUND)
			addNode(graph, name);
	int neighborIndex = searchNode(graph, name);
	//check if neighbor already exists
	if (searchNeighbor(&graph->nodes[nodeIndex], name) == NOT_FOUND)
		addNeighbor(&graph->nodes[nodeIndex], &graph->nodes[neighborIndex], cost);
}

int alreadyVisited(char* visited, char name, int count) {
	for (int i = 0; i <= count; i++) {
		if (visited[i] == name)
			return 1;
	}
	return 0;
}

void depthFirstSearch(Node node, char* visited, int* count) {
	printf("%c", node.name);
	visited[*count] = node.name; *count = *count + 1;
	if (node.neighborCount != 0) {
		for (int i = 0; i < node.neighborCount; i++) {
			if (!alreadyVisited(visited, node.neighbors[i]->name, *count))
				depthFirstSearch(*node.neighbors[i], visited, count);
		}
	}
}

int main() {
	Graph graph;
	initGraph(&graph);

	char input[100];

	//read console input and build graph
	//while (1) {
	//	if (fgets(input, 100, stdin)) { //fgets returns NULL at EOF (EOF in VS cmd line: enter ctrl+z enter)
	//		buildGraphFromInput(input, &graph);
	//	}
	//	else
	//		break;
	//}

	//debug only
	//required output:  X-Y-3\n Y-0\n Z-X-Y-7\n
	buildGraphFromInput("Y-X-3\n", &graph);
	buildGraphFromInput("X-Y-3\n", &graph);
	buildGraphFromInput("X-Z-4\n", &graph);
	buildGraphFromInput("Z-X-4\n", &graph);
	printGraph(graph);

	//traverse graph with Dijkstra's Algorithm
	//if (graph.nodeCount > 0) {
	//	char* visited = (char*)malloc(graph.nodeCount*sizeof(char)); int count = 0;
	//	depthFirstSearch(graph.nodes[0], visited, &count);
	//}

	printf("\n");



	return EXIT_SUCCESS;
}