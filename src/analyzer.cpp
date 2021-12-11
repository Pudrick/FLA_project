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
    std::cout << "opening begins and tm_name is " << tm_name << std::endl;
    if(tm_file.is_open() == false)
    {
        std::cout << "Can't open TM file." << std::endl;
        std::exit(1);
    }
    std::cout << "if has ended" << std::endl;
    tmFileAna(tm_file);
    std::cout << "prepare to close" << std::endl;
    tm_file.close();
}

void analyzer::tmFileAna(std::ifstream &filestream)
{
    std::cout << filestream.is_open() << std::endl;
}