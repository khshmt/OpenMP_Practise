#include <omp.h>

#include <chrono>
#include <iostream>
#include <thread>
#include <cmath>

void helloOpenMP() {
  omp_set_num_threads(std::thread::hardware_concurrency()/2);
#pragma omp parallel
  {
#pragma omp critical
    std::cout << "Hello, OpenMP! " << omp_get_thread_num() << "/"
              << omp_get_num_threads() << '\n';
  }
}

void parallizeForLoop() {
    constexpr int length{1024*1024*64};
    float* a = new float[length];
    float* b = new float[length];
    float* c = new float[length];
    float* result = new float[length];

    auto p1 = std::chrono::steady_clock::now();
    #pragma omp parallel for // when using for loop with OpenMP pragma do not use size_t for the index(size_t i = 0) of curly braces for initialization(int i{0})
    for (int i = 0; i < length; ++i) {
        result[i] = a[i] + b[i] * std::erff(c[i]);
    }
    auto p2 = std::chrono::steady_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p2-p1).count() << '\n';

    delete[] a;
    delete[] b;
    delete[] c;
    delete[] result;
}

void sections() {
#pragma omp parallel sections
    {
#pragma omp section
        {
            for(int i = 0; i<1000; ++i)
                std::cout << i;
        }
#pragma omp section
        {
            for(int i = 0; i<100; ++i) 
                std::cout << static_cast<char>('a' + (i%26));
        }
    }
}


void single_master() {
    omp_set_num_threads(std::thread::hardware_concurrency());
#pragma omp parallel 
    {
#pragma omp single
        std::cout << "gathering input: " << omp_get_thread_num() << '\n';
#pragma omp critical
        std::cout << "in parallel on: " << omp_get_thread_num() << '\n';
#pragma omp barrier
#pragma omp master
        std::cout << "output on : " << omp_get_thread_num() << '\n';
    }
}
int main(int argc, char* argv[]) {
//   helloOpenMP();
//   parallizeForLoop();
//   sections();
    single_master();
    return 0;
}