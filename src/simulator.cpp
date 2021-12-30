#include<string>
#include<iostream>
#include<sstream>
#include<algorithm>

#include"../include/simulator.h"
#include"../include/analyzer.h"
using std::string;

bool InString(char ch, string str);
void display_v();
bool check_end(string state_now, std::vector<string> end_state);

void simulate(class TM &machine, string input)
{
    // std::cout << "start to simulate" << std::endl;
    CheckInputValid(machine, input);    
    SimulateRunning(machine, input);
}

void CheckInputValid(class TM &machine, string input)
{
    string validS;
    std::cout << "Input: " << input << std::endl;
    // std::cout << "valid chs is ";
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
        // std::cout << "ch and input is " << ch << "-----" << input << std::endl;
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
    
    // std::cout << "start simulate running" << std::endl;
    std::vector<string> final_states = machine.get_final();
    string state_now = machine.get_state();
    // std::cout << "get state is " << state_now << std::endl;
    int step = 0;
    int numOfTapes = 0;
    std::vector<int> head_pos;
    auto delta_vector = machine.get_delta();
    // std::cout << delta_vector[0].find("prev_symbols")->second << std::endl;
    numOfTapes = delta_vector[0].find("prev_symbols")->second.size();
    for(int i=0;i<numOfTapes;i++)
        head_pos.push_back(0);
    std::vector<string> tapes;
    string input_temp = input;
    input_temp.append("______________________________________");
    tapes.push_back(input_temp);
    for(int i=1;i<numOfTapes;i++)
        tapes.push_back("_________________________________________");

    // std::cout << "numof is " << numOfTapes << std::endl;

    // std::cout << "big while start." << std::endl;
    while(!check_end(state_now, final_states))
    {
        // std::cout << "enter while loop" << std::endl;
        static int stop = 0;
        if(stop >= 30)
            std::exit(0);
        stop++;
        //current state is not at Final state
        // std::cout << "enter verbose loop" << std::endl;
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
                std::cout << "i_Ch = " << i_ch << std::endl;

                std::cout << "Index" << i_tape << " : " << index_str << std::endl;

                string tape_str;
                for(int i_temp = i_ch;i_temp < len;i_temp++)
                {
                    // std::cout << std::to_string(tapes[i_tape][i_temp]) << " will be add to tape string" << std::endl;
                    tape_str.append(std::to_string(tapes[i_tape][i_temp]));
                    if(i_temp < 10)
                        tape_str.append(" ");
                    else
                        tape_str.append("  ");
                }
                std::cout << "Tape" << i_tape << "  : " << tape_str << std::endl;

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

        // std::cout << "finished verbose loop\n";
        for(auto delta : delta_vector)
        {
            // delta is ："<旧状态> <旧符号组> <新符号组> <⽅向组> <新状态>
            // name_list = {"prev_state", "prev_symbols", "new_symbols", "directions", "new_state"};
            // if(std::find(v.begin(), v.end(), x) != v.end()) {
            //     /* v contains x */
            // } else {
            //     /* v does not contain x */
            // }
            // std::cout << "for loop begins\n";
            int stop=0;
            if(stop >= 10)
                std::exit(0);
            stop++;
            // std::cout << "mark state now is " << state_now << std::endl;
            // std::exit(0);
            // std::cout << "mark find state and real state is " << delta.find("prev_state")->second << " and " << state_now << std::endl;
            bool check_state = (delta.find("prev_state")->second == state_now);
            if(!check_state)
                continue;
            bool check_symbol = true;
            string prev_symbols = delta.find("prev_symbols")->second;
            for(int i=0;i<numOfTapes;i++)
            {
                // std::cout << "mark read is " << tapes[i][head_pos[i]] << " and its int is " << int(tapes[i][head_pos[i]]) << " and prevsym is " << prev_symbols[i] << std::endl;
                if((tapes[i][head_pos[i]] == prev_symbols[i]) || prev_symbols[i] == '*' || tapes[i][head_pos[i]] == '*')
                {
                    check_symbol = check_symbol && true;
                    continue;
                }
                if((tapes[i][head_pos[i]] == 0 && prev_symbols[i] == '_') || (tapes[i][head_pos[i]] == '_' && prev_symbols[i] == 0))
                {
                    check_symbol = check_symbol && true;
                    continue;
                }
                else
                    check_symbol = check_symbol && false;
            }
            // std::cout << "mark check_state and check_symbol is "<< check_state << " " << check_symbol << std::endl;
            if(check_state && check_symbol)
            {
                // std::cout << "both check is true" << std::endl;
                string new_symbols = delta.find("new_symbols")->second;
                string directions = delta.find("directions")->second;
                for(int i=0;i<numOfTapes;i++)
                {
                    // std::cout << new_symbols[i] << " will in tape " << i << std::endl;
                    // std::cout << "dir is " << directions[i] << std::endl;
                    if(new_symbols[i] != '*')
                        tapes[i][head_pos[i]] = new_symbols[i];
                    if(directions[i] == 'r')
                        head_pos[i]++;
                    else if(directions[i] == 'l')
                        head_pos[i]--;
                    // std::cout << "tape " << i << " is " << tapes[i] << std::endl;
                    // std::cout << "ptr in tape " << i << " is " << tapes[i][head_pos[i]] << std::endl;
                }
                // std::cout << "mark state will set as " << delta.find("new_state")->second << std::endl;
                machine.set_state(delta.find("new_state")->second);
                break;
            }
        }
        state_now = machine.get_state();
    }
    //TODO:return result
    // just a raw edition, need to edit
    tapes[0].erase(std::remove(tapes[0].begin(), tapes[0].end(), '_'), tapes[0].end());
    std::cout << "Result: " << tapes[0] << std::endl;
    std::cout << "==================== END ====================" << std::endl;
}

bool check_end(string state_now, std::vector<string> end_list)
{
    for(auto item : end_list)
        if(item == state_now)
            return true;
    return false;
}