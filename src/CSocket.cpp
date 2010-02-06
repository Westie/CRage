#include "StdInc.h"
#include "CSocket.h"

CSocket::CSocket(CMaster *pMaster, string strChild, Config *pConfig)
{
	m_bActive = false;
	m_bIsWaiting = false;

	m_pMaster = pMaster;
	m_strChild = strChild;
	m_pConfig = pConfig;

	constructBot();
}

CSocket::~CSocket()
{
}

void CSocket::constructBot()
{
#ifdef WIN32
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 0), &wsa) != 0)
	{
		// Error handling
	}
#endif

	Config *pConfig = m_pMaster->m_pConfig;

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		// Error handling
	}

	// bind to ip
	stringmap mapNetworkConfig = (*pConfig)["Network"];
	if (!mapNetworkConfig["bind"].empty())
	{
		sockaddr_in bindaddr;
		bindaddr.sin_addr.s_addr = inet_addr(mapNetworkConfig["bind"].c_str());
		bindaddr.sin_family = AF_INET;
		bind(m_Socket, (sockaddr *)&bindaddr, sizeof(bindaddr));
	}

	// connect to ip, port
	sockaddr_in destaddr;
	destaddr.sin_addr.s_addr = inet_addr(mapNetworkConfig["host"].c_str());
	destaddr.sin_port = htons(toNumber(mapNetworkConfig["port"]));
	connect(m_Socket, (sockaddr *)&destaddr, sizeof(destaddr));

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

int CSocket::Output(string strRaw)
{
	strRaw += IRC_EOL;
	return send(m_Socket, strRaw.c_str(), strRaw.length(), 0);
}

int CSocket::OutputFormat(string strFormat, ...)
{
	va_list args;
	va_start(args, strFormat);
	char szBuffer[256];
	_vsnprintf(szBuffer, sizeof(szBuffer), strFormat.c_str(), args);
	int iRet = Output(szBuffer);
	va_end(args);
	return iRet;
}
