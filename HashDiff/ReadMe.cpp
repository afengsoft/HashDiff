// ReadMe.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HashDiff.h"
#include "ReadMe.h"
#include "afxdialogex.h"


// CReadMe �Ի���

IMPLEMENT_DYNAMIC(CReadMe, CDialogEx)

CReadMe::CReadMe(CWnd* pParent /*=NULL*/)
	: CDialogEx(CReadMe::IDD, pParent)
{

}

CReadMe::~CReadMe()
{
}

void CReadMe::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SHOW, m_StaticText);
}


BEGIN_MESSAGE_MAP(CReadMe, CDialogEx)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CReadMe ��Ϣ�������

BOOL CReadMe::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CRect rect;
	GetClientRect(&rect);     //ȡ�ͻ�����С  
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	m_bkbrush.CreateSolidBrush(RGB(255,255,255)); 
	m_stafont.CreatePointFont(150,_T("����"));
	m_StaticText.SetFont(&m_stafont);
	CString szText = _T("https://github.com/ggg4566/HashDiff\r\n")
		_T("author:flystart of ms509\r\n")
		_T("email:root@flystart.org\r\n\r\n")
		_T("1.ȫ���������Թ���ԱȨ������\r\n")
		_T("2.ͨ������ hash�����ļ������������ض��ļ���С�Լ��ض�Ŀ¼\r\n")
		_T("3.�ļ�����ʹ�ú�everything��ͬ�ķ�������֧�� NTFS��ʽ����");
	m_StaticText.SetWindowText(szText);
	return TRUE; 
}

void CReadMe::ReSize(void)
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


void CReadMe::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED) {

		ReSize();
	}
}

HBRUSH CReadMe::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	HBRUSH org_hbr = hbr;
	//TODO:  �ڴ˸��� DC ���κ�����
	switch (nCtlColor)
	{

	case CTLCOLOR_STATIC:
		switch (pWnd->GetDlgCtrlID())//��ĳһ���ض��ؼ������ж�  
		{
			// first CEdit control ID  
		case IDC_STATIC_SHOW:   
			{
				pDC->SetTextColor(RGB(255,255,255)); // change the text color  
				pDC->SetBkMode(TRANSPARENT);
				hbr=(HBRUSH)GetStockObject(NULL_BRUSH);
			}
			break;
			// second CEdit control ID  
		default:
			hbr = org_hbr;
		}
		break;
	default:
		hbr = org_hbr;
	}
	
	return hbr;
}


void CReadMe::OnPaint()
{
	
		CPaintDC dc(this); // device context for painting
		// TODO: �ڴ˴������Ϣ����������
		// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
		CRect rect;
		GetClientRect(rect);
		//dc.FillSolidRect(rect,RGB(78,143,246)); //���ñ���ɫ
		dc.FillSolidRect(rect,RGB(97, 120, 140));
}