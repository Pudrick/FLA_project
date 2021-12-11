#include<iostream>
#include<vector>
#include<string>

#include"../include/analyzer.h"

using std::string;

//global variables to mark modes
bool HELP_MODE = false;
bool VERBOSE_MODE = false;

int main(int argc, char *argv[])
{
    std::vector<string> arguments;

    // arguments analyzing
    for(int i=0;i < argc;i++)
    {
        string arg = argv[i];//although they are not the same type
        if(arg == "-h" || arg == "--help")
        {
            HELP_MODE = true;
            std::cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>" << std::endl;
            return 0;
        }
        else if(arg == "-v" || arg == "--verbose")
            VERBOSE_MODE = true;
        else 
            arguments.push_back(arg);
    }
    analyzer Ana(*arguments.begin());
    return 0; 
}