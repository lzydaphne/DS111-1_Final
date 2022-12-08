#include "my_Algo.h"

using namespace std;

class my_data
{
    // friend class my_DS; // not causing error ?
public:
    int get_station_num();
    Graph *read_map();
    my_station **read_station();
    void read_fee();
    int read_user();
    // for fee
    int discount[3]; // electric, lady, road
    int regular[3];
    int waiting;
    float switching; // bug: if switching is "int", eof will always be 0 (false), need to be float
    int transferring;

private:
    // for map
    string start_station;
    int num_start_station; //車站的數字表示法
    string end_station;
    int num_end_station; //車站的數字表示法
    int distance;        // required time between stations
    // for station
    int station_id;
    int station_num = 0;
    string bike_type;
    int num_elec;
    int num_lady;
    int num_road;
    // for user
    int user_num = 0;
    int start_time;
    int end_time;
};