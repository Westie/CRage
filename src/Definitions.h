#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include <map>
#include <string>

typedef std::map<std::string, std::string> stringmap;
typedef std::map<std::string, stringmap> map2d;

// Config["section"]["key"] = "value";
typedef map2d Config;

#endif
