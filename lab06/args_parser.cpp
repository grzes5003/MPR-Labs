//
// Created by xgg on 2 Jun 2022.
//
#include <iostream>
#include "args.h"

#include <boost/program_options.hpp>
#include <boost/optional.hpp>
namespace po = boost::program_options;


inline boost::optional<args> parse(int argc, char *argv[]) {
    args _args{};

    po::options_description desc("Allowed options");
    desc.add_options()
            ("help", "produce help message")
            ("vec_size,n", po::value<int>(&_args.vector_size), "set compression level")
            ("blocks,b", po::value<int>(&_args.blocks), "set compression level");

    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).options(desc).run(), vm);
    po::notify(vm);
    // Check if there are enough args or if --help is given
    if (vm.count("help") || !vm.count("vec_size") || !vm.count("blocks")) {
        std::cerr << desc << "\n";
        return boost::none;
    }

    return _args;
}