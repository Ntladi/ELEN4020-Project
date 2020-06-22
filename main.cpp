#include "Parser.hpp"

int main() {
	Parser data("SenateHansard1979vol2.xml");
	data.printDebates();

	auto speakers = data.getSpeakers();

	for(auto debate: speakers)
	{
		for(auto speaker: debate)
		{
			std::cout << speaker << std::endl;
		}
		std::cout << std::endl;
	}

}
