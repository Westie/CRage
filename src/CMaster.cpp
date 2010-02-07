#include "StdInc.h"
#include "CMaster.h"

CMaster::CMaster(string strKey, NetworkConfig_t *pNetworkConfig, Config_t *pConfig)
{
	m_pConfig = pConfig;
	m_pNetworkConfig = pNetworkConfig;
	m_strBotGroup = strKey;

	// oPlugins
	// oModes

	if (pNetworkConfig != NULL)
	{
		if ((*pNetworkConfig)["delimeter"].empty())
			(*pNetworkConfig)["delimeter"] = "!";

		if ((*pNetworkConfig)["rotation"].empty())
			(*pNetworkConfig)["rotation"] = "SEND_DEF";

		if ((*pNetworkConfig)["quitmsg"].empty())
			(*pNetworkConfig)["quitmsg"] = "CRage is going to bed :(";
		
		if ((*pNetworkConfig)["version"].empty())
			(*pNetworkConfig)["version"] = "CRage " BOT_VERSION " (rel. " BOT_RELDATE ")";

		// explode owners
	}
}

CMaster::~CMaster()
{
	_onDestruct();
}

void CMaster::_onDestruct()
{
}

void CMaster::Loop()
{
}

bool CMaster::_addChild(string strChild, stringmap mapInfo)
{
	if (doesChildExist(strChild))
		return false;

	mapInfo["slave"] = m_bMasterPresent ? "true" : "false";

	if (!m_bMasterPresent)
	{
		m_bMasterPresent = true;
		m_strMasterKey = string(strChild);
	}

	mapInfo["timewait"] = "1";
	mapInfo["loadtime"] = toString(GetTickCount() + 1);

	m_lstBotObjects.push_back(new CSocket(this, string(strChild), mapInfo));
	return true;
}

bool CMaster::addChild(string strChild, string strNickname, string strUsername, string strRealname)
{
	stringmap mapInfo;
	mapInfo["nickname"] = strNickname;
	mapInfo["username"] = !strUsername.empty() ? strUsername : strChild;
	mapInfo["realname"] = !strRealname.empty() ? strRealname : strNickname;
	mapInfo["altnick"] = strNickname + toString(GetRandom(0, 10));
	mapInfo["reactevent"] = "false";

	return _addChild(strChild, mapInfo);
}

list<CSocket *> *CMaster::getChildren()
{
	return &m_lstBotObjects;
}

CSocket *CMaster::getChildObject(string strChild)
{
	foreach (list<CSocket *>, m_lstBotObjects, i)
	{
		if (strChild == (*i)->m_strChild)
			return *i;
	}
	return NULL;
}

void CMaster::setNickname(string strChild, string strNewNick)
{
	CSocket *pChild = getChildObject(strChild);
	if (pChild != NULL)
	{
		//pChild->setNickname(strNewNick);
	}
}

bool CMaster::removeChild(string strChild, string strReason)
{
	foreach (list<CSocket *>, m_lstBotObjects, i)
	{
		if (strChild == (*i)->m_strChild)
		{
			if ((*i)->m_mapBasic["slave"] != "true")
				return false;

			(*i)->destructBot(strReason);
			m_lstBotObjects.erase(i);
			delete *i;
			*i = NULL;

			return true;
		}
	}
	return false;
}

bool CMaster::doesChildExist(string strChild)
{
	return getChildObject(strChild) != NULL;
}

bool CMaster::resetChild(string strChild, string strMessage)
{
	CSocket *pChild = getChildObject(strChild);
	if (pChild == NULL)
		return false;

	pChild->destructBot(strMessage);
	pChild->constructBot();

	return true;
}

string CMaster::getChildConfig(string strKey)
{
	string strValue;
	if (m_pCurrentBot != NULL && !(strValue = m_pCurrentBot->m_mapBasic[string(strKey)]).empty())
		return strValue;
	return "";
}

string CMaster::getMasterConfig(string strKey)
{
	string strValue;
	if (m_pNetworkConfig != NULL && !(strValue = (*m_pNetworkConfig)[string(strKey)]).empty())
		return strValue;
	return "";
}

void CMaster::sendRaw(string strMessage)
{
}
