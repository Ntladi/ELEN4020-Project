#ifndef PARSER_HPP_
#define PARSER_HPP_
#include <iostream>
#include <vector>
#include <omp.h>
#include <algorithm>
#include <map>
#include "pugixml.hpp"
using Data = std::vector<std::vector<std::string> >;
using Debates = std::vector<pugi::xml_node *>;
using SpeakerMap = std::map<std::string, int>;
using DebateMap = std::map<int, pugi::xml_node *>;

class Parser
{
public:
	Parser(const char *); // Loads the Hansard Data and Parses the debates and speakers
	void printDebates(); // Prints out the dates of all debates
	Data getData() const; // Returns a Vector containing a vector of speakers
	SpeakerMap getSpeakers() const; // Return a map containing the key value pairs of all unique speakers
	DebateMap getDebateMap() const; // Returns a map containing the key value pairs of all the debates
	std::string getDebateHeading (pugi::xml_node *);
private:
	pugi::xml_document hansardData; // Stores a reference to the Hansard data file
	pugi::xml_node debateParentNode; // The root node for all debates
	Data parsedData; // A vector of vector of speakers
	Debates debateNodes; // A vector of all debate pointers
	SpeakerMap speakers; // A map of tags used to identify speakers
	DebateMap debateMap; // A map to identify all the debates
private:
	void parseDebatesParallel(); // Traverse the Hansard to find all the debates
	void parseSpeakersParallel(); // Traverse the debates to find all the speakers
	void parseUniqueSpeakersParallel(); // Traverse the parsed data and find all unique speakers
	void populateDebates(pugi::xml_node &); // Populate the debateChildNodes vector with pointers to all the debates
	std::vector<std::string> organizeSpeakers(pugi::xml_node &); // Sort and store the speakers while removing all duplicates
	std::string charToString(const char* str) { return str; } // Convert from char array to string
	std::string getNodeValue(const char* str); // Extract std::string values from pugi::xml_node objects
};

#endif
