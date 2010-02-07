#include "StdInc.h"
#include "CSocket.h"

CSocket::CSocket(CMaster *pMaster, string strChild, stringmap mapBasic)
{
	m_Socket = INVALID_SOCKET;
	m_bActive = false;
	m_bIsWaiting = false;

	m_pMaster = pMaster;
	m_strChild = strChild;
	m_mapBasic = mapBasic;

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

	NetworkConfig_t *pNetworkConfig = m_pMaster->m_pNetworkConfig;
	Config_t *pConfig = m_pMaster->m_pConfig;

	m_Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (m_Socket == INVALID_SOCKET)
	{
		// Error handling
		return;
	}

	// bind to ip
	if (!(*pNetworkConfig)["bind"].empty())
	{
		sockaddr_in bindaddr;
		bindaddr.sin_addr.s_addr = inet_addr((*pNetworkConfig)["bind"].c_str());
		bindaddr.sin_family = AF_INET;
		bind(m_Socket, (sockaddr *)&bindaddr, sizeof(bindaddr));
	}

	// connect to ip, port
	sockaddr_in destaddr;
	destaddr.sin_addr.s_addr = inet_addr((*pNetworkConfig)["host"].c_str());
	if (destaddr.sin_addr.s_addr == INADDR_NONE)
	{
		hostent *he = gethostbyname((*pNetworkConfig)["host"].c_str());
		if (he == NULL)
		{
			// Error handling
			return;
		}
		memcpy(&(destaddr.sin_addr), he->h_addr_list[0], 4);
	}
	string strPort = (*pNetworkConfig)["port"];
	if (strPort.empty())
		destaddr.sin_port = htons(6667);
	else
		destaddr.sin_port = htons(toNumber(strPort));
	destaddr.sin_family = AF_INET;
	connect(m_Socket, (sockaddr *)&destaddr, sizeof(destaddr));

#ifndef WIN32
	// set non-blocking (??)
#endif

	// send server password, if there is one (PASS)
	if (!(*pNetworkConfig)["password"].empty())
	{
		OutputFormat("PASS %s", (*pNetworkConfig)["password"].c_str());
	}

	// identify (NICK, USER)
	OutputFormat("NICK %s" IRC_EOL "USER %s x x :%s", m_mapBasic["nickname"].c_str(), m_mapBasic["username"].c_str(), m_mapBasic["realname"].c_str());

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
	OutputFormat("QUIT :%s", strMessage.c_str());

	// Delete ping timer

	closesocket(m_Socket);

	m_bActive = false;
	m_bIsWaiting = false;
}

int CSocket::Output(string strRaw)
{
	strRaw += IRC_EOL;
	printf("[out] %s", strRaw.c_str());
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

void CSocket::Input()
{
	if (m_pMaster == NULL)
		return;

	if (!isSocketActive())
	{
		if (!m_bIsWaiting)
		{
			destructBot();
			m_bIsWaiting = true;
			constructBot();
		}
		return;
	}

	if (m_bIsWaiting)
		return;

	char buf[256];
	size_t cnt = recv(m_Socket, buf, 255, 0);
	buf[cnt] = '\0';
	std::string strPacket(buf);
	while (cnt == 255)
	{
		memset(buf, 0, sizeof(buf));
		cnt = recv(m_Socket, buf, 255, 0);
		buf[cnt] = '\0';
		strPacket += buf;
	}
	
	std::string::size_type lastPos = strPacket.find_first_not_of("\r\n", 0);
	std::string::size_type pos = strPacket.find_first_of("\r\n", lastPos);
	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		m_pMaster->getSend(this, strPacket.substr(lastPos, pos - lastPos));

		lastPos = strPacket.find_first_not_of("\r\n", pos);
		pos = strPacket.find_first_of("\r\n", lastPos);
	}
}

bool CSocket::isSocketActive()
{
	return m_bActive && m_Socket != INVALID_SOCKET;
}
