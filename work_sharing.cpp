#include <omp.h>

#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
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
#pragma omp single //nowait
        std::cout << "gathering input: " << omp_get_thread_num() << '\n';
#pragma omp critical
        std::cout << "in parallel on: " << omp_get_thread_num() << '\n';
#pragma omp barrier
#pragma omp master
        std::cout << "output on : " << omp_get_thread_num() << '\n';
    }
}

void sync() {
    std::cout <<  "atomic\n";
    int sum{0};
#pragma omp parallel for num_threads(4)
    for(int i = 0; i < 100; ++i) {
#pragma omp atomic
        ++sum;
    }
    std::cout << sum << '\n';

    std::cout << "=====sync=====\n";
    std::cout << "ordered\n";
    std::vector<int> squares;
#pragma omp parallel for ordered
    for (int i = 0; i < 20; ++i) {
        printf("%d: %d\n", omp_get_thread_num(), i);
        int j = i*i;
#pragma omp ordered
        squares.push_back(j);
    }

    for(const auto square : squares) {
        std::cout << "- " << square << '\n';
    }
}

int main(int argc, char* argv[]) {
    // helloOpenMP();
    // parallizeForLoop();
    // sections();
    // single_master();
    sync();
    return 0;
}