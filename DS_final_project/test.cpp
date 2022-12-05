#include <iostream>
#include <fstream>
using namespace std;
int main()
{
    //假設檔案內有 1~6 6個整數，假設檔案的最後有一個空格字符
    ifstream in;
    in.open("data.txt");
    if (in.fail())
    {
        cout << "file opening is failed...";
        exit(1);
    }
    while (!in.eof())
    {
        int value;
        in >> value;
        cout << value << endl;
        cout << "good()=" << in.good() << ",";
        cout << "fail()=" << in.fail() << ",";
        cout << "bad()=" << in.bad() << ",";
        cout << "eof()=" << in.eof() << endl;
    }
    return 0;
}
