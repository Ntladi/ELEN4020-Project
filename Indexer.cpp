#include "Indexer.hpp"

Indexer::Indexer(const char *hansardFile):
		parser(hansardFile), data(parser.getData()), speakers(parser.getSpeakers()), image(data.size(), speakers.size())
{
	index = new int*[speakers.size()];

	for (auto i = 0; i < speakers.size(); i++)
		index[i] = new int[data.size()];

	populateZeros();
	populateIndexParallel();
}

Indexer::~Indexer()
{
	for (auto i = 0; i < speakers.size(); i++)
		delete index[i];

	delete index;
}

void Indexer::populateZeros()
{
	#pragma omp parallel for collapse(2) schedule(static)
	for(auto i = 0; i < speakers.size(); i++)
		for(auto j = 0; j < data.size(); j++)
			index[i][j] = 0;
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
			index[rowIndex][i] = 1;
		}
	}
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

int ** Indexer::getMap() const
{
	return index;
}

void Indexer::exportBitmap(const std::string & fileName)
{
	bitmap_image image(21, 80);
	image.set_all_channels(255, 255, 255);

	#pragma omp parallel for schedule(static)
	for(auto i = 0; i < 21; i++)
		for(auto j = 0; j < 80; j++)
			if (index[j][i] == 1)
				image.set_pixel(i, j, 0,0,0);

	image.save_image(fileName);
}
