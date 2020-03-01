#ifndef FILESYSTEM_OFT_H
#define FILESYSTEM_OFT_H

#include <stdlib.h>

class OFT {
public:
	OFT();

	~OFT();

	void init();

	void init(int d, int l);

	int readChar(char *);

	int writeChar(char *);

	int seek(int index);

	char *getBuffer();

	int getCurrentPosition();

	int getDescriptor();

	int getLength();

	int getSlot();

	int getStatus();

	void flush();

private:

	const int SIZE = 64;

	int currentPosition;
	int descriptor;
	int length;
	char *buffer;
};

#endif //FILESYSTEM_OFT_H
