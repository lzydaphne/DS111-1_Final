#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <climits>
#include <stdlib.h>
#define station_max_num 1000
#define bike_max_num 10000
#define user_max_num 100000
using namespace std;

// A structure to represent a node in adjacency list
typedef struct AdjListNode
{
    int dest;
    int weight;
    struct AdjListNode *next = NULL;
} node;

// A structure to represent an adjacency list
typedef struct AdjList
{
    // Pointer to head node of list
    struct AdjListNode *head = NULL;
} adjList;

class Graph
{
    int n;

public:
    adjList *bike_graph_List;
    int *dist_graph;
    Graph()
    {
        n = 0;
    }

    Graph(int nodeCount)
    {
        n = nodeCount;
        bike_graph_List = new adjList[n];
    }

    node *newAdjListNode(int dest, int weight);

    void addEdge(int source, int dest, int weight);
    int *&dijkstra(int src);

    // friend void dijkstra(Graph &Dgraph, int src);
    // friend class my_MinHeap;
};

//! -------------------------graph_MinHeap-----------------------------
// Structure to represent a min heap node
typedef struct MinHeapNode
{
    int v;    // 站名
    int dist; // 距離
} MNode;

//! Structure to represent a min heap, for "graph"
class graph_MinHeap
{
public:
    // Number of heap nodes present currently
    int size = 0;

    // Capacity of min heap
    int capacity;

    // This is needed for decreaseKey()
    int *pos;
    MNode **array;

    graph_MinHeap(int cap)
    {
        capacity = cap;
        pos = new int[capacity];
        array = new MNode *[capacity];
    };
    // MHeap *createMinHeap(int capacity);
    MNode *newMinHeapNode(int v,
                          int dist);

    void swapMinHeapNode(MNode **a,
                         MNode **b);
    // to heapify a subtree with the root at given index
    void minHeapify(int idx);
    void decreaseKey(int v, int dist);

    // to extract(remove + return) the root which is the minimum element
    MNode *extractMin();
    bool isInMinHeap(int v);
    int isEmpty();
    // void printArr(int dist[], int n);
    // void printHeapSort(ofstream &ofs);
    friend int *&Graph::dijkstra(int src);
};

//! --------------------------bike_MinHeap------------------
//! A class for Min Heap，for "bike_type"
typedef struct BMaxHeapNode
{
    string bike_type;
    int id = -1;
    float rental_price = -1;
    int rental_count;
    //*
    int returned_time = 0;
} BMNode;

class bike_MaxHeap
{
public:
    int capacity = bike_max_num;             // maximum possible size of min heap
    int heap_size = 0;                       // Current number of elements in min heap
    BMNode *harr = new BMNode[bike_max_num]; // pointer to array of elements in heap
    // int *pos = new int[capacity];            // 紀錄每個node的index

    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return (2 * i + 1); }  // to get index of left child of node at index i
    int right(int i) { return (2 * i + 2); } // to get index of right child of node at index i
    BMNode getMax() { return harr[0]; }      // Returns the minimum key (key at root) from min heap

    void BMNode_swap(BMNode *x, BMNode *y)
    {
        BMNode temp = *x;
        *x = *y;
        *y = temp;
    }

    //* Inserts a new node "把整個bike_node都丟到heap中，方便比較rental_price和id"
    void insertKey(BMNode &newNode);
    void MINinsertKey(BMNode &newNode);
    // to heapify a subtree with the root at given index
    void MaxHeapify(int i);
    BMNode extractMax(); // 回傳最大rental_price的BMNod
    // void MinHeapify(int i);
    // BMNode extractMin();    //  algo
    bool isEmpty();
    // void printHeapSort(ofstream &ofs);
};

//! -------------------user-------------------------------------

BMNode MinNode(BMNode &node1, BMNode &node2);
BMNode findMinimumElement(bike_MaxHeap &heap, int n);
