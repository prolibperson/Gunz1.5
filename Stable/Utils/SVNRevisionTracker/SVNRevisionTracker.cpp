// SVNRevisionTracker.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include <string>
using namespace std;


#define TARGETNAME_SVNREVISION_CPP	"SVNRevision.cpp"


int ParseRevision(const char* pszFileName, string& strRevision);
bool BuildRevisionCPP(const string& strTargetFileName, const string& strRevision);


int _tmain(int argc, _TCHAR* argv[])
{
	if (argc <= 1)
	{
		printf("MAIET Tools - SVNRevisionTracker \n");
		printf("usage: SVNRevisionTracker [XMLFILENAME] \n");
		return -1;
	}

	const _TCHAR* pszFileName = argv[1];

	string strRevision;
	int nParseResult = ParseRevision(pszFileName, strRevision);
	if (nParseResult == 1)
	{
		BuildRevisionCPP(TARGETNAME_SVNREVISION_CPP, strRevision);
		return 0;
	}
	else
	{
		if (nParseResult == -1) 
		{
			printf("Load failed : '%s' \n", pszFileName);
			return -1;
		} 
		else
		{
			printf("Parse failed : '%s' \n", pszFileName);
			return -1;
		}
	}

	return 0;
}

// -1 : Load Failed
//  0 : Parse Failed
//  1 : Success
int ParseRevision(const char* pszFileName, string& strRevision)
{
	TiXmlDocument doc(pszFileName);
	if (doc.LoadFile() == false)
		return -1;	// Load Failed

	TiXmlElement* pElemInfo = doc.FirstChildElement("info");
	if (pElemInfo)
	{
		TiXmlElement* pElemEntry = pElemInfo->FirstChildElement("entry");
		if (pElemEntry)
		{
			TiXmlElement* pElemCommit = pElemEntry->FirstChildElement("commit");
			if (pElemCommit)
			{
				const char* pszRevision = pElemCommit->Attribute("revision");
				if (pszRevision)
				{
					strRevision = pszRevision;
					return 1;
				}
			}
		}
	}

	return 0;	// Parse Failed
}

bool BuildRevisionCPP(const string& strTargetFileName, const string& strRevision)
{
	string strOutput;
	strOutput = ""
		"int GlobalGetSVNRevision()" "\n"
		"{" "\n"
		"	return " + strRevision + ";" "\n"
		"}" "\n"
		;

//	OutputDebugString(strOutput.c_str());
//	OutputDebugString("\n");

	FILE* pFP = fopen(strTargetFileName.c_str(), "w+b");
	if (pFP == false) 
		return false;
	fprintf(pFP, strOutput.c_str());
	fclose(pFP);

	return true;
}
