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

	void _onDestruct();
	void Loop();
	bool addChild(string strChild, string strNickname, string strUsername = "", string strRealname = "");
	list<CSocket *> *getChildren();
	CSocket *getChildObject(string strChild);
	void setNickname(string strChild, string strNewNick);
	bool removeChild(string strChild, string strReason = "");
	bool doesChildExist(string strChild);
	bool resetChild(string strChild, string strMessage = "Rehash!");
	string getChildConfig(string strKey);
	string getMasterConfig(string strKey);
	void getSend(CSocket *pSocket, string strLine);
	void sortChunks(vector<string> *vecChunks);
	void sendRaw(string strMessage);

	NetworkConfig_t *m_pNetworkConfig;
	Config_t *m_pConfig;

private:
	bool _addChild(string strChild, stringmap mapInfo);
	void _onRaw(vector<string> vecChunks);
	void _onConnect();

	string m_strBotGroup;
	bool m_bMasterPresent;
	string m_strMasterKey;
	list<CSocket *> m_lstBotObjects;
	CSocket *m_pCurrentBot;
	string m_strCurrentLine;
	list<string> m_lstOwners;
};

#endif