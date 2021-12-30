//arguments getin: tm file name
#include<iostream>
#include<string>
#include<vector>
#include<fstream>
#include<limits>
#include<algorithm>
#include<regex>
#include<memory>
#include<array>
#include<sstream>

#include"../include/analyzer.h"

using std::string;

#define IGNORE_WHOLE_LINE std::numeric_limits<std::streamsize>::max()

void TM::set_verbose(bool value)
{
    VERBOSE_MODE = value;
}

TM::TM(string tm_name, bool v_mode)
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
    // std::cout << "prepare to open tmfileana" << std::endl;
    tmFileAna(tm_file);
    CurrentState = q0;
    // std::cout << "prepare to close" << std::endl;
    tm_file.close();
}

void TM::tmFileAna(std::ifstream &filestream)
{
    while(!filestream.eof())
    {
        if(filestream.fail())
        {
            std::cerr << "Can't read the TM file." << std::endl;
            std::exit(2);
        }
        char first = filestream.peek();
        // std::cout << "first is " << first << std::endl;
        // std::cout << "first is " << int(first) << std::endl;
        if(first == ';' || first == 10 || first == 0 || first == -1)// ASCII 10 is '\n'.
        {
            // why the last empty line will get first == -1 ??
            // maybe -1 represent to eof.
            // std::cout << "get a comment with first " << int(first) << std::endl;
            filestream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        // string head_regex = "[^=]*";
        // std::regex head(head_regex);
        std::regex head_regex("[^ ]*");// get string before " "
        std::smatch flag;
        string line;
        
        std::getline(filestream,line);
        // std::cout << "line is " << line << std::endl;
        // std::cout << "string to regex is [" << line  << ']'<< std::endl;
        std::regex_search(line, flag, head_regex);
        // std::cout << "get flag " << flag.str() << std::endl;
        // std::cout << "use regex will be "<< head.str() << '.' << std::endl;
        // continue;

        
        // if(first == '#')
        // {
        //     char flag_temp[2];
        //     filestream.get(flag_temp, 3);
        //     // std::cout << flag << std::endl;
        //     string flag = flag_temp;
        if(flag.str() == "#Q" || flag.str() == "#F")
        {
            // std::cout << "Add Q of F begin" << std::endl;
            // std::unique_ptr<std::vector<string>> element;
            std::vector<string> *element;
            if(flag.str() == "#Q")
                element = &Q;
            else
                element = &F;
            // filestream.ignore(std::numeric_limits<std::streamsize>::max(),'{');
            
            // string state_set;
            // std::getline(filestream,state_set,'}');
            std::smatch state_match;
            std::regex in_bracket("\\{(.*)\\}");
            // std::cout << "start regex search" << std::endl;
            std::regex_search(line, state_match, in_bracket);

            auto state_set = state_match[1];
            // std::cout << "Q or F is " << state_set.str() << std::endl;
            // here we need [1], otherwise the curly bracket will be inside.
            auto pos = 0;
            std::stringstream state_stream(state_set.str());
            string state;
            
            // while((pos = state_set.str().find(','))  !=  string::npos)
            while(std::getline(state_stream, state, ','))
            {
                (*element).push_back(state);
                // std::cout << "push is " << state << std::endl;
                // state_set.str().erase(0,pos + 1);// 1 is length for ','
            }// the last token don't have ',' so have to push it manually.
            // (*element).push_back(state_set.str());
            // std::cout << "push finished" << std::endl;

            //just for more robustness.
            for(auto str : (*element))
            {
                str.erase(std::remove(str.begin(),str.end(),' '),str.end());// WTF?? MAGIC!
                // std::cout << "final Q or F is " << str << std::endl;
            }
                // std::cout << "token is " << i << std::endl;
            // std::cout << "Add Q or F finished." << std::endl;
        }
        if(flag.str() == "#S")
        {
            // std::cout << "Add S begin." << std::endl;
            std::vector<char> S_ban_list = {' ', ',', ';', '{', '}', '*', '_'};
            // filestream.ignore(std::numeric_limits<std::streamsize>::max(),'{');
            string input_set;
            // std::getline(filestream, input_set, '}');

            std::smatch input_match;
            std::regex in_bracket("\\{(.*)\\}");
            // std::cout << "start regex search of input" << std::endl;
            std::regex_search(line, input_match, in_bracket);
            input_set = input_match[1].str();

            // std::cout << "input set is " << input_set << std::endl;
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
                // std::cout << "to append in S is " << ch << std::endl;
                S.push_back(ch);
            }
            // std::cout << "Add S finished." << std::endl;
        }
        if(flag.str() == "#G")
        {
            // std::cout << "Add G begin." << std::endl;
            // std::cout << "G line is " << line << std::endl;
            std::vector<char> G_ban_list = {' ', ',', ';', '{', '}', '*'};
            // filestream.ignore(std::numeric_limits<std::streamsize>::max(),'{');
            line.erase(0,6);
            line.pop_back();
            string tape_set;
            // std::getline(filestream, tape_set, '}');
            tape_set = line;
            // std::cout << "tape set in G is " << tape_set << std::endl;
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
                // std::cout << "to append in G is " << ch << std::endl;
                G.push_back(ch);
            }
            // std::cout << "Add G finished." << std::endl;
        }
        if(flag.str() == "#q0")
        {
            // std::cout << "Add q0 begin." << std::endl;
            // char q_flag;
            // filestream.get(&q_flag,2);
            // std::cout << "qflag is " << q_flag << std::endl;
            // if(q_flag != '0')
            // {
            //     std::cerr << "invalid flag of q0" << std::endl;
            //     std::exit(4);
            // }
            // std::cout << "line in q0 is " << line << std::endl;
            std::stringstream q0stream(line);
            q0stream.ignore(IGNORE_WHOLE_LINE, '=');
            q0stream.ignore();// there is a ' ' after '='
            std::getline(q0stream, q0);
            // std::cout << "q0 is " << q0 << std::endl;
            // std::cout << "Add q0 finished." << std::endl;
        }
        if(flag.str() == "#B")
        {
            // std::cout << "Add B begin." << std::endl;
            std::stringstream Bstream(line);
            Bstream.ignore(IGNORE_WHOLE_LINE,'=');
            Bstream.ignore();
            B = Bstream.get();
            // std::cout << "B is " << B << std::endl;
            // std::cout << "Add B finished." << std::endl;
        }
        if(flag.str() == "#N")
        {
            // std::cout << "Add N begin." << std::endl;
            std::stringstream Nstream(line);
            Nstream.ignore(IGNORE_WHOLE_LINE, '=');
            Nstream.ignore();// there is a ' ' after '='
            string temp;
            std::getline(Nstream,temp);
            N = std::stoi(temp);
            // std::cout << "N is " << N << std::endl;
            // std::cout << "Add N finished." << std::endl;
        }
        if(first != '#')// means delta
        {
            // std::cout << "Add delta begin." << std::endl;
            // std::regex head_regex("[^ ]*");
            string delta_set = line;
            // std::getline(filestream, delta_set);
            // std::unique_ptr<string> temp;// it is not so good to make it
            std::array<string, 5> name_list;
            name_list = {"prev_state", "prev_symbols", "new_symbols", "directions", "new_state"};
            auto pos = 0;
            auto index = name_list.begin();
            std::map<string, string> delta_item;
            // std::cout << "deltaset is " << delta_set << std::endl;
            // std::cout << "head of deltaset is " << (int)delta_set[0] << std::endl;
            // std::cout << "first is " << first << std::endl;
            // for(;index != name_list.end();index++)
            //     std::cout << *index << " wooo" << std::endl;
            // std::cout << "start loop\n";
            while((pos = delta_set.find(' '))  !=  string::npos)
            {
                // (*element).push_back(state_set.substr(0, pos));
                // delta[name_list[index]] = delta_set.substr(0, pos);
                // std::cout << "start insert.\n";
                // std::cout << "to insert is " << *index << " and " << delta_set.substr(0, pos) << std::endl;
                delta_item.insert({*index, delta_set.substr(0, pos)});
                // std::cout << "insert success.\n";
                // std::cout << "index is " << *index << std::endl;
                // std::cout << "val is " << delta_set.substr(0, pos) << std::endl;
                delta_set.erase(0,pos + 1);// 1 is length for ' '
                // std::cout << "erase successed.\n";
                index++;
            }// the last token don't have ',' so have to push it manually.
            std::cout << "loop end.\n";
            // std::cout << "last index is " << *index << std::endl;
            // std::cout << "last www is " << delta_set.substr(0, pos) << std::endl;
            delta_item.insert({*index, delta_set.substr(0, pos)});
            delta.push_back(delta_item);
            // for(auto str : delta)
            //     std::cout << "delta is " << str.second << std::endl;
            // std::cout << "Add delta finished." << std::endl;
        }
    }
}

std::vector<char> TM::getS()
{
    return S;
}

bool TM::get_verbose()
{
    return VERBOSE_MODE;
}

void TM::set_state(string state)
{
    CurrentState = state;
}

string TM::get_state()
{
    return CurrentState;
}

std::vector<std::map<string, string>> TM::get_delta()
{
    return delta;
}

std::vector<string> TM::get_final()
{
    return F;
}