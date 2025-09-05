#include "../headers/programOptions.hpp"

ProgramOptions::ProgramOptions(int argc, char * argv[])
{    
    /*create description for app*/
    _description = std::make_unique<opt::options_description>("All options");

    /*add some options*/
    _description->add_options()

        ("configPath", opt::value<std::string>(), 
        "path to configuration file. Required option\n")        
    ;

    /*parse and save args from command line*/
    try
    {
        opt::store(opt::parse_command_line(argc, argv, *_description.get()), _variables);
    }
    catch(const opt::unknown_option & e)
    {
        std::cout << *_description.get() << std::endl;
    }
}

int ProgramOptions::init()
{
    if(_variables.empty())
    {
        std::cout << *_description.get() << std::endl;
        return 1;
    }
    return 0;
}
std::string ProgramOptions::getConfigPath()
{
    if(_variables.count("configPath"))
        return _variables["configPath"].as<std::string>();

    return "";
}