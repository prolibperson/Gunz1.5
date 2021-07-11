#include "stdafx.h"
#include "MArticFileSystem.h"
#include "MCRC32.h"
#include <iostream>
#include <algorithm>
#include <sys/types.h>
#include <sys/stat.h>
#include <tlhelp32.h>

#include "..\..\sdk\zlib\include\zlib.h"
#include <list>
using namespace std;

bool GzipCompress(string strBuff, string &strBuffOut)
{
	char* comp = new char[CHUNK];

	int have;
	int status;
	z_stream strm;
	strm.next_in = (Bytef *)strBuff.c_str();
	strm.avail_in = strBuff.size();
	strm.total_out = 0;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	if (deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, WINDOWS_BITS | GZIP_ENCODING, 8, Z_DEFAULT_STRATEGY) != Z_OK)
	{
		delete[]comp;
		return false;
	}

	do {
		strm.next_out = (Bytef *)comp;
		strm.avail_out = CHUNK;

		status = deflate(&strm, Z_FINISH);

		if (status < 0)
		{
			printf("%s:%d: returned a bad status of %d.\n", __FILE__, __LINE__, status);
			delete[]comp;
			return false;
		}
		have = CHUNK - strm.avail_out;
		strBuffOut += string(comp, have);
	} while (strm.avail_out == 0);

	if (deflateEnd(&strm) != Z_OK)
	{
		delete[]comp;
		return false;
	}

	delete[]comp;
	return true;
}

bool GzipDecompress(string strBuff, string &strBuffOut)
{
	unsigned full_length = strBuff.size();
	unsigned half_length = strBuff.size() / 2;

	unsigned uncompLength = full_length;
	char* uncomp = new char[uncompLength];

	z_stream strm;
	strm.next_in = (Bytef *)strBuff.c_str();
	strm.avail_in = strBuff.size();
	strm.total_out = 0;
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;

	bool done = false;

	if (inflateInit2(&strm, WINDOWS_BITS | GZIP_ENCODING) != Z_OK)
	{
		delete[]uncomp;
		return false;
	}

	while (!done)
	{
		// If our output buffer is too small
		if (strm.total_out >= uncompLength)
		{
			// Increase size of output buffer  
			char* uncomp2 = new char[uncompLength + half_length];
			memcpy(uncomp2, uncomp, uncompLength);
			uncompLength += half_length;
			delete[]uncomp;
			uncomp = uncomp2;
		}

		strm.next_out = (Bytef *)(uncomp + strm.total_out);
		strm.avail_out = uncompLength - strm.total_out;

		// Inflate another chunk.  
		int err = inflate(&strm, Z_SYNC_FLUSH);
		if (err == Z_STREAM_END)
			done = true;
		else if (err != Z_OK)
		{
			break;
		}
	}

	if (inflateEnd(&strm) != Z_OK)
	{
		delete[]uncomp;
		return false;
	}

	strBuffOut = string(uncomp, strm.total_out);

	delete[]uncomp;

	return true;
}

void ConvertChar(char* pData, int _size)
{
	if (!pData) return;

	WORD w;
	BYTE b, bh;

	for (int i = 0; i < _size; i++)
	{
		b = *pData ^ 0xFF;
		w = b << 3;
		bh = (w & 0xff00) >> 8;
		b = w & 0xff;
		*pData = BYTE(b | bh);
		pData++;

	}
}

void RecoveryChar(char* pData, int _size)
{
	if (!pData) return;

	BYTE b, bh, d;

	for (int i = 0; i < _size; i++)
	{
		b = *pData;
		bh = b & 0x07;
		d = (bh << 5) | (b >> 3);
		*pData = d ^ 0xff;
		pData++;
	}
}

void getExtension(string &szOutExt, char *szString)
{
	int nLenght = strlen((char *)szString);
	for (int i = nLenght - 1; i >= 0; i--)
	{
		if (szString[i] == '.')
		{
			szOutExt = string(szString + i + 1);
			return;
		}
	}
}

void getLastFolder(string& szOutExt, string szString)
{
	string localString = szString;
	int len = szString.length();

	if (localString[len - 1] == '\\' || localString[len - 1] == '/')
		localString[len - 1] = 0;

	std::size_t found = localString.find_last_of("/\\");

	int foundIndex;

	if (found == -1)
		foundIndex = 0;
	else
		foundIndex = found + 1;

	szOutExt = localString.substr(foundIndex);
}

