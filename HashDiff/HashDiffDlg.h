
// HashDiffDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "HashCalc.h"
#include "HashCompare.h"
#include "Search.h"
#include "ReadMe.h"
// CHashDiffDlg �Ի���
class CHashDiffDlg : public CDialogEx
{
// ����
public:
	CHashDiffDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_HASHDIFF_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	POINT old;
	CTabCtrl m_TabView;
	CDialog* pDialog[4];
	int m_CurSelTab;
	CSearch m_SearchDlg;
	CHashCompare m_HashCompareDlg;
	CHashCalc m_HashCalcDlg;
	CReadMe m_ReadMeDlg;
	CRect m_rect;
	afx_msg void OnTcnSelchangeTabView(NMHDR *pNMHDR, LRESULT *pResult);
	void ReSize(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
