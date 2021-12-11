//arguments getin: tm file name
#include<iostream>
#include<string>
#include<vector>
#include<fstream>

#include"../include/analyzer.h"

using std::string;

analyzer::analyzer(string tm_name)
{
    tm = tm_name;
    tm_file.open(tm_name);
    if(!tm_file.is_open())
    {
        std::cerr << "Can't open TM file." << std::endl;
        std::exit(1);
    }
    tmFileAna(tm_file);
    tm_file.close();
}

void analyzer::tmFileAna(std::ifstream &filestream)
{
    std::cout << filestream.is_open() << std::endl;
}