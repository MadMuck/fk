#pragma once

class CLogModule
{
public:
	CLogModule(void);
	~CLogModule(void);
public:
	static  CLogModule & GetInstance()
	{
		return m_pLogModule;
	};
	static CLogModule m_pLogModule;
	/**
	* ��ӡ��־
	*
	*/
	void WriteLog(const char *pPutFromat, ...);
	void LogCharToChar(BYTE src[],string & dst,int len);
	void DebugPrintf( const char *p, ...);
};

#define LOGMODULEINSTANCE CLogModule::GetInstance()
