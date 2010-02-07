#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <map>
#include <string>

typedef std::map<std::string, std::string> stringmap;
typedef std::map<std::string, stringmap> map2d;

// Config["section"]["key"] = "value";
typedef map2d Config_t;
typedef stringmap NetworkConfig_t;

#define foreach(type, elem, itname) for (type::iterator itname = elem.begin(); itname != elem.end(); ++itname)

#ifndef WIN32
#define Sleep(ms) usleep(ms * 1000)
#endif

#include <time.h>
static int GetRandom(int min, int max)
{
	srand((rand() % (time(NULL) % 3167) - max * 20) + max * 10);
	return (rand() % (max - min + 1)) + min;
}

#define IRC_EOL "\r\n"
#define CORE_SLEEP 25

#endif
