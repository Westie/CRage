#ifndef _STRINGUTILS_H
#define _STRINGUTILS_H

int toNumber(std::string &str);
long toLongNumber(std::string &str);

std::string toString(int num, int radix = 10);

std::string trimString(const char *str);
std::string trimString(std::string str);

#endif
