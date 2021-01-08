
// mouse_move.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CmousemoveApp:
// See mouse_move.cpp for the implementation of this class
//

class CmousemoveApp : public CWinApp
{
public:
	CmousemoveApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CmousemoveApp theApp;
