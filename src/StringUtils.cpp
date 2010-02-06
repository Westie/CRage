#include "StdInc.h"
#include "StringUtils.h"

int toNumber(std::string &str)
{
	return atoi(str.c_str());
}

long toLongNumber(std::string &str)
{
	return atol(str.c_str());
}