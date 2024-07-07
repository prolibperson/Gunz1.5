// MainDoc.cpp : CMainDoc 클래스의 구현
//

#include "stdafx.h"
#include "ItemThumbnailGenerator.h"

#include "MainDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainDoc

IMPLEMENT_DYNCREATE(CMainDoc, CDocument)

BEGIN_MESSAGE_MAP(CMainDoc, CDocument)
END_MESSAGE_MAP()


// CMainDoc 생성/소멸

CMainDoc::CMainDoc()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

CMainDoc::~CMainDoc()
{
}

BOOL CMainDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 여기에 다시 초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}




// CMainDoc serialization

void CMainDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 여기에 저장 코드를 추가합니다.
	}
	else
	{
		// TODO: 여기에 로딩 코드를 추가합니다.
	}
}


// CMainDoc 진단

#ifdef _DEBUG
void CMainDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMainDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMainDoc 명령
