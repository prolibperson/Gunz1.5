// ShuffleLineOrder.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <vector>
#include <map>
#include <string>
#include <windows.h>

#pragma comment(lib, "winmm.lib")

using namespace std;

////////////////////////////////////////////////////////////////////////////////
// [[SHUFFLE_LINE]] tag 의 형태로 표기한 부분을 찾아서 서로 자리를 바꾼다.
// 예시 코드는 아래와 같다... 

/* [[SHUFFLE_LINE]] ZGame */ int m_pMyCharacter;
/* [[SHUFFLE_LINE]] ZGame */ int m_someVar;
/* [[SHUFFLE_LINE]] ZGame */ int m_barbar;

// tag <- 부분은 이름표다. 한 소스파일 내에서 두 개 이상의 태그를 사용하면 같은 태그끼리만 섞이게 된다.
// tag 앞뒤엔 공백이 필요하고 tag 내에 공백이 포함되면 안된다.

// 이 툴의 용도:
// ZGame 객체 포인터를 찾아서 특정 오프셋을 더해 m_pMyCharacter를 얻는 핵이 있다.
// 비슷하게, 내 캐릭터 객체에서 고정된 오프셋을 더해 HPAP module을 얻어서 무적 핵을 제작한다.
// 패치를 주기적으로 한다는 가정하에, 패치를 내보낼 때마다 해커에게 오프셋을 다시 찾아야 하는 번거로움을 주자는 목적이다.
// 멤버 변수만이 아니라 MCommandParameterType의 enum 순서를 뒤섞는 것에도 사용할 수 있다.
////////////////////////////////////////////////////////////////////////////////


#define TOK_SWAP	"[[SHUFFLE_LINE]]"
#define SWAPFORMAT	TOK_SWAP##"%s"


typedef vector<string>	VecString;
typedef vector<int>		VecInt;

typedef map<string, VecInt>		MapTaggedLine;
typedef MapTaggedLine::iterator	ItorTaggedLine;

void _SwapString(string& s1, string& s2)
{
	if (&s1 == &s2) return;

	// 성능 신경안씀
	string temp(s1);
	s1 = s2;
	s2 = temp;
}


int _tmain(int argc, _TCHAR* argv[])
{
	FILE* fp = NULL;
	VecString vecLine;
	MapTaggedLine mapTaggedLine;

	char* szFile = argv[1];
	//char* szFile = "test.h";//todok 디버그용
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

	// 스왑 토큰이 있는 라인을 모두 찾는다
	mapTaggedLine.clear();
	char szTag[1024];
	int numLine = (int)vecLine.size();
	size_t pos = string::npos;
	for (int i=0; i<numLine; ++i)
	{
		pos = vecLine[i].find(TOK_SWAP);
		if (pos != string::npos)
		{
			// 토큰 시작지점에서 파싱
			const char* szTokFound = vecLine[i].c_str() + pos;
			if (1 != sscanf(szTokFound, SWAPFORMAT, szTag))
			{
				printf("토큰을 발견했으나. 파싱 에러\n=> %s\n", vecLine[i].c_str());
				goto TERMINATE;
			}
			mapTaggedLine[szTag].push_back(i);	// 라인 번호 기억해두기
		}
	}

	if (mapTaggedLine.empty())
	{
		printf("이 파일엔 토큰이 없어요...\n");
		goto TERMINATE;
	}

	// 같은 태그를 가진 라인끼리 섞는다
	srand(timeGetTime());
	for (ItorTaggedLine it=mapTaggedLine.begin(); it!=mapTaggedLine.end(); ++it)
	{
		const string& strTag = it->first;
		VecInt& vecLineIdx = it->second;
		//printf("%s\n", strTag.c_str()); //todok 디버그용

		int num = (int)vecLineIdx.size();
		if (num < 2)
			continue;

		for (int repeat=0; repeat<2; ++repeat)	// 몇번 반복해주자
		{
			// 인덱스 i의 줄과 i가 아닌 임의의 인덱스 k의 줄의 내용을 서로 맞바꾼다
			for (int i=0; i<num; ++i)
			{
				int k = i;
				while (k == i)
					k = rand() % num;

				int lineIdx1 = vecLineIdx[i];
				int lineIdx2 = vecLineIdx[k];
				//printf("\t%s\n\t%s\n", vecLine[lineIdx1].c_str(), vecLine[lineIdx2].c_str()); //todok 디버그용
				_SwapString(vecLine[lineIdx1], vecLine[lineIdx2]);
			}
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

	printf("라인 자리 바꾸기 완료...\n");



TERMINATE:
	printf("\n\n");
	system("pause");
	return 0;
}

