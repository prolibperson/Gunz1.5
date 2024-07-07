#pragma once

#include "MDatabase.h" 


#include <string>
#include <map>
using namespace std;

class MServerStatusMgr;

class MLocatorDBMgr : public MDatabase
{
public:
	MLocatorDBMgr(void);
	virtual ~MLocatorDBMgr(void);

	CString BuildDSNString( const CString& strDSN, 
							const CString& strUserName, 
							const CString& strPassword );
	bool Connect( const CString& strDSNConnect );
	void Disconnect();

	bool GetServerStatus( MServerStatusMgr* pServerStatusMgr );

	bool StartUpLocaterStauts( const int nLocatorID, 
							   const string& strIP, 
							   const int nPort, 
							   const int nDuplicatedUpdateTime );
	bool UpdateLocaterStatus( const int nLocatorID, 
							  const DWORD nRecvCount, 
							  const DWORD nSendCount, 
							  const DWORD nBlockCount, 
							  const DWORD nDuplicatedCount );

	bool InsertLocatorLog( const int nLocatorID, const map<string, DWORD>& CountryStatistics );

	MDatabase* GetDB() { return m_pDB; }

	void SetDB(MDatabase* pDB) { m_pDB = pDB; }

	void ReleaseDB()
	{
		if (0 != m_pDB)
		{
			m_pDB->Disconnect();
			delete m_pDB;
		}
	};

private :
	bool CheckOpen();

private :
	CString				m_strDSNConnect;
	MDatabase* m_pDB;
};