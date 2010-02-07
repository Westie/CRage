#include "StdInc.h"
#include "CConfigParser.h"
#include "CControl.h"

#include <map>
#include <fstream>

#ifndef WIN32
#include <dirent.h>
#endif

CConfigParser::CConfigParser()
{
}

CConfigParser::~CConfigParser()
{
}

void CConfigParser::parseDirectory()
{
#ifdef WIN32
	HANDLE fHandle;
	WIN32_FIND_DATA wfd;
	fHandle = FindFirstFile("./Configuration/*.ini", &wfd);

	do
	{
		if (!((wfd.cFileName[0] == '.') && ((wfd.cFileName[1] == '.' && wfd.cFileName[2] == 0) || wfd.cFileName[1] == 0)))
		{
			if (!(wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				parseConfigFile(wfd.cFileName);
			}
		}
	}
	while (FindNextFile(fHandle,&wfd));
	FindClose(fHandle);
#else
	struct dirent *pDirEntry;
	DIR *pHandle = opendir("./Configuration/");
	if (pHandle != NULL)
	{
		while ((pDirEntry = readdir(pHandle)) != NULL)
		{
			struct stat s;
			int ret = stat(("./Configuration/" + pDirEntry->d_name, &s);
			if (ret != 0 && !S_ISDIR(s.st_mode))
			{
				size_t namelen = strlen(pDirEntry->d_name);
				const char *short = pDirEntry->d_name;
				if (short[namelen - 4] == '.' && short[namelen - 3] == 'i' && short[namelen - 2] == 'n' && short[namelen - 1] == 'i' && short[namelen] == '\0')
				{
					parseConfigFile(short);
				}
			}
		}
		closedir(pHandle);
	}
#endif
}

void CConfigParser::parseConfig(string strConfig)
{

}

void CConfigParser::parseConfigFile(string strConfig)
{
	string strPath = "./Configuration/" + strConfig;

	if (strConfig[0] == '~')
		return;
	

	// parse it into mapConfig
	ifstream stream(strPath.c_str(), std::ios::in);
	if (stream.fail())
	{
		// Error handling
		return;
	}
	
	NetworkConfig_t *pNetworkConfig = new NetworkConfig_t;
	Config_t *pConfig = new Config_t;
	char szBuffer[512];
	string strLine;
	string strCurrentSection;
	while (!stream.eof())
	{
		stream.getline(szBuffer, sizeof(szBuffer));
		if (szBuffer[0] != '\0' && szBuffer[0] != ';')
		{
			strLine = trimString(szBuffer);
			if (strLine[0] == '[' && *(strLine.end() - 1) == ']')
			{
				strCurrentSection = strLine.substr(1);
				*(strCurrentSection.end() - 1) = '\0';

				if (strCurrentSection[0] != '~')
					(*pConfig)[string(strCurrentSection)]["nickname"] = strCurrentSection;
			}
			else
			{
				string::size_type eqpos = strLine.find('=');
				if (eqpos != string::npos)
				{
					string strKey = trimString(strLine.substr(0, eqpos));
					string strValue = trimString(strLine.substr(eqpos + 1));

					if (strValue[0] == '"' && *(strValue.end() - 1) == '"')
					{
						strValue = strValue.substr(1);
						*(strValue.end() - 1) = '\0';
					}

					if (strCurrentSection[0] != '~')
					{
						(*pConfig)[string(strCurrentSection)][string(strKey)] = strValue;
					}
					else
					{
						(*pNetworkConfig)[string(strKey)] = strValue;
					}
				}
			}
		}
	}

	stream.close();

	if (pConfig->size() < 1)
	{
		delete pNetworkConfig;
		delete pConfig;
		return;
	}

	/*
	// iterate the sections
	foreach (Config_t, mapConfig, i)
	{
		string strSection = i->first;
		stringmap mapSection = i->second;

		// iterate the items in this section
		foreach (stringmap, mapSection, j)
		{
			string strKey = j->first;
			string strValue = j->second;
			// mapConfig[strSection][strKey] == strValue;
		}
	}
	*/

	std::string strName = strConfig;
	*(strName.end() - 4) = '\0'; // remove ".ini"

	// add a new master for this config
	CControl::m_mapBots[string(strName)] = new CMaster(strName, pNetworkConfig, pConfig);
}