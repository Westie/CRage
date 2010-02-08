class CMaster;

#ifndef _CMASTER_H
#define _CMASTER_H

#include <list>
#include <vector>
#include "CSocket.h"

class CMaster
{
public:
	CMaster(string strKey, NetworkConfig_t *pNetworkConfig, Config_t *pConfig);
	~CMaster();

	list<CSocket *>
		*getChildren();

	CSocket
		*getChildObject(string strChild);

	NetworkConfig_t
		*m_pNetworkConfig;

	Config_t
		*m_pConfig;

	void
		_onDestruct(),
		Loop(),
		setNickname(string strChild, string strNewNick),
		getSend(CSocket *pSocket, string strLine),
		sortChunks(vector<string> *vecChunks),
		sendRaw(string strMessage),
		printDebug(string sFormat, int iDebugLevel, ...);

	bool
		addChild(string strChild, string strNickname, string strUsername = "", string strRealname = ""),
		removeChild(string strChild, string strReason = ""),
		doesChildExist(string strChild),
		resetChild(string strChild, string strMessage = "Rehash!");

	string
		getChildConfig(string strKey),
		getMasterConfig(string strKey);

private:
	bool
		_addChild(string strChild, stringmap mapInfo),
		m_bMasterPresent;

	void
		_onRaw(vector<string> vecChunks),
		_onJoin(vector<string> vecChunks),
		_onKick(vector<string> vecChunks),
		_onPart(vector<string> vecChunks),
		_onQuit(vector<string> vecChunks),
		_onMode(vector<string> vecChunks),
		_onNick(vector<string> vecChunks),
		_onNotice(vector<string> vecChunks),
		_onCTCP(vector<string> vecChunks),
		_onPrivmsg(vector<string> vecChunks),
		_onTopic(vector<string> vecChunks),
		_onError(vector<string> vecChunks),
		_onConnect();

	string
		m_strBotGroup,
		m_strCurrentLine,
		m_strMasterKey;

	list<CSocket *>
		m_lstBotObjects;

	CSocket
		*m_pCurrentBot;

	list<string>
		m_lstOwners;
};

#endif