void getPath(string& szOutExt, string szString)
{
	string localString = szString;
	int len = szString.length();

	if (localString[len - 1] == '\\' || localString[len - 1] == '/')
		localString[len - 1] = 0;

	std::size_t found = localString.find_last_of("/\\");

	szOutExt = localString.substr(0, found);
}

void getPathWithoutExt(string &output, const char *szString)
{
	if (strlen(szString) == 0)
		return;

	char *out = nullptr;
	int nLenght = strlen(szString);
	for (int i = nLenght - 1; i >= 0; i--)
	{
		if (szString[i] == '.')
		{
			out = new char[i];
			memcpy(out, szString, i);
			out[i] = 0;
			break;
		}
	}
	output = string(out);
}

bool deleteLastFolder(char *szString)
{
	int nLenght = strlen((char *)szString);
	bool slashFound = false;
	for (int i = nLenght - 1; i >= 0; i--)
	{
		if (szString[i] == '\\' || szString[i] == '/')
		{
			slashFound = true;
			szString[i + 1] = 0;

			return true;
		}
	}

	if (!slashFound)
		szString[0] = 0;

	return false;
}

void deleteExt(char *szPath, string &out)
{
	int nLenght = strlen(szPath);
	char *crap = new char[nLenght];
	strcpy_s(crap, nLenght, szPath);
	for (int i = nLenght - 1; i >= 0; i--)
	{
		if (szPath[i] == '.')
		{
			crap[i] = 0;
			out = string(crap);
			break;
		}
	}
}

void MArticFileSystem::ListFiles(string folderPath, string partPath, string ext, string exclu)
{
	WIN32_FIND_DATAA		findFileData;
	HANDLE					hFind;
	string                  szPath;
	string                  szPatternPath;
	list<string>            folderList;
	string					szFilePath;
	string					szExt;
	string					szLastFolder;

	string					_folderPath = folderPath;
	string					_partPath = partPath;
	string					_ext = ext;
	string					_exclu = exclu;

	transform(_folderPath.begin(), _folderPath.end(), _folderPath.begin(), tolower);
	transform(_partPath.begin(), _partPath.end(), _partPath.begin(), tolower);
	transform(_ext.begin(), _ext.end(), _ext.begin(), tolower);
	transform(_exclu.begin(), _exclu.end(), _exclu.begin(), tolower);

	if (_folderPath == "")
		szPatternPath = ".\\*";
	else
		szPatternPath = _folderPath + "\\*";

	hFind = FindFirstFileA(szPatternPath.c_str(), &findFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		while (FindNextFileA(hFind, &findFileData) != 0)
		{
			szPath = _strlwr(findFileData.cFileName);

			if (szPath != "." && szPath != "..")
			{
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					folderList.push_back(_folderPath + "\\" + szPath);
				}
				else
				{
					getExtension(szExt, findFileData.cFileName);
					if (_exclu != "")
					{
						if (_exclu != szExt)
						{
							szFilePath = _partPath + szPath;
							pReadedFiles.push_back(szFilePath);
						}
					}
					else
					{
						if (_ext != "" && _ext == szExt)
						{
							szFilePath = _partPath + szPath;
							pReadedFiles.push_back(szFilePath);
						}
						else if (_ext == "")
						{
							szFilePath = _partPath + szPath;
							pReadedFiles.push_back(szFilePath);
						}
					}
				}
			}
		}
		FindClose(hFind);
	}

	string partPathExtended = _partPath;

	for (list<string>::iterator it = folderList.begin(); it != folderList.end(); it++)
	{
		getLastFolder(szLastFolder, (*it));
		_partPath = szLastFolder + "\\";

		if (partPathExtended == "")
			ListFiles((*it), _partPath, _ext);
		else
			ListFiles((*it), partPathExtended + _partPath, _ext);
	}
}

