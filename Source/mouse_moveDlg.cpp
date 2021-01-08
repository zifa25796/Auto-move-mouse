
// mouse_moveDlg.cpp : implementation file
//


#include "pch.h"
#include "framework.h"
#include "mouse_move.h"
#include "mouse_moveDlg.h"
#include "afxdialogex.h"
#include <time.h>
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(disable:4996)


// CmousemoveDlg dialog



CmousemoveDlg::CmousemoveDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_MOUSE_MOVE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ifStart = TRUE;
}

void CmousemoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
}

BEGIN_MESSAGE_MAP(CmousemoveDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON, &CmousemoveDlg::OnBnClickedButton)
END_MESSAGE_MAP()


// CmousemoveDlg message handlers

BOOL CmousemoveDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	GetDlgItem(IDC_EDIT1)->ShowWindow(FALSE);
	// Init the value of freq, start and end time
	SetDlgItemInt(IDC_EDIT_MIN, rate_min);
	SetDlgItemInt(IDC_EDIT_OFFICE_START, office_start);
	SetDlgItemInt(IDC_EDIT_OFFICE_END, office_end);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CmousemoveDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CmousemoveDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

DWORD WINAPI ThreadProc(LPVOID lpParam) {
	CmousemoveDlg* CMouse = (CmousemoveDlg*)lpParam;
	while (TRUE) {
		SYSTEMTIME sys;
		GetLocalTime(&sys);
		// Check the current time is in office hour
		if (sys.wHour > CMouse->start && sys.wHour < CMouse->end) {
			// Use for loop to count down
			for (int i = CMouse->freq; i > 0; i--) {
				// Refresh the btn text
				CString str;
				str.Format(_T("End (Next Refresh: %d min)"), i);
				CMouse->SetDlgItemText(IDC_BUTTON, str);
				Sleep(1000 * 60); // One Min
			}
			// Move the mouse
			mouse_event(MOUSEEVENTF_MOVE, 10, 10, 0, 0);
			mouse_event(MOUSEEVENTF_MOVE, -10, -10, 0, 0);
			CMouse->showLog(_T("Moved"));
		}
		else {
			// Refresh the btn text
			CString str = _T("End (Currently off office hour)");
			CMouse->SetDlgItemText(IDC_BUTTON, str);
			Sleep(1000 * 60); // One Min
			CMouse->showLog(_T("Off hour"));
		}

		
	}
}


void CmousemoveDlg::OnBnClickedButton()
{
	// If clocked the btn
	if (ifStart) {
		GetDlgItem(IDC_EDIT1)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_Freq)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_MIN)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_OFFICE)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_TO)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFICE_START)->ShowWindow(FALSE);
		GetDlgItem(IDC_EDIT_OFFICE_END)->ShowWindow(FALSE);

		// Get all dlg value
		freq = GetDlgItemInt(IDC_EDIT_MIN);
		start = GetDlgItemInt(IDC_EDIT_OFFICE_START);
		end = GetDlgItemInt(IDC_EDIT_OFFICE_END);

		// Create the thread that proc the time and move mouse
		hThread = CreateThread(NULL, 0, ThreadProc, this, 0, 0);
	}
	else {
		// End the thread
		TerminateThread(hThread, NULL);
		WaitForSingleObject(hThread, INFINITE);
		// Set the btn text
		SetDlgItemText(IDC_BUTTON, _T("Start"));
		m_Edit.SetSel(0, -1);
		m_Edit.ReplaceSel(_T(""));

		GetDlgItem(IDC_EDIT1)->ShowWindow(FALSE);
		GetDlgItem(IDC_STATIC_Freq)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_MIN)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_OFFICE)->ShowWindow(TRUE);
		GetDlgItem(IDC_STATIC_TO)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFICE_START)->ShowWindow(TRUE);
		GetDlgItem(IDC_EDIT_OFFICE_END)->ShowWindow(TRUE);
	}
	ifStart = !ifStart;
}

void CmousemoveDlg::showLog(CString log) {
	// Write log
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString str;
	str.Format(_T("%d-%02d-%02dT%02d:%02d > %s"), 
		sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, log);


	m_Edit.SetSel(-1, -1);
	m_Edit.ReplaceSel(str + _T("\r\n"));
}