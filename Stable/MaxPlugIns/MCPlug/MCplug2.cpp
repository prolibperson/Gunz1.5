/**********************************************************************
 *<
	FILE: MCplug2.cpp

	DESCRIPTION:	Appwizard generated plugin

	CREATED BY: 

	HISTORY: 

 *>	Copyright (c) 1997, All Rights Reserved.
 **********************************************************************/

#include "MCplug2.h"
#include "stdmat.h"	// maxsdk

#define MCPLUG2_CLASS_ID	Class_ID(0x501e02c0, 0x1a538efa)

#define KEY_GAP				1

HINSTANCE hInstance;
int controlsInit = FALSE;

BOOL WINAPI DllMain(HINSTANCE hinstDLL,ULONG fdwReason,LPVOID lpvReserved)
{
	hInstance = hinstDLL;				// Hang on to this DLL's instance handle.

	if (!controlsInit) 
	{
		controlsInit = TRUE;
#if MAX_VERSION_MAJOR <= 11
		InitCustomControls(hInstance);	// Initialize MAX's custom controls
#endif
		InitCommonControls();			// Initialize Win95 controls
	}
			
	return (TRUE);
}

__declspec( dllexport ) const WCHAR* LibDescription()
{
	return GetString(IDS_LIBDESCRIPTION);
}

__declspec( dllexport ) int LibNumberClasses()
{
	return 1;
}

__declspec( dllexport ) ClassDesc* LibClassDesc(int i)
{
	switch(i) 
	{
		case 0: return GetMCplug2Desc();
		default: return 0;
	}
}

__declspec( dllexport ) ULONG LibVersion()
{
	return VERSION_3DSMAX;
}

WCHAR *GetString(int id)
{
	static WCHAR buf[256];

	if (hInstance)
		return LoadStringW(hInstance, id, buf, sizeof(buf)) ? buf : NULL;
	return NULL;
}


/////////////////////////////////////////////////////////
//

MCplug2::MCplug2()
{
	bAlwaysSample			= true;
	nKeyFrameStep			= KEY_GAP;
	nMeshFrameStep			= KEY_GAP;
	nPrecision				= KEY_GAP;
	nStaticFrame			= 0;
}

MCplug2::~MCplug2()
{
}

int MCplug2::ExtCount() 
{	
	return 1;
}

const WCHAR *MCplug2::Ext(int n)
{		
	switch(n) 
	{
	case 0:
		return _T(L"elu");
	}
	return _T(L"");
}

const WCHAR *MCplug2::LongDesc()
{
	return GetString(IDS_LONGDESC);
}
	
const WCHAR *MCplug2::ShortDesc()
{			
	return GetString(IDS_SHORTDESC);
}

const WCHAR *MCplug2::AuthorName()
{			
	return _T(L"elhorus");
}

const WCHAR *MCplug2::CopyrightMessage()
{	
	return GetString(IDS_COPYRIGHT);
}

const WCHAR *MCplug2::OtherMessage1()
{		
	return _T(L"");
}

const WCHAR *MCplug2::OtherMessage2()
{		
	return _T(L"");
}

unsigned int MCplug2::Version()
{				
	return 100;
}

void MCplug2::ShowAbout(HWND hWnd)
{			
}

class MCplug2ClassDesc:public ClassDesc2 {
	public:
	int 			IsPublic() {return 1;}
	void *			Create(BOOL loading = FALSE) 
	{
		return new MCplug2;
	}
	const WCHAR *	ClassName()		{return GetString(IDS_CLASS_NAME);}
	SClass_ID		SuperClassID()	{return SCENE_EXPORT_CLASS_ID;}
	Class_ID		ClassID()		{return MCPLUG2_CLASS_ID;}
	const WCHAR* 	Category()		{return GetString(IDS_CATEGORY);}
	const WCHAR*	InternalName()	{ return _T(L"MCplug2"); }	// returns fixed parsable name (scripter-visible name)
	HINSTANCE		HInstance()		{ return hInstance; }		// returns owning module handle
};

static MCplug2ClassDesc MCplug2Desc;
ClassDesc2* GetMCplug2Desc() {return &MCplug2Desc;}

