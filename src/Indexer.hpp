#ifndef INDEXER_HPP_
#define INDEXER_HPP_
#include <fstream>
#include <bitset>
#include "Parser.hpp"

struct IndexedData {
	SpeakerMap speakers;
	DebateMap debates;
	std::string fileName;
};

class Indexer
{
public:
	Indexer(const char *); // Constructor takes in the name of the Hansard file as an argument
	~Indexer(); // Destructor frees memory taken up by the index array
	IndexedData exportData(const std::string &); // Export the bitmap to a file
	void printMap() const; // Print the bitmap to the console
private:
	Parser parser; // Parses debate and speaker data from the Hansard file
	Data data; // Stores the parsed data from the Hansard file
	SpeakerMap speakers; // Stores a map of unique speakers that are in the parsed data
	bool ** index; // Stores the indexed data
private:
	void initializeIndex(); // Populate the index array with zeros
	void populateIndexParallel(); // Index the parsed data in parallel
	void populateBitMapParallel(); // Populate the bitmap image with the indexed data
};

#endif
