#include <iostream>
#include <cstring>

using namespace std;

//const char* fun()
const char* fun()
{
    char aa[32];
    strcpy(aa, "abcde");    
    cout << "aa = " << aa << endl;

    std::string str = std::string(aa);
    cout << "str = " << str << endl;
    //const char * char_str = str.c_str();
    const char * char_str = str.c_str();
    return char_str;
}

int main(int argc, char** argv) {
    string str = "12345789";
    cout << "after to uint64: " << stoul(str) << endl;
    string str2 = "";
    cout << "length: " << str2.length() << endl;
    const char * ret = fun();
    cout << "ret: " << ret << endl;
    return 0;    
}
