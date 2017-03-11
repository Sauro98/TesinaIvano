#include <iostream>
#include "deflate_encoder.h"

int main(int argc, char** argv) {
	Deflate_encoder encoder = Deflate_encoder("Deflate late");
	encoder.encode();
	system("Pause");
}
