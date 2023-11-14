// https://www.h5w3.com/246425.html
// https://blog.csdn.net/t13506920069/article/details/125727946
#include <iostream>
#include <functional>
#include <fstream>

/*
class Defer
{
public:
	explicit Defer(std::function<void()> && func)
	{
		m_func = std::move(func);
	}

	~Defer()
	{
		m_func();
	}

private:
	std::function<void()> m_func;
};

#define defer_name(name, count) name##count##private
#define defer_link(class, count) defer_name(class, count)
#define defer(expr) auto defer_link(Defer, __COUNTER__) = Defer(expr)
*/

using FN_DEFER = std::function<void()>;

class Defer
{
public:
    Defer(FN_DEFER fn) : func(fn){}

    ~Defer()
    {   
        if(func)
        {   
            func();
        }   
    }   
private:
    FN_DEFER func;
};

#define defer_name(name, count) name##count
#define defer_link(name, count) defer_name(name, count)
//#define defer(expr) Defer defer_link(__Defer__,__COUNTER__) ([&](){expr;})
#define defer(expr) Defer defer_link(Defer_,__COUNTER__) ([&](){expr;})
//#define defer(expr) Defer defer_name(Defer_,__COUNTER__) ([&](){expr;})
// use example:
// 1):
// defer(std::cout << "close fd." << std::endl);
//
// 2):
// defer(std::cout << "close fd." << std::endl;);
//
// 3):
// {
//    defer(std::cout << "close fd." << std::endl;);
// }
//
// 4):
// std::ofstream ofs;
// defer(
//    std::cout << "ofs_closed" << std::endl;
//    ofs.close();
// )
// ofs.open("test.txt");
// 

void my_print()
{
	defer(std::cout << "func: my_print, defer counter: " << __COUNTER__ << std::endl;);
	std::cout << "func: my_print, counter: " << __COUNTER__ << std::endl;
}

int main()
{
	std::ofstream ofs;
    defer(std::cout << "zhangjie, defer: " << std::endl;);
    defer(
        std::cout << "ofs_closed, defer" << std::endl;
        std::cout << "ofs_closed, defer 222" << std::endl;
        ofs.close();
    );

	my_print();	

    {
        defer(std::cout << "local area, defer." << std::endl);
    }
	
	/*defer ([&]{
		std::cout << "ofs_closed" << std::endl;
		ofs.close();
	});	*/
	ofs.open("test.txt");
    std::string text = "hello defer!";
	ofs.write(text.c_str(), text.size());
    std::cout << "end main, counter: " << __COUNTER__ << ", line:" << __LINE__ << std::endl;
    return 0;
}
