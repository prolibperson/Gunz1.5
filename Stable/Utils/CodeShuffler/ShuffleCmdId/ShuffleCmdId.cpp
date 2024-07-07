#include "stdafx.h"
#include <string>
#include <vector>
#include <set>
#include <windows.h>
#include <assert.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

// 이 프로그램은 커맨드 ID를 섞어주는 기능을 가지고 있습니다.
// 실행 인자로 MSharedCommandTable.h를 넣어주면 파싱해서
// 커맨드 ID에 겹치지 않는 랜덤번호를 부여합니다.

#define TOK_BEGIN	"[[SHUFFLE_BEGIN]]"
#define TOK_END		"[[SHUFFLE_END]]"
#define TOK_RANGE	"[[SHUFFLE_RANGE]]"
#define RANGEFORMAT	"//"##TOK_RANGE##"(%d,%d)"	//[[SHUFFLE_RANGE]](70000,79999) 이런 식으로 커맨드ID 랜덤 범위가 써져 있다.

typedef vector<string>	VecString;
typedef vector<int>		VecId;
typedef set<int>		SetId;
typedef SetId::iterator	ItorId;

int FindToken(const char* szTok, VecString& vec)
{
	int numLine = (int)vec.size();
	size_t pos = string::npos;
	for (int i=0; i<numLine; ++i)
	{
		pos = vec[i].find(szTok);
		if (pos != string::npos)
		{
			return i;
		}
	}

	return -1;
}

// 문자열 한 줄을 받아서 커맨드 ID 정의가 들어있는지 검사한다
bool _IsCommandString(string& str)
{
	return string::npos != str.find("#define MC_"); // 이렇게 검사하면 주석처리된 커맨드도 포함될 수 있지만 별 문제가 되진 않는다
}

int CountCommands(VecString& vec, int nBeginLine, int nEndLine)
{
	int numCmd = 0;
	int numLine = nEndLine - nBeginLine;
	for (int i=0; i<numLine; ++i)
	{
		if (_IsCommandString(vec[nBeginLine+i]))
			++numCmd;
	}
	return numCmd;
}

