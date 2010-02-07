#include "StdInc.h"

#include "CControl.h"
#include "CConfigParser.h"
#include "CMaster.h"

int main(int args, char *argv[])
{
	CControl::m_pConfigParser = new CConfigParser;
	CControl::m_pConfigParser->parseDirectory();

	while (true)
	{
		for (map<string, CMaster *>::iterator i = CControl::m_mapBots.begin(); i != CControl::m_mapBots.end(); ++i)
		{
			//i->second->Loop();
		}

		//Timers::Scan();
		Sleep(CORE_SLEEP);
	}

	return 0;
}