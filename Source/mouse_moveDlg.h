
// mouse_moveDlg.h : header file
//

#pragma once
#define rate_min 25
#define office_start 8
#define office_end 17


// CmousemoveDlg dialog
class CmousemoveDlg : public CDialog
{
// Construction
public:
	CmousemoveDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOUSE_MOVE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton();

	BOOL ifStart;
	HANDLE hThread;
	CEdit m_Edit;
	void showLog(CString log);
	// Move freq
	int freq;
	// Starting time of office
	int start;
	// Ending time of office
	int end;
};
