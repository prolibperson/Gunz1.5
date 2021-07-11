#include <iostream>
#include <Shlwapi.h>
#include <tchar.h>

#include "ZReplayInfo.h"


#define MAXFILENAME		512

void Manual()
{
	printf("ReplayInfo for GunZ the Duel\n");
	printf(" This program is created by MAIET Entertainment for solemn use of assisting \n");
	printf("GunZ GM to analyze the reported replay files. \n");
	printf("This program will create [filename].txt file with the replay file information.\n");
	printf("There are two ways to use this program.\n");
	printf("\n");
	printf("[Method 1]\n");
	printf(" 1.Copy the ReplayInfo.exe program to the folder that contains the replay files\n");
	printf(" 2.Run the program: ReplayInfo.exe and press Enter.\n");
	printf(" 3.Find the text files of all the replay files inside the folder.\n");
	printf("\n");
	printf("[Method 2]\n");
	printf(" 1.Run the program : ReplayInfo.exe\n");
	printf(" 2.Drag and drop the replay file folder over the running program’s screen.\n");
	printf(" 3.When the folder’s directory appears on the screen, press enter.\n");
	printf(" 4.Find the text files of all the replay files inside the folder.\n");
	printf("\n");
	printf("리플레이 인포 프로그램 사용 방법\n");
	printf(" 1.리플레이 파일들이 있는 폴더로 프로그램을 복사 -> 프로그램 실행 -> 엔터입력\n");
	printf(" 2.리플레이 파일들이 있는 폴더를 실행된 프로그램 화면위로 드래그 한 후 엔터입력\n");
	printf(" 3.위의 두 가지중 어느 방법을 사용하셔도 괜찮습니다. 사용 후 프로그램이 있는 폴더 안에 리플레이 파일명을 포함한 텍스트 파일이 생성됩니다\n");
}
// 드래그된 폴더 주소 스트링 얻기
void GetFolderPath(char* filename)
{
	char c = {0};
	int c_cnt = 0;

	while( c != '\n' && (c = getchar()) != EOF) 
	{
		if(c == '\n' && c_cnt == 0)
		{	// 폴더 링크하지 않고 리플레이있는 폴더에서 엔터 쳤을경우 처리해준다.
			GetModuleFileName( NULL, filename, _MAX_DIR );
			PathRemoveFileSpec( filename );
			return;
		}
		if(c == '"' || c == '\n')
			continue;
		filename[c_cnt] = c;
		c_cnt++;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	// 사용 설명
	Manual();

	char fileFullName[512] = "0";
	char folderPath[512] = "0";
	GetFolderPath(folderPath);
	sprintf(fileFullName, "%s\\*.gzr", folderPath);

	HANDLE hSrc;
	WIN32_FIND_DATA wfd;
	hSrc = FindFirstFile(fileFullName, &wfd);		// 해당폴더의 첫번째 파일을 wfd에 넣는다
	{
		if (hSrc == INVALID_HANDLE_VALUE)
		{
			fprintf(stderr, "Can't Find [%s].\n", folderPath);
			return 0;
		} 

		do
		{
			sprintf(fileFullName, "%s\\%s", folderPath, wfd.cFileName);

			ZReplayLoader zReplayLoader;
			zReplayLoader.Load(fileFullName);	// 리플레이 파일에서 정보 로딩
			zReplayLoader.Save(wfd.cFileName);	// 리플레이 정보 txt파일에 저장
		}
		while	(FindNextFile(hSrc, &wfd));		// wfd의 다음 파일로 넘어 간다.
	}
	FindClose(hSrc);		// 파일 핸들 닫기

	return 0;
}
