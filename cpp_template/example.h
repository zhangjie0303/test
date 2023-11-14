#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <stdio.h>

struct Example
{
    template <typename T, typename U>
    void print(T t, U u) {
      printf("The values are: %d and %f\n", t, u);
	}
};

#endif

