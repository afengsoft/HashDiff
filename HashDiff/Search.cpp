// Search.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HashDiff.h"
#include "Search.h"
#include "afxdialogex.h"
#include "Volume.h"


#define  WM_UPDATELIST WM_USER+1
// CSearch �Ի���

IMPLEMENT_DYNAMIC(CSearch, CDialogEx)

CSearch::CSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSearch::IDD, pParent)
	, m_ShowFile(_T(""))
	, m_SearchKey(_T(""))
{
	pThread = NULL;
	pThreadSearch = NULL;
	InitializeCriticalSection(&m_cs);
	bExit = FALSE;
}

CSearch::~CSearch()
{
}

void CSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SHOWPATH, m_ShowFile);
	DDX_Text(pDX, IDC_EDIT_KEYWORD, m_SearchKey);
	DDX_Control(pDX, IDC_LIST_SHOW_FILES, m_ListShowFiles);
	DDX_Control(pDX, IDC_STATIC_STATUS, m_StatusRect);
	DDX_Control(pDX, IDOK, m_ButtonQuery);
}


BEGIN_MESSAGE_MAP(CSearch, CDialogEx)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, &CSearch::OnBnClickedOk)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SHOW_FILES, &CSearch::OnNMClickListShowFiles)
	ON_MESSAGE(WM_UPDATELIST, &CSearch::OnUpdatelist)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LIST_SHOW_FILES, &CSearch::OnLvnGetdispinfoListShowFiles)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CSearch ��Ϣ�������


BOOL CSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CRect rect,show_rect,status_rect,list_rect;
	GetClientRect(&rect);     //ȡ�ͻ�����С  
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;
	m_StatusRect.GetClientRect(status_rect);
	m_status.Create(WS_CHILD | WS_VISIBLE | CBRS_BOTTOM, status_rect, this, 3);
	
	CStatic *pStatic =(CStatic *)GetDlgItem(IDC_STATIC_STATUS);
	pStatic->ShowWindow(SW_HIDE);

	int nPanel[2] = { status_rect.Width()/2+100,-1 };      //�ָ�ߴ�
	m_status.SetParts(2, nPanel);  //�ָ�״̬��
	m_status.SetText(_T("Already��"), 0, 0); //��һ����������"���ǵ�һ��ָʾ��"

	m_ListShowFiles.GetClientRect(&list_rect);
	m_ListShowFiles.SetExtendedStyle(m_ListShowFiles.GetExtendedStyle() | LVS_EX_FULLROWSELECT|LVS_OWNERDATA);
	
	// Ϊ�б���ͼ�ؼ��������  
	int wlist = list_rect.Width();

	m_ListShowFiles.InsertColumn(0, _T("ID"), LVCFMT_LEFT, 40);
	m_ListShowFiles.InsertColumn(1, _T("File"), LVCFMT_LEFT, 400);
	m_ListShowFiles.InsertColumn(2, _T("Time"), LVCFMT_LEFT, 120);
	m_ListShowFiles.SetItemCountEx(m_dataList.size(), LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
	//��m_dateList��push_back���ݺ�


	
	initdata.init(NULL);
	AllDrivers = initdata.getJ();
	pThread = AfxBeginThread(initThread, (LPVOID)this);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSearch::ReSize(void)
{
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С
	CRect recta;
	GetClientRect(&recta);     //ȡ�ͻ�����С  
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�
	CPoint OldBRPoint, BRPoint; //���½�
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�  
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID
		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
}

void CSearch::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED) {

		ReSize();
	}
}

UINT CSearch::SearchFileThread(LPVOID pParam) {
	UINT ret = 0;
	CSearch * pObj = (CSearch*)pParam;
	CString keyword = pObj->m_SearchKey;
	ret = pObj->SearchFile(keyword);
	return ret;
}

UINT CSearch::SearchFile(CString keyword) {

	UINT ret = 0;
	keyword.TrimLeft();	// �Է�ȫ���ǿո�
	keyword.TrimRight();
	if(keyword.GetLength()<2)
	{
		m_status.SetText(_T("keyword lenght too short,at least 2 char."),0,0);
		return ret;
	}
	cmpStrStr cmpstrstr(false, false);
	vector<string>* pignorepath = initdata.getIgnorePath();
	m_status.SetText(_T("Search file ..."),0,0);
	for ( list<Volume>::iterator lvolit = initdata.volumelist.begin();
		lvolit != initdata.volumelist.end(); ++lvolit ) {
			// c d e volumelist
			if(bExit ==TRUE)
			{
				return ret;
			}
			lvolit->findFile(keyword, cmpstrstr, pignorepath);

			// ��ListBox����ʾ
			for ( vector<CString>::iterator vstrit = lvolit->rightFile.begin();
				vstrit != lvolit->rightFile.end(); ++vstrit) {
					std::vector<CString>  vecTempDatda;
					size_t nCount = 0;
					nCount =m_dataList.size();
					CString id = _T("");
					id.Format(_T("%d"),++nCount);
					vecTempDatda.push_back(id);
					CString file = *vstrit;
					vecTempDatda.push_back(file);
					vecTempDatda.push_back(GetFileTime(file));
					m_dataList.push_back(vecTempDatda);
			}
			lvolit->rightFile.clear();	

	}
	m_status.SetText(_T("Search file have end!"),0,0);
	
	bIsSearch = TRUE;
	m_ListShowFiles.SetItemCountEx(m_dataList.size(), LVSICF_NOINVALIDATEALL | LVSICF_NOSCROLL);
	m_ListShowFiles.Invalidate();
	m_ButtonQuery.EnableWindow(TRUE);
	//PostMessage(WM_UPDATELIST,0,0);
	
	return ret;
}

