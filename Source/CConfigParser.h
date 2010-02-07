class CConfigParser;

#ifndef _CCONFIGPARSER_H
#define _CCONFIGPARSER_H

class CConfigParser
{
public:
	CConfigParser();
	~CConfigParser();

	void parseDirectory();
	void parseConfig(string strConfig);
	void parseConfigFile(string strConfig);

private:

};

#endif
