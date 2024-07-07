
// ArticGUICompilerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArticGUICompiler.h"
#include "ArticGUICompilerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <experimental/filesystem>


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


// CArticGUICompilerDlg dialog



CArticGUICompilerDlg::CArticGUICompilerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ARTICGUICOMPILER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CArticGUICompilerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, DecompileList);
	DDX_Control(pDX, IDC_LIST2, CompileList);
}

BEGIN_MESSAGE_MAP(CArticGUICompilerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CArticGUICompilerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CArticGUICompilerDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CArticGUICompilerDlg::OnBnClickedButton3)
END_MESSAGE_MAP()


// CArticGUICompilerDlg message handlers

BOOL CArticGUICompilerDlg::OnInitDialog()
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

	ShowWindow(SW_MINIMIZE);

	// TODO: Add extra initialization here
	FillListBoxes();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CArticGUICompilerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CArticGUICompilerDlg::OnPaint()
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
HCURSOR CArticGUICompilerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CArticGUICompilerDlg::OnBnClickedButton1()
{
	int selCount = DecompileList.GetSelCount();
	CArray<int, int> arrayListSel;
	arrayListSel.SetSize(selCount);
	DecompileList.GetSelItems(selCount, arrayListSel.GetData());

	for (int i = 0; i < selCount; ++i)
	{
		m_fileList = new MArticFileList();

		CString strAppDir;
		GetModuleFileName(NULL, CStrBuf(strAppDir, MAX_PATH), MAX_PATH);
		::PathRemoveFileSpec(CStrBuf(strAppDir, MAX_PATH));
		CString fileName;
		DecompileList.GetText(arrayListSel[i], fileName);
		CStringA fileNameA(fileName);

		strAppDir.Append("\\");
		strAppDir.Append(fileName);

		CStringA pathA(strAppDir);
		m_fileList->Decompress(pathA);

		if (m_fileList->GetListSize() > 0)
			delete m_fileList;
	}
	FillListBoxes();
}


void CArticGUICompilerDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here

			// Multi selections

	int selCount = CompileList.GetSelCount();
	CArray< int, int > arrayListSel;
	arrayListSel.SetSize(selCount);     // make room in array
	CompileList.GetSelItems(selCount,
		arrayListSel.GetData());    // copy data to array

	for (int i = 0; i < selCount; ++i)
	{
		m_fileList = new MArticFileList();

		CString strAppDir;
		GetModuleFileName(NULL, CStrBuf(strAppDir, MAX_PATH), MAX_PATH);
		::PathRemoveFileSpec(CStrBuf(strAppDir, MAX_PATH));
		CString fileName;
		CompileList.GetText(arrayListSel[i], fileName);
		CStringA fileNameA(fileName);

		strAppDir.Append("\\");
		strAppDir.Append(fileName);

		CStringA pathA(strAppDir);
		m_fileList->Compress(pathA, "");

		if (m_fileList->GetListSize() > 0)
			delete m_fileList;
	}
	FillListBoxes();
}

void CArticGUICompilerDlg::get_files(LPCTSTR const& s, LPCTSTR const& fileExt)
{
	for (auto& p : std::experimental::filesystem::directory_iterator(s))
	{
		if (std::experimental::filesystem::is_directory(p))
		{
			CompileList.AddString((LPCTSTR)(p.path().filename().string().c_str()));
		}
		else
		{
			string ext = p.path().filename().extension().string();
			if (ext == fileExt)
			{
				DecompileList.AddString((LPCTSTR)(p.path().filename().string().c_str()));
			}
		}
	}
}


void CArticGUICompilerDlg::FillListBoxes()
{
	CString strAppDir;
	GetModuleFileName(NULL, CStrBuf(strAppDir, MAX_PATH), MAX_PATH);
	::PathRemoveFileSpec(CStrBuf(strAppDir, MAX_PATH));

	DecompileList.ResetContent();
	CompileList.ResetContent();


	get_files(strAppDir,".afs");
}

void CArticGUICompilerDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	FillListBoxes();
}

BOOL CArticGUICompilerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (GetAsyncKeyState(VK_CONTROL) && 97)
		{
			CompileList.SelItemRange(TRUE, 0, CompileList.GetCount());
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}