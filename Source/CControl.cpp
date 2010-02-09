#include "StdInc.h"
#include "CControl.h"

map<string, CMaster *> CControl::m_mapBots;
CConfigParser *CControl::m_pConfigParser;

void CControl::botCreate(string strConfig)
{
}

bool CControl::botRemove(string strConfig)
{
	strConfig = strConfig.c_str();
	if (m_mapBots[strConfig] != NULL)
	{
		m_mapBots[strConfig]->_onDestruct();
		m_mapBots[strConfig] = NULL;
		return true;
	}
	return false;
}

void CControl::botSend(string strConfig, string strMessage)
{
	strConfig = strConfig.c_str();
	if (m_mapBots[strConfig] != NULL)
	{
		m_mapBots[strConfig]->sendRaw(strMessage);
	}
}

void CControl::handleExitSignal()
{
#ifdef WIN32
	for (map<string, CMaster *>::iterator i = m_mapBots.begin(); i != m_mapBots.end(); ++i)
	{
		if (i->second != NULL)
		{
			printf("child: '%s'\n", i->first.c_str());
			//(*i).second->_onDestruct();
			delete i->second;
			i->second = NULL;
			i = m_mapBots.erase(i);
			if (i == m_mapBots.end())
				break;
		}
	}
#endif
}

/* IDK.. dimensions???
map2d CControl::botGetInfo(string strConfig)
{
	map2d mapReturn;

	
	[ iteration through bots ]
	{
		if (!strConfig.empty() && strConfig != strBotGroup)
			break;

		stringmap mapTemp;
		[ iteration through bot objects ]
		{
			mapTemp[
		}
	}
}*/
