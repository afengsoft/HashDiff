
// HashDiff.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CHashDiffApp:
// �йش����ʵ�֣������ HashDiff.cpp
//

class CHashDiffApp : public CWinApp
{
public:
	CHashDiffApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CHashDiffApp theApp;