void MArticFileSystem::MakeSingleFileHeader(string *outStream, char *pEncFileBuffer, UINT nEncFileSize, char *pDecFileBuffer, UINT nDecFileSize, string szFilePath)
{
	int size = sizeof(AFSFileHeader) + szFilePath.length();

	char *pOutHeader = new char[size];
	memset(pOutHeader, 0, size);

	((AFSFileHeader *)(pOutHeader))->crc32enc = MCRC32::BuildCRC32((const UCHAR*)pEncFileBuffer, nEncFileSize);
	((AFSFileHeader *)(pOutHeader))->crc32dec = MCRC32::BuildCRC32((const UCHAR*)pDecFileBuffer, nDecFileSize);
	((AFSFileHeader *)(pOutHeader))->encFileSize = nEncFileSize;
	((AFSFileHeader *)(pOutHeader))->decFileSize = nDecFileSize;
	((AFSFileHeader *)(pOutHeader))->fileNameLen = szFilePath.length();

	memcpy(pOutHeader + sizeof(AFSFileHeader), szFilePath.c_str(), szFilePath.length());

	*outStream += string((const char*)pOutHeader, size);
	delete[]pOutHeader;
}

void MArticFileSystem::MakeFileHeader(string &out)
{
	string output;

	unsigned char *pListFileHeader;
	int nSize;
	list<AFSFileHeader*>::iterator it;
	UINT index = 0;

	for (it = pFileList.begin(); it != pFileList.end(); it++)
	{
		nSize = sizeof(AFSListFileHeader);
		pListFileHeader = new unsigned char[nSize];
		memset(pListFileHeader, 0, nSize);

		((AFSListFileHeader *)pListFileHeader)->bufferPos = index;
		((AFSListFileHeader *)pListFileHeader)->nameLen = (*it)->fileNameLen;
		((AFSListFileHeader *)pListFileHeader)->decFileSize = (*it)->decFileSize;
		((AFSListFileHeader *)pListFileHeader)->crc32dec = (*it)->crc32dec;

		output += string((const char*)pListFileHeader, nSize);
		output += string((*it)->GetName(), (*it)->fileNameLen);

		index += sizeof(AFSFileHeader) + (*it)->fileNameLen + (*it)->encFileSize;

		delete[](char*)(*it);
	}

	GzipCompress(output, out);
}

void MArticFileSystem::readFile(const char *szFile, char **pBuffer, UINT *nSize)
{
	struct stat st;
	stat(szFile, &st);
	*nSize = st.st_size;

	FILE* pFile;
	fopen_s(&pFile, szFile, "rb");

	*pBuffer = new char[*nSize];

	memset(*pBuffer, 0, *nSize);

	fread(*pBuffer, *nSize, 1, pFile);

	fclose(pFile);
}

void MArticFileSystem::createFile(string szFullPathFileName, char *pFileBuffer, int nFileSize)
{
	string szFilePath = szFullPathFileName;
	string szDirectory;
	int nPathLen = szFilePath.length();
	std::transform(szFilePath.begin(), szFilePath.end(), szFilePath.begin(), tolower);

	getPath(szDirectory, szFilePath);

	string szTreeFolder;
	char *szFolder;
	char szPartDirectory[MAX_PATH];

	strcpy(szPartDirectory, szDirectory.c_str());

	szFolder = strtok(szPartDirectory, "\\");
	do
	{
		if (szFolder)
		{
			szTreeFolder += string(szFolder) + "\\";
			if (!CreateDirectoryA(szTreeFolder.c_str(), NULL))
			{
				DWORD dError = GetLastError();
				if (dError == ERROR_PATH_NOT_FOUND)
					cout << "FAILL" << endl;
			}
		}
	} while (szFolder = strtok(NULL, "\\"));

	FILE* pFile;

	fopen_s(&pFile, szFilePath.c_str(), "wb");

	fwrite(pFileBuffer, nFileSize, 1, pFile);

	fclose(pFile);
}

