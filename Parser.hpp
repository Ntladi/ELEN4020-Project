#ifndef PARSER_HPP_
#define PARSER_HPP_
#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <map>
#include "pugixml.hpp"
using Debates = std::vector<pugi::xml_node *>;
using Speakers = std::vector<std::vector<std::string> >;
using SpeakerKeys = std::vector<int>;

class Parser
{
public:
	Parser(const char *); // Loads the Hansard Data and Parses the debates and speakers
	void printDebates(); // Prints out the dates of all debates
	Speakers getSpeakers() const; // Returns a Vector containing a vector of speakers
	Debates getDebates() const; // Returns a vector of pointers to the debates
	SpeakerKeys getKeys() const; // Return a vector of ints to map the speakers to their respective debates
private:
	pugi::xml_document hansardData; // Stores a reference to the Hansard data file
	pugi::xml_node debateParentNode; // The root node for all debates
	Debates debateChildNodes; // The vector of all debate pointers
	Speakers speakers; // The vector of vector of speakers
	SpeakerKeys speakerMap; // The keys for mapping speakers to debates
private:
	void parseSpeakersParallel(); // Traverse the debates to find all the speakers
	void populateDebates(pugi::xml_node &); // Populate the debateChildNodes vector with pointers to all the debates
	void parseDebatesParallel(); // Traverse the Hansard to find all the debates
	std::vector<std::string> mapSpeakers(pugi::xml_node &); // Sort and store the speakers while removing all duplicates
	std::string charToString(const char* str) { return str; } // Convert from
	std::string getNodeValue(const char* str); // Extract std::string values from pugi::xml_node objects
};

#endif
