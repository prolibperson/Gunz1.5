
#include "stdafx.h"
#include "ReplayConverter.h"
#include "MSharedCommandTable.h"
#include "MBlobArray.h"
#include "MMatchUtil.h"
#include "CommonLog.h"
#include "ZFile.h"
#include <assert.h>

//-----------------------------------------------------------------------------
static Logger logger;

void InitLog( const char* fileName )
{
	logger.Init( fileName );
}

void CloseLog()
{
	logger.Close();
}

void Log( const char* format, ... )
{
	va_list args;
	char szTemp[MAX_PATH];

	va_start( args, format );
	vsprintf( szTemp, format, args );
	va_end( args );

	logger.Print( szTemp );
}
//-----------------------------------------------------------------------------
Replay::Replay() :
	m_nVersion(0),
	m_fGameTime(0.0f)
{
	memset( &m_StageSetting, 0, sizeof(REPLAY_STAGE_SETTING_NODE) );
	memset( &m_DuelQueueInfo, 0, sizeof(MTD_DuelQueueInfo) );
}

Replay::~Replay()
{
}

bool Replay::Load( const string& filename )
{
	struct FileScope
	{
		FileScope() : file(NULL) {}
		~FileScope() { if( file ) zfclose( file ); }

		ZFile* file;
	};

	FileScope fileScope;
	fileScope.file = zfopen( filename.c_str() );

	if( !fileScope.file )
		return false;

	if( !LoadHeader( fileScope.file ) )
		return false;

	if( !LoadStageSetting( fileScope.file ) )
		return false;

	if( m_nVersion >= 4 )
		if( !LoadStageSettingEtc( fileScope.file ) )
			return false;

	if( !LoadCharInfo( fileScope.file ) )
		return false;

	if( !LoadCommandStream( fileScope.file ) )
		return false;

	return true;
}

bool Replay::LoadHeader( ZFile* file )
{
	unsigned int version = 0;
	unsigned int header;
	int nRead;

	nRead = zfread( &header, sizeof(header), 1, file );

	if( nRead == 0 || header != GUNZ_REC_FILE_ID )
	{
		return false;
	}

	nRead = zfread( &version, sizeof(version), 1, file );
	if( ( nRead == 0 ) || ( version > GUNZ_REC_FILE_VERSION ))
		return false;

	m_nVersion = version;
	return true;
}

bool Replay::LoadStageSetting( ZFile* file )
{
	int nRead = zfread( &m_StageSetting, sizeof(REPLAY_STAGE_SETTING_NODE), 1, file );

	if( nRead == 0 )
		return false;

	Log( "\t맵이름: %s\n", m_StageSetting.szMapName );

	m_MapName = m_StageSetting.szMapName;
	transform( m_MapName.begin(), m_MapName.end(), m_MapName.begin(), tolower );
	return true;
}

bool Replay::LoadStageSettingEtc( ZFile* file )
{
	if( m_StageSetting.nGameType == MMATCH_GAMETYPE_DUEL )
	{
		MTD_DuelQueueInfo duelQueueInfo;

		int nRead = zfread( &duelQueueInfo, sizeof(duelQueueInfo), 1, file );

		if( nRead == 0 )
			return false;
	}

	return true;
}

