#include <stdio.h>
#include <ucontext.h>
#include <unistd.h>

#include <iostream>

int main(int argc, const char *argv[]) {
    ucontext_t context;

    getcontext(&context);
    puts("Hello world");
    sleep(1);
    setcontext(&context);

    return 0;
}

