#include "OFT.h"

OFT::OFT() {
	buffer = new char[SIZE];
	currentPosition = -1;
	descriptor = -1;
	length = -1;
}

OFT::~OFT() {
	// wow beautiful. Is in necessary?
	delete this;
}

void OFT::init() {
	currentPosition = -1;
	descriptor = -1;
	length = -1;
	flush();
}

void OFT::init(int d, int l) {
	flush();
	currentPosition = 0;
	descriptor = d;
	length = l;
}

int OFT::readChar(char *c) {
	*c = buffer[currentPosition % 64];
	currentPosition++;
	return getStatus();
}

int OFT::writeChar(char *c) {
	buffer[currentPosition % 64] = *c;
	currentPosition++;
	if (currentPosition == length + 1)
		length++;
	return getStatus();
}

int OFT::seek(int index) {
	if (index <= length && length >= 0) {
		currentPosition = index;
		return 0;
	}
	return -1;
}

char *OFT::getBuffer() {
	return buffer;
}

int OFT::getCurrentPosition() {
	return currentPosition;
}

int OFT::getDescriptor() {
	return descriptor;
}

int OFT::getLength() {
	return length;
}

int OFT::getSlot() {
	int slot = (currentPosition / 64) + 1;
	return slot;
}

int OFT::getStatus() {
	if (currentPosition == 192)
		return 4;
	else if (currentPosition == 128 && currentPosition == length)
		return 3;
	else if (currentPosition == 64 && currentPosition == length)
		return 2;
	else if (currentPosition == 0 && currentPosition == length)
		return 1;
	else if (currentPosition == 128 && currentPosition < length)
		return -3;
	else if (currentPosition == 64 && currentPosition < length)
		return -2;
	else if (currentPosition == 0 && currentPosition < length)
		return -1;
	else
		return 0;
}

void OFT::flush() {
	for (int i = 0; i < SIZE; i++)
		buffer[i] = '\0';
}
