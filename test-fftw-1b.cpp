// Do fft on one dimension.

#include <fftw3.h>
#include <iostream>
#include <vector>
#include <complex>

/*
  1234
  5678

  array(0,1) = 2
  nrows=2 ncols=4

  axis=1 means 2 FFTs each of length 4.
 */

// all sorts of evil in this function
static fftwf_complex* cast( const std::vector<std::complex<float> >& p)
{ 
    return const_cast<fftwf_complex*>( reinterpret_cast<const fftwf_complex*>(p.data()) ); 
}

fftwf_plan plan_1b(int sign, int axis, int nrows, int ncols, fftwf_complex *in, fftwf_complex *out)
{
    // (r,c) element at in + r*stride + c*dist

    const int rank = 1;         // dimension of transform
    int n = ncols;              // along rows
    int howmany = nrows;
    int stride = 1;
    int dist = ncols;
    if (axis == 0) {            // along columns
        n = nrows;
        howmany = ncols;
        stride = ncols;
        dist = 1;
    }
    int *inembed=&n, *onembed=&n;

    unsigned int flags =  FFTW_ESTIMATE|FFTW_PRESERVE_INPUT;

    return fftwf_plan_many_dft(rank, &n, howmany,
                               in, inembed,
                               stride, dist,
                               out, onembed,
                               stride, dist,
                               sign, flags);
}

void dump(const std::vector<std::complex<float>>& data, int nrows, int ncols)
{
    for (int irow = 0; irow<nrows; ++irow) {
        for (int icol = 0; icol<ncols; ++icol) {
            std::cerr << data[irow*ncols + icol] << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";
}

int main()
{
    const int nrows = 3;
    const int ncols = 8;
    const int size = nrows*ncols;

    std::vector<std::complex<float>> v(size, 0), v1(size,0), v2(size,0);

    const int irow = 1;
    const int icol = 2;
    v[irow*ncols + icol] = 1;

    auto p1 = plan_1b(FFTW_FORWARD, 0, nrows, ncols, cast(v), cast(v1));
    fftwf_execute_dft(p1, cast(v), cast(v1));
    
    auto p2 = plan_1b(FFTW_FORWARD, 1, nrows, ncols, cast(v), cast(v2));
    fftwf_execute_dft(p2, cast(v), cast(v2));
    
    dump(v, nrows, ncols);
    dump(v1, nrows, ncols);
    dump(v2, nrows, ncols);

    return 0;
}
