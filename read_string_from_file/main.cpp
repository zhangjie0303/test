#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

//从文件读入到string里
std::string readFileIntoString(const char * filename)
{
	ifstream ifile(filename);
	//将文件读入到ostringstream对象buf中
	ostringstream buf;
	char ch;
	while(buf&&ifile.get(ch))
	buf.put(ch);
	//返回与流对象buf关联的字符串
	return buf.str();
}

int main(int argc, char** argv)
{
	//文件名
	string fn="a.txt";//这里如果用VS可能要来个强制转换(char*)"a.txt"
	string str;
	str=readFileIntoString((const char*)fn.c_str());
	cout<<str<<endl;
    return 0;
}