BOOL CALLBACK MCplug2OptionsDlgProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam) {
	static MCplug2 *imp = NULL;

	switch(message) 
	{
		case WM_INITDIALOG:
			imp = (MCplug2 *)lParam;
			CenterWindow(hWnd,GetParent(hWnd));
			return TRUE;

		case WM_CLOSE:
			EndDialog(hWnd, 0);
			return TRUE;
	}
	return FALSE;
}

bool g_bExport_DebugMesh_Out, g_Debug_Text_Out, g_MeshOut, g_AniOut, g_VertexAni_Out, g_tm_Ani_out;
INT_PTR CALLBACK EluDlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Interval animRange;
	switch (msg) {

		case WM_INITDIALOG:
			Button_SetCheck(GetDlgItem(hWnd, IDC_DEBUG_BIP_MESH_OUT), g_bExport_DebugMesh_Out); 
			Button_SetCheck(GetDlgItem(hWnd, IDC_DEBUG_TEXT_OUT)	, g_Debug_Text_Out);
			Button_SetCheck(GetDlgItem(hWnd, IDC_MESH_OUT)			, g_MeshOut);
			Button_SetCheck(GetDlgItem(hWnd, IDC_ANI_OUT)			, g_AniOut);
			Button_SetCheck(GetDlgItem(hWnd, IDC_VERTEX_ANI_OUT)		, g_VertexAni_Out);
			Button_SetCheck(GetDlgItem(hWnd, IDC_TM_ANI_OUT)		, g_tm_Ani_out);


			CenterWindow(hWnd, GetParent(hWnd));


			break;

		case WM_COMMAND:
			switch (LOWORD(wParam)) 
			{
			case IDOK:

				g_bExport_DebugMesh_Out = (Button_GetCheck(GetDlgItem(hWnd, IDC_DEBUG_BIP_MESH_OUT)) != FALSE);// (GetButtonCheck(hWnd, IDC_DEBUG_BIP_MESH_OUT));
				g_Debug_Text_Out = (Button_GetCheck(GetDlgItem(hWnd, IDC_DEBUG_TEXT_OUT))!= FALSE); 
				g_MeshOut = (Button_GetCheck(GetDlgItem(hWnd, IDC_MESH_OUT))!= FALSE); 
				g_AniOut = (Button_GetCheck(GetDlgItem(hWnd, IDC_ANI_OUT))!=FALSE);
				g_VertexAni_Out = (Button_GetCheck(GetDlgItem(hWnd, IDC_VERTEX_ANI_OUT))!=FALSE);
				g_tm_Ani_out = (Button_GetCheck(GetDlgItem(hWnd, IDC_TM_ANI_OUT))!=FALSE);

				EndDialog(hWnd, 1);
				break;

			case IDCANCEL:
				EndDialog(hWnd, 0);
				break;
			}
			break;
			default:
				return FALSE;
	}
	return TRUE;
} 
      
// Dummy function for progress bar
DWORD WINAPI fn(LPVOID arg)
{
	return(0);
}

