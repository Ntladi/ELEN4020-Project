#include "SearchIndex.hpp"

SearchIndex::SearchIndex(const IndexedData &data) :
		speakers(data.speakers), debates(data.debates)
{
	initializeIndex();
	loadData(data.fileName);
}

SearchIndex::~SearchIndex()
{
	for (auto i = 0; i < speakers.size(); i++)
		delete index[i];

	delete index;
}

void SearchIndex::initializeIndex()
{
	index = new bool*[speakers.size()];
	for (auto i = 0; i < speakers.size(); i++)
		index[i] = new bool[debates.size()];

	#pragma omp parallel for collapse(2) schedule(static)
	for (auto i = 0; i < speakers.size(); i++)
		for (auto j = 0; j < debates.size(); j++)
			index[i][j] = 0;
}

void SearchIndex::printMap() const
{
	std::cout << "Speakers: " << speakers.size() << std::endl;
	std::cout << "Debates: " << debates.size() << std::endl;
	std::cout << "Bitmap: " << std::endl;
	for(auto i = 0; i< speakers.size(); i++)
	{
		for(auto j = 0; j < debates.size(); j++)
			std::cout << index[i][j];

		std::cout << std::endl;
	}
}

void SearchIndex::loadData(const std::string & fileName)
{
	std::ifstream input(fileName, std::ios::binary);


	for(int i = 0; i < speakers.size(); i++)
	{
		bool buffer[debates.size()];
		input.read((char *) &buffer, sizeof(buffer));

		#pragma omp parallel for schedule(static)
		for(auto j = 0; j < debates.size(); j++)
			index[i][j] = buffer[j];

	}
}

void SearchIndex::printSpeakers() const
{
	std::cout << "Speaker Key-Value pairs: " << std::endl;
	for(auto speaker: speakers)
		std::cout << speaker.first << " = row " << speaker.second << std::endl;

}

void SearchIndex::printDebates() const
{
	std::cout << "Debate Key-Value pairs: " << std::endl;
	for(auto debate: debates)
		std::cout << "col " << debate.first << " = " << debate.second << std::endl;
}

void SearchIndex::findDebates(const std::string & speaker1Key, const std::string & speaker2Key)
{
	std::cout << "Debates where '" << speaker1Key << "' and '" << speaker2Key << "' appear:" << std::endl;
	auto speaker1Index = speakers[speaker1Key];
	auto speaker2Index = speakers[speaker2Key];

	#pragma omp parallel for
	for(auto i = 0; i < debates.size(); i++)
	{
		if(index[speaker1Index][i] & index[speaker2Index][i])
		{
			#pragma omp critical(cout)
			std::cout << debates[i] << " col = "<< i << std::endl;
		}

	}

}

void SearchIndex::findSpeakers(const int & debate1Index, const int & debate2Index)
{
	std::cout <<"Row indeces of the speakers that appered in debates: " << debates[debate1Index] << std::endl
		<< "and " << debates[debate2Index] << std::endl;
	#pragma omp parallel for schedule(static)
	for(auto i = 0; i < speakers.size(); i++)
		if(index[i][debate1Index] & index[i][debate2Index])
			#pragma omp critical(cout)
			std::cout << i << std::endl;
}
