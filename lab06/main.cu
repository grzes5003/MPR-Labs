#include "cuda_vec_add.cu"
#include <iostream>
#include "args_parser.cpp"
#include <boost/format.hpp>

int main(int argc, char *argv[]) {
    boost::optional<args> _args = parse(argc, argv);
    if (!_args) {
        std::cerr << "Invalid parsing; exiting" << std::endl;
        return 1;
    }
    std::pair<float, int> result = gpu::vec_add(_args->vector_size, _args->blocks);

    std::cout << boost::format("%2% %1%!\n") % result.first % result.second;
    return 0;
}