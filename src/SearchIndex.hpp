#ifndef SEARCHINDEX_HPP_
#define SEARCHINDEX_HPP_
#include "Indexer.hpp"

class SearchIndex
{
public:
	SearchIndex(const IndexedData &);
	~SearchIndex(); // Destructor frees memory taken up by the index array
	void printMap() const; // Print the bitmap to the console
	void printSpeakers() const; // Print a list of available speakers and their row indices on the bitmap
	void printDebates() const; // Print a list of the debates (identified by header) and their column indices on the bitmap
	void findDebates(const std::string &, const std::string &); // Find the debates that two speakers participated in
	void findSpeakers(const int &, const int &); // Find all speakers that participated in two given debates

private:
	std::ifstream input; // Ifstream object pointing to a
	SpeakerMap speakers; // A map of tags used to identify speakers
	DebateMap debates; // A map to identify all the debates
	bool ** index; // Stores the indexed data
private:
	void initializeIndex(); // Populate the index array with zeros
	void loadData(const std::string &); // Load the data from the bitmap image into memory
};

#endif
