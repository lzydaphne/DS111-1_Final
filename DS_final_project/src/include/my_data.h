#include "my_Algo.h"

using namespace std;

class my_data
{
    // friend class my_DS; // not causing error ?
public:
    int get_station_num();
    Graph *read_map();
    bike_MinHeap **read_bike();
    void read_bike_info();
    int read_user();

    // for fee
    int discount[3]; // electric, lady, road
    int regular[3];
    int waiting;
    float switching; // bug: if switching is "int", eof will always be 0 (false), need to be float
    int transferring;

private:
    //* for map
    string start_station;
    int num_start_station; // 車站的數字表示法
    string end_station;
    int num_end_station; // 車站的數字表示法
    int distance;        // required time between stations
    //* for station
    int station_num = 0; // 總站數
    //* for user
    int user_num = 0;
    int start_time;
    int end_time;
    //* for bike info
    float depreciation;
    int rental_limit;
    int count_bike_type = 0;
    //* for bike
    string bike_type;
    int bike_id;
    string station_id;
    int rental_price;
    int rental_count;
};