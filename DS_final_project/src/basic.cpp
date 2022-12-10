#include "./nthu_bike.h"
#include "./include/my_data.h"
#include <fstream>
#include <cmath> //round

void basic(string selectedCase)
{
    cout << "start your basic version of data structure final from here!" << endl;

    int basic_revenue = 0;

    my_data read_data;
    int user_num = read_data.read_user_num();
    int station_num = read_data.get_station_num();
    read_data.read_bike_info();

    Graph basic_graph;
    basic_graph = *(read_data.read_map()); // recieve  object's pointer of graph
    bike_MaxHeap **basic_stations = read_data.read_bike();

    read_data.read_user();
    //! ----------------------start--basic----------------------------
    ofstream ofs_user, ofs_log;
    ofs_user.open("user_result.txt", ios::out);
    ofs_log.open("transfer_log.txt", ios::out);
    int idx = 0;

    // temp variable
    int tuser_ID;
    int *tAC_bike_type;
    int tlen_AC;
    int tstart_time;
    int tend_time;
    int tuser_start_station;
    int tuser_end_station;
    int tarrive_time;

    //! 避免重複計算所以用2d array紀錄最短路徑
    read_data.shortest_record = new int *[station_num];
    // 初始化為NULL
    for (int i = 0; i < station_num; i++)
        read_data.shortest_record[i] = NULL;

    while (idx < read_data.all_user_list_idx)
    {
        // read data
        tuser_ID = read_data.all_user_list[idx].user_ID;
        tAC_bike_type = read_data.all_user_list[idx].AC_bike_type;
        tlen_AC = read_data.all_user_list[idx].len_AC;
        tstart_time = read_data.all_user_list[idx].start_time;
        tend_time = read_data.all_user_list[idx].end_time;
        tuser_start_station = read_data.all_user_list[idx].user_start_station;
        tuser_end_station = read_data.all_user_list[idx].end_time;
        tarrive_time = read_data.all_user_list[idx].arrive_time;
        idx++;

        BMNode target;
        // 不同的AC BIKE TYPE
        for (int i = 0; i < tlen_AC; i++)
        {
            target = basic_stations[tuser_start_station][i].extractMax();
            if (target.rental_price > 0 && target.returned_time <= tstart_time && target.rental_count < read_data.rental_limit)
            {
                // 確認上述條件都滿足才計算最短路徑
                // 已經有紀錄了
                int shortest_path = -1;
                if (!read_data.shortest_record[tuser_start_station][tuser_end_station])
                {
                    read_data.shortest_record[tuser_start_station] = basic_graph.dijkstra(tuser_start_station, tuser_end_station);
                }
                shortest_path = read_data.shortest_record[tuser_start_station][tuser_end_station];
            }
        }
    }

    /*
string path = "./testcases/" + selectedCase + "/bike_info.txt";
string line;
ifstream myfile(path);
if (myfile.is_open())
{

    while (getline(myfile, line))
    {
        cout << line << endl;
    }
    myfile.close();
}
else
    cout << "not open";*/
}