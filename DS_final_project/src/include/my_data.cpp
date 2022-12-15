#include "my_data.h"

/*
 todo 目前計算站數，是直接掃過一次map，有沒有更快的方法?
*/
// 計算站數
int my_data::get_station_num()
{
    string path = "./testcases/" + select + "/map.txt";
    cout << "path: " << path;
    ifstream ifs(path, ios::in);
    if (!ifs.is_open())
    {
        cout << "  !!!!Failed to open station file.\n";
        return 0;
    }
    else
    {
        int tmp;
        string s1, s2;
        bool checkStation[station_max_num] = {false}; // every element == false
        while (ifs >> s1 >> s2 >> tmp)
        {
            num_start_station = stoi(s1.erase(0, 1)); //! remove first char;
            num_end_station = stoi(s2.erase(0, 1));

            if (!checkStation[num_start_station])
            {
                checkStation[num_start_station] = true;
                station_num++;
            }
            if (!checkStation[num_end_station])
            {
                checkStation[num_end_station] = true;
                station_num++;
            }
        }
    }
    ifs.close();
    return station_num;
}

// 建graph
Graph *my_data::read_map()
{
    // read station data
    // cout << "select: " << select << endl;
    string path = "./testcases/" + select + "/map.txt";
    ifstream ifs(path, ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open map file.\n";
        return 0;
    }
    else
    {
        Graph *graph_ptr = new Graph(station_num); // create an object pointer
        // graph_ptr->initial_graph(station_num);
        while (ifs >> start_station >> end_station >> distance)
        { // overloading >> operator?
            // add edges into graph
            num_start_station = stoi(start_station.erase(0, 1)); //! remove first char;
            num_end_station = stoi(end_station.erase(0, 1));     //! remove first char;

            graph_ptr->addEdge(num_start_station, num_end_station, distance); // station id start from 0

            cout << "start: " << num_start_station << " end: " << num_end_station << " distance: " << distance << "\n";
        }
        // graph_ptr->print_graph();
        // cout << "min distance: " << graph_ptr->dijkstra(0, 5) << endl;
        ifs.close();
        return graph_ptr; // return the object pointer
    }
}

int my_data::read_user_num()
{
    //	ios::in=>檔案open為輸入模式(istream default)
    // ifstream ifs("../../testcases/case1/user.txt", ios::in);
    string path = "./testcases/" + select + "/user.txt";
    ifstream ifs(path, ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open user file.\n";
        return 0;
    }
    else
    {
        string s;
        while (std::getline(ifs, s))
        {
            // cout << "test" << endl;
            user_num++;
            // cout << "user_num:" << user_num << endl;
        }
    }
    ifs.close();
    return user_num;
}

void my_data::read_user()
{
    string path = "./testcases/" + select + "/user.txt";
    ifstream ifs(path, ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open user file.\n";
        return;
    }
    else
    {
        all_user_list = new UNode[user_num];

        while (ifs >> user_ID >> AC_bike_type >> start_time >> end_time >> user_start_station >> user_end_station)
        {

            num_user_ID = stoi(user_ID.erase(0, 1));
            num_user_start_station = stoi(user_start_station.erase(0, 1));
            num_user_end_station = stoi(user_end_station.erase(0, 1));

            arr_AC_bike_type = new int[count_bike_type];
            stringstream ss;
            ss.str(AC_bike_type);
            while (ss.good())
            {
                string substr;
                getline(ss, substr, ',');
                arr_AC_bike_type[arr_index++] = stoi(substr.erase(0, 1));
                cout << "arr_ac " << arr_AC_bike_type[0] << endl;
            }
            // test cout
            cout << "num_user_ID: " << num_user_ID << "\n"
                 << "station_id: " << station_id << "\n"
                 << "start_time: " << start_time << "\n"
                 << "end_time: " << end_time << "\n"
                 << "num_user_start_station: " << num_user_start_station << "\n"
                 << "num_user_end_station: " << num_user_end_station
                 << endl;

            UNode newUNode;
            newUNode.user_ID = num_user_ID;
            newUNode.start_time = start_time;
            newUNode.end_time = end_time;
            newUNode.user_start_station = num_user_start_station;
            newUNode.user_end_station = num_user_end_station;
            // 確保進行deep copy
            int i = 0;
            newUNode.AC_bike_type = new int[count_bike_type](); // set to 0
            memset(newUNode.AC_bike_type, -1, count_bike_type);
            while (arr_AC_bike_type[i] != -1)
            {
                cout
                    << "arr_AC_bike_type: " << arr_AC_bike_type[i++] << endl;
                /* code */
            }

            memcpy(newUNode.AC_bike_type, arr_AC_bike_type, arr_index + 1);
            // cout << "test33" << endl;

            newUNode.len_AC = arr_index;
            arr_index = 0; // 歸零

            // 把UNode推到list中
            all_user_list[all_user_list_idx++] = newUNode;
            // cout << "test44" << endl;
            delete[] arr_AC_bike_type;
            cout << "-------------------------------" << endl;
        }
    }
    ifs.close();
    return;
}

void my_data::read_bike_info()
{
    //	ios::in=>檔案open為輸入模式(istream default)
    string path = "./testcases/" + select + "/bike_info.txt";

    ifstream ifs(path, ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open user file.\n";
        return;
    }
    else
    {
        ifs >> depreciation >> rental_limit;
        string s;
        while (std::getline(ifs, s))
        {
            count_bike_type++;
        }
        count_bike_type--;
    }
    cout << "count_bike_type: " << count_bike_type << endl;
    ifs.close();
    return;
}

