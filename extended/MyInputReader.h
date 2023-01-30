#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

#include "../ElementaryClasses.h"


namespace MyInputReader {

using namespace std;


void parseTernaryField(string token, Rule &targetRule, size_t targetIdx) {
    size_t pos = token.find('*');
    if (pos == token.npos) {
        pos = token.size();
    }
    unsigned prefix_val = pos == 0 ? 0 : stoi(token.substr(0, pos), 0, 2);
    unsigned lower = prefix_val << (token.size() - pos);
    unsigned upper = lower + (1U << (token.size() - pos)) - 1;

    targetRule.prefix_length[targetIdx] = pos;
    targetRule.range[targetIdx] = {lower, upper};
}


// Parse binary format of tuples with stars (e.g. 10010***)
vector<Rule> parseBinaryFilters(const string &filename) {
    ifstream in = ifstream(filename);
    string line;

    vector<Rule> rules;
    while (getline(in, line) && !line.empty()) {
        istringstream line_stream(line);
        vector<string> tokens{istream_iterator<string>(line_stream),
                              istream_iterator<string>()};
        Rule rule{static_cast<int>(tokens.size())};

        for (size_t i = 0; i < tokens.size(); ++i) {
            parseTernaryField(tokens[i], rule, i);
        }
        
        rules.push_back(rule);
    }

    for (size_t i = 0; i < rules.size(); ++i) {
        rules[i].priority = i;

        printf("%lu) ", i);
        rules[i].Print();
    }

    return rules;
}

}  // namespace MyInputReader