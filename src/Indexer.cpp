#include "Indexer.hpp"

Indexer::Indexer(const char *hansardFile):
		parser(hansardFile), data(parser.getData()), speakers(parser.getSpeakers())
{
	initializeIndex();
	populateIndexParallel();
}

Indexer::~Indexer()
{
	for (auto i = 0; i < speakers.size(); i++)
		delete index[i];

	delete index;
}

void Indexer::initializeIndex()
{
	index = new bool*[speakers.size()];
	for (auto i = 0; i < speakers.size(); i++)
		index[i] = new bool[data.size()];

	#pragma omp parallel for collapse(2) schedule(static)
	for(auto i = 0; i < speakers.size(); i++)
		for(auto j = 0; j < data.size(); j++)
			index[i][j] = false;
}

void Indexer::populateIndexParallel()
{
	#pragma omp parallel for schedule(dynamic)
	for(auto i = 0; i < data.size(); i++)
	{
		auto debate = data.at(i);
		for(auto j = 0; j < debate.size(); j++)
		{
			auto speakerTag = debate.at(j);
			auto rowIndex = speakers[speakerTag];
			index[rowIndex][i] = true;
		}
	}
}

IndexedData Indexer::exportData(const std::string & fileName)
{
	std::ofstream output("output.bin", std::ios::binary);
	for(auto i = 0; i < speakers.size(); i++)
	{
		bool buffer[data.size()];
		#pragma omp parallel for schedule(static)
		for(auto j = 0; j< data.size(); j++)
			buffer[j] = index[i][j];

		output.write((char*) &buffer, sizeof(buffer));
	}
	output.close();

	IndexedData indexedData;
	indexedData.debates = parser.getDebates();
	indexedData.speakers = parser.getSpeakers();
	indexedData.fileName = fileName;

	return indexedData;
}

void Indexer::printMap() const
{
	std::cout << "Speakers: " << speakers.size() << std::endl;
	std::cout << "Debates: " << data.size() << std::endl;
	for(auto i = 0; i< speakers.size(); i++)
	{
		for(auto j = 0; j < data.size(); j++)
			std::cout << index[i][j];

		std::cout << std::endl;
	}
}

