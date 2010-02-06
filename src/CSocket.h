class CSocket;

#ifndef _CSOCKET_H
#define _CSOCKET_H

#include "CMaster.h"

class CSocket
{
public:
	CSocket(CMaster *pMaster, string strChild, Config_t *pConfig);
	~CSocket();

	void constructBot();
	void destructBot();
	void destructBot(string strMessage);
	int Output(string strRaw);
	int OutputFormat(string strFormat, ...);

private:
	SOCKET m_Socket;
	bool m_bActive;
	bool m_bIsWaiting;
	CMaster *m_pMaster;
	string m_strChild;
	Config_t *m_pConfig;
};

#endif
