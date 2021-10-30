#include <unsupported/Eigen/FFT>
#include <fftw3.h>              // must precede ei_twss_impl.hpp
#include "ei_twss_impl.hpp"

#include <chrono>
#include <vector>
#include <thread>
#include <iostream>

using scalar_t = double;
using complex_t = std::complex<double>;
using MyFFT = Eigen::FFT<double, Eigen::twss::fftw_impl<scalar_t> >;

const size_t size = 1024;

void fwdrev(MyFFT& fft, int id, int ntimes)
{
    std::vector<complex_t> c(size*size);
    std::vector<complex_t> c2(size*size);
    auto & impl = fft.impl();

    std::cerr << "running " << id << std::endl;

    while (ntimes) {
        impl.fwd2(c.data(), c2.data(), size, size);

        impl.inv2(c.data(), c2.data(), size, size);

        --ntimes;        
    }
    std::cerr << "finished " << id << std::endl;
}

void warm_up(MyFFT& fft)
{
    std::cerr << "Warming\n";
    std::vector<complex_t> c(size*size);
    std::vector<complex_t> c2(size*size);
    auto & impl = fft.impl();
    impl.fwd2(c.data(), c2.data(), size, size);
    impl.inv2(c.data(), c2.data(), size, size);
}
             

int main(int argc, char* argv[])
{
    const int nworkers = atoi(argv[1]);    
    const int ntimes = atoi(argv[2]);

    using namespace std::chrono;

    MyFFT fft;

    steady_clock::time_point t1 = steady_clock::now();
    warm_up(fft);
    steady_clock::time_point t2 = steady_clock::now();

    std::vector<std::thread> workers;

    std::cerr << "Starting workers\n";
    for (int ind=0; ind<nworkers; ++ind) {
        workers.emplace_back(fwdrev, std::ref(fft), ind, ntimes);
    }
    std::cerr << "Waiting for workers\n";
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
    steady_clock::time_point t3 = steady_clock::now();
    duration<double> dt1 = duration_cast<duration<double>>(t2 - t1);
    duration<double> dt2 = duration_cast<duration<double>>(t3 - t2);
    std::cout << nworkers*ntimes
              << " " << nworkers << " " << ntimes
              << " " << dt1.count() << " " << dt2.count() << std::endl; 

    return 0;
}
