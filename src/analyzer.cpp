//arguments getin: tm file name
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<limits>
#include<algorithm>

#include"../include/analyzer.h"

using std::string;

#define IGNORE_WHOLE_LINE std::numeric_limits<std::streamsize>::max()

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
            // maybe -1 represent to eof.
            std::cout << "get a comment with first " << int(first) << std::endl;
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
                filestream.ignore(std::numeric_limits<std::streamsize>::max(),'{');
                string state_set;
                std::getline(filestream,state_set,'}');
                std::cout << "state set is " << state_set << std::endl;
                auto pos = 0;
                while((pos = state_set.find(','))  !=  string::npos)
                {
                    Q.push_back(state_set.substr(0, pos));
                    std::cout << "push is " << state_set.substr(0, pos) << std::endl;
                    state_set.erase(0,pos + 1);// 1 is length for ','
                }// the last token don't have ',' so have to push it manually.
                Q.push_back(state_set);
                
                //just for more robustness.
                for(auto str : Q)
                    str.erase(std::remove(str.begin(),str.end(),' '),str.end());// WTF?? MAGIC!
                    // std::cout << "token is " << i << std::endl;
            }
            if(flag == "#S")
            {
                std::vector<char> S_ban_list = {' ', ',', ';', '{', '}', '*', '_'};
                filestream.ignore(std::numeric_limits<std::streamsize>::max(),'{');
                string input_set;
                std::getline(filestream, input_set, '}');
                std::cout << "input set is " << input_set << std::endl;
                for(auto ch : input_set)
                {
                    if(ch == ',')
                        continue;
                    for(auto check : S_ban_list)
                        if(ch == check)
                        {
                            std::cerr << "Invalid input symbol." << std::endl;
                            std::exit(3);
                        }
                    std::cout << "to append in S is " << ch << std::endl;
                    S.push_back(ch);
                }
            }
            if(flag == "#G")
            {
                std::vector<char> G_ban_list = {' ', ',', ';', '{', '}', '*'};
                filestream.ignore(std::numeric_limits<std::streamsize>::max(),'{');
                string tape_set;
                std::getline(filestream, tape_set, '}');
                std::cout << "tape set is " << tape_set << std::endl;
                for(auto ch : tape_set)
                {
                    if(ch == ',')
                        continue;
                    for(auto check : G_ban_list)
                        if(ch == check)
                        {
                            std::cerr << "Invalid input symbol." << std::endl;
                            std::exit(3);
                        }
                    std::cout << "to append in G is " << ch << std::endl;
                    G.push_back(ch);
                }
            }
            if(flag == "#q")
            {
                char q_flag;
                filestream.get(&q_flag,2);
                std::cout << "qflag is " << q_flag << std::endl;
                if(q_flag != '0')
                {
                    std::cerr << "invalid flag of q0" << std::endl;
                    std::exit(4);
                }
                filestream.ignore(IGNORE_WHOLE_LINE, '=');
                filestream.ignore();// there is a ' ' after '='
                std::getline(filestream, q0);
                std::cout << "q0 is " << q0 << std::endl;
            }
            if(flag == "#B")
            {
                filestream.ignore(IGNORE_WHOLE_LINE,'=');
                filestream.ignore();
                B = filestream.get();
                std::cout << "B is " << B << std::endl;
            }
            
        }
    }
}