void MArticFileSystem::Compress(const char *szDirPath, const char *szOutputPath, bool bUpdateFiles)
{
	string finalFile;
	string holyShit;

	char *pBuffer;
	UINT size;

	char *pEncBuffer;
	size_t nEncSize;

	ListFiles(szDirPath, "", "", "mrs");

	size_t			nProps;
	string			szHeaderBuffer;

	char *encHeader;
	char *plainHeader;

	int i = 0;
	for (list<string>::iterator it = pReadedFiles.begin(); it != pReadedFiles.end(); it++)
	{
		holyShit = szDirPath;
		holyShit += "\\" + (*it);
		MArticFileSystem::readFile(holyShit.c_str(), &pBuffer, &size);

		string outBuff;
		GzipCompress(string(pBuffer, size), outBuff);

		nEncSize = outBuff.length();

		pEncBuffer = new char[nEncSize];
		memset(pEncBuffer, 0, nEncSize);

		memcpy(pEncBuffer, outBuff.c_str(), outBuff.length());

		i++;
		cout << "Compressing: " << (*it).c_str() << endl;
		MArticFileSystem::MakeSingleFileHeader(&szHeaderBuffer, pEncBuffer, nEncSize, pBuffer, size, (*it));

		encHeader = new char[szHeaderBuffer.length()];
		memcpy(encHeader, szHeaderBuffer.c_str(), szHeaderBuffer.length());
		ConvertChar(encHeader, szHeaderBuffer.length());

		plainHeader = new char[szHeaderBuffer.length() + 1];
		memcpy(plainHeader, szHeaderBuffer.c_str(), szHeaderBuffer.length());
		((AFSFileHeader *)plainHeader)->GetName()[((AFSFileHeader *)plainHeader)->fileNameLen] = 0;

		finalFile += string(encHeader, szHeaderBuffer.length());
		finalFile += string(pEncBuffer, nEncSize);

		getLastFolder(szFinalName, szDirPath);

		if (bUpdateFiles)
		{
			string singleFile = string(plainHeader, szHeaderBuffer.length());
			singleFile += string(pEncBuffer, nEncSize);

			int nUpdateBuffer = singleFile.length();
			char *szUpdateBuffer = new char[nUpdateBuffer];
			memcpy(szUpdateBuffer, singleFile.c_str(), nUpdateBuffer);

			ConvertChar(szUpdateBuffer, nUpdateBuffer);

			createFile("updatefiles\\" + szFinalName + "\\" + ((AFSFileHeader *)plainHeader)->GetName(), szUpdateBuffer, nUpdateBuffer);
		}

		pFileList.push_back((AFSFileHeader *)plainHeader);

		delete[]encHeader;
		delete[]pEncBuffer;
		delete[]pBuffer;
		szHeaderBuffer.clear();
	}

	if (i != pReadedFiles.size())
	{
		cout << "Fail packing files" << endl;
		system("pause");
		return;
	}

	string fileList;
	MakeFileHeader(fileList);

	finalFile = fileList + finalFile;

	AFSHeader *head = new AFSHeader();
	head->numFiles = (i);
	head->fileListSize = fileList.length();

	finalFile = string((const char*)head, sizeof(AFSHeader)) + finalFile;

	string szFileName = ".\\";

	char tmp[MAX_PATH];
	strcpy(tmp, szDirPath);

	if (deleteLastFolder(tmp))
	{
		szFileName = string(tmp) + "\\";
	}

	szFileName += string(szOutputPath) + "\\" + string(szFinalName) + ".afs";
	std::transform(szFileName.begin(), szFileName.end(), szFileName.begin(), ::tolower);

	createFile(szFileName, (char*)finalFile.c_str(), finalFile.length());
}

