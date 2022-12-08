#include "my_DS.h"

using namespace std;

//! ------------------- Graph -------------------

node *Graph::newAdjListNode(int dest, int weight)
{
    node *newNode = (node *)malloc(sizeof(node));
    newNode->dest = dest;
    newNode->weight = weight;
    newNode->next = NULL;
    return newNode;
}

void Graph::addEdge(int source, int dest, int weight)
{
    node *newNode = newAdjListNode(dest, weight);

    newNode->next = bike_graph_List[source].head;
    bike_graph_List[source].head = newNode;

    // Since graph is undirected,
    // add an edge from dest to src also
    newNode = newAdjListNode(source, weight);
    newNode->next = bike_graph_List[dest].head;
    bike_graph_List[dest].head = newNode;
}

void Graph::displayEdges()
{
    // struct AdjListNode *ptr = MyGraph->array->head;
    node *ptr = bike_graph_List->head;

    for (int i = 0; i < n; i++)
    {
        while (!(bike_graph_List[i]).head)
        // while (!MyGraph->array[i].head)
        {
            cout << ptr->weight << " ";
            ptr = ptr->next;
        }
    }
}
// ! -----------------------------------------------------

void my_MinHeap::dijkstra(Graph &Dgraph, int src)
{

    // Get the number of vertices in graph
    int V = Dgraph.n;

    // dist values used to pick
    // minimum weight edge in cut
    int dist[V];

    // minHeap represents set E
    struct MinHeap *minHeap = my_MinHeap::createMinHeap(V);

    // Initialize min heap with all
    // vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap->array[v] = newMinHeapNode(v, dist[v]);
        minHeap->pos[v] = v;
    }

    // Make dist value of src vertex
    // as 0 so that it is extracted first
    minHeap->array[src] = my_MinHeap::newMinHeapNode(src, dist[src]);
    minHeap->pos[src] = src;
    dist[src] = 0;
    decreaseKey(minHeap, src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap->size = V;

    // In the following loop,
    // min heap contains all nodes
    // whose shortest distance
    // is not yet finalized.
    while (!isEmpty(minHeap))
    {
        // Extract the vertex with
        // minimum distance value
        struct MinHeapNode *minHeapNode =
            extractMin(minHeap);

        // Store the extracted vertex number
        int u = minHeapNode->v;

        // Traverse through all adjacentvertices of u (the extracted vertex) and update
        // their distance values
        // struct AdjListNode *pCrawl = graph->array[u].head;
        struct AdjListNode *pCrawl = Dgraph.bike_graph_List[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is
            // not finalized yet, and distance to v
            // through u is less than its
            // previously calculated distance
            if (isInMinHeap(minHeap, v) &&
                dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance
                // value in min heap also
                decreaseKey(minHeap, v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }

    // print the calculated shortest distances
    printArr(dist, V);
}

//! ------------------- Min Heap -------------------

struct MinHeap *my_MinHeap::createMinHeap(int capacity)
{
    struct MinHeap *minHeap =
        (struct MinHeap *)
            malloc(sizeof(struct MinHeap));
    minHeap->pos = (int *)malloc(
        capacity * sizeof(int));
    minHeap->size = 0;
    minHeap->capacity = capacity;
    minHeap->array =
        (struct MinHeapNode **)
            malloc(capacity *
                   sizeof(struct MinHeapNode *));
    return minHeap;
}

struct MinHeapNode *my_MinHeap::newMinHeapNode(int v,
                                               int dist)
{
    struct MinHeapNode *minHeapNode =
        (struct MinHeapNode *)
            malloc(sizeof(struct MinHeapNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void my_MinHeap::swapMinHeapNode(struct MinHeapNode **a,
                                 struct MinHeapNode **b)
{
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}
void my_MinHeap::minHeapify(struct MinHeap *minHeap,
                            int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < minHeap->size &&
        minHeap->array[left]->dist <
            minHeap->array[smallest]->dist)
        smallest = left;

    if (right < minHeap->size &&
        minHeap->array[right]->dist <
            minHeap->array[smallest]->dist)
        smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MinHeapNode *smallestNode =
            minHeap->array[smallest];
        MinHeapNode *idxNode =
            minHeap->array[idx];

        // Swap positions
        minHeap->pos[smallestNode->v] = idx;
        minHeap->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&minHeap->array[smallest],
                        &minHeap->array[idx]);

        minHeapify(minHeap, smallest);
    }
}

int my_MinHeap::isEmpty(struct MinHeap *minHeap)
{
    return minHeap->size == 0;
}

void my_MinHeap::decreaseKey(struct MinHeap *minHeap,
                             int v, int dist)
{
    // Get the index of v in heap array
    int i = minHeap->pos[v];

    // Get the node and update its dist value
    minHeap->array[i]->dist = dist;

    // Travel up while the complete
    // tree is not heapified.
    // This is a O(Logn) loop
    while (i && minHeap->array[i]->dist <
                    minHeap->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        minHeap->pos[minHeap->array[i]->v] =
            (i - 1) / 2;
        minHeap->pos[minHeap->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&minHeap->array[i],
                        &minHeap->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

struct MinHeapNode *my_MinHeap::extractMin(struct MinHeap *
                                               minHeap)
{
    if (isEmpty(minHeap))
        return NULL;

    // Store the root node
    struct MinHeapNode *root =
        minHeap->array[0];

    // Replace root node with last node
    struct MinHeapNode *lastNode =
        minHeap->array[minHeap->size - 1];
    minHeap->array[0] = lastNode;

    // Update position of last node
    minHeap->pos[root->v] = minHeap->size - 1;
    minHeap->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --minHeap->size;
    minHeapify(minHeap, 0);

    return root;
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool my_MinHeap::isInMinHeap(struct MinHeap *minHeap, int v)
{
    if (minHeap->pos[v] < minHeap->size)
        return true;
    return false;
}

// A utility function used to print the solution
void my_MinHeap::printArr(int dist[], int n)
{
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}