bool Replay::LoadCharInfo( ZFile* file )
{
	int nRead;

	// character info
	int nCharacterCount;
	zfread( &nCharacterCount, sizeof(nCharacterCount), 1, file );

	Log( "\t캐릭터 수: %d\n", nCharacterCount );

	for( int i = 0; i < nCharacterCount; i++ )
	{
		bool bHero;
		nRead = zfread( &bHero, sizeof(bHero), 1, file );
		if( nRead != 1 ) return false;

		MTD_CharInfo info;

		if( m_nVersion < 2 ) 
		{
			nRead = zfread(&info, sizeof(info)-4, 1, file );
			if( nRead != 1 ) return false;
			info.nClanCLID = 0;
		}
		else 
		{
			nRead = zfread( &info, sizeof(info), 1, file );
			if( nRead != 1 ) return false;
		}

		Log( "\t\t[%s][%s]\n", info.szClanName, info.szName );

		MUID uid;
		nRead = zfread(&uid,sizeof(uid),1,file);
		if(nRead!=1) return false;

		ZCharacterProperty charProperty;
		nRead=zfread(&charProperty,sizeof(charProperty),1,file);
		if(nRead!=1) return false;

		float hp;
		nRead=zfread(&hp,sizeof(float),1,file);
		if(nRead!=1) return false;

		float ap;
		nRead=zfread(&ap,sizeof(float),1,file);
		if(nRead!=1) return false;

		ZCharacterStatus charStatus;
		nRead=zfread(&charStatus,sizeof(charStatus),1,file);
		if(nRead!=1) return false;

		// ZCharacterItem.cpp => bool ZCharacterItem::Load(ZFile *file)
		for(int j=0;j<MMCIP_END;j++)
		{
			int nBullet;
			nRead=zfread(&nBullet,sizeof(nBullet),1,file);
			if(nRead!=1) return false;

			int nBulletAMagazine;
			nRead=zfread(&nBulletAMagazine,sizeof(nBulletAMagazine),1,file);
			if(nRead!=1) return false;
		}

		rvector pos;
		nRead=zfread(&pos,sizeof(pos),1,file);
		if(nRead!=1) return false;

		rvector dir;
		nRead=zfread(&dir,sizeof(dir),1,file);
		if(nRead!=1) return false;

		MMatchTeam teamID;
		nRead=zfread(&teamID,sizeof(teamID),1,file);
		if(nRead!=1) return false;

		bool bDie;
		nRead=zfread(&bDie,sizeof(bDie),1,file);
		if(nRead!=1) return false;

		if( m_nVersion >= 2 )
		{
			bool bAdminHide;
			nRead=zfread(&bAdminHide,sizeof(bAdminHide),1,file);
			if(nRead!=1) return false;
		}
	}

	return true;
}

