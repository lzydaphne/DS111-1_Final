#include "./nthu_bike.h"
#include <fstream>
// rounded to the nearest integer
int round(float input)
{
    int lower = (int)input;
    if (input >= (lower + 0.5))
        return lower + 1;
    else
        return lower;
}

void basic(string selectedCase)
{
    // insert your code here
    cout << "start your basic version of data structure final from here!" << endl;

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
        cout << "not open";
}