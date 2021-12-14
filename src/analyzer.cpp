//arguments getin: tm file name
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<limits>

#include"../include/analyzer.h"

using std::string;

void analyzer::set_verbose(bool value)
{
    VERBOSE_MODE = value;
}

analyzer::analyzer(string tm_name, bool v_mode)
{
    set_verbose(v_mode);
    tm = tm_name;
    tm_file.open(tm_name);
    // std::cout << "opening begins and tm_name is " << tm_name << std::endl;
    if(tm_file.is_open() == false)
    {
        std::cerr << "Can't open TM file." << std::endl;
        std::exit(1);
    }
    std::cout << "if has ended" << std::endl;
    tmFileAna(tm_file);
    std::cout << "prepare to close" << std::endl;
    tm_file.close();
}

void analyzer::tmFileAna(std::ifstream &filestream)
{
    while(!filestream.eof())
    {
        if(filestream.fail())
        {
            std::cerr << "Can't read the TM file." << std::endl;
            std::exit(2);
        }
        char first = filestream.peek();
        // std::cout << "first is " << int(first) << std::endl;
        if(first == ';' || first == 10 || first == 0 || first == -1)// ASCII 10 is '\n'.
        {
            // why the last empty line will get first == -1 ??
            std::cout << "get a comment" << std::endl;
            filestream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        if(first == '#')
        {
            char flag_temp[2];
            filestream.get(flag_temp, 3);
            // std::cout << flag << std::endl;
            string flag = flag_temp;
            if(flag == "#Q")
            {
                
            }
        }
    }
}