BOOL MCplug2::SupportsOptions(int ext, DWORD options) {
	assert(ext == 0);	// We only support one extension
	return(options == SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
}

///////////////////////////////////////////////////

#define SCENE_EXPORT_SCALE_ANI (1<<1) // 맵 오브젝트는 스케일 에니메이션 지원 되는걸로 출력..BspExporter 와 동기화는 맞춰준다..

int MCplug2::DoExport(const WCHAR *name,ExpInterface *ei,Interface *i, BOOL suppressPrompts, DWORD options)
{
	exportSelected = (options & SCENE_EXPORT_SELECTED) ? TRUE : FALSE;
//	suppressPrompts = true;
//	BOOL tm_ani = (options & SCENE_EXPORT_SCALE_ANI) ? TRUE : FALSE;

	ip = i;// 3DMAX 정보 인터페이스...


	m_nBeginFrame			= 0;

	//////////////////////////////////
	// run dlg

	if(!suppressPrompts) {
		if (!DialogBoxParam(hInstance, MAKEINTRESOURCE(IDD_ELU_EXPORT),ip->GetMAXHWnd(), EluDlgProc, (LPARAM)this)) 
			return 1;
	}
	_swprintf( szFmtStr, L"%%4.%df", 4);

	wcsncpy( FileName, name, wcslen(name)-4);

	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	// options 에서 정보받아 선택된 object 만 출력하는..코드.. 추가..
	
	ip->ProgressStart( GetString(IDS_PROGRESS_MSG), TRUE, fn, NULL );

	nTotalNodeCount = 0;
	nCurNode = 0;

	get_mtrl_list(ip->GetRootNode(), nTotalNodeCount); // mtrl list 를 채운다.
	
	export_info();
	export_mtrl_list();

	int numChildren = ip->GetRootNode()->NumberOfChildren();

	for (int idx=0; idx<numChildren; idx++) {

		if (ip->GetCancel())	break;

		export_all_object(ip->GetRootNode()->GetChildNode(idx));
	}

	ip->ProgressEnd();

	int b = 0;
	char temp[256];
	char filename[256];
	char file_text[256];
	char file_bin[256];
	char file_etc[256];

	memset(temp, 0, strlen(temp));
	
	for(int a = 0; a < (int)wcslen(name)-4; a++)
		temp[b++] = name[a];

	temp[b] = '\0';
	strcpy( filename, temp ); 

	strcpy(file_text, filename);
	strcat(file_text,"_debug.txt");

	strcpy(file_bin, filename);
	strcat(file_bin,".elu");

	strcpy(file_etc, filename);
	strcat(file_etc,".elm");

	// 빈 mtrl 을 제거한다..

	m_mesh_list.ClearVoidMtrl();

	if(g_bExport_DebugMesh_Out)		m_mesh_list.export_text(file_text);
	if(g_MeshOut)				m_mesh_list.export_bin(file_bin);

	if(g_AniOut)				m_mesh_list.export_ani(file_bin,RAniType_Bone);
	if(g_VertexAni_Out)		m_mesh_list.export_ani(file_bin,RAniType_Vertex);
	if(g_tm_Ani_out)			m_mesh_list.export_ani(file_bin,RAniType_Tm);

//	m_mesh_list.export_etc(file_etc);

	return 1;
}

void MCplug2::get_mtrl_list(INode* node, int& nodeCount)
{
	nodeCount++;
	
	mtlList.AddMtl(node->GetMtl());

	for (int c = 0; c < node->NumberOfChildren(); c++) {
		get_mtrl_list(node->GetChildNode(c), nodeCount);
	}
}

MSTR MCplug2::GetCfgFilename()
{
	MSTR filename;
	
	filename += ip->GetDir(APP_PLUGCFG_DIR);
	filename += L"\\";
	filename += CFGFILENAME;

	return filename;
}

BOOL MCplug2::export_all_object(INode* node) 
{
	if(exportSelected && node->Selected() == FALSE)	return TREE_CONTINUE;

	nCurNode++;
	ip->ProgressUpdate((int)((float)nCurNode/nTotalNodeCount*100.0f)); 

	if (ip->GetCancel())	return FALSE;
	
	if(!exportSelected || node->Selected()) {

		ObjectState os = node->EvalWorldState(0); 

		if (os.obj) {
			switch(os.obj->SuperClassID()) {
				case GEOMOBJECT_CLASS_ID: 
				case HELPER_CLASS_ID: 
					export_object_list(node); 
					break;
			}
		}
	}	
	
	for (int c = 0; c < node->NumberOfChildren(); c++) {
		if (!export_all_object(node->GetChildNode(c)))
			return FALSE;
	}

	return TRUE;
}

void MCplug2::export_info()
{
//	frame 은 0부터 시작해야 한다.

	Interval range = ip->GetAnimRange();

	m_nBeginFrame = range.Start();

	int frame = range.End() - range.Start();

	m_mesh_list.m_max_frame = frame / GetTicksPerFrame();

//	m_mesh_list.m_max_frame = range.End() / GetTicksPerFrame();
}

//////////////////////////////////////////////////////////////////
// util

BOOL MCplug2::TMNegParity(Matrix3 &m)
{
	return (DotProd(CrossProd(m.GetRow(0),m.GetRow(1)),m.GetRow(2))<0.0)?1:0;
}

/*
#define TRIOBJ_CLASS_ID 	 	0x0009	  
#define EDITTRIOBJ_CLASS_ID	0xe44f10b3	// base triangle mesh
#define POLYOBJ_CLASS_ID		0x5d21369a	// polygon mesh
#define PATCHOBJ_CLASS_ID  		0x1030
#define NURBSOBJ_CLASS_ID		0x4135
*/

TriObject* MCplug2::GetTriObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0))) { 
		TriObject *tri = (TriObject *) obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}

