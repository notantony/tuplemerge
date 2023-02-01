#include "ElementaryClasses.h"
#include "PartitionSort/SortableRulesetPartitioner.h"
#include "IO/InputReader.h"
#include "IO/OutputWriter.h"

#include "extended/MyInputReader.h"
#include "extended/Utils.h"
#include "extended/RuleProcessing.h"

#include "BruteForce.h"
#include "TupleMerge/TupleMergeOnline.h"
#include "TupleMerge/TupleMergeOffline.h"

#include "PartitionSort/PartitionSort.h"
#include <stdio.h>

#include <assert.h>
#include <memory>
#include <chrono>
#include <string>
#include <sstream>

using namespace std;


/*
 * Parameters:
 * <input_file>
 */
int main(int argc, char* argv[]) {
	unordered_map<string, string> args = ParseArgs(argc, argv);

	// e.g. ./filters_data/acl1.rules
	string filterFile = GetOrElse(args, "f", "./custom_data/example1.txt");

	vector<Rule> rules = MyInputReader::parseBinaryFilters(filterFile);
	Utils::dumpRules(rules);
	rules = RuleProcessing::expandStars(rules, 2, {FieldDA});
	Utils::dumpRules(rules);
	rules = RuleProcessing::filterOverridden(rules, true);
	Utils::dumpRules(rules);
	printf("%lu\n\n", rules.size());

	// rules = Random::shuffle_vector(rules);

	args["TM.Limit.Collide"] = "10";  // Max number of collisions per table, ~ `c` from the paper
	TupleMergeOnline tmOnline = TupleMergeOnline(args);
	tmOnline.ConstructClassifier(rules);
	printf("%lu tables:\n", tmOnline.NumTables());

	Utils::dumpClf(tmOnline);

	printf("Done\n");
	return 0;
}


