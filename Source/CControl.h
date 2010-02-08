class CControl;

#ifndef _CCONTROL_H
#define _CCONTROL_H

#include "CMaster.h"
#include "CConfigParser.h"

class CControl
{
public:
	static map<string, CMaster *> m_mapBots;
	static CConfigParser *m_pConfigParser;

	static bool botRemove(string strConfig);

	static void botCreate(string strConfig),
		botSend(string strConfig, string strMessage),
		handleExitSignal();
	//static map2d botGetInfo(string strConfig = "");
};

#endif
