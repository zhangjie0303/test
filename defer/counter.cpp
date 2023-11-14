#include <iostream>
#include <functional>

using FN_DEFER = std::function<void()>;

class Zhang
{
public:
    Zhang(FN_DEFER fn) : func(fn){
        if(func)
        {   
            func();
        }   
    }

    ~Zhang()
    {   
        /*if(func)
        {   
            func();
        }*/   
    } 

private:
    FN_DEFER func;
};

#define zhang_name(name, count) name##count
#define zhang_link(name, count) zhang_name(name, count)
#define zhang(expr) Zhang zhang_link(Zhang_, __COUNTER__) ([&](){expr;})
//#define zhang(expr) Zhang zhang_link(Zhang_, __COUNTER__) ([aa](){expr;})
//#define zhang(expr) Zhang zhang_link(Zhang_, __COUNTER__) ([aa]()mutable{expr;})

int main(int argc, char** argv)
{
    int aa = 1;
    zhang(aa = 100; std::cout << "in zhang, aa=" << aa << std::endl;);
    std::cout << "in main, aa=" << aa << std::endl;

	int m = 0;
	int n = 0;
	[&, n] (int a) mutable { m = ++n + a; }(4);
	std::cout << m << " | " << n << std::endl;
}
