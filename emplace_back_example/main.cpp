// https://zhuanlan.zhihu.com/p/496401940
#include<iostream>
#include <vector>
using namespace std;

class MyTest
{
public:
    //普通构造
    MyTest(int id,int age):m_id(id),m_age(age)
    { 
        cout << "ceate MyTest class..." << this << endl;
    }
    //拷贝构造
    MyTest(const MyTest &t):m_id(t.m_id),m_age(t.m_age)
    {  
        cout << "copy construct called..." << this << endl;
    }
    //移动构造
    MyTest(const MyTest &&t)
    {
        m_id = std::move(t.m_id);
        m_age = std::move(t.m_age);
        cout << "move contruct called.." << this << endl;
    }
    //析构
    ~MyTest()
    {
        cout << "destory MyTest class..." << this << endl;
    }
private:
    int m_id; //id成员
    int m_age;//age成员
};

int main(int argc, char *argv[])
{
    vector<MyTest> vec;
    vec.reserve(2);  //预先分配内存
    cout << "\n ------ push_back --------" << endl;
    vec.push_back(MyTest(1,20));
    cout << "\n ------ emplace_back --------" << endl;
    vec.emplace_back(1,20);
    cout << "\n -------- finish -------- " << endl;
}
