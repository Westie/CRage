#include "StdInc.h"
#include "StringUtils.h"

int toNumber(string &str)
{
	return atoi(str.c_str());
}

long toLongNumber(string &str)
{
	return atol(str.c_str());
}

string toString(int num, int radix)
{
	char tmp[12];
	itoa(num, tmp, radix);
	return string(tmp);
}

string trimString(const char *str)
{
	string ret(str);

	string::size_type OffStart = 0;
	foreach (string, ret, i)
	{
		bool bBreak = false;
		switch (*i)
		{
		case ' ':
		case '\t':
			OffStart++;
			break;
		default:
			bBreak = true;
			break;
		}
		if (bBreak)
			break;
	}

	string::size_type OffEnd = 0;
	for (string::reverse_iterator i = ret.rbegin(); i != ret.rend(); ++i)
	{
		if (*i == '\0')
			continue;

		bool bBreak = false;
		switch (*i)
		{
		case ' ':
		case '\n':
		case '\r':
		case '\t':
			OffEnd++;
			break;
		default:
			bBreak = true;
			break;
		}
		if (bBreak)
			break;
	}

	ret = ret.substr(OffStart, ret.length() - OffEnd - OffStart);
	return ret;
}

string trimString(string &str)
{
	string ret(str);

	string::size_type OffStart = 0;
	foreach (string, ret, i)
	{
		bool bBreak = false;
		switch (*i)
		{
		case ' ':
		case '\t':
			OffStart++;
			break;
		default:
			bBreak = true;
			break;
		}
		if (bBreak)
			break;
	}

	string::size_type OffEnd = 0;
	for (string::reverse_iterator i = ret.rbegin(); i != ret.rend(); ++i)
	{
		if (*i == '\0')
			continue;

		bool bBreak = false;
		switch (*i)
		{
		case ' ':
		case '\n':
		case '\r':
		case '\t':
			OffEnd++;
			break;
		default:
			bBreak = true;
			break;
		}
		if (bBreak)
			break;
	}

	ret = ret.substr(OffStart, ret.length() - OffEnd - OffStart);
	return ret;
}


