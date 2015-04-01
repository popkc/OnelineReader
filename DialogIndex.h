#if !defined(AFX_DIALOGINDEX_H__2FACF1F9_A77D_46A9_8E11_1D5490041630__INCLUDED_)
#define AFX_DIALOGINDEX_H__2FACF1F9_A77D_46A9_8E11_1D5490041630__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogIndex.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogIndex dialog
#include "DialogIndexSetting.h"
class CDialogIndexSetting;
typedef std::map<long,CString> MapIndex;

struct indexInfo
{
	long pos;
	CString name;
};

struct indexThreadInfo
{
	unsigned short id;
	long startPos;
	bool moveNextLine;
	HWND hWnd;
};

bool moveToNextLine(long &,int,int);
UINT threadIndex(LPVOID);
bool detectIndex(CString &);

class CDialogIndex : public CDialog
{
// Construction
public:
	CDialogIndex(CWnd* pParent = NULL);   // standard constructor
	void makeRegex();
	
// Dialog Data
	//{{AFX_DATA(CDialogIndex)
	enum { IDD = IDD_DIALOG_INDEX };
	CListCtrl	m_listIndex;
	CProgressCtrl	m_progressIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogIndex)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWinThread *thd;
	void indexEnd();
	unsigned short id;
	void indexStart();
	CToolBar m_toolbar;
	bool changed;
	CDialogIndexSetting *dIS;
	//CImageList iml;
	// Generated message map functions
	//{{AFX_MSG(CDialogIndex)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListIndex(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonJump();
	afx_msg void OnMenuIndexBuild();
	afx_msg void OnMenuIndexClear();
	afx_msg void OnMenuIndexContinue();
	afx_msg void OnMenuIndexDelete();
	afx_msg void OnMenuIndexSetting();
	afx_msg void OnMenuIndexStop();
	afx_msg void OnKeydownListIndex(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	afx_msg LRESULT OnThreadMSG(WPARAM,LPARAM);
	afx_msg BOOL OnToolTipsNotify(UINT,NMHDR*,LRESULT *);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGINDEX_H__2FACF1F9_A77D_46A9_8E11_1D5490041630__INCLUDED_)
