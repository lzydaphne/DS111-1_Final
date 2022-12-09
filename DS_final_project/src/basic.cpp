#include "./nthu_bike.h"
#include "./include/my_data.h"
#include <fstream>
#include <cmath> //round

void basic(string selectedCase)
{
    // insert your code here
    cout << "start your basic version of data structure final from here!" << endl;
    my_data read_data;
    int user_num = read_data.read_user();
    int station_num = read_data.get_station_num();
    int revenue_1 = 0;

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