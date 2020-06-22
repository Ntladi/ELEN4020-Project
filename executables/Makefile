all: project

project: ../src/main.cpp ../src/Indexer.cpp ../src/Parser.cpp ../src/SearchIndex.cpp
	g++ ../src/main.cpp ../src/Indexer.cpp ../src/Parser.cpp ../src/SearchIndex.cpp --std=c++17 -lgomp -fopenmp -o project

clean:
	rm project
