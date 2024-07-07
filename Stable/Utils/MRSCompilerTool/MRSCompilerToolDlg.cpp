
// MRSCompilerToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MRSCompilerTool.h"
#include "MRSCompilerToolDlg.h"
#include "afxdialogex.h"
#include "MGZip.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMRSCompilerToolDlg dialog



CMRSCompilerToolDlg::CMRSCompilerToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MRSCOMPILERTOOL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMRSCompilerToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_DecompileList);
	DDX_Control(pDX, IDC_LIST2, m_CompileList);
	DDX_Control(pDX, IDC_BUTTON1, m_decompileButton);
}

BEGIN_MESSAGE_MAP(CMRSCompilerToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMRSCompilerToolDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMRSCompilerToolDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CMRSCompilerToolDlg message handlers

BOOL CMRSCompilerToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	FillListBoxes();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMRSCompilerToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMRSCompilerToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMRSCompilerToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMRSCompilerToolDlg::OnBnClickedButton1()
{
	int selCount = m_DecompileList.GetSelCount();
	// TODO: Add your control notification handler code here
	for (int i = 0; i < selCount; ++i)
	{
		CString strAppDir = L"";
		GetModuleFileName(NULL, CStrBuf(strAppDir, MAX_PATH), MAX_PATH);
		::PathRemoveFileSpec(CStrBuf(strAppDir, MAX_PATH));
		CString fileName = L"";
		m_DecompileList.GetText(i, fileName);
		CStringA fileNameA(fileName);

		strAppDir.Append(L"\\");
		strAppDir.Append(fileName);

		CStringA pathA(strAppDir);
		MZip::RecoveryZip((const char*)pathA);

		CStringA zipName = fileNameA;
		zipName.Replace(".mrs", ".zip");

		rename(fileNameA, zipName);
	}
	FillListBoxes();
}

void CMRSCompilerToolDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	int selCount = m_CompileList.GetSelCount();
	for (int i = 0; i < selCount; ++i)
	{
		CString strAppDir;
		GetModuleFileName(NULL, CStrBuf(strAppDir, MAX_PATH), MAX_PATH);
		::PathRemoveFileSpec(CStrBuf(strAppDir, MAX_PATH));
		CString fileName;
		m_CompileList.GetText(i, fileName);
		CStringA fileNameA(fileName);

		strAppDir.Append(L"\\");
		strAppDir.Append(fileName);

		CStringA pathA(strAppDir);
		MZip::ConvertZip(const_cast<char*>((const char*)pathA));

		CStringA mrsName = fileNameA;
		mrsName.Replace(".zip", ".mrs");

		rename(fileNameA, mrsName);
	}
	FillListBoxes();
}

void CMRSCompilerToolDlg::FillListBoxes()
{
	m_DecompileList.ResetContent();
	m_CompileList.ResetContent();
	CFileFind mrsFiles;
	BOOL isFinished = mrsFiles.FindFile(L"*.mrs");
	while (isFinished)
	{
		isFinished = mrsFiles.FindNextFileW();
		m_DecompileList.AddString(mrsFiles.GetFileName());
	}

	mrsFiles.Close();

	isFinished = mrsFiles.FindFile(L"*.zip");
	while (isFinished)
	{
		isFinished = mrsFiles.FindNextFileW();
		m_CompileList.AddString(mrsFiles.GetFileName());
	}
	mrsFiles.Close();
}