class CSocket;

#ifndef _CSOCKET_H
#define _CSOCKET_H

#include "CMaster.h"

class CSocket
{
public:
	CSocket(CMaster *pMaster, string strChild, stringmap mapBasic);
	~CSocket();

	void constructBot();
	void destructBot();
	void destructBot(string strMessage);
	int Output(string strRaw);
	int OutputFormat(string strFormat, ...);
	void Input();

	string m_strChild;
	stringmap m_mapBasic;
private:
	bool isSocketActive();

	SOCKET m_Socket;
	bool m_bActive;
	bool m_bIsWaiting;
	CMaster *m_pMaster;
};

#endif
