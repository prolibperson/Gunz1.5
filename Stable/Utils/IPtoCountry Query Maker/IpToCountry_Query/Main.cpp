#include <stdio.h>
#include <iostream>
#include <windows.h>

#define FILE_LIMIT_QUERY_LINE 50000
char* g_csFileName = 0;

void Separator(char* szOutbuf, char* cSeparator)
{
	char *token = NULL;
	token = strtok(szOutbuf, cSeparator); // 첫번째 토큰 추출
	sprintf(szOutbuf, "%s", token);

	while ((token = strtok(NULL, cSeparator)) != NULL) // 토큰이 발생할 때까지 추출
		sprintf(szOutbuf, "%s%s", szOutbuf, token);
}

char* CharInsert(char* to, char* from, int pos, char* s) {
	strncpy(to,from,pos);
	strcpy(to+pos,s);
	strcpy(to+pos+strlen(s),from+pos);
	return to;
}

// 스트링에 "'"문자가 있으면 "'"를 한개 더 붙여준다(쿼리라서...)
void Insert_Char(char* szOutbuf)
{
	int nStrTemp = 0;
	char separator2[2] = {0};
	sprintf(separator2, "'");
	while( szOutbuf[nStrTemp] != NULL )
	{
		if(szOutbuf[nStrTemp] == separator2[0])
		{
			char tmp[256];
			CharInsert(tmp, szOutbuf, nStrTemp, "'");
			strcpy(szOutbuf, tmp);
			printf("%s\n", szOutbuf);
			++nStrTemp;
		}
		++nStrTemp;
	}
}

int g_nOutPutFileNumber = 1;

void OutPutFileName(char* csOutFileName, char* csInName)
{
	int nFileName = 0;
	while(1)
	{
		if(csInName[nFileName] == '.')
		{
			char buf[8] = {0};
			sprintf(buf, "%d", g_nOutPutFileNumber);
			CharInsert(csOutFileName, csInName, nFileName, buf);
			break;
		}
		++nFileName;
	}
	++g_nOutPutFileNumber;
}

main()
{
	FILE *rfp=NULL;
	FILE *wfp=NULL;
	rfp = fopen( "ip-to-country.csv" , "r" );
	if(!rfp)
	{
		::MessageBox(NULL, " 'ip-to-country.csv' File does not exist. ", "MessageBox", MB_OK);
		return;
	}


	char* csFileName = new char[32];
	OutPutFileName(csFileName, "ip-to-country.sql");
	wfp = fopen( csFileName , "w" );
	// 결과창에 각 라인정보를 그리지 않는다.
	fputs("SET NOCOUNT ON;\n\n", wfp);
	// 일단 IPtoCountry를 지워준다.
	fputs("DELETE IPtoCountry\n\n", wfp);

	char szbuf[256] = {0};
	int i = 0;	int j = 0;
	unsigned long ulLineCount = 0;
	while ( !feof(rfp) )
	{
		fgets( szbuf , sizeof(szbuf) , rfp );
		if(strlen(szbuf) < 1)	continue; // 비어있는 라인일때
		szbuf[strlen(szbuf)-1] = 0x00;
		/////////////////////////////////////////////////////////////////////////////////
		// 예) szbuf == "1040982608","1040982615","CI","CIV","COTE D'IVOIRE"
		/////////////////////////////////////////////////////////////////////////////////


		// 스트링에서 '"'를 축출
		char separator[2] = {'"', 0};
		Separator(szbuf, separator);
		/////////////////////////////////////////////////////////////////////////////////
		// 예) szbuf == 1040982608,1040982615,CI,CIV,COTE D'IVOIRE
		/////////////////////////////////////////////////////////////////////////////////


		// 스트링에 "'"문자가 있으면 "'"를 한개 더 붙여준다(쿼리라서...)
		Insert_Char(szbuf);
		/////////////////////////////////////////////////////////////////////////////////
		// 예) szbuf == 1040982608,1040982615,CI,CIV,COTE D''IVOIRE
		/////////////////////////////////////////////////////////////////////////////////

		char csCharArr[5][64];
		while(1)
		{	// csCharArr에 각 스트링 세팅
			if(ulLineCount >= FILE_LIMIT_QUERY_LINE)
			{	// SQL에서 라인이 10만이상되면 오버플로우가 걸림
				fclose(wfp);
				OutPutFileName(csFileName, "ip-to-country.sql");
				wfp = fopen( csFileName , "w" );
				// 결과창에 각 라인정보를 그리지 않는다.
				fputs("SET NOCOUNT ON;\n\n", wfp);
				ulLineCount = 0;
			}

			if(szbuf[i] == ',')
			{
				if(j == 4)
				{	// 마지막 인자는 바로 복사해준다(마지막 인자중에 ','문자가 있는경우 때문에)
					strcpy(csCharArr[j], szbuf);
					j = i = 0;
					break;
				}
				strncpy(csCharArr[j], szbuf, i);
				csCharArr[j][i]=0;

				strcpy(szbuf, szbuf+(i+1));
				++j;
				i = 0;
				/////////////////////////////////////////////////////////////////////////////////
				// 예) csCharArr[0]=1040982608, csCharArr[1]=1040982615, csCharArr[2]=CI
				//		, csCharArr[3]=CIV, csCharArr[4]=COTE D''IVOIRE
				/////////////////////////////////////////////////////////////////////////////////
			}

			if(szbuf[i] == 0x00)
			{
				strncpy(csCharArr[j], szbuf, i);
				csCharArr[j][i]=0;
				break;
			}
			++i;
		}
		
		sprintf(szbuf, "INSERT INTO IPtoCountry(IPFrom, IPTo, CountryCode2, CountryCode3, CountryName) Values (%s, %s, '%s', '%s', '%s')\n", 
			csCharArr[0], csCharArr[1], csCharArr[2], csCharArr[3], csCharArr[4]);

		fputs(szbuf , wfp);
		szbuf[0] = 0x00;
		j = i = 0;
		++ulLineCount;
	}

	if(csFileName)
		delete []csFileName;

	fclose(rfp);
	fclose(wfp);

	return true;
}
