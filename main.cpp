#include "Indexer.hpp"
#include "bitmap_image.hpp"

int main() {
	Indexer indexer("SenateHansard1979vol2.xml");
	indexer.printMap();
	indexer.exportBitmap("output.bmp");
}