void MArticFileSystem::Decompress(const char *szFilePath)
{
	char *listFile;
	char *list;

	string fullList;
	int readIndex = 0;

	AFSHeader *header = new AFSHeader();

	FILE *pFile;
	fopen_s(&pFile, szFilePath, "rb");

	if (!pFile) return;

	fread(header, sizeof(AFSHeader), 1, pFile);

	char *crap = new char[header->fileListSize];
	fread(crap, header->fileListSize, 1, pFile);

	string aww = string(crap, header->fileListSize);
	GzipDecompress(aww, fullList);

	AFSListFileHeader ** fileList;
	fileList = new AFSListFileHeader*[header->numFiles];

	for (int i = 0; i < (UINT)header->numFiles; i++)
	{
		fileList[i] = new AFSListFileHeader();

		memcpy(fileList[i], (void *)(fullList.c_str() + readIndex), sizeof(AFSListFileHeader));
		readIndex += sizeof(AFSListFileHeader);

		listFile = new char[sizeof(AFSListFileHeader) + fileList[i]->nameLen];

		memcpy(listFile, fileList[i], sizeof(AFSListFileHeader));

		memcpy(listFile + sizeof(AFSListFileHeader), (void *)(fullList.c_str() + readIndex), fileList[i]->nameLen);
		readIndex += fileList[i]->nameLen;
		delete[]listFile;
	}

	int lol = ftell(pFile) - sizeof(AFSHeader);
	if (lol == header->fileListSize)
		cout << "LOOOL" << endl;

	AFSFileHeader ** fileHeaders;
	fileHeaders = new AFSFileHeader*[header->numFiles];

	char *pFileBuffer;
	char *decFileBuffer;
	size_t outFileSize;

	for (int i = 0; i < (UINT)header->numFiles; i++)
	{
		fileHeaders[i] = new AFSFileHeader();
		fread(fileHeaders[i], sizeof(AFSFileHeader), 1, pFile);

		RecoveryChar((char *)fileHeaders[i], sizeof(AFSFileHeader));
		list = new char[sizeof(AFSFileHeader) + fileHeaders[i]->fileNameLen + 1];
		memset(list, 0, sizeof(AFSFileHeader) + fileHeaders[i]->fileNameLen);
		memcpy(list, fileHeaders[i], sizeof(AFSFileHeader));

		fread(list + sizeof(AFSFileHeader), fileHeaders[i]->fileNameLen, 1, pFile);

		RecoveryChar(list + sizeof(AFSFileHeader), fileHeaders[i]->fileNameLen);
		list[sizeof(AFSFileHeader) + fileHeaders[i]->fileNameLen] = 0;
		cout << "Decompressing: " << ((AFSFileHeader*)list)->GetName() << endl;

		pFileBuffer = new char[fileHeaders[i]->encFileSize];
		decFileBuffer = new char[fileHeaders[i]->decFileSize];
		memset(pFileBuffer, 0, fileHeaders[i]->encFileSize);
		memset(decFileBuffer, 0, fileHeaders[i]->decFileSize);
		UINT size = fileHeaders[i]->encFileSize;
		outFileSize = 0;

		fread(pFileBuffer, fileHeaders[i]->encFileSize, 1, pFile);

		string outBuff;
		GzipDecompress(string(pFileBuffer, size), outBuff);
		memcpy(decFileBuffer, outBuff.c_str(), outBuff.length());
		fileHeaders[i]->decFileSize = outBuff.length();

		getPathWithoutExt(szFinalName, szFilePath);

		string fullPath = szFinalName + "\\" + string(((AFSFileHeader*)list)->GetName());

		createFile(fullPath, decFileBuffer, fileHeaders[i]->decFileSize);

		delete[] list;
		delete[] decFileBuffer;
		delete[] pFileBuffer;
	}
	fclose(pFile);
}

void MArticFileSystem::DecompressRecurrent(const char *dirPath)
{
	ListFiles(dirPath, "", "afs");

	string tmp;
	for (list<string>::iterator it = pReadedFiles.begin(); it != pReadedFiles.end(); it++)
	{
		tmp = "\"" + (*it) + "\"";
		Decompress((*it).c_str());
	}
}

void MArticFileSystem::TemporalFix(const char *dirPath)
{
	ListFiles(dirPath, "", "");

	string tmp;
	for (list<string>::iterator it = pReadedFiles.begin(); it != pReadedFiles.end(); it++)
	{
		tmp = dirPath + string("\\") + (*it);

		char* pBuffer;
		UINT size;

		MArticFileSystem::readFile(tmp.c_str(), &pBuffer, &size);

		AFSFileHeader *head = new AFSFileHeader;
		memcpy(head, pBuffer, sizeof(AFSFileHeader));

		RecoveryChar((char*)head, sizeof(AFSFileHeader));

		char *fullHead = new char[sizeof(AFSFileHeader) + head->fileNameLen + 1];
		memcpy(fullHead, head, sizeof(AFSFileHeader));
		memcpy(fullHead + sizeof(AFSFileHeader), pBuffer + sizeof(AFSFileHeader), head->fileNameLen);

		RecoveryChar((char*)fullHead + sizeof(AFSFileHeader), head->fileNameLen);

		char *name = (char*)fullHead + sizeof(AFSFileHeader);

		for (int i = 0; i < head->fileNameLen; i++)
		{
			name[i] = tolower(name[i]);
		}

		name[head->fileNameLen] = 0;

		ConvertChar(name, head->fileNameLen);

		memcpy(pBuffer + sizeof(AFSFileHeader), name, head->fileNameLen);

		string buf = string(fullHead, sizeof(AFSFileHeader) + head->fileNameLen);

		size_t a = (*it).length();
		createFile("updatefiles\\" + (*it), pBuffer, size);
	}
}

