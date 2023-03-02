#pragma once
#include <string>
#include <stdint.h>
class ITape
{
public:
	virtual void set_files(std::string in, std::string out) = 0;
	virtual void sort_tape() = 0;
};

