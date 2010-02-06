class CControl;

#ifndef _CCONTROL_H
#define _CCONTROL_H

class CControl
{
public:
	static void botCreate(string strConfig);
	static void botRemove(string strConfig);
	static void botSend(string strConfig, string strMessage);
	//static map2d botGetInfo(string strConfig = "");
};

#endif
