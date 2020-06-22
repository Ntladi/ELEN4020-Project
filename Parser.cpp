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
	omp_lock_t writelock;
	omp_init_lock(&writelock);
	#pragma omp parallel
	{
		auto thread_id = omp_get_thread_num();
		auto num_threads = omp_get_num_threads();
		for(auto i = thread_id; i < debateChildNodes.size(); i+=num_threads)
		{
			pugi::xml_node speech = *debateChildNodes.at(i);
			auto allSpeakers = mapSpeakers(speech);

			omp_set_lock(&writelock);
			speakers.push_back(allSpeakers);
			speakerMap.push_back(i);
			omp_unset_lock(&writelock);
		}
	}
}

std::vector<std::string> Parser::mapSpeakers(pugi::xml_node & debate)
{
	std::vector<std::string> allSpeakers;
	auto children = debate.children("debateSection");

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
					debateChildNodes.push_back(&debate);
					break;
				}
			}
		}
		debate = debate.next_sibling("debateSection");
	}
}

Debates Parser::getDebates() const
{
	return debateChildNodes;
}

std::string Parser::getNodeValue(const char* str)
{
	std::string value = charToString(str);
	std::string result;

	for(auto letter: value)
		result += ::tolower(letter);

	return result;
}

Speakers Parser::getSpeakers() const
{
	return speakers;
}

void Parser::printDebates()
{
	for(auto j: debateChildNodes)
	{
		std::cout << j->child_value("heading") << std::endl;
	}
}

SpeakerKeys Parser::getKeys() const
{
	return speakerMap;
}
