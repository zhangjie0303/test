#include <string>
#include"first.h"
#include"second.h"

void second()
{
    printf("This is second!\n");
    first();
}

std::string second_str()
{
    return "second str | " + first_str();    
}
