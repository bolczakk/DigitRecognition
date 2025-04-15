#ifndef CONSOLE_H
#define CONSOLE_H

#include <Windows.h>
#include <iostream>

class Console {
public:
	const static void printProgressBar(int current, size_t total);
	const static void hideCursor();
	const static void showCursor();
};

#endif
