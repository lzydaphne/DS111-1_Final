#include "./nthu_bike.h"
#include "./my_data.h"
#include <fstream>
#include <cmath> //round
using namespace std;

void basic(string selectedCase)
{
    cout << "start your basic version of data structure final from here!" << endl;
    int basic_revenue = 0;

    my_data read_data;
    read_data.select = selectedCase;
    int user_num = read_data.read_user_num();
    cout << "user_num: " << user_num << endl;
    int station_num = read_data.get_station_num();
    cout << "station_num: " << station_num << endl;
    read_data.read_bike_info();

    Graph basic_graph(station_num);
    basic_graph = *(read_data.read_map()); // recieve  object's pointer of graph
    bike_MaxHeap **basic_stations = read_data.read_bike();
    // cout << basic_stations[4][1].extractMax().id << endl;

    read_data.read_user();
    read_data.sort_users(); // sort user by start_time
    //! ----------------------start--basic----------------------------
    ofstream ofs_user, ofs_log, ofs_status;
    ofstream check_ofs_user, check_ofs_log, check_ofs_status;

    ofs_user.open("user_result.txt", ios::out);
    ofs_log.open("transfer_log.txt", ios::out);
    ofs_status.open("station_status.txt", ios::out);

    check_ofs_user.open("check_user_result.txt", ios::out);
    // check_ofs_log.open("check_transfer_log.txt", ios::out);
    // check_ofs_status.open("check_station_status.txt", ios::out);
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
    //* 儲存單一station中，單一車種的符合資格的備胎bike
    // why? 因為不僅僅是rental price的大小，其實有時候必須要騎price比較小的，因為只有它完全符合條件，但之前的實作，會無法記憶這種已經被迭代掉的bike，因此這次用heap來儲存
    // 等等，既然你都符合標準了，那變數應該只剩rental price!?

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

    // int countZ = 0;
    // int countA = 0;
    // int countB = 0;
    // int countC = 0;

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

        // cout << "tAC_bike_type: " << tAC_bike_type[0] << endl;

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

        //* 當可以騎多個車型，每種車型都各自建立一個heap來拿到最好的選擇
        cout << "-----Single station--------------------------" << endl;
        for (int i = 0; i < tlen_AC; i++)
        {
            BMNode target; //! 符合條件的bike_node
            BMNode tmp_target;
            cout << "-----diff type--------------------------" << endl;
            cout << "tAC_bike_type[i] " << tAC_bike_type[i] << endl;
            find = 0;
            //! 抓出node
            target = basic_stations[tuser_start_station][tAC_bike_type[i]].extractMax();

            //* 開始計算最短路徑
            // 已經有紀錄了
            if (!read_data.shortest_record[tuser_start_station])
            {
                // 回傳single source 的dist array
                read_data.shortest_record[tuser_start_station] = basic_graph.dijkstra(tuser_start_station);
                int idx = 0;

                /*while (read_data.shortest_record[tuser_start_station][idx])
                {
                    cout << "dij: " << read_data.shortest_record[tuser_start_station][idx] << endl;
                    idx++;
                }*/
            }
            shortest_path = read_data.shortest_record[tuser_start_station][tuser_end_station];
            cout << "  shortest_path " << shortest_path << endl;

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
                }
                //* 這邊抓出來的bike要放回去
                // todo 最後要優化，把這些條件放在一起檢查
                else if ((target.rental_count >= read_data.rental_limit) || (tstart_time + shortest_path >= tend_time) || (target.returned_time > tstart_time))
                {
                    cout << "not okay" << endl;
                    bike_case = 1;
                }

                else
                {
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
                        //! 不同種車種的情況下，把備胎放回去
                        basic_stations[tuser_start_station][stoi(tmp.bike_type)].insertKey(tmp);
                        cout << "store_types_bike: " << tmp.id << endl;
                        tmp = store_types_bike[i];
                        // cout << "tmp.bike_id: --2 " << tmp.id << endl;
                    }
                    else if ((store_types_bike[i].id != tmp.id) && (store_types_bike[i].rental_price <= tmp.rental_price))
                    {
                        cout << "store_types_bike inserted-2: " << store_types_bike[i].id << endl;
                        basic_stations[tuser_start_station][stoi(store_types_bike[i].bike_type)].insertKey(store_types_bike[i]);
                    }
                }
            }
        }
        cout << "check store_types_bike--2: " << tmp.id << endl;
        BMNode target;
        target = tmp;

        if ((target.rental_price > 0) && (target.returned_time <= 1440)) // 有找到目標車車
        {
            cout << "find!-------------------------------" << endl;
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

            // cout << "basic_stations[tuser_end_station][stoi(target.bike_type)]  heapsize" << basic_stations[tuser_end_station][stoi(target.bike_type)].heap_size << endl;
            cout << basic_stations[tuser_end_station][stoi(target.bike_type)].harr[0].id << endl;

            basic_stations[tuser_end_station][stoi(target.bike_type)].insertKey(target);
            cout << "real target inserted " << target.id << endl;

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
            cout << "not find ---------------------" << endl;
            /*
            LNode log_store;
            log_store.user_ID = user_id;
            log_store.bike_ID = 0;
            log_store.returned_time = 0;
            log_store.start_time = 0;
            log_store.user_start_station = 0;
            log_store.user_end_station = 0;
            log_output[log_idx++] = log_store;*/

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
        //
        ss.str("");
        ss.clear();
    }
    delete[] store_BMNode;
    delete[] store_types_bike;

    // todo 這是為了檢查方便，務必改回依據user request順序的答案形式
    //*otuput sorted user_request

    read_data.mergeSort(check_user_output, 0, check_user_idx - 1);
    cUNode user_store;
    string uStr;
    for (int p = 0; p < check_user_idx; p++)
    {
        user_store = check_user_output[p];
        ss << user_store.user_ID;
        string uStr = "U" + ss.str();
        ss.str("");
        ss.clear();

        check_ofs_user << uStr << " " << user_store.AC << " "
                       << user_store.bike_ID << " "
                       << user_store.bike_start_time << " " << user_store.bike_end_time << " " << user_store.revenue << endl;
        ss.str("");
        ss.clear();
    }
    delete[] check_user_output;

    // output transfer log
    // 把log的bike用userID進行排序小到大
    read_data.mergeSort(log_output, 0, log_idx - 1);
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
    delete[] log_output;
    // output final bike inventory

    int Barr_idx = 0;
    int station_heap_size = 0;
    // 計算低一station的各種車型的車輛個數

    for (int i = 0; i < read_data.station_num; i++)
    {
        ss << i;
        string station_id = "S" + ss.str();
        // cout << "here!" << endl;
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
            BMNode *ptr;
            ptr = basic_stations[i][k].harr;
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
        ss.str("");
        ss.clear();
    }
    // test
    cout << "basic_revenue: " << basic_revenue << endl;

    // cout << "countZ: " << countZ << endl;
    // cout << "countA: " << countA << endl;
    // cout << "countB: " << countB << endl;
    // cout << "countC: " << countC << endl;

    // todo delete all new operation!

    for (int i = 0; i < station_num; i++)
    {
        delete[] read_data.shortest_record[i];
        for (int j = 0; j < read_data.count_bike_type; j++)
            delete[] basic_stations[i][j].harr;
    }
    for (int i = 0; i < station_num; i++)
        delete[] basic_stations[i];
    delete[] basic_graph.bike_graph_List;
    delete[] read_data.all_user_list;

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