PatchObject* MCplug2::GetPatchObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	if (obj->CanConvertToType(Class_ID(PATCHOBJ_CLASS_ID, 0))) { 
		PatchObject *tri = (PatchObject *) obj->ConvertToType(t, Class_ID(PATCHOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}

PolyObject* MCplug2::GetPolyObjectFromNode(INode *node, TimeValue t, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(t).obj;

	if (obj->CanConvertToType(Class_ID(POLYOBJ_CLASS_ID, 0))) { 
		PolyObject *tri = (PolyObject *) obj->ConvertToType(t, Class_ID(POLYOBJ_CLASS_ID, 0));
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else {
		return NULL;
	}
}

static Point3 basic_tva[3] = 
{ 
	Point3(0.0,0.0,0.0),Point3(1.0,0.0,0.0),Point3(1.0,1.0,0.0)
};

static Point3 basic_tvb[3] = 
{ 
	Point3(1.0,1.0,0.0),Point3(0.0,1.0,0.0),Point3(0.0,0.0,0.0)
};

static int nextpt[3] = {1,2,0};
static int prevpt[3] = {2,0,1};

void MCplug2::make_face_uv(Face *f, Point3 *tv)
{
	int na,nhid,i;
	Point3 *basetv;
	nhid = 2;
	if (!(f->flags&EDGE_A))  nhid=0;
	else if (!(f->flags&EDGE_B)) nhid = 1;
	else if (!(f->flags&EDGE_C)) nhid = 2;
	na = 2-nhid;
	basetv = (f->v[prevpt[nhid]]<f->v[nhid]) ? basic_tva : basic_tvb;
	
	for (i=0; i<3; i++) {  
		tv[i] = basetv[na];
		na = nextpt[na];
	}
}

#define CTL_CHARS  31
#define SINGLE_QUOTE 39

WCHAR* MCplug2::FixupName(WCHAR* name)
{
	static WCHAR buffer[256];
	WCHAR* cPtr;
	
    wcscpy(buffer, name);
    cPtr = buffer;
	
    while(*cPtr) {
		if (*cPtr == L'"')
			*cPtr = SINGLE_QUOTE;
        else if (*cPtr <= CTL_CHARS)
			*cPtr = _T(L'_');
        cPtr++;
    }
	
	return buffer;
}

void MCplug2::CommaScan(WCHAR* buf)
{
    for(; *buf; buf++) 
		if (*buf == ',') 
			*buf = '.';
}

MSTR MCplug2::Format(int value)
{
	WCHAR buf[50];
	
	_swprintf(buf, _T(L"%d"), value);
	return buf;
}


MSTR MCplug2::Format(float value)
{
	WCHAR buf[40];
	
	_swprintf(buf, szFmtStr, value);
	CommaScan(buf);
	return MSTR(buf);
}

MSTR MCplug2::Format(Point3 value)
{
	WCHAR buf[120];
	WCHAR fmt[120];
	
	_swprintf(fmt, L"%s %s %s", szFmtStr, szFmtStr, szFmtStr);
	_swprintf(buf, fmt, value.x, value.y, value.z);

	CommaScan(buf);
	return buf;
}

MSTR MCplug2::Format(Color value)
{
	WCHAR buf[120];
	WCHAR fmt[120];
	
	_swprintf(fmt, L"%s %s %s", szFmtStr, szFmtStr, szFmtStr);
	_swprintf(buf, fmt, value.r, value.g, value.b);

	CommaScan(buf);
	return buf;
}

MSTR MCplug2::Format(AngAxis value)
{
	WCHAR buf[160];
	WCHAR fmt[160];
	
	_swprintf(fmt, L"%s %s %s %s", szFmtStr, szFmtStr, szFmtStr, szFmtStr);
	_swprintf(buf, fmt, value.axis.x, value.axis.y, value.axis.z, value.angle);

	CommaScan(buf);
	return buf;
}


MSTR MCplug2::Format(Quat value)
{
	Point3 axis;
	float angle;
	AngAxisFromQ(value, &angle, axis);
	
	return Format(AngAxis(axis, angle));
}

MSTR MCplug2::Format(ScaleValue value)
{
	WCHAR buf[280];
	
	_swprintf(buf, L"%s %s", Format(value.s).data(), Format(value.q).data());
	CommaScan(buf);
	return buf;
}

