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
		if ((*pNetworkConfig)["delimiter"].empty())
		{
			(*pNetworkConfig)["delimiter"] = "!";
		}

		if ((*pNetworkConfig)["rotation"].empty())
		{
			(*pNetworkConfig)["rotation"] = "SEND_DEF";
		}

		if ((*pNetworkConfig)["quitmsg"].empty())
		{
			(*pNetworkConfig)["quitmsg"] = "CRage is going to bed :(";
		}

		if ((*pNetworkConfig)["debuglevel"].empty())
		{
			(*pNetworkConfig)["debuglevel"] = "0";
		}

		if ((*pNetworkConfig)["version"].empty())
		{
			(*pNetworkConfig)["version"] = "CRage " BOT_VERSION " (rel. " BOT_RELDATE "); http://outrage.typefish.co.uk";
		}

		/* Parse the owners! */
		string
			sOwners = (*m_pNetworkConfig)["owners"];

		string::size_type
			lastPos = sOwners.find_first_not_of(',', 0),
			pos = sOwners.find_first_of(',', lastPos);
	
		while (string::npos != pos || string::npos != lastPos)
		{
			string
				sOwner = trimString(sOwners.substr(lastPos, pos - lastPos));
	
			if (!sOwner.empty())
			{
				m_lstOwners.push_back(sOwner);
			}
		
			lastPos = sOwners.find_first_not_of(',', pos);
			pos = sOwners.find_first_of(',', lastPos);
		}
	}

	if (pConfig != NULL)
	{
		foreach(Config_t, (*m_pConfig), i)
		{
			_addChild(i->first, i->second);
		}
	}
}


CMaster::~CMaster()
{
	_onDestruct();
}


void CMaster::_onDestruct()
{
	foreach (list<CSocket *>, m_lstBotObjects, i)
	{
		(*i)->destructBot();
	}
}


void CMaster::Loop()
{
	foreach (list<CSocket *>, m_lstBotObjects, i)
	{
		(*i)->Input();
		/*if (!(*i)->isClone())
			invokeEvent("onTick");*/
	}
}


bool CMaster::_addChild(string strChild, stringmap mapInfo)
{
	if (doesChildExist(strChild))
		return false;

	mapInfo["reactevent"] = "true";
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
		pChild->setNickname(strNewNick);
	}
}