void ReplaceId(VecString& vecLine, VecId& vecId, int nBeginLine, int nEndLine)
{
	char szOldId[256];
	char szNewId[256];

	VecId::iterator it = vecId.begin();
	int numLine = nEndLine - nBeginLine;
	for (int i=0; i<numLine; ++i)
	{
		string& str = vecLine[i+nBeginLine];
		if (_IsCommandString(str))
		{
			if (it == vecId.end())
			{
				printf("내부 오류: ID 할당 중인데 모자란다.\n");
				__asm int 3;
			}

			int oldId = 0;
			if (1 != sscanf(str.c_str(), "%*s %*s %d", &oldId) || oldId == 0)
			{
				printf("커맨드가 정의된 행에서 ID 부분을 찾는데 실패했습니다.(%d행)\n", i);
				__asm int 3;
			}

			sprintf(szOldId, "%d", oldId);
			sprintf(szNewId, "%d", (*it));
			size_t pos = str.find(szOldId);
			str.replace(pos, strlen(szOldId),szNewId); 

			++it;
		}
	}

	if (it != vecId.end())
	{
		printf("내부 오류: ID를 모두 할당했지만 남는 것이 있다.\n");
		__asm int 3;
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	VecString vecLine;
	VecId vecId;
	SetId setId;

	char* szFile = argv[1];
	if (!szFile)
	{
		printf("명령행 인수로 대상 파일을 지정하세요.\n");
		goto TERMINATE;
	}
	FILE* fp = fopen(szFile, "rt");
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

	// SHUFFLE_BEGIN을 찾는다
	int nBeginLine = FindToken(TOK_BEGIN, vecLine);
	if (nBeginLine == -1)
	{
		printf("%s <= 토큰을 찾을 수 없습니다.\n", TOK_BEGIN);
		goto TERMINATE;
	}

	// SHUFFLE_END를 찾는다
	int nEndLine = FindToken(TOK_END, vecLine);
	if (nEndLine == -1)
	{
		printf("%s <= 토큰을 찾을 수 없습니다.\n", TOK_END);
		goto TERMINATE;
	}

	// 검증
	if (!(nBeginLine < nEndLine))
	{
		printf("%s은 %s 앞에 있어야 합니다.\n%s:%d line\n%s:%d line\n", TOK_BEGIN, TOK_END, TOK_BEGIN, nBeginLine, TOK_END, nEndLine);
		goto TERMINATE;
	}

	// SHUFFLE_RANGE를 찾는다
	int nRangeLine = FindToken(TOK_RANGE, vecLine);
	if (nRangeLine == -1)
	{
		printf("%s <= 토큰을 찾을 수 없습니다.\n", TOK_RANGE);
		goto TERMINATE;
	}
	// SHUFFLE_RANGE 파싱
	int nRangeBegin, nRangeEnd;
	if (2 != sscanf(vecLine[nRangeLine].c_str(), RANGEFORMAT, &nRangeBegin, &nRangeEnd))
	{
		printf("%s 파싱 실패\n=> %s\n=> %s와 똑같은 형태여야 합니다.\n", TOK_RANGE, vecLine[nRangeLine].c_str(), RANGEFORMAT);
		goto TERMINATE;
	}

	printf("[%d,%d] 의 범위에서 랜덤ID를 만들어냅니다...\n", nRangeBegin, nRangeEnd);

	// BEGIN END 사이의 라인을 한 줄씩 확인하면서 '(공백)#define MC_'로 시작하는 행의 갯수를 센다
	int numCmd = CountCommands(vecLine, nBeginLine, nEndLine);

	printf("%d 개의 커맨드에 랜덤ID를 부여합니다...\n", numCmd);

	// RANGE 사이에서 겹치지 않는 랜덤값을 위의 갯수대로 생성해낸다
	srand(timeGetTime());
	setId.clear();
	for (int i=0; i<numCmd; ++i)
	{
		while (1)	// 중복값을 피해야 하므로 기존에 없는 숫자가 나올때까지 루프
		{
			int r = rand() % (nRangeEnd - nRangeBegin + 1);
			r += nRangeBegin;
			if (setId.find(r) == setId.end())
			{
				setId.insert(r);
				break;
			}
		}
	}

	// 검증
	if (setId.size() != numCmd)
	{
		printf("내부 오류 발생. 디버깅 필요.\n");
		goto TERMINATE;
	}
	for (ItorId it=setId.begin(); it!=setId.end(); ++it)
	{
		if (!(nRangeBegin <= (*it) && (*it) <= nRangeEnd))
		{
			printf("내부 오류 발생. 디버깅 필요. 랜덤범위를 벗어남\n");
			int id = *it;
			goto TERMINATE;
		}
	}

	// set으로 id를 저장했기 때문에 값이 정렬되어 있다. 랜덤으로 섞자
	vecId.clear();
	while (!setId.empty())
	{
		int r = rand() % (int)setId.size();
		ItorId it = setId.begin();
		for (int i=0; i<r; ++i)
			++it;
		vecId.push_back(*it);
		setId.erase(it);
	}

	assert(vecId.size() == numCmd);

	// 각 라인에서 ID위치를 찾아 새 값으로 replace한다
	ReplaceId(vecLine, vecId, nBeginLine, nEndLine);

	
	// 파일을 다시 쓴다.
	fp = fopen(szFile, "wt");
	if (!fp) {
		printf("파일을 열 수 없습니다. %s\n", szFile);
		goto TERMINATE;
	}

	int numLine = (int)vecLine.size();
	for (int i=0; i<numLine; ++i)
	{
		fputs(vecLine[i].c_str(), fp);
	}
	fclose(fp);

	printf("ID 섞기 완료...\n");
	

TERMINATE:
	printf("\n\n");
	system("pause");
	return 0;
}

