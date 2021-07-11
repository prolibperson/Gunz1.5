// RandMaskVal.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

// MMemoryProxy.cpp의 
// #define PTR_OFFSET 0x06C4BAF8
// ↑의 마스크 값을 빌드하기 전에 변경해 주는 간단한 프로그램

typedef vector<string>	VecString;


int _tmain(int argc, _TCHAR* argv[])
{
	FILE* fp = NULL;
	VecString vecLine;

	char* szFile = argv[1];
	//szFile = "test.h";//todok 디버그용
	if (!szFile)
	{
		printf("명령행 인수로 대상 파일을 지정하세요.\n");
		goto TERMINATE;
	}
	fp = fopen(szFile, "rt");
	if (!fp) {
		printf("파일을 열 수 없습니다. %s\n", szFile);
		goto TERMINATE;
	}

	// 파일 전체를 문자열 버퍼에 읽어들인다
	char buffer[4096];
	while (1)
	{
		if (NULL == fgets(buffer, 4096, fp))
			break;

		vecLine.push_back(string(buffer));
	}

	fclose(fp);

	// 마스크값 디파인을 찾는다
	srand(timeGetTime());
	int numLine = (int)vecLine.size();
	for (int i=0; i<numLine; ++i)
	{
		if (string::npos != vecLine[i].find("#define PTR_OFFSET"))
		{
			// 적당히 값을 만든다
			// (너무 큰 값은 그 자체로 바이트 패턴이 되기 쉬워서 
			// 일단 마스크 값만 알아내면 여러군데의 중요한 변수를 찾기가 오히려 쉬워질 우려가 ..)
			int r = rand() % 0xFF00;
			r += 0xFF;

			char sz[4096];
			sprintf(sz, "#define PTR_OFFSET 0x%X\n", r);
			vecLine[i] = sz;
			break;
		}
	}

	// 파일을 다시 쓴다.
	fp = fopen(szFile, "wt");
	if (!fp) {
		printf("파일을 열 수 없습니다. %s\n", szFile);
		goto TERMINATE;
	}

	for (int i=0; i<numLine; ++i)
	{
		fputs(vecLine[i].c_str(), fp);
	}
	fclose(fp);

	printf("랜덤 마스크 값 완료...\n");


TERMINATE:
	printf("\n\n");
	system("pause");

	return 0;
}

