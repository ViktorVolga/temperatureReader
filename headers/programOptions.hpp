#pragma once

#include <boost/program_options.hpp>
#include <memory>
#include <iostream>

namespace opt = boost::program_options;

class ProgramOptions 
{   
    std::unique_ptr<opt::options_description> _description;
    opt::variables_map _variables;
public:
    ProgramOptions(int argc, char * argv[]);
    int init();
    std::string getConfigPath();
};