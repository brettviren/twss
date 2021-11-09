#include "boost/multi_array.hpp"

#include <vector>
#include <iostream>

int main()
{
    using array_t = boost::multi_array_ref<int, 2>;

    std::vector<int> a{0,1,2,3,4,5};

    array_t a1(a.data(), boost::extents[2][3],
               boost::fortran_storage_order());
    for (int ind=0; ind<6; ++ind) {
        std::cerr << ind << ":" << a1.data()[ind] << " ";
    }
    std::cerr << "\n";
    for (int i=0; i<2; ++i) {
        for (int j=0; j<3; ++j) {
            std::cerr << "["<<i<<"]["<<j<<"]:" << a1[i][j] << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";


    std::vector<size_t> extents{2,3};
    array_t a2(a.data(), extents,
               boost::c_storage_order());
    for (int ind=0; ind<6; ++ind) {
        std::cerr << ind << ":" << a2.data()[ind] << " ";
    }
    std::cerr << "\n";
    for (int i=0; i<2; ++i) {
        for (int j=0; j<3; ++j) {
            std::cerr << "["<<i<<"]["<<j<<"]:" << a2[i][j] << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";


    std::vector<size_t> extents3{3,2};
    array_t a3(a.data(), extents3,
               boost::c_storage_order());
    for (int ind=0; ind<6; ++ind) {
        std::cerr << ind << ":" << a3.data()[ind] << " ";
    }
    std::cerr << "\n";
    for (int i=0; i<3; ++i) {
        for (int j=0; j<2; ++j) {
            std::cerr << "["<<i<<"]["<<j<<"]:" << a3[i][j] << " ";
        }
        std::cerr << "\n";
    }
    std::cerr << "\n";
    return 0;
}
