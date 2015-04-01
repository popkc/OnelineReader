#if !defined(AFX_DIALOGSEARCH_H__01AE90E3_EC18_40FC_AE67_44D8C805F5CA__INCLUDED_)
#define AFX_DIALOGSEARCH_H__01AE90E3_EC18_40FC_AE67_44D8C805F5CA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSearch.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSearch dialog
UINT threadSearch(LPVOID);
UINT threadSearchQuick(LPVOID);

BYTE findAnsiPre(const long& pos);
BYTE findAnsiNext(const long& pos);

struct searchInfo
{
	long endPos;
	BOOL isDown;
	CString *text;
	HWND hWnd;
	unsigned short id;
};


class CDialogSearch : public CDialog
{
// Construction
public:
	CDialogSearch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSearch)
	enum { IDD = IDD_DIALOG_SEARCH };
	CButton	m_checkSmartSeg;
	CProgressCtrl	m_progressSearch;
	CButton	m_buttonCancel;
	CButton	m_buttonSearch;
	CString	m_editSearch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWinThread *thd;
	searchInfo si;
	unsigned short id;
	LRESULT OnThreadSearchMessage(WPARAM,LPARAM);
	bool searching;
	bool stop;
	// Generated message map functions
	//{{AFX_MSG(CDialogSearch)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSearch();
	afx_msg void OnButtonCancel();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSEARCH_H__01AE90E3_EC18_40FC_AE67_44D8C805F5CA__INCLUDED_)
