#pragma once

using namespace std;
#include <string>
#include <list>
void getLastFolder(char **szOutExt, string szString);

/* CHUNK is the size of the memory chunk used by the zlib routines. */
#define CHUNK 0x4000

/* These are parameters to deflateInit2. See
http://zlib.net/manual.html for the exact meanings. */
#define WINDOWS_BITS 15
#define GZIP_ENCODING 16

//para V2 header partido en 2, inicio y intermedio

#pragma pack(1)

struct AFSHeader
{
	DWORD numFiles;
	DWORD fileListSize;
};

struct AFSListFileHeader
{
	unsigned long	bufferPos;
	unsigned short	nameLen;
	unsigned long	decFileSize;
	unsigned long	crc32dec;

	char *GetName   () const { return (char *)(this + 1);   }
};

struct AFSFileHeader
{
	unsigned long	crc32enc;
	unsigned long	crc32dec;
	unsigned long	encFileSize;
	unsigned long	decFileSize;
	unsigned short	fileNameLen;

	char *GetName   () const { return (char *)(this + 1);   }
};

#pragma pack()

class MArticFileSystem
{
private:
	
	list<AFSFileHeader*>		pFileList;
	list<string>				pReadedFiles;

public:

	string						szFinalName;

	void MakeFileHeader(string &out);
	void MakeSingleFileHeader(string *outStream, char *pEncFileBuffer, UINT nEncFileSize, char *pDecFileBuffer, UINT nDecFileSize, string szFilePath);

	void ListFiles(string folderPath, string partPath = "", string ext = "", string exclu = "");

	void readFile(const char *szFile, char **pBuffer, UINT *nSize);
	void createFile(string szFullPathFileName, char *pFileBuffer, int nFileSize);

	void Compress(const char *szDirPath, const char *szOutputPath, bool bUpdateFiles = false);
	void Decompress(const char *szFilePath);
	void DecompressRecurrent(const char *dirPath);
	void TemporalFix(const char *dirPath);

	void DecompressFile(const char *szFilePath, const char *szFileName);

	void MRS2AFS(const char *szDirBasePath);
};

class MArticFileList : public MArticFileSystem
{
private:
	list<string>	pReadedFiles;

public:
	int GetListSize()	{	return pReadedFiles.size();	}
};

