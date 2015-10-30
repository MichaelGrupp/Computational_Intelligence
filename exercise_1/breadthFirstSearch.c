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

//memory handling
void addNeighborCapacityIfFull(Node* node) {
	if (node->neighborCount >= node->neighborCapacity) {
		node->neighborCapacity += ADD_CAPACITY;
		Node** temp = (Node**)realloc(node->neighbors, node->neighborCapacity*sizeof(Node*));
		if (temp == NULL) {
			//fprintf(stderr, "Reallocation failed --> %s\n", strerror(errno));
			free(node->neighbors);
			exit(EXIT_FAILURE);
		}
		else {
			node->neighbors = temp;
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
void addNeighbor(Node* node, const Node* neighbor) {
	addNeighborCapacityIfFull(node);
	node->neighbors[node->neighborCount++] = neighbor;
	//adding a neighbor is a mutual operation --> add node also to neighbor
	if (searchNeighbor(neighbor, node->name) == NOT_FOUND)
		addNeighbor(neighbor, node);
}

void printNode(Node* node) {
	printf("%c", node->name);
	if (node->neighborCount >= 1) {
		printf("-");
		for (int i = 0; i < node->neighborCount; i++)
			printf("%c", node->neighbors[i]->name);
	}
	printf("\n");
}

//process the input from system, e.g. X-AB
void buildGraphFromInput(char* input, Graph* graph) {
	char name = input[0];
	if (name == '\n') return; //catch empty case
	//check if node already exists
	if (searchNode(graph, name) == NOT_FOUND) {
		addNode(graph, name);
	}
	int nodeIndex = searchNode(graph, name);
	//add neighbors after '-'
	if (input[1] == '\n') return; //catch one letter case
	name = input[2];
	for (int i = 2; name != '\n'; i++) {
		if (searchNode(graph, name) == NOT_FOUND) {
			addNode(graph, name);
		}
		int neighborIndex = searchNode(graph, name);
		//check if neighbor already exists
		if (searchNeighbor(&graph->nodes[nodeIndex], name) == NOT_FOUND)
			addNeighbor(&graph->nodes[nodeIndex], &graph->nodes[neighborIndex]);
		name = input[i + 1];
	}
}

void enqueue(char* queue, int *size, char name) {
	if (*size > 0) {
		for (int i = (*size) - 1; i >= 0; i--) {
			queue[i + 1] = queue[i]; //shifty shifty
			queue[i] = 0;
		}
	}
	queue[0] = name;
	(*size)++;
}

char dequeue(char* queue, int* size) {
	*size = *size-1;
	return queue[*size];
}

//only traverse graph with BFS, option for searching a certain node not implemented
void breadthFirstSearch(Graph* graph) {
	if (graph->nodeCount == 0) return; //catch empty case

	//this array has the same order as the "nodes" array in graph
	int* visited = (int*)malloc(graph->nodeCount*sizeof(int));
	
	//mark all the vertices as not visited
	for (int i = 0; i < graph->nodeCount; i++) {
		visited[i] = 0;
	}

	//create a "queue" for node names
	char* queue = (char*)malloc((graph->nodeCount * 2)*sizeof(char));
	int queueSize = 0, count = 1;

	//mark the start node as visited and enqueue it
	visited[0] = 1;
	enqueue(queue, &queueSize, graph->nodes[0].name);

	while (queueSize != 0) {
		//dequeue a node name
		char n = dequeue(queue, &queueSize);
		
		//print: desired output is e.g. XYZ
		printf("%c", n);

		int index = searchNode(graph, n);
		Node node = graph->nodes[index];

		// Get all adjacent vertices of the dequeued vertex s
		// If a adjacent has not been visited, then mark it visited
		// and enqueue it
		for (int i = 0; i < node.neighborCount; i++) {
			int neighborIndexInGraph = searchNode(graph, node.neighbors[i]->name);
			if (!visited[neighborIndexInGraph]) {
				visited[neighborIndexInGraph] = 1;
				enqueue(queue, &queueSize, node.neighbors[i]->name);
			}
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
	buildGraphFromInput("A-BD\n", &graph);
	buildGraphFromInput("B-AC\n", &graph);
	buildGraphFromInput("C-BG\n", &graph);
	buildGraphFromInput("D-AEF\n", &graph);
	buildGraphFromInput("E-DG\n", &graph);
	buildGraphFromInput("F-D\n", &graph);
	buildGraphFromInput("G-CE\n", &graph);

	//traverse graph with BFS
	breadthFirstSearch(&graph);
	printf("\n");

	return EXIT_SUCCESS;
}