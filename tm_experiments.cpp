// TODO: remove unused import & copyright?
#include "ElementaryClasses.h"
#include "PartitionSort/SortableRulesetPartitioner.h"
#include "IO/InputReader.h"
#include "IO/OutputWriter.h"
#include "Simulation.h"

#include "BruteForce.h"
#include "TupleMerge/TupleMergeOnline.h"
#include "TupleMerge/TupleMergeOffline.h"

#include "PartitionSort/PartitionSort.h"
#include <stdio.h>

#include "extended/RuleProcessing.h"

#include <assert.h>
#include <memory>
#include <chrono>
#include <string>
#include <sstream>

using namespace std;

/*
 * Parameters:
 * -f Filter File
 */
int main(int argc, char* argv[]) {
	unordered_map<string, string> args = ParseArgs(argc, argv);

	// e.g. ./filters_data/acl1.rules 
	string filterFile = GetOrElse(args, "f", "./acl1_seed_1.rules");

	bool doShuffle = GetBoolOrElse(args, "shuffle", true);

	vector<Rule> rules = InputReader::ReadFilterFile(filterFile);
	// rules = Random::shuffle_vector(rules);

	rules = RuleProcessing::expandStars(rules, 1, {FieldSA, FieldDA}, false);

	size_t rulesBefore = rules.size();
	rules = RuleProcessing::filterOverridden(rules);
	printf("Filter: %lu -> %lu rules\n", rulesBefore, rules.size());

	args["TM.Limit.Collide"] = "10";  // Max number of collisions per table, ~ `c` from the paper

	TupleMergeOnline tmOnline = TupleMergeOnline(args);
	tmOnline.ConstructClassifier(rules);
	cout << tmOnline.NumTables() << " tables\n";

	printf("Done\n");
	return 0;
}


