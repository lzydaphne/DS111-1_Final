#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <climits>

using namespace std;

// A structure to represent a
// node in adjacency list
typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next;
} node;

// A structure to represent
// an adjacency list
typedef struct AdjList
{

    // Pointer to head node of list
    struct AdjListNode *head;
} adjList;

class my_station //!
{
public:
    int *electric = new int[101]; // will be the pointer point to its min heap, 101th record the heap size
    int *lady = new int[101];
    int *road = new int[101];
};

// Structure to represent a min heap node
struct MinHeapNode
{
    int v;
    int dist;
};

// Structure to represent a min heap
struct MinHeap
{

    // Number of heap nodes present currently
    int size;

    // Capacity of min heap
    int capacity;

    // This is needed for decreaseKey()
    int *pos;
    struct MinHeapNode **array;
};

// A class for Min Heap
class my_MinHeap
{
    int *harr;          // pointer to array of elements in heap
    int capacity = 100; // maximum possible size of min heap
    int heap_size = 0;  // Current number of elements in min heap
public:
    // Constructor
    my_MinHeap(int *bike_type, int my_heap_size)
    {
        harr = bike_type;
        heap_size = my_heap_size;
    }

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return (2 * i + 1); }  // to get index of left child of node at index i
    int right(int i) { return (2 * i + 2); } // to get index of right child of node at index i

    int getMin() { return harr[0]; } // Returns the minimum key (key at root) from min heap

    void swap(int *x, int *y)
    {
        int temp = *x;
        *x = *y;
        *y = temp;
    }

    //!
    struct MinHeap *createMinHeap(int capacity);
    struct MinHeapNode *newMinHeapNode(int v,
                                       int dist);
    void swapMinHeapNode(struct MinHeapNode **a,
                         struct MinHeapNode **b);
    void minHeapify(struct MinHeap *minHeap,
                    int idx);
    void decreaseKey(struct MinHeap *minHeap,
                     int v, int dist);
    // Inserts a new key 'k'
    void insertKey(int k);

    // to heapify a subtree with the root at given index
    void MinHeapify(int i);

    // to extract(remove + return) the root which is the minimum element
    struct MinHeapNode *my_MinHeap::extractMin(struct MinHeap *
                                                   minHeap);
    bool isInMinHeap(struct MinHeap *minHeap, int v);
    int isEmpty(struct MinHeap *minHeap);
    void printArr(int dist[], int n);
    void printHeapSort(ofstream &ofs);

    void dijkstra(Graph &Dgraph, int src);
};

class Graph
{
    // list<node> *adjList;

    int n;
    adjList *bike_graph_List;
    // while (!(bike_graph_List[i])->head)
public:
    Graph()
    {
        n = 0;
    }

    Graph(int nodeCount)
    {
        n = nodeCount;
        // bikeList = new list<node>[n];
        // 指向list head of list
        bike_graph_List = (adjList *)
            malloc(n * sizeof(adjList));
    }

    node *newAdjListNode(int dest, int weight);

    void addEdge(int source, int dest, int weight);

    void displayEdges();
    // {
    //     for (int i = 0; i < n; i++)
    //     {
    //         list<node> tempList = adjList[i];
    //         showList(i, tempList);
    //     }
    // }

    // friend void dijkstra(Graph &Dgraph, int src);
    friend class my_MinHeap;
};
