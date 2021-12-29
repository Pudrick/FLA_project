#include<string>
#include<iostream>
#include<sstream>
#include<algorithm>

#include"../include/simulator.h"
#include"../include/analyzer.h"
using std::string;

bool InString(char ch, string str);
void display_v();

void simulate(class TM &machine, string input)
{
    std::cout << "start to simulate" << std::endl;
    CheckInputValid(machine, input);    
    SimulateRunning(machine, input);
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
    }
    if(machine.get_verbose() == true)
    {
        std::cout << "Input: " << input << std::endl;
        std::cout << "==================== RUN ====================" << std::endl;          
    }
}

bool InString(char ch, string str)
{
    for(auto i : str)
        if(i == ch)
            return true;
    return false;
}

void SimulateRunning(class TM &machine, string input)
{
    
    std::cout << "start simulate running" << std::endl;
    std::vector<string> final_states = machine.get_final();
    string state_now = machine.get_state();
    int step = 0;
    int numOfTapes = 0;
    std::vector<int> head_pos;
    auto delta_vector = machine.get_delta();
    // std::cout << delta_vector[0].find("prev_symbols")->second << std::endl;
    numOfTapes = delta_vector[0].find("prev_symbols")->second.size();
    for(int i=0;i<numOfTapes;i++)
        head_pos.push_back(0);
    std::vector<string> tapes;
    tapes.push_back(input);
    for(int i=1;i<numOfTapes;i++)
        tapes.push_back("_");

    // std::cout << "numof is " << numOfTapes << std::endl;
    while(std::find(final_states.begin(),final_states.end(),state_now) == final_states.end())
    {
        //current state is not at Final state
        if(machine.get_verbose() == true)
        {
            std::cout << "Step   : " << step << std::endl;
            for(int i_tape=0;i_tape<numOfTapes;i_tape++)//ith tape
            {
                auto len = tapes[i_tape].size();
                string index_str;

                int i_ch = 0;
                for(int i_temp = 0;tapes[i_tape][i_temp] == '_';i_temp++)
                {
                    i_ch = i_temp;
                    break;
                }
                while(i_ch < len)
                {                                        
                    index_str.append(std::to_string(i_ch));
                    index_str.append(" ");
                    i_ch++;
                }

                std::cout << "Index" << i_tape << " : " << index_str << std::endl;

                string tape_str;
                for(int i_temp = i_ch;i_temp < len;i_temp++)
                {
                    tape_str.append(std::to_string(tapes[i_tape][i_temp]));
                    if(i_temp < 10)
                        tape_str.append(" ");
                    else
                        tape_str.append("  ");
                }
                std::cout << "Tape" << i_tape << " : " << tape_str << std::endl;

                int arrow_pos = index_str.find(head_pos[i_tape]);
                std::cout << "Head" << i_tape << "  : ";
                string head_str;
                for(int i_temp = i_ch;i_temp != head_pos[i_tape];i_temp++)
                {
                    head_str.append(" ");
                    if(i_temp < 10)
                        head_str.append(" ");
                    else
                        head_str.append("  ");
                }
                head_str.append("^");
                std::cout << head_str << std::endl;
            }
            std::cout << "State  : " << state_now;
        }//display in verbose

        for(auto delta : delta_vector)
        {
            // delta is ："<旧状态> <旧符号组> <新符号组> <⽅向组> <新状态>
            // name_list = {"prev_state", "prev_symbols", "new_symbols", "directions", "new_state"};
            // if(std::find(v.begin(), v.end(), x) != v.end()) {
            //     /* v contains x */
            // } else {
            //     /* v does not contain x */
            // }
            bool check_state = delta.find("prev_state")->second == state_now;
            bool check_symbol = true;
            string prev_symbols = delta.find("prev_symbols")->second;
            for(int i=0;i<numOfTapes;i++)
            {
                if(tapes[i][head_pos[i]] == prev_symbols[i])
                    check_symbol = check_symbol && true;
                else
                    check_symbol = check_symbol && false;
            }
            if(check_state && check_symbol)
            {
                string new_symbols = delta.find("new_symbols")->second;
                string directions = delta.find("directions")->second;
                for(int i=0;i<numOfTapes;i++)
                {
                    tapes[i][head_pos[i]] = new_symbols[i];
                    if(directions[i] == 'r')
                        head_pos[i]++;
                    else if(directions[i] == 'l')
                        head_pos[i]--;
                }
                machine.set_state(delta.find("new_state")->second);
                break;
            }
        }
    }
    //TODO:return result
}