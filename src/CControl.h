class CControl;

#ifndef _CCONTROL_H
#define _CCONTROL_H

#include "CMaster.h"

class CControl
{
public:
	static map<string, CMaster *> m_mapBots;
	static Config_t *m_pConfig;

	static void botCreate(string strConfig);
	static bool botRemove(string strConfig);
	static void botSend(string strConfig, string strMessage);
	//static map2d botGetInfo(string strConfig = "");
};

#endif
