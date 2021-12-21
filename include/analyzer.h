#pragma once

#include<string>
#include<vector>
#include<iostream>
#include<fstream>
#include<map>

using std::string;

class TM
{
    public:
        TM(string tm_name, bool v_mode);
        void set_verbose(bool value);
        string get_tm();
        bool get_verbose();
        std::vector<char> getS();

    private:
        string tm;
        std::ifstream tm_file;
        std::vector<string> Q;
        std::vector<char> S;
        std::vector<char> G;
        string q0;
        char B = '_';
        std::vector<string> F;
        int N;

        // map is a better tool
        // struct trans_func{
        //     string prev_state;
        //     string new_state;
        //     string prev_symbols;
        //     string new_symbols;
        //     string directions;
        // }delta;

        std::map<string, string> delta;

        void tmFileAna(std::ifstream &filestream);// Analyze the imput
        // std::ifstream as arguments need to be &.
        bool VERBOSE_MODE = false;
};