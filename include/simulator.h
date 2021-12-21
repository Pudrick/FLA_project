#pragma once

#include"./analyzer.h"

using std::string;

// class simulator
// {
//     public:
//         simulator(const TM& machine, string input);
//         //why must be const TM &?
//     private:
//         TM turing;
//         string input;
// };

void simulate(class TM &machine, string input);

void CheckInputValid(class TM &machine, string input);