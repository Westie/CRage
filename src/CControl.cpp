#include "StdInc.h"
#include "CControl.h"

map<string, CMaster *> CControl::m_mapBots;
Config *CControl::m_pConfig;

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
