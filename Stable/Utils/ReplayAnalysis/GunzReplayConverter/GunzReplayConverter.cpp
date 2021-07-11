// GunzReplayConverter.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "ReplayConverter.h"

using namespace std;

int _tmain( int argc, _TCHAR* argv[] )
{
	if( argc < 2 )
	{
		cout << "리플레이 파일들이 있는 폴더를 저에게 드래그 해서 실행해 주세요.\n";
		getchar();
		return -1;
	}

	char szModuleFileName[_MAX_DIR] = {0,};
	GetModuleFileName( NULL, szModuleFileName, _MAX_DIR );
	PathRemoveFileSpec( szModuleFileName );
	SetCurrentDirectory( szModuleFileName );

	InitLog( "log.txt" );

	for( int i = 1; i < argc; ++i )
	{
		cout << "폴더: " << argv[i] << "\n";
		Log( "폴더: %s\n", argv[i] );
	}

	ReplayConverter replayConverter;

	for( int i = 1; i < argc; ++i )
	{
		string path = argv[i];
		path += "\\";
		string filter = path;
	//	filter += "*";
		filter += "*."; filter += GUNZ_REC_FILE_EXT;

		WIN32_FIND_DATA findFileData;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		hFind = FindFirstFile( filter.c_str(), &findFileData );

		if( hFind == INVALID_HANDLE_VALUE )
		{
			continue;
		}

		cout << "파일: " << findFileData.cFileName << "\n";
		Log( "파일: %s\n", findFileData.cFileName );
		if( !replayConverter.Add( path + findFileData.cFileName ) )
		{
			cout << "[[파일 읽기 실패]]: " << findFileData.cFileName << "\n";
			Log( "[[파일 읽기 실패]]: %s\n", findFileData.cFileName );
		}

		while( FindNextFile( hFind, &findFileData ) ) 
		{
			cout << "파일: " << findFileData.cFileName << "\n";
			Log( "파일: %s\n", findFileData.cFileName );
			if( !replayConverter.Add( path + findFileData.cFileName ) )
			{
				cout << "[[파일 읽기 실패]]: " << findFileData.cFileName << "\n";
				Log( "[[파일 읽기 실패]]: %s\n", findFileData.cFileName );
			}
		}

		FindClose( hFind );
	}

	if( !replayConverter.SaveCSV() )
	{
		cout << "[[CSV 파일 저장 실패]]\n";
	}

	CloseLog();

	cout << "...아무키나 누르세요.\n";
	getchar();

	return 0;
}