void my_data::read_bike()
{
    string path = "./testcases/" + select + "/bike.txt";
    ifstream ifs(path, ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open station file.\n";
        return;
    }
    else
    {
        //! 把bike station建好
        bike_MaxHeap **stations_ptr = new bike_MaxHeap *[station_num];
        for (int i = 0; i < station_num; i++)
        {
            cout << "station_num: " << station_num << endl;
            // bike_MaxHeap *station_ptr;
            // stations_ptr[i] = station_ptr;
            stations_ptr[i] = new bike_MaxHeap[count_bike_type];
            // station_ptr = new bike_MaxHeap[count_bike_type];
        };

        while (ifs >> bike_type >> bike_id >> station_id >> rental_price >> rental_count)
        {
            int num_bike_type = stoi(bike_type.erase(0, 1));
            int num_station_id = stoi(station_id.erase(0, 1));

            BMNode tmp;
            tmp.bike_type = bike_type;
            tmp.id = bike_id;
            tmp.rental_price = rental_price;
            tmp.rental_count = rental_count;
            /*cout
                << "num_station_id: " << num_station_id << "  num_bike_type: " << num_bike_type << endl;*/

            stations_ptr[num_station_id][num_bike_type].insertKey(tmp);
        }
        // test output
        for (int i = 0; i < station_num; i++)
        {
            // cout << "testtest, i: " << i << endl;
            for (int j = 0; j < count_bike_type; j++)
            {
                float tmp = 0;

                while ((tmp = stations_ptr[i][j].extractMax().rental_price) > 0)
                {
                    cout
                        << "bike " << tmp << endl;
                }
                cout << "final: " << i << " " << j << " " << endl;
            }
        }
        ifs.close();
        basic_stations = stations_ptr;
        //  return stations_ptr;
    }
}

void my_data::merge(UNode *&arr, int p, int q, int r)
{
    // Create L ← A[p..q] and M ← A[q+1..r]
    int n1 = q - p + 1;
    int n2 = r - q;

    UNode *L = new UNode[n1];
    UNode *M = new UNode[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];

    // Maintain current index of sub-arrays and main array
    int i, j, k;
    i = 0;
    j = 0;
    k = p;

    // Until we reach either end of either L or M, pick larger among
    // elements L and M and place them in the correct position at A[p..r]
    while (i < n1 && j < n2)
    {
        //* sort by start_time
        if (L[i].start_time < M[j].start_time)
            arr[k] = L[i++];
        else if (L[i].start_time > M[j].start_time)
            arr[k] = M[j++];
        else
        {
            //* sort by user_id
            if (L[i].user_ID <= M[j].user_ID)
                arr[k] = L[i++];
            else
                arr[k] = M[j++];
        }

        k++;
    }
    // When we run out of elements in either L or M,
    // pick up the remaining elements and put in A[p..r]
    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = M[j++];
}
void my_data::mergeSort(UNode *&arr, int l, int r)
{
    if (l < r)
    {
        // m is the point where the array is divided into two subarrays
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted subarrays
        merge(arr, l, m, r);
    }
}
void my_data::sort_users()
{
    mergeSort(all_user_list, 0, user_num - 1);
    /* cout << "Sorted array: \n";
    for (int i = 0; i < size; i++)
    cout << arr[i] << " ";
  cout << endl;
    */
}

void my_data::merge(BMNode *&arr, int p, int q, int r)
{
    // Create L ← A[p..q] and M ← A[q+1..r]
    int n1 = q - p + 1;
    int n2 = r - q;

    BMNode *L = new BMNode[n1];
    BMNode *M = new BMNode[n2];

    for (int i = 0; i < n1; i++)
        L[i] = arr[p + i];
    for (int j = 0; j < n2; j++)
        M[j] = arr[q + 1 + j];

    // Maintain current index of sub-arrays and main array
    int i, j, k;
    i = 0;
    j = 0;
    k = p;

    // Until we reach either end of either L or M, pick larger among
    // elements L and M and place them in the correct position at A[p..r]
    while (i < n1 && j < n2)
    {
        //* sort by bike_id
        if (L[i].id < M[j].id)
            arr[k] = L[i++];
        else
            arr[k] = M[j++];

        k++;
    }
    // When we run out of elements in either L or M,
    // pick up the remaining elements and put in A[p..r]
    while (i < n1)
        arr[k++] = L[i++];

    while (j < n2)
        arr[k++] = M[j++];

    delete[] L;
    delete[] M;
}
void my_data::mergeSort(BMNode *&arr, int l, int r)
{
    if (l < r)
    {
        // m is the point where the array is divided into two subarrays
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        // Merge the sorted subarrays
        merge(arr, l, m, r);
    }
}
/*
void my_data::read_fee()
{
    ifstream ifs("./test_case/fee.txt", ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open fee file.\n";
    }
    else
    {
        int count = 0;
        while (!ifs.eof())
        {
            // store the fee information
            if (count < 3)
            { // the first 3 lines
                ifs >> bike_type >> discount[count] >> regular[count];
            }
            else
            {
                ifs >> waiting;
                ifs >> switching;
                ifs >> transferring;
            }
            count++;
        }
    }
    ifs.close();
}
*/