#include "Parser.hpp"

Parser::Parser(const char *hansardFile)
{
	if (!hansardData.load_file(hansardFile))
	{
		std::cerr << "Could not open file" << std::endl;
		throw;
	}

	debateParentNode = hansardData.child("akomaNtoso").child("debate")
			.child("debateBody");

	parseDebatesParallel();
	parseSpeakersParallel();
	parseUniqueSpeakersParallel();

}

void Parser::parseDebatesParallel()
{
	#pragma omp parallel
	{
		#pragma omp single
		{
			pugi::xml_node debate = debateParentNode.first_child();
			populateDebates(debate);
		}
	}
}

void Parser::parseSpeakersParallel()
{
	for(auto i = 0; i < debateNodes.size(); i++)
	{
		pugi::xml_node * speech = debateNodes.at(i);
		auto allSpeakers = organizeSpeakers(speech, i);

		parsedData.push_back(allSpeakers);
		debateMap.insert(std::pair<int, std::string> (i, getNodeValue(speech->child_value("heading"))));

	}
}

void Parser::parseUniqueSpeakersParallel()
{
	std::vector<std::string> allSpeakers;

	#pragma omp parallel for schedule(dynamic)
	for(auto i = 0; i < parsedData.size(); i++)
		for(auto j = 0; j < parsedData.at(i).size(); j++)
		{
			#pragma omp critical
			allSpeakers.push_back(parsedData.at(i).at(j));
		}
	sort(allSpeakers.begin(), allSpeakers.end());
	allSpeakers.erase(unique(allSpeakers.begin(), allSpeakers.end()), allSpeakers.end());

	#pragma omp parallel for schedule(static)
	for(auto i = 0; i < allSpeakers.size(); i++)
	{
		#pragma omp critical
		speakers.insert(std::pair<std::string, int>(allSpeakers.at(i), i));
	}
}

std::vector<std::string> Parser::organizeSpeakers(pugi::xml_node * debate, const int & i)
{
	std::vector<std::string> allSpeakers;
	auto children = debate->children("debateSection");

	for(auto child: children)
	{
		if(child.child("speech"))
		{
			auto speech = child.child("speech");
			while(speech)
			{
				std::string tag = getNodeValue(speech.attribute("by").value());
				allSpeakers.push_back(tag);
				speech = speech.next_sibling("speech");
			}
		}
	}

	sort(allSpeakers.begin(), allSpeakers.end());
	allSpeakers.erase(unique(allSpeakers.begin(), allSpeakers.end()), allSpeakers.end());

	return allSpeakers;


}

void Parser::populateDebates(pugi::xml_node & debate)
{
	while(debate)
	{
		#pragma omp task
		{
			auto children = debate.children("debateSection");

			for(auto child: children)
			{
				if(child.child("speech"))
				{
					#pragma omp critical
					debateNodes.push_back(&debate);
					break;
				}
			}
		}
		debate = debate.next_sibling("debateSection");
	}
}

std::string Parser::getNodeValue(const char* str)
{
	std::string value = charToString(str);
	std::string result;

	for(auto letter: value)
		result += ::tolower(letter);

	return result;
}

Data Parser::getData() const
{
	return parsedData;
}

SpeakerMap Parser::getSpeakers() const
{
	return speakers;
}

DebateMap Parser::getDebates() const
{
	return debateMap;
}
