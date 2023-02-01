#pragma once

#include <cstdio>
#include <stdexcept>
#include <vector>

#include "../ElementaryClasses.h"
#include "Utils.h"
#include "LogUtils.h"


namespace RuleProcessing {

using namespace std;

vector<Rule> expandField(const Rule &rule, size_t dim, unsigned expand_bits,
                         bool verbose = false) {
    print_verbose(verbose, "\nRule %d: ", rule.priority);
    if (verbose) rule.Print();

    vector<Rule> result;
    expand_bits = min(expand_bits, 32U - rule.prefix_length[dim]);
    unsigned new_prefix_len = rule.prefix_length[dim] + expand_bits;
    unsigned new_suffix_len = 32U - new_prefix_len;

    print_verbose(verbose, "dim: %lu, Expanding %u bits, prefix len %d -> %d\n",
            dim, expand_bits, rule.prefix_length[dim], new_prefix_len);

    // Non-starred prefix part = <some value in range> & <mask with prefix_length bits in the beginning>
    unsigned mask = ~((1U << (32U - rule.prefix_length[dim])) - 1U);
    unsigned prefix = (rule.range[dim][LowDim] & mask);
    // We need only the extracted prefix
    prefix = prefix >> (32U - rule.prefix_length[dim]);

    print_verbose(verbose, "General prefix: %s\n", Utils::as_binary(prefix, rule.prefix_length[dim]).c_str());

    unsigned rule_l = rule.range[dim][LowDim];
    unsigned rule_r = rule.range[dim][HighDim];

    for (size_t i = 0; i < (1U << expand_bits); ++i) {
        unsigned chunk_prefix = ((prefix << expand_bits) | i);
        unsigned chunk_l = (chunk_prefix << new_suffix_len);
        unsigned chunk_r = chunk_l + (1U << new_suffix_len) - 1U;

        print_verbose(verbose, "    Chunk %lu, prefix: %s, (%u %u)", i, Utils::as_binary(chunk_prefix).c_str(), chunk_l, chunk_r);

        // TODO: this case is trivial for ternary rules - test it before using with more complex data
        if (chunk_l > rule_r || chunk_r < rule_l) {  // whether the chunk has empty intersection with original range 
            print_verbose(verbose, "- dropped");
        } else {
            Rule chunk(rule);
            chunk.range[dim][LowDim] = chunk_l;
            chunk.range[dim][HighDim] = chunk_r;
            result.push_back(chunk);
        }

        print_verbose(verbose, "\n");
    }
    return result;
}

vector<Rule> expandStars(vector<Rule> rules, unsigned expand_bits,
                         vector<size_t> expand_dims = {FieldSA, FieldDA},
                         bool verbose = false) {
    sort(rules.begin(), rules.end(),
         [](const Rule &a, const Rule &b) { return a.priority > b.priority; });

    vector<Rule> result;

    
    for (const Rule &rule: rules) {
        vector<Rule> expanded_rules;
        for (size_t dim : expand_dims) {
            vector<Rule> expanded = expandField(rule, dim, expand_bits, verbose);
            result.insert(result.end(), expanded.begin(), expanded.end());
        }
    }

    for (size_t i = 0; i < result.size(); ++i) {
        result[i].priority = result.size() - i;
    }

    return result;
}


vector<Rule> filterOverridden(vector<Rule> rules, bool verbose = false) {
    sort(rules.begin(), rules.end(),
         [](const Rule &a, const Rule &b) { return a.priority > b.priority; });

    vector<Rule> results;
    for (const Rule &candidate : rules) {
        size_t masterRuleId = -1;
        for (size_t i = 0; masterRuleId == -1 && i < results.size(); ++i) {
            const Rule &included = results[i];
            if (included.OverridesRule(candidate)) {
                masterRuleId = i;
            }
        }
        if (masterRuleId == -1) {
            results.push_back(candidate);
            if (verbose) {
                printf("Added: ");
                candidate.Print(false);
            }
        } else {
            if (verbose) {
                printf("Filtered: ");
                candidate.Print(false, false);
                printf("(already have ");
                results[masterRuleId].Print(false, false);
                printf(")\n");
            }
        }
    }
    return results;
}

}  // namespace RuleProcessing
