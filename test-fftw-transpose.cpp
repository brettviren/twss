#include <fftw3.h>
#include <iostream>
#include <vector>

// based on example from fftw3 faq
fftwf_plan plan_transpose(int rows, int cols, float *in, float *out)
{
    const unsigned flags = FFTW_ESTIMATE; /* other flags are possible */
    fftw_iodim howmany_dims[2];

    howmany_dims[0].n  = rows;
    howmany_dims[0].is = cols;
    howmany_dims[0].os = 1;

    howmany_dims[1].n  = cols;
    howmany_dims[1].is = 1;
    howmany_dims[1].os = rows;

    return fftwf_plan_guru_r2r(/*rank=*/ 0, /*dims=*/ NULL,
                               /*howmany_rank=*/ 2, howmany_dims,
                               in, out, /*kind=*/ NULL, flags);
}



void dump(const std::vector<float>& data, int nrows, int ncols)
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
    std::vector<float> v{0,1,2,3,4,5};
    dump(v, 2, 3);
    auto plan = plan_transpose(2, 3, v.data(), v.data());
    fftwf_execute_r2r(plan, v.data(), v.data());
    dump(v, 3, 2);
    std::cerr << "\n";
    return 0;
}
