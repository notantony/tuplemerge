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

	args["TM.Limit.Collide"] = "10";  // Max number of collisions per table, ~ `c` from the paper
	TupleMergeOnline tmOnline = TupleMergeOnline(args);
	tmOnline.ConstructClassifier(rules);
	cout << tmOnline.NumTables() << " tables\n";

	printf("Done\n");
	return 0;
}


