#include "my_algo.h"

using namespace std;

typedef struct LogNode
{
    int user_ID; // 直接換成int
    int bike_ID;
    int user_start_station;
    int user_end_station;
    int start_time;
    int returned_time;

} LNode;

typedef struct checkUserNode
{
    int user_ID; // 直接換成int
    int AC;
    int bike_ID;
    int bike_start_time;
    int bike_end_time;
    int revenue;

} cUNode;

typedef struct UserNode
{
    int user_ID;
    int *AC_bike_type;
    int len_AC = 0;
    int start_time;
    int end_time;
    int user_start_station;
    int user_end_station;
    //*
    int arrive_time;

} UNode;

class my_data
{
    // friend class my_DS; // not causing error ?
public:
    int get_station_num();
    Graph *read_map();
    bike_MaxHeap **read_bike();
    void read_bike_info();
    int read_user_num();
    void read_user();
    void sort_users(); //* merge sort user by start_time
    // void sort_bikes();                             //* merge sort bike by ID
    void mergeSort(UNode *&arr, int l, int r);     //* merge sort user by start_time
    void mergeSort(BMNode *&arr, int l, int r);    //* merge sort bike by ID
    void mergeSort(LNode *&arr, int l, int r);     //* merge sort bikeLog by ID
    void mergeSort(cUNode *&arr, int l, int r);    //* merge sort user_result by ID
    void merge(UNode *&arr, int p, int q, int r);  //* merge sort user by start_time
    void merge(BMNode *&arr, int p, int q, int r); //* merge sort bike by ID
    void merge(LNode *&arr, int p, int q, int r);  //* merge sort bikeLog by ID
    void merge(cUNode *&arr, int p, int q, int r); //* merge sort user_result by ID

    //! for fee
    // int discount[3]; // electric, lady, road
    // int regular[3];

    int waiting;
    float switching; // bug: if switching is "int", eof will always be 0 (false), need to be float
    int transferring;

    // private:
    //*
    string select;
    //* for map
    string start_station;
    int num_start_station; // 車站的數字表示法
    string end_station;
    int num_end_station;   // 車站的數字表示法
    int distance;          // required time between stations
    int **shortest_record; //* 儲存計算過的最短路徑，避免重複計算
    //* for station
    int station_num = 0; // 總站數
    //* for user
    UNode *all_user_list;      // 把所有user都蒐集起來
    int all_user_list_idx = 0; // 可以直接視為size來用
    int user_num = 0;          // user總數
    string user_ID;
    int num_user_ID;
    string AC_bike_type; // 方便識別","相隔的bike_type
    int *arr_AC_bike_type;
    int arr_index = 0;
    int start_time;
    int end_time;
    string user_start_station;
    int num_user_start_station;
    string user_end_station;
    int num_user_end_station;
    //* for bike info
    float depreciation;
    int rental_limit;
    int count_bike_type = 0;
    //* for bike
    string bike_type;
    int bike_id;
    string station_id;
    float rental_price;
    int rental_count;
    //*for bikes
    // bike_MaxHeap **basic_stations;

    //-------------------------------
    float *demend_compare;
    // 1. 在void my_data::read_user()讀到總和
};