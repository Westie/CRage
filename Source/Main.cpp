#include "StdInc.h"

#include "CControl.h"
#include "CConfigParser.h"
#include "CMaster.h"
#include <signal.h>

bool g_bRunning = false;

#ifdef WIN32
BOOL WINAPI SignalHandler(DWORD dwEvent)
{
	switch (dwEvent)
	{
	case CTRL_C_EVENT:
	case CTRL_CLOSE_EVENT:
		//g_bRunning = false;
		CControl::handleExitSignal();
		return FALSE;
	}
	
	return TRUE;
}
#endif

int main(int args, char *argv[])
{
	CControl::m_pConfigParser = new CConfigParser;
	CControl::m_pConfigParser->parseDirectory();

#ifdef WIN32
	SetConsoleCtrlHandler((PHANDLER_ROUTINE)SignalHandler, TRUE);
#endif

	g_bRunning = true;

	while (g_bRunning)
	{
		for (map<string, CMaster *>::iterator i = CControl::m_mapBots.begin(); i != CControl::m_mapBots.end(); ++i)
		{
			i->second->Loop();
		}

		//Timers::Scan();
		Sleep(CORE_SLEEP);
	}

	return 0;
}