bool Replay::LoadCommandStream( ZFile* file )
{
	float fGameTime;
	zfread(&fGameTime, sizeof(fGameTime), 1, file);
	m_fGameTime = fGameTime;

	int nCommandCount = 0;

	int nSize;
	float fTime;
	while( zfread(&fTime, sizeof(fTime), 1, file) )
	{
		nCommandCount++;

		char CommandBuffer[1024];

		MUID uidSender;
		zfread(&uidSender, sizeof(uidSender), 1, file);
		zfread(&nSize, sizeof(nSize), 1, file);

		if(nSize<0 || nSize>sizeof(CommandBuffer))
		{
			return false;
		}
		zfread(CommandBuffer, nSize, 1, file);

		if( m_nVersion <= 2 )
		{
			// 버전 2 이하는 고려하지 않음
		}
		else
		{
			unsigned short int nDataCount = 0;

			// Get Total Size
			unsigned short nTotalSize = 0;
			char* pData = CommandBuffer;
			memcpy(&nTotalSize, pData, sizeof(nTotalSize));

			nDataCount += sizeof(nTotalSize);

			// Command
			unsigned short int nCommandID = 0;
			memcpy(&nCommandID, pData+nDataCount, sizeof(nCommandID));
			nDataCount += sizeof(nCommandID);

			// serial number
			unsigned char serialNumber;
			memcpy(&serialNumber, pData+nDataCount, sizeof(serialNumber));
			nDataCount += sizeof(serialNumber);

			switch( nCommandID )
			{
			case MC_MATCH_GAME_DEAD:
				{
				//	MUID uidAttacker;
				//	memcpy(&uidAttacker, pData+nDataCount, sizeof(MUID));
					nDataCount += sizeof(MUID);

				//	unsigned int attackerArg;
				//	memcpy(&attackerArg, pData+nDataCount, sizeof(unsigned int));
					nDataCount += sizeof(unsigned int);

					MUID uidVictim;
					memcpy(&uidVictim, pData+nDataCount, sizeof(MUID));
					nDataCount += sizeof(MUID);

				//	unsigned int victimArg;
				//	memcpy(&victimArg, pData+nDataCount, sizeof(unsigned int));
					nDataCount += sizeof(unsigned int);

					RecentPositionConstIter it = m_RecentPosition.find( uidVictim );
					if( it != m_RecentPosition.end() )
					{
						const rvector& pos = it->second;

						DIE_INFO dieInfo;
						dieInfo.fTime = fTime;
						dieInfo.x = pos.x;
						dieInfo.y = pos.y;
						dieInfo.z = pos.z;

						m_DieInfoList.push_back( dieInfo );
						Log( "\t\t\t[시간:%f][위치:%f, %f, %f]\n", fTime, pos.x, pos.y, pos.z );
					}
					else
					{
						Log( "[[죽은 플레이어의 MUID가 없음]]\n" );
					}
				}
				break;

			case MC_PEER_BASICINFO:
				{
					int nSize;
					memcpy( &nSize, pData+nDataCount, sizeof(nSize) );
					nDataCount += sizeof(nSize);

					ZPACKEDBASICINFO binfo;
					memcpy(&binfo, pData+nDataCount, sizeof(binfo));
					nDataCount += sizeof(binfo);

					rvector pos;
					pos.x = Roundf(binfo.posx);
					pos.y = Roundf(binfo.posy);
					pos.z = Roundf(binfo.posz);
					m_RecentPosition[uidSender] = pos;
				}
				break;
			}
		}
	}

	return true;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
ReplayConverter::ReplayConverter()
{

}

ReplayConverter::~ReplayConverter()
{
	for( ReplayListIter i = m_ReplayList.begin(); i != m_ReplayList.end(); ++i )
		delete i->second;
}

bool ReplayConverter::Add( const std::string& replayFileName )
{
	assert( replayFileName.length() );

	Replay* replay = new Replay();
	if( !replay->Load( replayFileName.c_str() ) )
	{
		return false;
	}

	m_ReplayList.insert( ReplayListPair( replay->GetMapName(), replay ) );
	return true;
}

bool ReplayConverter::SaveCSV()
{
	typedef map<string, FILE*> SaveFileList;
	typedef SaveFileList::const_iterator SaveFileListConstIter;
	
	struct SCOPE
	{
		SCOPE() {}
		~SCOPE()
		{
			for( SaveFileListConstIter i = saveFileList.begin(); i != saveFileList.end(); ++i )
				fclose( i->second );
		}

		SaveFileList saveFileList;
	};

	SCOPE scope;

	for( ReplayListConstIter i = m_ReplayList.begin(); i != m_ReplayList.end(); ++i )
	{
		FILE* fp = NULL;

		SaveFileListConstIter j = scope.saveFileList.find( i->first );
		if( j == scope.saveFileList.end() )
		{
			string fileName = i->second->GetMapName();
			fileName += ".csv";
			fp = fopen( fileName.c_str(), "w" );
			if( !fp )
			{
				cout << "[[CSV 파일 생성 실패: " << fileName << "]]\n";
				return false;
			}

			cout << "CSV 파일 생성: " << fileName << "\n";

			scope.saveFileList[i->second->GetMapName()] = fp;
		}
		else
		{
			fp = j->second;
		}

		const DieInfoList& dieInfoList = i->second->GetDieInfoList();
		for( DieInfoConstIter k = dieInfoList.begin(); k != dieInfoList.end(); ++k )
		{
			const DIE_INFO& DieInfo = *k;
			fprintf( fp, "%f,%f,%f,%f\n", DieInfo.fTime, DieInfo.x, DieInfo.y, DieInfo.z );
		}
	}

	return true;
}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------