bool CMaster::removeChild(string strChild, string strReason)
{
	foreach (list<CSocket *>, m_lstBotObjects, i)
	{
		if (strChild == (*i)->m_strChild)
		{
			if ((*i)->m_mapBasic["slave"] != "true")
			{
				return false;
			}

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


void CMaster::getSend(CSocket *pSocket, string strLine)
{
	if (strLine.length() < 3)
	{
		return;
	}

	m_pCurrentBot = pSocket;
	m_strCurrentLine = strLine;

	vector<string>
		vecParts;

	string::size_type
		lastPos = strLine.find_first_not_of(' ', 0),
		pos = strLine.find_first_of(' ', lastPos);

	int cnt = 0;
	while (string::npos != pos || string::npos != lastPos)
	{
		if (cnt == 3)
		{
			vecParts.push_back(strLine.substr(lastPos));
			break;
		}

		vecParts.push_back(strLine.substr(lastPos, pos - lastPos));

		lastPos = strLine.find_first_not_of(' ', pos);
		pos = strLine.find_first_of(' ', lastPos);
		++cnt;
	}

	sortChunks(&vecParts);

	bool bReactEvent = (getChildConfig("reactevent") != "false");

	/* Ouch, this will hurt. */
	if(vecParts[0] == "PING")
	{
		pSocket->OutputFormat("PONG " + vecParts[1]);
	}
	else if(vecParts[1] == "PONG")
	{
		// iNoReply = 0;
		//iHasBeenReply = true;
		//return;
	}

	if(!bReactEvent)
	{
		_onRaw(vecParts);
		return;
	}

	/* This will hurt even more! */
	if(vecParts[1] == "JOIN")
	{
		_onJoin(vecParts);
	}
	else if(vecParts[1] == "PART")
	{
		_onPart(vecParts);
	}
	else if(vecParts[1] == "KICK")
	{
		_onKick(vecParts);
	}
	else if(vecParts[1] == "QUIT")
	{
		_onQuit(vecParts);
	}
	else if(vecParts[1] == "MODE")
	{
		_onMode(vecParts);
	}
	else if(vecParts[1] == "NICK")
	{
		_onNick(vecParts);
	}
	else if(vecParts[1] == "NOTICE")
	{
		_onNotice(vecParts);
	}
	else if(vecParts[1] == "PRIVMSG")
	{
		if(vecParts[3][0] == '\001')
		{
			vecParts[3] = vecParts[3].substr(1);
			_onCTCP(vecParts);

			return;
		}

		switch(vecParts[2])
		{
			case '&':
			case '#':
			{
				_onMesssage(vecParts);
				return;
			}

			default:
			{
				_onPrivMessage(vecParts);
			}
		}
	}
	else if(vecParts[1] == "TOPIC")
	{
		_onTopic(vecParts);
	}
	else if(vecParts[1] == "ERROR")
	{
		_onError(vecParts);
	}
	else
	{
		_onRaw(vecParts);
	}

	return;
}


void CMaster::_onJoin(vector<string> &vecChunks)
{
	printDebug("[JOIN] %s joined %s", 2, vecChunks[0].c_str(), vecChunks[2].c_str());
}


void CMaster::_onKick(vector<string> &vecChunks)
{
}


void CMaster::_onPart(vector<string> &vecChunks)
{
}


void CMaster::_onQuit(vector<string> &vecChunks)
{
}


void CMaster::_onMode(vector<string> &vecChunks)
{
}


void CMaster::_onNick(vector<string> &vecChunks)
{
}


void CMaster::_onNotice(vector<string> &vecChunks)
{
}


void CMaster::_onCTCP(vector<string> &vecChunks)
{
}


void CMaster::_onPrivMessage(vector<string> &vecChunks)
{
}


void CMaster::_onMessage(vector<string> &vecChunks)
{
}


void CMaster::_onTopic(vector<string> &vecChunks)
{
}


void CMaster::_onError(std::vector<string> &vecChunks)
{
	if (vecChunks[2] == ":Closing Link:")
	{
		m_pCurrentBot->closeSocket();
	}
}


void CMaster::_onRaw(vector<string> &vecChunks)
{
	switch (toNumber(vecChunks[1]))
	{
		case 001:
		{
			_onConnect();
			return;
		}

		case 433:
		{
			if (!getChildConfig("altnick").empty())
			{
				printf(getChildConfig("altnick").c_str());
				m_pCurrentBot->setNickname(getChildConfig("altnick"));
			}
	
			return;
		}
	}

	if (vecChunks[1] == "ERROR")
	{
		if (vecChunks[2] == ":Closing Link:")
		{
			m_pCurrentBot->closeSocket();
		}
	}
}


void CMaster::_onConnect()
{
	//invokeEvent("onConnect");

	string
		strChannels = (*m_pNetworkConfig)["channels"];

	string::size_type
		lastPos = strChannels.find_first_not_of(',', 0),
		pos = strChannels.find_first_of(',', lastPos);

	while (string::npos != pos || string::npos != lastPos)
	{
		string strChannel = trimString(strChannels.substr(lastPos, pos - lastPos));

		if (!strChannel.empty())
		{
			sendRaw("JOIN " + strChannel);
		}

		lastPos = strChannels.find_first_not_of(',', pos);
		pos = strChannels.find_first_of(',', lastPos);
	}
}


void CMaster::sortChunks(vector<string> *vecChunks)
{
	vector<string>::size_type size = vecChunks->size();

	if (size > 0 && (*vecChunks)[0][0] == ':')
	{
		(*vecChunks)[0] = (*vecChunks)[0].substr(1);
	}
	else if (size <= 0)
	{
		vecChunks->push_back("");
		++size;
	}

	if (size <= 1)
	{
		vecChunks->push_back("");
		++size;
	}

	if (size > 2 && (*vecChunks)[2][0] == ':')
	{
		(*vecChunks)[2] = (*vecChunks)[2].substr(1);
	}
	else if (size <= 2)
	{
		vecChunks->push_back("");
		++size;
	}

	if (size > 3 && (*vecChunks)[3][0] == ':')
	{
		(*vecChunks)[3] = (*vecChunks)[3].substr(1);
	}
	else if (size <= 3)
	{
		vecChunks->push_back("");
		++size;
	}
}


void CMaster::printDebug(string sFormat, int iDebugLevel, ...)
{
	if(iDebugLevel <= toNumber((*m_pNetworkConfig)["debuglevel"]))
	{
		va_list
			vArgs;

		char
			sBuffer[256];

		va_start(vArgs, iDebugLevel);
		vsnprintf(sBuffer, sizeof(sBuffer), sFormat.c_str(), vArgs);
		printf("%s\r\n", sBuffer);
	}

	return;
}


/* Do we really need to do more in regards to this? */
void CMaster::sendRaw(string strMessage)
{
	m_pCurrentBot->Output(strMessage);
}
