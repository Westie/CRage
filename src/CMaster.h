class CMaster;

#ifndef _CMASTER_H
#define _CMASTER_H

class CMaster
{
public:
	CMaster(string strKey, Config_t *pConfig);
	~CMaster();

	void _onDestruct();
	void sendRaw(string strMessage);

	Config_t *m_pConfig;

private:
	string m_strBotGroup;
};

#endif
