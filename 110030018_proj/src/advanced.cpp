#include "./nthu_bike.h"
#include "./my_data.h"
#include <fstream>
#include <cmath> //round
using namespace std;

void advanced(string selectedCase)
{
    // cout << "change" << endl;
    cout << "start your advanced version of data structure final from here!" << endl;
    //
    int basic_revenue = 0;

    my_data read_data;
    cout << "selectedCase " << selectedCase << endl;
    read_data.select = selectedCase;
    int user_num = read_data.read_user_num();
    cout << "user_num: " << user_num << endl;
    int station_num = read_data.get_station_num();
    cout << "station_num: " << station_num << endl;
    read_data.read_bike_info();

    Graph basic_graph(station_num);
    basic_graph = *(read_data.read_map()); // recieve  object's pointer of graph
    bike_MaxHeap **basic_stations = read_data.read_bike();

    //! decide FBT magic number
    int FBT_magic_number;
    if ((0 <= read_data.bike_total_num) && (read_data.bike_total_num <= 30))
        FBT_magic_number = 1;
    else if ((30 < read_data.bike_total_num) && (read_data.bike_total_num <= 60))
        FBT_magic_number = 2;
    else if ((60 < read_data.bike_total_num) && (read_data.bike_total_num <= 90))
        FBT_magic_number = 3;
    else if ((90 < read_data.bike_total_num) && (read_data.bike_total_num <= 1500))
        FBT_magic_number = 5;
    else if ((1500 < read_data.bike_total_num) && (read_data.bike_total_num <= 3000))
        FBT_magic_number = 9;
    else if ((3000 < read_data.bike_total_num) && (read_data.bike_total_num <= 5000))
        FBT_magic_number = 7;
    else if ((5000 < read_data.bike_total_num) && (read_data.bike_total_num <= 7000))
        FBT_magic_number = 8;
    else if ((7000 < read_data.bike_total_num) && (read_data.bike_total_num <= 9000))
        FBT_magic_number = 9;
    else if ((9000 < read_data.bike_total_num) && (read_data.bike_total_num <= 10000))
        FBT_magic_number = 10;

    read_data.read_user();
    read_data.sort_users();
    //! ----------------------start--basic----------------------------
    ofstream ofs_user, ofs_log, ofs_status;
    // ofstream check_ofs_user, check_ofs_log, check_ofs_status;

    // ofs_user.open("user_result.txt", ios::out);
    ofs_user.open("./result/" + selectedCase + "/user_result.txt", ios::out);
    // ofs_log.open("transfer_log.txt", ios::out);
    ofs_log.open("./result/" + selectedCase + "/transfer_log.txt", ios::out);
    // ofs_status.open("station_status.txt", ios::out);
    ofs_status.open("./result/" + selectedCase + "/station_status.txt", ios::out);

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
    int single_revenue = 0;
    int shortest_path;

    //! 避免重複計算所以用array紀錄最短路徑
    read_data.shortest_record = new int *[station_num];
    // 初始化為NULL
    for (int i = 0; i < station_num; i++)
        read_data.shortest_record[i] = NULL;

    // 用來放單一station中，每個bike_type的可行代表(target)
    //  BMNode *store_bikes = new BMNode[read_data.all_user_list[idx].len_AC];

    //* 儲存單一station單一車種的不符合資格的備胎bike
    BMNode *store_BMNode = new BMNode[bike_max_num];

    //------------------------------------------------------

    BMNode *store_types_bike = new BMNode[read_data.count_bike_type];

    stringstream ss;

    cout << "user total num: " << read_data.user_num << endl;

    // 為了check user_result的 LNode
    cUNode *check_user_output = new cUNode[read_data.user_num];
    int check_user_idx = 0;

    // 為了儲存transfer log的 LNode
    LNode *log_output = new LNode[read_data.user_num];
    int log_idx = 0;

    //* wait_list
    bike_MaxHeap wait_list;

    int *pick_station = new int[read_data.station_num];

    while (idx < read_data.all_user_list_idx)
    {
        // read data
        tuser_ID = read_data.all_user_list[idx].user_ID;
        tAC_bike_type = read_data.all_user_list[idx].AC_bike_type;
        tlen_AC = read_data.all_user_list[idx].len_AC;
        tstart_time = read_data.all_user_list[idx].start_time;
        tend_time = read_data.all_user_list[idx].end_time;
        tuser_start_station = read_data.all_user_list[idx].user_start_station;
        tuser_end_station = read_data.all_user_list[idx].user_end_station;
        // tarrive_time = read_data.all_user_list[idx].arrive_time;
        idx++;

        cout << "--------------new user----------------------" << endl;

        cout << "tuser_ID: " << tuser_ID << "\n"
             << "tstart_time: " << tstart_time << "\n"
             << "tend_time: " << tend_time << "\n"
             << "tuser_start_station: " << tuser_start_station << "\n"
             << "tuser_end_station: " << tuser_end_station
             << endl;

        // BMNode compare;
        single_revenue = 0;
        int find; // 有找到符合目標的bike

        shortest_path = -1;
        // 不同的AC BIKE TYPE

        int tmp_idx = 0;
        int tmp2_idx = 0;

        for (int i = 0; i < read_data.count_bike_type; i++)
        {
            store_types_bike[i].id = -1;
            store_types_bike[i].rental_price = -1;
        }

        //* 開始計算最短路徑
        // 已經有紀錄了
        if (!read_data.shortest_record[tuser_start_station])
        {
            // 回傳single source 的dist array
            read_data.shortest_record[tuser_start_station] = basic_graph.dijkstra(tuser_start_station);

            int idx = 0;
            /*
            while (read_data.shortest_record[tuser_start_station][idx])
            {
                cout << "dij: " << read_data.shortest_record[tuser_start_station][idx] << endl;
                idx++;
            }*/
        }

        //! Free bike transfer
        //! 選出最近的三個站點(station)
        int nearest_stations[3];
        // int *pick_station = new int[read_data.station_num];

        // deep copy
        for (int i = 0; i < read_data.station_num; i++)
            pick_station[i] = read_data.shortest_record[tuser_start_station][i];

        //* pick first three nearest station

        pick_station[tuser_start_station] = INT_MAX;
        int first = INT_MAX,
            second = INT_MAX, third = INT_MAX;
        int first_idx = -1, second_idx = -1, third_idx = -1;
        for (int q = 0; q < read_data.station_num; q++)
        {
            if (pick_station[q] < first)
            {
                third_idx = second_idx;
                second_idx = first_idx;
                first_idx = q;
                third = second;
                second = first;
                first = pick_station[q];
            }
            else if (pick_station[q] < second && pick_station[q] != first)
            {
                third_idx = second_idx;
                second_idx = q;
                third = second;
                second = pick_station[q];
            }
            else if (pick_station[q] < third && pick_station[q] != second && third_idx != second_idx)
            {
                third_idx = q;
                third = pick_station[q];
            }
        }

        nearest_stations[0] = first_idx;
        nearest_stations[1] = second_idx;
        nearest_stations[2] = third_idx;
        cout << first_idx << " " << second_idx << " " << third_idx << endl;

        //-------------------------------
        int max_bike_type[3] = {0}; // 用來儲存「每個station中，有最大heap size的車種type」
        int max_type_size[3] = {0}; // 用來儲存「每個station中，有最大heap size的車種type的size」
        for (int i = 0; i < 3; i++)
        {
            int max = 0;
            int max_station = -1; // 紀錄是哪一個bike_type
            for (int j = 0; j < read_data.count_bike_type; j++)
            {
                if (basic_stations[nearest_stations[i]][j].heap_size > max)
                {
                    max = basic_stations[nearest_stations[i]][j].heap_size;
                    max_station = j;
                    // 紀錄是哪一個bike_type
                }
            }
            if (max == 0) // 這個station完全沒有足夠的bike了
            {
                cout << " this station has not enough bike! " << endl;
                // todo 應該要把下一個鄰近的拿進來繼續抓剩餘的車，但是先不用
                // 如果三個都沒車?????
                // todo 應該說，有車再implement，沒車就算了
            }

            max_bike_type[i] = max_station; // 如果是-1，代表沒車
            max_type_size[i] = max;
            cout << "  max_bike_type[i]: " << max_bike_type[i] << endl;
            cout << "  max_type_size[i]: " << max_type_size[i] << endl;
            // 開始放入 user start station
            // if (max_type_size[i] > FBT_magic_number) // 多於一台車再FBT
            if (max_type_size[i] > FBT_magic_number) // 多於FBT_magic_number車再FBT
            {
                // // check min
                // for (int q = 0; q < basic_stations[nearest_stations[i]][max_bike_type[i]].heap_size; q++)
                // {
                //     cout << " id: " << basic_stations[nearest_stations[i]][max_bike_type[i]].harr[q].id << " price: " << basic_stations[nearest_stations[i]][max_bike_type[i]].harr[q].rental_price << endl;
                // }
                // todo 可以看看extractMax/extractMin的效果

                BMNode tmp = basic_stations[nearest_stations[i]][max_bike_type[i]].extractMax();
                // BMNode tmp = findMinimumElement(basic_stations[nearest_stations[i]][max_bike_type[i]], basic_stations[nearest_stations[i]][max_bike_type[i]].heap_size);
                // //! reach rental limit
                if (tmp.rental_count >= read_data.rental_limit)
                {
                    basic_stations[nearest_stations[i]][max_bike_type[i]].insertKey(tmp);
                    continue;
                }

                cout
                    << "transfered id: " << tmp.id << endl;
                cout << "nearest_stations[i] : " << nearest_stations[i] << endl;
                // 求出nearest_stations[i]站點的距離
                if (!read_data.shortest_record[nearest_stations[i]])
                {
                    // 回傳single source 的dist array
                    read_data.shortest_record[nearest_stations[i]] = basic_graph.dijkstra(nearest_stations[i]);
                }
                int transfer_path = read_data.shortest_record[nearest_stations[i]][tuser_start_station];
                cout << "path: " << transfer_path << endl;
                //* 把bike的returned time加上轉運時間
                // 所以下面抓target的時候，FBT的bike已經會在start station，而returned time已經加上轉運時間
                // 剩下user的start time要處理
                //! 這些FBT的BIKE，要修改的只有RETURN TIME，且會被插入到START STATION
                int transfer_start_time = tmp.returned_time;
                tmp.returned_time += transfer_path;
                cout << "transfered_bike-id: " << tmp.id << "  transfered_bike returned time:" << tmp.returned_time << endl;
                /* todo 要做出extractmin
                //  if (transfer_start_time == tmp.returned_time)
                //  {
                //      cout << "dont transfer to the same station" << endl;
                //      // continue;
                //  }*/

                basic_stations[tuser_start_station][max_bike_type[i]].insertKey(tmp);
                //-output to transfer log------------
                ss << nearest_stations[i];
                string start_station = "S" + ss.str();
                ss.str("");
                ss.clear();
                ss << tuser_start_station;
                string end_station = "S" + ss.str();
                ss.str("");
                ss.clear();
                ofs_log
                    << tmp.id << " " << start_station << " " << end_station << " " << transfer_start_time << " " << tmp.returned_time << " " << -1 << endl;
                // }
            }
        }

        //-------------------------------

        //* 當可以騎多個車型，每種車型都各自建立一個heap來拿到最好的選擇
        cout
            << "-----Single station--------------------------" << endl;
        for (int i = 0; i < tlen_AC; i++)
        {
            BMNode target; //! 符合條件的bike_node
            BMNode tmp_target;
            cout << "-----diff type--------------------------" << endl;
            cout << "tAC_bike_type[i] " << tAC_bike_type[i] << endl;
            find = 0;
            //! 抓出node
            target = basic_stations[tuser_start_station][tAC_bike_type[i]].extractMax();

            shortest_path = read_data.shortest_record[tuser_start_station][tuser_end_station];
            cout << "  shortest_path " << shortest_path << endl;
            cout << "  tuser_start_station " << tuser_start_station << endl;
            cout << "  tuser_end_station " << tuser_end_station << endl;
            int idx = 0;

            //*開始在同一站中針對特定bike type抓取bike
            int bike_case = 0;
            tmp_idx = 0;

            while (!find) // 代表節點位置為空
            {
                cout << "targetID " << target.id << endl;
                cout << "target price " << target.rental_price << endl;
                //* heap已經為空，price=-10
                if (target.rental_price < -1)
                {
                    bike_case = -1;
                    cout << " no bike  " << endl;
                    // continue;
                }
                //* 這邊抓出來的bike要放回去
                // todo 最後要優化，把這些條件放在一起檢查
                else if (target.rental_count >= read_data.rental_limit)
                {
                    bike_case = 1;
                    cout
                        << "target.rental_count >= read_data.rental_limit" << endl;
                }
                else if (tstart_time + shortest_path >= tend_time)
                {
                    bike_case = 1;
                    cout << "(tstart_time + shortest_path > tend_time)" << endl;
                }
                // else if (target.returned_time > tstart_time)
                else if (target.returned_time + shortest_path >= tend_time)
                {
                    bike_case = 1;
                    cout
                        << "target.returned_time + shortest_path > tend_time" << endl;
                }
                else
                {
                    if (target.returned_time > tstart_time)
                    {
                        //! user wait for bike when no bike is available
                        // 目前是等rental price最多的那一台
                        cout << "user wait for bike" << endl;
                        //* WAIT
                        {
                            BMNode tmp;
                            // tstart_time = target.returned_time;
                            if (!wait_list.isEmpty())
                            {
                                tmp = wait_list.extractMax();
                                //! 改變標準: 轉運車越早到越好
                                if (target.returned_time < tmp.returned_time)
                                {
                                    cout << "new waited bike target-id: " << target.id << endl;
                                    // 把tmp放回去，不等tmp
                                    basic_stations[tuser_start_station][stoi(tmp.bike_type)].insertKey(tmp);
                                    // 等target
                                    wait_list.insertKey(target);
                                }
                                else
                                {
                                    cout << "original waited bike tmp-id: " << tmp.id << endl;
                                    // basic_stations[tuser_start_station][stoi(target.bike_type)].insertKey(target);
                                    store_BMNode[tmp_idx++] = target;
                                    wait_list.insertKey(tmp);
                                }
                            }
                            else
                                wait_list.insertKey(target);

                            cout << "~~~~~put into wait-list~~~~~" << endl;
                            target = basic_stations[tuser_start_station][tAC_bike_type[i]].extractMax();
                            continue;
                        }
                    }

                    cout << "nice!! " << endl;
                    bike_case = 0;
                }
                //-------------------------------
                if (bike_case > 0)
                {
                    cout << "bike_case >0: " << target.id << endl;

                    store_BMNode[tmp_idx++] = target;
                    target = basic_stations[tuser_start_station][tAC_bike_type[i]].extractMax();
                    continue;
                }
                else if (bike_case == 0)
                {
                    if ((tmp_target.rental_price == -1) || (target.rental_price > tmp_target.rental_price) || ((target.rental_price == tmp_target.rental_price) && (target.id < tmp_target.id)))
                    {
                        //! 同一種車種的情況下，把備胎放回去，因為發現更好的
                        if ((tmp_target.rental_price != -1))
                        {
                            cout << "bike_case 0: " << tmp_target.id << endl;
                            cout << tmp_target.rental_price << endl;

                            basic_stations[tuser_start_station][stoi(tmp_target.bike_type)].insertKey(tmp_target);
                        }
                        tmp_target.bike_type = target.bike_type;
                        tmp_target.id = target.id;
                        tmp_target.rental_count = target.rental_count;
                        tmp_target.rental_price = target.rental_price;
                        tmp_target.returned_time = target.returned_time;
                        cout << "tmp_target.id-bike0 " << tmp_target.id << endl;
                    }
                    else
                    {
                        cout << "bike_case 0-2 : " << target.id << endl;
                        // 符合基本條件，卻沒有比tmp好，就放回去
                        store_BMNode[tmp_idx++] = target;
                    }

                    // pick_best_BMNode->insertKey(target);
                    target = basic_stations[tuser_start_station][tAC_bike_type[i]].extractMax();
                    cout << "--------------------------" << endl;
                    continue;
                }
                else
                {
                    find = 1;
                    target = tmp_target; // 就算是空的也會return-10
                    if (target.id >= 0)
                    {
                        store_types_bike[tmp2_idx++] = target;

                        // store_stations->insertKey(target);

                        cout
                            << "~~~~successful extract max~~~~~~~~" << endl;
                        cout << "target.id " << target.id << endl;
                        cout << "target.bike_type " << target.bike_type << endl;
                        cout << "target.rental_count " << target.rental_count << endl;
                        cout << "target.rental_price " << target.rental_price << endl;
                        cout << "target.returned_time " << target.returned_time << endl;
                        cout
                            << "~~~~end extract max~~~~~~~~" << endl;
                        /* code */
                    }

                    // cout<< "store_bikes[i].id " << store_bikes[i].id << endl;
                    for (int j = 0; j < tmp_idx; j++)
                    {

                        cout << "store_BMNode[j]:" << store_BMNode[j].id << endl;
                        if (store_BMNode[j].id < 0)
                            continue;

                        basic_stations[tuser_start_station][tAC_bike_type[i]].insertKey(store_BMNode[j]);
                    }
                }
            }
            tmp_idx = 0;
            // tmp2_idx = 0;
        }
        tmp2_idx = 0;

        //! start to output

        ss << tuser_ID;
        string user_id = "U" + ss.str();

        BMNode tmp = store_types_bike[0];

        cout << "check store_types_bike " << tmp.id << endl;
        if (tmp.id >= 0)
        {
            for (int i = 0; i < tlen_AC; i++)
            {
                if (store_types_bike[i].id >= 0)
                {
                    if ((store_types_bike[i].rental_price > tmp.rental_price) || ((store_types_bike[i].rental_price == tmp.rental_price) && (store_types_bike[i].id < tmp.id)))
                    {
                        //! 不同一種車種的情況下，把備胎放回去
                        basic_stations[tuser_start_station][stoi(tmp.bike_type)].insertKey(tmp);
                        cout << "store_types_bike: " << tmp.id << endl;
                        tmp = store_types_bike[i];
                    }
                    else if ((store_types_bike[i].id != tmp.id) && (store_types_bike[i].rental_price <= tmp.rental_price))
                    {
                        cout << "store_types_bike inserted-2: " << store_types_bike[i].id << endl;
                        basic_stations[tuser_start_station][stoi(store_types_bike[i].bike_type)].insertKey(store_types_bike[i]);
                    }
                }
            }
        }
        cout << "final bike choice: " << tmp.id << endl;
        BMNode target;
        target = tmp;
    FIND:
        if ((target.rental_price > 0) && (target.returned_time <= 1440)) // 有找到目標車車
        {
            cout << "find!-------------------------------" << endl;

            //! FBT---when wait not use--------
            while (!wait_list.isEmpty())
            {
                BMNode tmp = wait_list.extractMax();
                cout << "put back wait node id: " << tmp.id << endl;
                basic_stations[tuser_start_station][stoi(tmp.bike_type)].insertKey(tmp);
            }
            //--------------
            // 計算revenue
            single_revenue = floor(shortest_path * target.rental_price);
            basic_revenue += single_revenue;
            target.rental_count++;
            target.rental_price -= read_data.depreciation;
            target.returned_time = tstart_time + shortest_path;

            //* 把拿出來的bike放到新的end_station中
            cout << "target.id " << target.id << endl;
            cout << "target.bike_type " << target.bike_type << endl;
            cout << "target.rental_count " << target.rental_count << endl;
            cout << "target.rental_price " << target.rental_price << endl;
            cout << "target.returned_time " << target.returned_time << endl;
            cout << "tuser_end_station " << tuser_end_station << endl;
            cout << "stoi(target.bike_type) " << stoi(target.bike_type) << endl;

            basic_stations[tuser_end_station][stoi(target.bike_type)].insertKey(target);
            cout << "real target insert in end station" << target.id << endl;

            // output to user_result.txt
            ofs_user
                << user_id << " " << 1 << " " << target.id << " " << tstart_time << " " << target.returned_time << " " << single_revenue << endl;

            cUNode user_sort;
            user_sort.user_ID = tuser_ID; // num
            user_sort.AC = 1;
            user_sort.bike_ID = target.id;
            user_sort.bike_start_time = tstart_time;
            user_sort.bike_end_time = target.returned_time;
            user_sort.revenue = single_revenue;
            check_user_output[check_user_idx++] = user_sort;

            LNode log_store;
            log_store.bike_ID = target.id;
            log_store.user_ID = stoi(user_id.erase(0, 1));
            log_store.returned_time = target.returned_time;
            log_store.start_time = tstart_time;
            log_store.user_start_station = tuser_start_station;
            log_store.user_end_station = tuser_end_station;
            log_output[log_idx++] = log_store;
        }
        else
        {
            cout << "--------------First not find -----------------" << endl;

            //! implement wait
            // todo 如果沒車可以等? 一定每次都要等嗎? 何時要等何時不用等?
            // 目前的做法: 找不到就等
            BMNode waited_bike;
            waited_bike = wait_list.extractMax();
            // int bike_start_time = 0;
            if (waited_bike.id != -10) // 不是空的
            {
                cout << "*****waited bike*****"
                     << "id: " << waited_bike.id << endl;
                tstart_time = waited_bike.returned_time;
                // waited_bike.returned_time = tstart_time + shortest_path;
                target = waited_bike;

                goto FIND;
            }

            cout << "--------------Truly not find -----------------" << endl;

            cUNode user_sort;
            user_sort.user_ID = tuser_ID; // num
            user_sort.AC = 0;
            user_sort.bike_ID = 0;
            user_sort.bike_start_time = 0;
            user_sort.bike_end_time = 0;
            user_sort.revenue = 0;
            check_user_output[check_user_idx++] = user_sort;
            // todo 記得把正確的形式改回來

            ofs_user
                << user_id << " " << 0 << " " << 0 << " " << 0 << " " << 0 << " " << 0 << endl;
        }
        while (!wait_list.isEmpty()) // 清空
            wait_list.extractMax();

        ss.str("");
        ss.clear();
    }
    delete[] pick_station;
    delete[] store_BMNode;
    delete[] store_types_bike;

    LNode log_store;
    // string uStr;
    for (int p = 0; p < log_idx; p++)
    {
        log_store = log_output[p];
        ss << log_store.user_ID;
        string uStr = "U" + ss.str();
        ss.str("");
        ss.clear();
        ss << log_store.user_start_station;
        string start_station = "S" + ss.str();
        ss.str("");
        ss.clear();
        ss << log_store.user_end_station;
        string end_station = "S" + ss.str();
        ss.str("");
        ss.clear();

        ofs_log
            << log_store.bike_ID << " " << start_station << " " << end_station << " " << log_store.start_time << " " << log_store.returned_time << " " << uStr << endl;
        ss.str("");
        ss.clear();
    }
    // ofs_log.flush();
    // ofs_log.close();
    delete[] log_output;
    // output final bike inventory

    int Barr_idx = 0;
    int station_heap_size = 0;
    // 計算低一station的各種車型的車輛個數

    for (int i = 0; i < read_data.station_num; i++)
    {
        ss << i;
        string station_id = "S" + ss.str();
        cout << "here!" << endl;
        station_heap_size = 0;
        for (int j = 0; j < read_data.count_bike_type; j++)
        {
            cout << "heap.size " << basic_stations[i][j].heap_size << endl;
            station_heap_size += basic_stations[i][j].heap_size;
        }
        cout << "station_heap_size: " << station_heap_size << endl;

        BMNode *Barr = new BMNode[station_heap_size];
        //! 把單一station的bike都蒐集起來放在Barr
        for (int k = 0; k < read_data.count_bike_type; k++)
        {
            // Pointer arithmetic is done in units of the size of the pointer type.
            // cout << "here!--2" << endl;
            BMNode *ptr;
            ptr = basic_stations[i][k].harr;
            // cout << "ptr: " << ptr << endl;
            // cout << "size:" << sizeof(BMNode) << endl;
            for (int m = 0; m < basic_stations[i][k].heap_size; m++)
            {
                // cout << "ptr->id: :" << ptr->id << endl;
                Barr[Barr_idx++] = *(ptr);
                ptr++;
            }
        }
        // 把單一station的bike用ID進行排序小到大
        read_data.mergeSort(Barr, 0, station_heap_size - 1);
        // cout << "here!--3" << endl;

        string bikeB;
        for (int q = 0; q < station_heap_size; q++)
        {
            bikeB = "B" + Barr[q].bike_type;
            cout
                << station_id << " " << Barr[q].id << " " << bikeB << " " << Barr[q].rental_price << " " << Barr[q].rental_count << endl;

            ofs_status << station_id << " " << Barr[q].id << " " << bikeB << " " << Barr[q].rental_price << " " << Barr[q].rental_count << endl;
        }
        Barr_idx = 0;
        delete[] Barr;
        // ofs_status.flush();
        // ofs_status.close();
        ss.str("");
        ss.clear();
    }
    // test
    cout << "advanced_revenue: " << basic_revenue << endl;
    selectedCase.erase(0, 4);
    int tc = stoi(selectedCase);
    switch (tc)
    {
    case 1:
        cout << "Increased by: " << (basic_revenue - 47437.0) / 47437.0 * 100 << " %" << endl;
        break;
    case 2:
        cout << "Increased by " << (basic_revenue - 926832.0) / 926832.0 * 100 << " %" << endl;
        break;
    case 3:
        cout << "Increased by " << (basic_revenue - 26425651.0) / 26425651.0 * 100.0 << " %" << endl;
        break;
    default:
        break;
    }

    // todo delete all new operation!
    for (int i = 0; i < station_num; i++)
    {
        delete[] read_data.shortest_record[i];
        for (int j = 0; j < read_data.count_bike_type; j++)
            delete[] basic_stations[i][j].harr;
    }
    for (int i = 0; i < station_num; i++)
        delete[] basic_stations[i];
    delete[] basic_stations;
    // delete &basic_graph;
    delete[] basic_graph.bike_graph_List;
    delete[] read_data.all_user_list;
    delete[] read_data.arr_AC_bike_type;
}