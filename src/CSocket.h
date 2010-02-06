class CSocket;

#ifndef _CSOCKET_H
#define _CSOCKET_H

class CSocket
{
public:
	CSocket();
	~CSocket();

	void constructBot();
	void destructBot();
	void destructBot(string strMessage);

private:
	SOCKET m_Socket;
	bool m_bActive;
	bool m_bIsWaiting;
};

#endif
