#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include "../TupleMerge/TupleMergeOnline.h"
#include "../ElementaryClasses.h"

namespace Utils {

using namespace std;

template <typename T>
string as_binary(T val, size_t len = 32) {
    string s;
    for (size_t i = 0; i < len; ++i) {
        char bit = (val & 1) ? '1' : '0';
        s.push_back(bit);
        val >>= 1;
    }
    reverse(s.begin(), s.end());
    return s;
}

void dumpRules(const vector<Rule> &rules) {
    for (size_t i = 0; i < rules.size(); ++i) {
        printf("%d) ", rules[i].priority);
        rules[i].Print();
    }
}

void dumpClf(TupleMergeOnline clf) {
    size_t tables_cnt = 0;
    for (SlottedTable *table : clf.tables) {
        printf("Table %lu, ", ++tables_cnt);
        printf("priority: %d ", table->MaxPriority());
        printf("tuple: ");
        TupleMergeUtils::PrintTuple(table->dims);
        printf("Rules: ");
        for (const Rule &rule : table->GetRules()) {
            printf("%d ", rule.priority);
        }
        printf("\n");
    }
}

}  // namespace Utils
