#include <unsupported/Eigen/FFT>
#include <fftw3.h>
#include "ei_twss_impl.hpp"

using scalar_t = double;
using complex_t = std::complex<double>;
using MyFFT = Eigen::FFT<double, Eigen::twss::fftw_impl<scalar_t> >;

#include <vector>

#include "utest.h"
// https://github.com/sheredom/utest.h

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


UTEST_MAIN();
