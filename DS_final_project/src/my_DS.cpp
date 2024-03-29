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

int *&Graph::dijkstra(int src)
{
    // Get the number of vertices in graph
    int V = n;

    // dist values used to pick minimum weight edge in cut
    int *dist = new int[V];

    // minHeap represents set E
    graph_MinHeap minHeap(station_max_num);

    // Initially size of min heap is equal to V
    minHeap.size = V;
    // Initialize minheap with all vertices. dist value of all vertices
    for (int v = 0; v < V; ++v)
    {
        dist[v] = INT_MAX;
        minHeap.array[v] = minHeap.newMinHeapNode(v, dist[v]);
        minHeap.pos[v] = v;
    }

    // Make dist value of src vertex as 0 so that it is extracted first
    dist[src] = 0;
    minHeap.decreaseKey(src, dist[src]);

    // In the following loop,min heap contains all nodes whose shortest distance is not yet finalized.
    while (!minHeap.isEmpty())
    {
        MNode *minHeapNode = minHeap.extractMin();
        /*
        cout << "minHeapNode:  "
             << "station: " << minHeapNode->v << "  "
             << "dist: " << minHeapNode->dist << "  " << endl;*/

        // Store the extracted vertex number
        int u = minHeapNode->v;

        // Traverse through all adjacent vertices of u (the extracted vertex) and update their distance values
        node *pCrawl = bike_graph_List[u].head;

        // struct AdjListNode *pCrawl = graph->array[u].head;
        while (pCrawl != NULL)
        {
            int v = pCrawl->dest;
            // cout << "v:  " << v << endl;
            // If shortest distance to v is not finalized yet, and distance to v
            // through u is less than its previously calculated distance
            if (minHeap.isInMinHeap(v) &&
                dist[u] != INT_MAX &&
                pCrawl->weight + dist[u] < dist[v])
            {
                dist[v] = dist[u] + pCrawl->weight;

                // update distance value in min heap also
                minHeap.decreaseKey(v, dist[v]);
                /*
                cout << "v:  " << v << "  dist[v]: " << dist[v] << endl;*/
            }
            pCrawl = pCrawl->next;
        }
    }
    // todo: warning: reference to local variable dist' returned
    dist_graph = dist;

    // cout << "src: " << src << endl;
    // for (int i = 0; i < V; i++)
    // {
    //     cout << "i: " << i << " dist: " << dist_graph[i] << endl;
    // }
    return dist_graph;
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
void graph_MinHeap::swapMinHeapNode(MNode **a, MNode **b)
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

    // Travel up while the complete tree is not heapified.
    // This is a O(Logn) loop
    while (i && this->array[i]->dist < this->array[(i - 1) / 2]->dist)
    {
        // Swap this node with its parent
        this->pos[this->array[i]->v] = (i - 1) / 2;
        this->pos[this->array[(i - 1) / 2]->v] = i;
        swapMinHeapNode(&this->array[i], &this->array[(i - 1) / 2]);

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

//! -------------------bike_MaxHeap-------------------

BMNode MinNode(BMNode &node1, BMNode &node2)
{
    if (node1.rental_price <= node2.rental_price)
        return node1;
    else
        return node2;
}

BMNode findMinimumElement(bike_MaxHeap &heap, int n)
{
    BMNode minimumElement = heap.harr[n / 2];
    int hIdx = n / 2;

    for (int i = 1 + n / 2; i < n; ++i)
    {
        minimumElement = MinNode(minimumElement, heap.harr[i]);
        if (minimumElement.id == heap.harr[i].id)
            hIdx = i;
    }

    // 這邊手動把min拉出來
    cout << "heap.harr[hIdx]: " << heap.harr[hIdx].id << endl;

    heap.harr[hIdx] = heap.harr[heap.heap_size - 1];
    heap.heap_size--;
    // cout << "root: " << root.rental_price << endl;
    // cout << "harr[0]: " << harr[0].rental_price << endl;
    // heap.MaxHeapify(hIdx);
    heap.MaxHeapify(0);

    return minimumElement;
}

//----------------------------------------
void bike_MaxHeap::insertKey(BMNode &newNode)
{
    if (this->heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }

    // First insert the new key at the end
    this->heap_size++;
    // cout << "heap_size: " << this->heap_size << endl;
    int i = this->heap_size - 1;
    harr[i].bike_type = newNode.bike_type;
    harr[i].id = newNode.id;
    harr[i].rental_count = newNode.rental_count;
    harr[i].rental_price = newNode.rental_price;
    harr[i].returned_time = newNode.returned_time;
    // MaxHeapify(i);

    // Fix the max heap property if it is violated

    while (i != 0 && harr[parent(i)].rental_price < harr[i].rental_price)
    {
        BMNode_swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
    // MaxHeapify(0);

    // harr[100] = heap_size;
};

void bike_MaxHeap::MINinsertKey(BMNode &newNode)
{
    if (this->heap_size == capacity)
    {
        cout << "\nOverflow: Could not insertKey\n";
        return;
    }

    // First insert the new key at the end
    this->heap_size++;
    // cout << "heap_size: " << this->heap_size << endl;
    int i = this->heap_size - 1;
    harr[i].bike_type = newNode.bike_type;
    harr[i].id = newNode.id;
    harr[i].rental_count = newNode.rental_count;
    harr[i].rental_price = newNode.rental_price;
    harr[i].returned_time = newNode.returned_time;
    // MaxHeapify(i);

    // Fix the max heap property if it is violated

    while (i != 0 && harr[parent(i)].rental_price > harr[i].rental_price)
    {
        BMNode_swap(&harr[i], &harr[parent(i)]);
        i = parent(i);
    }
};
void bike_MaxHeap::MaxHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int largest = i;
    // cout << " l " << l << " r " << r << "largest " << largest << endl;
    // cout << " harr[l].rental_price: " << harr[l].rental_price << endl;
    // cout << " harr[r].rental_price: " << harr[r].rental_price << endl;
    if (l < heap_size && harr[l].rental_price > harr[i].rental_price)
        largest = l;
    if (r < heap_size && harr[r].rental_price > harr[largest].rental_price)
        largest = r;

    // cout << "largest " << largest << endl;

    // 兩邊有相同的rental_price
    // if (harr[l].rental_price != 0 && (harr[l].rental_price == harr[r].rental_price))
    if (l < heap_size && r < heap_size && harr[l].rental_price != 0 && (harr[l].rental_price == harr[r].rental_price) && harr[l].rental_price > harr[i].rental_price)
    {
        if (harr[l].id < harr[r].id)
            largest = l;
        else
            largest = r;
    }

    if (largest != i)
    {
        BMNode_swap(&harr[i], &harr[largest]);
        MaxHeapify(largest);
    }

    // harr[100] = heap_size;
}

/*
void bike_MaxHeap::MinHeapify(int i)
{
    int l = left(i);
    int r = right(i);
    int smallest = i;
    // cout << " l " << l << " r " << r << "largest " << largest << endl;
    // cout << " harr[l].rental_price: " << harr[l].rental_price << endl;
    // cout << " harr[r].rental_price: " << harr[r].rental_price << endl;
    if (l < heap_size && harr[l].rental_price < harr[i].rental_price)
        smallest = l;
    if (r < heap_size && harr[r].rental_price < harr[smallest].rental_price)
        smallest = r;

    // cout << "largest " << largest << endl;

    // 兩邊有相同的rental_price
    // if (harr[l].rental_price != 0 && (harr[l].rental_price == harr[r].rental_price))
    if (l < heap_size && r < heap_size && harr[l].rental_price != 0 && (harr[l].rental_price == harr[r].rental_price) && harr[l].rental_price < harr[i].rental_price)
    {
        if (harr[l].id < harr[r].id)
            smallest = l;
        else
            smallest = r;
    }

    if (smallest != i)
    {
        BMNode_swap(&harr[i], &harr[smallest]);
        MinHeapify(smallest);
    }
}*/
// Method to remove minimum element (or root) from min heap
BMNode bike_MaxHeap::extractMax()
{

    // 如果沒有想要的車:
    if (heap_size <= 0)
    {
        BMNode tmp;
        tmp.id = -10;
        tmp.rental_price = -10;
        tmp.returned_time = 1050; // 刻意製造不符合的時間，因為最多1040
        return tmp;
    }
    // else if (heap_size == 1)
    // {
    // cout << "about to empty !!!!!!" << endl;
    // heap_size--;
    // harr[100] = heap_size; // don't forget to update heap size
    // return harr[0];
    // }

    // Store the max value, and remove it from heap
    BMNode root = harr[0];
    /*
        // 檢查returned_time < start_time
        if (root.returned_time < )
如果該root滿足 returned_time < start_time*/
    harr[0] = harr[heap_size - 1];
    heap_size--;
    // cout << "root: " << root.rental_price << endl;
    // cout << "harr[0]: " << harr[0].rental_price << endl;
    MaxHeapify(0);

    // harr[100] = heap_size;
    return root;
}
/*BMNode bike_MaxHeap::extractMin()
{

    // 如果沒有想要的車:
    if (heap_size <= 0)
    {
        BMNode tmp;
        tmp.id = -10;
        tmp.rental_price = -10;
        tmp.returned_time = 1050; // 刻意製造不符合的時間，因為最多1040
        return tmp;
    }
    // else if (heap_size == 1)
    // {
    // cout << "about to empty !!!!!!" << endl;
    // heap_size--;
    // harr[100] = heap_size; // don't forget to update heap size
    // return harr[0];
    // }

    // Store the max value, and remove it from heap
    BMNode root = harr[0];
    harr[0] = harr[heap_size - 1];
    heap_size--;
    // cout << "root: " << root.rental_price << endl;
    // cout << "harr[0]: " << harr[0].rental_price << endl;
    MinHeapify(0);

    // harr[100] = heap_size;
    return root;
}
*/
bool bike_MaxHeap::isEmpty()
{
    // cout << "heap size: " << heap_size << endl;
    if (heap_size == 0)
        return true;
    else
        return false;
}

//! -------------------user-------------------
