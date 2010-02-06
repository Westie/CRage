#include "StdInc.h"
#include "CMaster.h"

CMaster::CMaster(string strKey, Config *pConfig)
{
	m_pConfig = pConfig;
	m_strBotGroup = strKey;
	// oPlugins
	// oModes
}

CMaster::~CMaster()
{
}

void CMaster::_onDestruct()
{
}

void CMaster::sendRaw(string strMessage)
{
}
