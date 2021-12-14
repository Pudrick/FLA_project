#pragma once

#include<string>
#include<vector>
#include<iostream>
#include<fstream>

using std::string;
class analyzer
{
    public:
    analyzer(string tm_name, bool v_mode);
    void set_verbose(bool value);

    private:
    string tm;
    std::ifstream tm_file;
    std::vector<string> Q;
    std::vector<string> S;
    std::vector<string> G;
    string q0;
    const char boank = '_';
    std::vector<string> F;
    int N;
    struct trans_func{
        string prev_state;
        string new_state;
        string prev_symbols;
        string new_symbols;
        string directions;
    }delta;
    void tmFileAna(std::ifstream &filestream);// Analyze the imput
    // std::ifstream as arguments need to be &.
    bool VERBOSE_MODE = false;
};