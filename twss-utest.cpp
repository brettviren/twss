#include "utest.h"
// https://github.com/sheredom/utest.h

#include <unsupported/Eigen/FFT>
#include <fftw3.h>              // must precede ei_twss_impl.hpp
#include "ei_twss_impl.hpp"

#include <vector>
#include <thread>
#include <iostream>

using scalar_t = double;
using complex_t = std::complex<double>;
using MyFFT = Eigen::FFT<double, Eigen::twss::fftw_impl<scalar_t> >;


UTEST(foo, bar) {
    ASSERT_TRUE(1);
}

UTEST(twss, construct) {
    MyFFT fft;
}
UTEST(twss, fwdrev1d) {
    MyFFT fft;
    const size_t size = 1024;
    std::vector<scalar_t> s(size);
    std::vector<complex_t> c(size);
    fft.fwd(c.data(), s.data(), (int)size);
    fft.inv(s.data(), c.data(), size);
}

void fwdrev(MyFFT& fft, int id)
{
    int ntimes = 10;
    const size_t size = 1024;
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

UTEST(twss, threads) {

    MyFFT fft;
    std::vector<std::thread> workers;
    const int nworkers = 10;
    std::cerr << "Starting workers\n";
    for (int ind=0; ind<nworkers; ++ind) {
        workers.emplace_back(fwdrev, std::ref(fft), ind);
    }
    std::cerr << "Waiting for workers\n";
    for (auto& worker : workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

UTEST_MAIN();
