#include "SearchIndex.hpp"

int main() {
	Indexer indexer("SenateHansard1979vol2.xml");

	auto data = indexer.exportData("output.bin");
	SearchIndex searcher(data);
	searcher.printMap();
	std::cout << std::endl;
	searcher.printSpeakers();
	std::cout << std::endl;
	searcher.printDebates();
	std::cout << std::endl;
	searcher.findDebates("#acting-chairman", "#acting-president");
	std::cout << std::endl;
	searcher.findSpeakers(0, 1);
}
