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
    ofstream ofs_user, ofs_log, ofs_status;
    ofs_user.open("user_result.txt", ios::out);
    ofs_log.open("transfer_log.txt", ios::out);
    ofs_status.open("station_status.txt", ios::out);
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
    // 每個user可以賺的錢
    int single_revenue;

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
        int shortest_path;
        int find = 1; // 有找到符合目標的bike
                      // 不同的AC BIKE TYPE

        for (int i = 0; i < tlen_AC; i++)
        {
            shortest_path = -1;
            target = basic_stations[tuser_start_station][i].extractMax();

            if (target.rental_price < 0 || target.rental_count < read_data.rental_limit)
            {
                find = 0;
                continue;
            }
            // cout << "this station doesn't have bike_type" << endl;

            if (target.returned_time > tstart_time)
            {
                // todo 待優化，這邊先以最保守的方式來拿記憶體空間
                // 目的是，為了要暫時儲存不符合time資格的max node
                int tmp_idx = 0;
                BMNode *store_BMNode = new BMNode[bike_max_num];

                while (find && target.returned_time > tstart_time)
                {
                    if (target.rental_price < 0)
                    { // 抽到沒車了
                        find = 0;
                        break;
                    }
                    store_BMNode[tmp_idx++] = target;
                    target = basic_stations[tuser_start_station][i].extractMax();
                }

                // 再一一insert回去
                for (int j = 0; j < tmp_idx; j++)
                    basic_stations[tuser_start_station][i].insertKey(store_BMNode[j]);

                tmp_idx = 0; // 歸零
                if (!find)
                    continue;
            }

            // 確認上述條件都滿足才計算最短路徑
            // 已經有紀錄了
            if (!read_data.shortest_record[tuser_start_station][tuser_end_station])
            {
                read_data.shortest_record[tuser_start_station] = basic_graph.dijkstra(tuser_start_station, tuser_end_station);
            }
            shortest_path = read_data.shortest_record[tuser_start_station][tuser_end_station];

            if (tstart_time + shortest_path > tend_time)
            {
                find = 0;
                continue;
            }

            //* 比較，不同bike_type之間最高的rental_price

            BMNode compare;
            if (tlen_AC > 1 && i == 0)
            {
                compare.bike_type = target.bike_type;
                compare.id = target.id;
                compare.rental_count = target.rental_count;
                compare.rental_price = target.rental_price;
                compare.returned_time = target.returned_time;
            }
            else if (i != 0 && target.rental_price > compare.rental_price)
            {
                compare.bike_type = target.bike_type;
                compare.id = target.id;
                compare.rental_count = target.rental_count;
                compare.rental_price = target.rental_price;
                compare.returned_time = target.returned_time;
                BMNode *ptr = &target;
                *ptr = compare; // 把target指向compare
            }
        }

        //! start to output
        stringstream ss;
        ss << tuser_ID;
        string user_id = "U" + ss.str();
        if (find) // 有找到目標車車
        {

            // 計算revenue
            single_revenue = floor(shortest_path * target.rental_price);
            basic_revenue += single_revenue;
            target.rental_count++;
            target.rental_price -= read_data.depreciation;
            target.returned_time = tstart_time + shortest_path;

            // output to user_result.txt
            ofs_user
                << user_id << " " << 1 << " " << target.id << " " << tstart_time << " " << target.returned_time << " " << single_revenue << endl;
            ofs_log << target.id << " " << tuser_start_station << " " << tuser_end_station << " " << tstart_time << " " << target.returned_time << " " << user_id << endl;
        }
        else
        {
            ofs_user
                << user_id << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
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