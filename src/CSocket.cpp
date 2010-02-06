#include "StdInc.h"
#include "CSocket.h"

CSocket::CSocket()
{
	m_bActive = false;
	m_bIsWaiting = false;

#ifdef WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
	{
		// Error handling
	}
#endif

	constructBot();
}

CSocket::~CSocket()
{
}

void CSocket::constructBot()
{
	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		// Error handling
	}

	// bind to ip

	// connect to ip, port

#ifndef WIN32
	// set non-blocking
#endif

	// send server password, if there is one

	// identify (USER, NICK)

	m_bActive = true;
	m_bIsWaiting = false;

	// create ping timer
}

void CSocket::destructBot()
{
	// TODO: use quit message from config
	destructBot("");
}

void CSocket::destructBot(string strMessage)
{
	// QUIT with strMessage

	// Delete ping timer

	closesocket(m_Socket);

	m_bActive = false;
	m_bIsWaiting = false;
}