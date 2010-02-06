#include "StdInc.h"
#include "CConfigParser.h"
#include <map>

#ifndef WIN32
#include <dirent.h>
#endif

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
	
	Config mapConfig;
	// parse it into mapConfig

	if (mapConfig.size() <= 1)
	{
		// Error handling
		return;
	}

	// iterate the sections
	for (Config::iterator i = mapConfig.begin(); i != mapConfig.end(); ++i)
	{
		string strSection = i->first;
		stringmap mapSection = i->second;

		// iterate the items in this section
		for (stringmap::iterator i = mapSection.begin(); i != mapSection.end(); ++i)
		{
			string strKey = i->first;
			string strValue = i->second;
			// mapConfig[strSection][strKey] == strValue;
		}
	}

	// add a new master for this config
}