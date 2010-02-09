class CSocket;

#ifndef _CSOCKET_H
#define _CSOCKET_H

#include "CMaster.h"

class CSocket
{
public:
	CSocket(CMaster *pMaster, string strChild, stringmap mapBasic);
	~CSocket();

	void
		constructBot(),
		destructBot(),
		destructBot(string strMessage),
		setNickname(string sNickname),
		Input();

	int
		OutputFormat(string strFormat, ...),
		Output(string strRaw),
		closeSocket();

	string
		m_strChild;

	stringmap
		m_mapBasic;

	map<string, map<string, int> >
		m_mapStatistics;

private:
	bool
		isSocketActive(),
		m_bActive,
		m_bIsWaiting;
	
	SOCKET
		m_Socket;

	CMaster
		*m_pMaster;
};

#endif
