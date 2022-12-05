#include <iostream>
using namespace std;

// extern 是為了讓其他include nthu_bike header的檔案知道這兩個函數的宣告已經存在，且定義會在其他地方
// https://stellvia7.pixnet.net/blog/post/83168971-%5B%E8%BD%89%5D%5Bcs%5D-c%2B%2B-%E8%B7%A8%E6%AA%94%E6%A1%88%E5%85%AC%E7%94%A8%E8%AE%8A%E6%95%B8
extern void basic(string selectedCase);
extern void advanced(string selectedCase);