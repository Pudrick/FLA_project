#include<iostream>
#include<vector>
#include<string>

#include"../include/analyzer.h"

using std::string;

//global variables to mark modes
// bool HELP_MODE = false;
// bool VERBOSE_MODE = false;

int main(int argc, char *argv[])
{
    std::vector<string> arguments;
    bool v_mode = false;
    // arguments analyzing
    for(int i = 1;i < argc;i++)// if i start from 0, argv[0] will be "./turing"
    {
        string arg = argv[i];//although they are not the same type
        if(arg == "-h" || arg == "--help")
        {
            // HELP_MODE = true;
            std::cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << std::endl;
            return 0;
        }
        else if(arg == "-v" || arg == "--verbose")
            v_mode = true;
        else 
            arguments.push_back(arg);
    }
    std::cout << "construction begin" << std::endl;
    analyzer Ana(*arguments.begin(), v_mode);
    return 0; 
}