#include <omp.h>

#include <cstdio>

void dataSharing() {
    int var = 10; // every thread thread will have its own copy of this var
#pragma omp parallel for lastprivate(var)//firstprivate(var)//private(var)
    for(int i = 0; i < 10; ++i) {
        printf("thread %d, var = %d\n", omp_get_thread_num(), var);
        var = 1000 + omp_get_thread_num();
    }

    printf("%d\n", var);
}

int main() {
  dataSharing();
  return 0;
}