void MArticFileSystem::DecompressFile(const char *szFilePath, const char *szFileName)
{
	char *list;
	AFSHeader *header = new AFSHeader();

	FILE *pFile;
	fopen_s(&pFile, szFilePath, "rb");
	fread(header, sizeof(AFSHeader), 1, pFile);

	AFSListFileHeader *file = new AFSListFileHeader();
	char ** fileList;
	fileList = new char*[header->numFiles];

	for (int i = 0; i < (UINT)header->numFiles; i++)
	{
		fread(file, sizeof(AFSListFileHeader), 1, pFile);
		fileList[i] = new char[sizeof(AFSListFileHeader) + file->nameLen];
		memcpy(fileList[i], file, sizeof(AFSListFileHeader));
		fread(fileList[i] + sizeof(AFSListFileHeader), file->nameLen, 1, pFile);
		fileList[i][sizeof(AFSListFileHeader) + file->nameLen] = 0;
	}

	int lol = ftell(pFile) - sizeof(AFSHeader);
	if (lol == header->fileListSize)
		cout << "LOOOL" << endl;

	AFSFileHeader * fileHeaders;

	char *pFileBuffer;
	char *decFileBuffer;

	for (int i = 0; i < (UINT)header->numFiles; i++)
	{
		if (strstr(((AFSListFileHeader*)fileList[i])->GetName(), szFileName))
		{
			fseek(pFile, sizeof(AFSHeader) + header->fileListSize + ((AFSListFileHeader*)fileList[i])->bufferPos, SEEK_SET);

			fileHeaders = new AFSFileHeader();
			fread(fileHeaders, sizeof(AFSFileHeader), 1, pFile);
			list = new char[sizeof(AFSFileHeader) + fileHeaders->fileNameLen];
			memset(list, 0, sizeof(AFSFileHeader) + fileHeaders->fileNameLen);
			memcpy(list, fileHeaders, sizeof(AFSFileHeader));
			fread(list + sizeof(AFSFileHeader), fileHeaders->fileNameLen, 1, pFile);
			list[sizeof(AFSFileHeader) + fileHeaders->fileNameLen] = 0;
			cout << ((AFSFileHeader*)list)->GetName() << endl;

			pFileBuffer = new char[fileHeaders->encFileSize];
			decFileBuffer = new char[fileHeaders->decFileSize];
			memset(pFileBuffer, 0, fileHeaders->encFileSize);
			memset(decFileBuffer, 0, fileHeaders->decFileSize);
			UINT size = fileHeaders->encFileSize - 5;

			fread(pFileBuffer, fileHeaders->encFileSize, 1, pFile);

			createFile(((AFSFileHeader*)list)->GetName(), decFileBuffer, fileHeaders->decFileSize);
			break;
		}
	}
	fclose(pFile);
}

bool IsProcessRunning(const char *szProcName)
{
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe32;
		pe32.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hProcessSnap, &pe32))
		{
			do
				if (!strstr(pe32.szExeFile, szProcName))
					return false;
			while (Process32Next(hProcessSnap, &pe32));
		}
	}
	CloseHandle(hProcessSnap);

	return true;
}

void MArticFileSystem::MRS2AFS(const char *szDirBasePath)
{
	ListFiles(szDirBasePath, "", "mrs");

	string tmpShit;
	char *tmp;

	for (list<string>::iterator it = pReadedFiles.begin(); it != pReadedFiles.end(); it++)
	{
		tmpShit = "mrs.exe d \"" + string(szDirBasePath) + "\\" + (*it) + "\"";
		system(tmpShit.c_str());
		tmpShit = "del \"" + string(szDirBasePath) + "\\" + (*it) + "\"";
		system(tmpShit.c_str());
		tmpShit.clear();
	}

	for (list<string>::iterator it = pReadedFiles.begin(); it != pReadedFiles.end(); it++)
	{
		tmp = new char[(*it).length()];
		memcpy(tmp, (*it).c_str(), (*it).length());

		deleteExt(tmp, tmpShit);

		tmpShit = "ArticFileSystem.exe e \"" + string(szDirBasePath) + "\\" + tmpShit + "\"";
		system(tmpShit.c_str());

		tmpShit.clear();
		delete[]tmp;
	}
}