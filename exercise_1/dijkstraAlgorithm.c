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
#define INFINITY 666

typedef struct Node {
	char name; //e.g. 'X'
	struct Node** neighbors;
	int* neighborCosts; //edge costs to neighbors - same order as neighbors array
	int neighborCount; //actual number of neighbors
	int neighborCapacity; //capacity of neighbors array
	//data for a Dijkstra search
	int visited, distance;
	char previous;
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
		printf(" - neighbours:");
		for (int i = 0; i < node.neighborCount; i++)
			printf(" %c(%d);", node.neighbors[i]->name, node.neighborCosts[i]);
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
	//quick and dirty and ugly af hack - cost is smaller than 100
	int cost;
	if(input[5]=='-')
		cost = input[4] - 48; //'0' is 48 in ASCII
	else {
		char str[2] = { input[4], input[5] };
		cost = atoi(str);
	}
	if (searchNode(graph, name) == NOT_FOUND)
		addNode(graph, name);
	int neighborIndex = searchNode(graph, name);
	//check if neighbor already exists
	if (searchNeighbor(&graph->nodes[nodeIndex], name) == NOT_FOUND)
		addNeighbor(&graph->nodes[nodeIndex], &graph->nodes[neighborIndex], cost);
}

//returns index of smallest distance node that is NOT known - returns -1 all nodes are known
int findSmallestDistanceNode(Graph* graph) {
	int smallestDistance = INFINITY;
	int index = -1;
	for (int i = 0; i < graph->nodeCount; i++) {
		if (graph->nodes[i].visited == 0) {
			if (graph->nodes[i].distance < smallestDistance) {
				smallestDistance = graph->nodes[i].distance;
				index = i;
		}}
	}
	return index;
}

//assign Node.previous and Node.distance with Dijkstra's algorithm
void dijkstraAlgorithm(Graph* graph, Node* start) {
	//initialize
	for (int i = 0; i < graph->nodeCount; i++) {
		graph->nodes[i].distance = INFINITY;
		graph->nodes[i].visited = 0; //0 = not known
		//graph->nodes[i].path = (char*)malloc(graph->nodeCount*sizeof(char)); //longest path goes through all nodes
	}
	//set distance of start node to 0 and its path to itself
	start->distance = 0;
	start->previous = start->name;

	//while there exist unknown vertices, find node b with smallest distance
	int bIndex = findSmallestDistanceNode(graph);
	while (bIndex != -1) {
		Node* b = &graph->nodes[bIndex];
		b->visited = 1;
		//for each a adjacent to b
		for (int i = 0; i < b->neighborCount; i++) {
			Node* a = b->neighbors[i];
			if (a->visited == 0) {
				if ((b->distance + b->neighborCosts[i]) < a->distance) {//b->neighborCosts[i] is cost of b to a
					a->distance = (b->distance + b->neighborCosts[i]); //"relax": decrease distance of a
					a->previous = b->name;
				}
			}
		}
		bIndex = findSmallestDistanceNode(graph);
	}
}

//print backtracking path and its distance (path cost) for each node after Dijkstra's algorithm
void printBacktrackingPaths(Graph* graph) {
	for (int i = 0; i < graph->nodeCount; i++) {
		printf("%c", graph->nodes[i].name);
		if (graph->nodes[i].distance != 0) { //if not start node
			Node* tmpNode = &graph->nodes[searchNode(graph, graph->nodes[i].previous)]; //get next node in backtracking path
			while (1) {
				//print next node in backtracking path
				printf("-%c", tmpNode->name);
				if (tmpNode->previous == tmpNode->name) 
					break; //reached start node
				tmpNode = &graph->nodes[searchNode(graph, tmpNode->previous)];
			}
		}
		printf("-%d\n", graph->nodes[i].distance);//print distance (path cost) at the end
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
	//required output:   	A-0\n, B-F-D-A-36\n, C-F-D-A-24\n, D-A-12\n, E-G-C-F-D-A-33\n, F-D-A-20\n, G-C-F-D-A-26\n, H-C-F-D-A-27\n, I-J-H-C-F-D-A-30\n, J-H-C-F-D-A-28\n
	buildGraphFromInput("A-B-56\n", &graph);
	buildGraphFromInput("A-D-12\n", &graph); 
	buildGraphFromInput("B-A-56\n", &graph); 
	buildGraphFromInput("B-C-18\n", &graph); 
	buildGraphFromInput("B-F-16\n", &graph); 
	buildGraphFromInput("B-H-21\n", &graph); 
	buildGraphFromInput("B-I-8\n", &graph); 
	buildGraphFromInput("C-B-18\n", &graph); 
	buildGraphFromInput("C-F-4\n", &graph); 
	buildGraphFromInput("C-G-2\n", &graph);
	buildGraphFromInput("C-H-3\n", &graph); 
	buildGraphFromInput("D-A-12\n", &graph); 
	buildGraphFromInput("D-E-45\n", &graph); 
	buildGraphFromInput("D-F-8\n", &graph); 
	buildGraphFromInput("E-D-45\n", &graph); 
	buildGraphFromInput("E-G-7\n", &graph); 
	buildGraphFromInput("F-B-16\n", &graph); 
	buildGraphFromInput("F-C-4\n", &graph); 
	buildGraphFromInput("F-D-8\n", &graph); 
	buildGraphFromInput("G-C-2\n", &graph); 
	buildGraphFromInput("G-E-7\n", &graph); 
	buildGraphFromInput("H-B-21\n", &graph); 
	buildGraphFromInput("H-C-3\n", &graph); 
	buildGraphFromInput("H-J-1\n", &graph); 
	buildGraphFromInput("I-B-8\n", &graph); 
	buildGraphFromInput("I-J-2\n", &graph); 
	buildGraphFromInput("J-H-1\n", &graph); 
	buildGraphFromInput("J-I-2\n", &graph); 
	printGraph(graph);

	//traverse graph with Dijkstra's Algorithm
	//start node is first node that was inserted (index 0)
	dijkstraAlgorithm(&graph, &graph.nodes[0]);
	printBacktrackingPaths(&graph);

	return EXIT_SUCCESS;
}