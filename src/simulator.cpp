#include<string>
#include<iostream>

#include"../include/simulator.h"
#include"../include/analyzer.h"
using std::string;

bool InString(char ch, string str);

void simulate(class TM &machine, string input)
{
    std::cout << "start to simulate" << std::endl;
    CheckInputValid(machine, input);    
}

void CheckInputValid(class TM &machine, string input)
{
    string validS;
    std::cout << "Input: " << input << std::endl;
    std::cout << "valid chs is ";
    for(auto ch : machine.getS())
    {
        std::cout << ch;
        validS.push_back(ch);
    }
    std::cout << std::endl;
    

    // bool IsValid = true;
    // char Errch = 0;
    for(auto ch : input)
    {
        std::cout << "ch and input is " << ch << "-----" << input << std::endl;
        if(InString(ch, validS) == false)
            if(machine.get_verbose() == true)
            {
                std::cerr << "==================== ERR ====================" << std::endl;
                std::cerr << "error: '" << ch << "' was not declared in the set of input symbols" << std::endl;
                std::cerr << "Input: " << input << std::endl;
                //there is 7 + index spaces
                for(int i=0;i < input.find(ch) + 7; i++)
                    std::cerr << " ";
                std::cerr << "^" << std::endl;
                std::cerr << "==================== END ====================" << std::endl;
                std::exit(5);
            }
        else if(machine.get_verbose() == true)
        {
            std::cout << "Input: " << input << std::endl;
            std::cout << "==================== RUN ====================" << std::endl;          
        }
    }
}

bool InString(char ch, string str)
{
    for(auto i : str)
        if(i == ch)
            return true;
    return false;
}