UINT CSearch::GetAllFiles()
{
	UINT ret = 0;
	for ( list<Volume>::iterator lvolit = initdata.volumelist.begin();
		lvolit != initdata.volumelist.end(); ++lvolit ) {
			// c d e volumelist
			lvolit->GetAllFile(AllFiles);
	}
	return ret;
}

UINT CSearch::initThread(LPVOID pParam) {
	CSearch * pObj = (CSearch*)pParam;
	if ( pObj ) {
		return pObj->realThread(NULL);
	}
	return false;
}

CString CSearch::GetFileTime(CString FileName)
{
	CString ret =_T("");
	if(!FileName.IsEmpty())
	{
		CFileStatus status;
		CFile::GetStatus(FileName, status);
		CTime time;
		time = status.m_ctime;
		ret = time.Format(_T("%Y-%m-%d %H:%M:%S"));
	}
	return ret;
}
UINT CSearch::realThread(LPVOID pParam) {
	char *pvol = initdata.getVol();
	CString Drive=_T("Driver: ");
	int num = AllDrivers;
	for ( int i=0; i<num; ++i ) {
		if(bExit == TRUE)
		{
			return 0;
		}
		initdata.initvolumelist((LPVOID)pvol[i]);
		CString showpro(_T("Counting "));
		showpro += pvol[i];
		Drive+=pvol[i];
		Drive+=_T(",");
		showpro += _T(" Drive...");
		m_status.SetText(showpro, 0, 0);
	}
	GetAllFiles();
	CString info;
	info.Format(_T("%s Count Finsh. Totals file :%d"),Drive,AllFiles.GetSize());
	m_status.SetText(info, 0, 0);
	isCount = true;
	return 0;	
}

// query file
void CSearch::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	SIZE_T nSizeOfList = m_dataList.size();
	for(SIZE_T i=0;i<nSizeOfList;i++)
	{
		m_dataList[i].clear();
	}
	m_dataList.clear();
	
	m_status.SetText(_T(""), 0, 0);
	m_status.SetText(_T(""), 0, 1);
	if(isCount ==false)
	{
		m_status.SetText(_T("All Driver have't count end.Please wait a moment."), 0, 1);
		return;
	}
	if(m_SearchKey.GetLength()==0)
	{
		m_status.SetText(_T("Please input filename."), 0, 1);
		return;
	}
	m_ButtonQuery.EnableWindow(FALSE);
	pThreadSearch = AfxBeginThread(SearchFileThread, (LPVOID)this);
}


void CSearch::OnNMClickListShowFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	NMLISTVIEW *pNMListView = (NMLISTVIEW*)pNMHDR;

	if (-1 != pNMListView->iItem)        // ���iItem����-1����˵�����б��ѡ��   
	{
		// ��ȡ��ѡ���б����һ��������ı�   
		// ��ѡ���������ʾ��༭����   
		m_ShowFile = m_ListShowFiles.GetItemText(pNMListView->iItem, 1);
		UpdateData(false);

	}
	*pResult = 0;
}


afx_msg LRESULT CSearch::OnUpdatelist(WPARAM wParam, LPARAM lParam)
{
	vector<CString> result;
	return 0;
}


void CSearch::OnLvnGetdispinfoListShowFiles(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	LV_ITEM* pItem = &(pDispInfo)->item;
	int nItem = pItem->iItem;
	if (nItem < 0 || nItem >= m_dataList.size())
	{
		return;
	}
	if (pItem->mask & LVIF_TEXT)
	{
		int nSubItem = pItem->iSubItem;
		_tcscpy(pItem->pszText, m_dataList[nItem][nSubItem]);
	}
	*pResult = 0;
}


void CSearch::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	bExit = TRUE;
	if(pThreadSearch!=NULL)
	{
			WaitForSingleObject(pThreadSearch->m_hThread, INFINITE );
	}
	if(pThread!=NULL)
	{
		WaitForSingleObject(pThread->m_hThread, INFINITE );
	}
	CDialogEx::OnClose();
}
