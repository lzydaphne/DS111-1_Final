#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <climits>
#define station_max_num 1000 // 暫時設定的，不然要把整個map都跑過一次才知道 = =
#define bike_max_num 10000
#define user_max_num 100000
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

class Graph
{
    // list<node> *adjList;
    int n;
    adjList *bike_graph_List;
    int *dist_graph;
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
    void dijkstra(int src, int dest);

    void displayEdges();
    // {
    //     for (int i = 0; i < n; i++)
    //     {
    //         list<node> tempList = adjList[i];
    //         showList(i, tempList);
    //     }
    // }

    // friend void dijkstra(Graph &Dgraph, int src);
    // friend class my_MinHeap;
};

//! -------------------------graph_MinHeap-----------------------------
// Structure to represent a min heap node
typedef struct MinHeapNode
{
    int v;
    int dist;
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
    friend void Graph::dijkstra(int src, int dest);
};

//! --------------------------bike_MinHeap------------------
//! A class for Min Heap，for "bike_type"
typedef struct BMaxHeapNode
{
    string bike_type;
    int id;
    float rental_price;
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

    //! 不太需要constructor
    // my_MinHeap(MNode *bike_type, int my_heap_size)

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
    //!

    //* Inserts a new node "把整個bike_node都丟到heap中，方便比較rental_price和id"
    void insertKey(BMNode &newNode);

    // to heapify a subtree with the root at given index
    void MaxHeapify(int i);
    BMNode extractMax(); // 回傳最大rental_price的BMNod

    bool isEmpty();
    void printHeapSort(ofstream &ofs);
};

//! -----------my_station-----------------
class my_station //! 每個station可能有多少種type的車?
// 只能從bike_info中得知
{
public:
    bike_MaxHeap *MHeap_ptr;
};

//
//! -------------------user-------------------------------------
