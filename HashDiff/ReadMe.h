#pragma once
#include "afxwin.h"


// CReadMe �Ի���

class CReadMe : public CDialogEx
{
	DECLARE_DYNAMIC(CReadMe)

public:
	CReadMe(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReadMe();

// �Ի�������
	enum { IDD = IDD_DIG_README };

protected:
	POINT old;
	CBrush m_bkbrush;
	CFont m_stafont;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void ReSize(void);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	CStatic m_StaticText;
};
