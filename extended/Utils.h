#pragma once

#include <cstdio>

#include "../TupleMerge/TupleMergeOnline.h"

namespace Utils {

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
