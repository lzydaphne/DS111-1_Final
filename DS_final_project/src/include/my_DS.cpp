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

int Graph::dijkstra(Graph &Dgraph, int src, int dest)
{
    // Get the number of vertices in graph
    int V = Dgraph.n;

    // dist values used to pick minimum weight edge in cut
    int *dist = new int[V];

    // minHeap represents set E
    graph_MinHeap minHeap(station_max_num);

    // Initialize minheap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap.array[v] = minHeap.newMinHeapNode(v, dist[v]);
        minHeap.pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    minHeap.array[src] = minHeap.newMinHeapNode(src, dist[src]);
    minHeap.pos[src] = src;
    dist[src] = 0;
    minHeap.decreaseKey(src, dist[src]);

    // Initially size of min heap is equal to V
    minHeap.size = V;

    // In the following loop,min heap contains all nodes
    // whose shortest distance is not yet finalized.
    while (!minHeap.isEmpty())
    {
        // Extract the vertex with
        // minimum distance value
        MNode *minHeapNode = minHeap.extractMin();

        // Store the extracted vertex number
        int u = minHeapNode->v;

        // Traverse through all adjacent vertices of u (the extracted vertex) and update their distance values
        struct AdjListNode *pCrawl = Dgraph.bike_graph_List[u].head;
        // struct AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;

            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (minHeap.isInMinHeap(v) &&
                dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also
                minHeap.decreaseKey(v, dist[v]);
            }
            pCrawl = pCrawl->next;
        }
    }
    return dist[dest];

    // print the calculated shortest distances
    // printArr(dist, V);
}

//! -------------------graph_MinHeap-------------------
/*
// A utility function to create a Min Heap
struct MinHeap *createMinHeap(int capacity)
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
*/
MNode *graph_MinHeap::newMinHeapNode(int v,
                                     int dist)
{
    MNode *minHeapNode =
        (MNode *)
            malloc(sizeof(MNode));
    minHeapNode->v = v;
    minHeapNode->dist = dist;
    return minHeapNode;
}

// A utility function to swap two
// nodes of min heap.
// Needed for min heapify
void graph_MinHeap::swapMinHeapNode(MNode **a,
                                    MNode **b)
{
    struct MinHeapNode *t = *a;
    *a = *b;
    *b = t;
}

void graph_MinHeap::minHeapify(int idx)
{
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < this->size &&
        this->array[left]->dist <
            this->array[smallest]->dist)
        smallest = left;

    if (right < this->size &&
        this->array[right]->dist <
            this->array[smallest]->dist)
        smallest = right;

    if (smallest != idx)
    {
        // The nodes to be swapped in min heap
        MNode *smallestNode =
            this->array[smallest];
        MNode *idxNode =
            this->array[idx];

        // Swap positions
        this->pos[smallestNode->v] = idx;
        this->pos[idxNode->v] = smallest;

        // Swap nodes
        swapMinHeapNode(&this->array[smallest],
                        &this->array[idx]);
        this->minHeapify(smallest);
    }
}

int graph_MinHeap::isEmpty()
{
    return this->size == 0;
}

void graph_MinHeap::decreaseKey(int v, int dist)
{
    // Get the index of v in heap array
    int i = this->pos[v];

    // Get the node and update its dist value
    this->array[i]->dist = dist;

    // Travel up while the complete
    // tree is not heapified.
    // This is a O(Logn) loop
    while (i && this->array[i]->dist <
                    this->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        this->pos[this->array[i]->v] =
            (i - 1) / 2;
        this->pos[this->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&this->array[i],
                        &this->array[(i - 1) / 2]);

        // move to parent index
        i = (i - 1) / 2;
    }
}

MNode *graph_MinHeap::extractMin()
{
    if (this->isEmpty())
        return NULL;

    // Store the root node
    MNode *root = this->array[0];

    // Replace root node with last node
    MNode *lastNode = this->array[this->size - 1];
    this->array[0] = lastNode;

    // Update position of last node
    this->pos[root->v] = this->size - 1;
    this->pos[lastNode->v] = 0;

    // Reduce heap size and heapify root
    --this->size;
    this->minHeapify(0);

    return root;
}

// A utility function to check if a given vertex
// 'v' is in min heap or not
bool graph_MinHeap::isInMinHeap(int v)
{
    if (this->pos[v] < this->size)
        return true;
    return false;
}
/*
// A utility function used to print the solution
void graph_MinHeap::printArr(int dist[], int n)
{
    printf("Vertex Distance from Source\n");
    for (int i = 0; i < n; ++i)
        printf("%d \t\t %d\n", i, dist[i]);
}*/

//! -------------------bike_MinHeap-------------------

void bike_MinHeap::insertKey(BMNode &newNode)
{
    if (this->heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }

    // First insert the new key at the end
    this->heap_size++;
    int i = this->heap_size - 1;
    harr[i].bike_type = newNode.bike_type;
    harr[i].id = newNode.id;
    harr[i].rental_count = newNode.rental_count;
    harr[i].rental_price = newNode.rental_price;

    // Fix the min heap property if it is violated
    while (i != 0 && harr[parent(i)].rental_price > harr[i].rental_price)
    {
        swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }

    // harr[100] = heap_size;
};

void bike_MinHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    if (l < heap_size && harr[l].rental_price < harr[i].rental_price)
        smallest = l;
    if (r < heap_size && harr[r].rental_price < harr[smallest].rental_price)
        smallest = r;
    if (smallest != i)
    {
        swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }

    // harr[100] = heap_size;
}

// Method to remove minimum element (or root) from min heap
int bike_MinHeap::extractMin()
{
    if (heap_size <= 0)
        return 10000;
    if (heap_size == 1)
    {
        // cout << "about to empty !!!!!!" << endl;
        heap_size--;
        // harr[100] = heap_size; // don't forget to update heap size
        return harr[0].rental_price;
    }

    // Store the minimum value, and remove it from heap
    BMNode root = harr[0];
    harr[0] = harr[heap_size - 1];
    heap_size--;
    MinHeapify(0);

    // harr[100] = heap_size;
    return root.rental_price;
}

bool bike_MinHeap::isEmpty()
{
    // cout << "heap size: " << heap_size << endl;
    if (heap_size == 0)
        return true;
    else
        return false;
}

void bike_MinHeap::printHeapSort(ofstream &ofs_)
{ // use reference to write the same output file in main
    // cout << "heap size: " << heap_size << endl;
    int min;
    while (heap_size != 0)
    {
        min = extractMin();
        cout << min << " ";
        ofs_ << min << " ";
    }
    cout << endl;
    ofs_ << endl;
}