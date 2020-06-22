#ifndef INDEXER_HPP_
#define INDEXER_HPP_
#include "Parser.hpp"
#include "bitmap_image.hpp"

class Indexer
{
public:
	Indexer(const char *);
	~Indexer();
	void printMap() const; // Print the bitmap to the console
	int ** getMap() const; // Return the bitmap array
	void exportBitmap(const std::string &); // Export the bitmap to a file
private:
	Parser parser; // Parses debate and speaker data from the Hansard file
	Data data; // Stores the parsed data from the Hansard file
	SpeakerMap speakers; // Stores a map of unique speakers that are in the parsed data
	int ** index; // Stores the indexed data
	bitmap_image image; // Exports the indexed data to a .bmp file
private:
	void populateZeros(); // Populate the index array with zeros
	void populateIndexParallel(); // Index the parsed data in parallel
	void populateBitMapParallel(); // Populate the bitmap image with the indexed data
};

#endif
