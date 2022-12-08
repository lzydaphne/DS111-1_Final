#include "my_data.h"

#define station_max_num 100 //暫時設定的，不然要把整個map都跑過一次才知道 = =

/*
 todo 目前計算站數，是直接掃過一次map，有沒有更快的方法?
*/
//計算站數
int my_data::get_station_num()
{
    ifstream ifs("../../testcases/case1/map.txt", ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open station file.\n";
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
        // string s;
        // while (std::getline(ifs, s))
        // {
        //     edge_num++;
        // }
    }
    ifs.close();
    return station_num;
}

//建graph
Graph *my_data::read_map()
{
    // read station data
    ifstream ifs("../../testcases/case1/map.txt", ios::in);
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

            graph_ptr->addEdge(num_start_station - 1, num_end_station - 1, distance); // station id start from 0
            // cout << "start: "<< start_station-1 << " end: " << end_station-1 << " distance: " << distance << "\n";
        }
        // graph_ptr->print_graph();
        // cout << "min distance: " << graph_ptr->dijkstra(0, 5) << endl;
        ifs.close();
        return graph_ptr; // return the object pointer
    }
}

int my_data::read_user()
{
    ifstream ifs("../../testcases/case1/user.txt", ios::in);
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
            user_num++;
        }
    }
    ifs.close();
    return user_num;
}

/*
my_station **my_data::read_station()
{
    ifstream ifs("./test_case/station.txt", ios::in);
    if (!ifs.is_open())
    {
        cout << "Failed to open station file.\n";
        return 0;
    }
    else
    {
        my_station **station_ptr = new my_station *[station_num]; // station_ptr point to an array, and the array will store each station's object pointer
        while (ifs >> station_id >> num_elec >> num_lady >> num_road)
        {
            // add bikes_id into priority queue (inplemented by min heap) respectively
            my_station *ms = new my_station();
            my_MinHeap mm_elec(ms->electric, 0); // sent in a pointer point to its min heap (there will be multiple min heap), and the initial heap size
            my_MinHeap mm_lady(ms->lady, 0);
            my_MinHeap mm_road(ms->road, 0);
            for (int i = 0; i < num_elec; i++)
            {
                mm_elec.insertKey(station_id * 100 + i);
            }
            for (int i = 0; i < num_lady; i++)
            {
                mm_lady.insertKey(station_id * 100 + i);
            }
            for (int i = 0; i < num_road; i++)
            {
                mm_road.insertKey(station_id * 100 + i);
            }
            station_ptr[station_id - 1] = ms; // station id start from 0, storing the station's object pointer
        }
        ifs.close();
        return station_ptr;
    }
}

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