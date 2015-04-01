#if !defined(AFX_DIALOGABOUT_H__9EF02D24_64DB_4B01_9E6E_657BD7DC26BF__INCLUDED_)
#define AFX_DIALOGABOUT_H__9EF02D24_64DB_4B01_9E6E_657BD7DC26BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogAbout.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogAbout dialog

class CDialogAbout : public CDialog
{
// Construction
public:
	CDialogAbout(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogAbout)
	enum { IDD = IDD_DIALOG_ABOUT };
	CString	m_about;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogAbout)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGABOUT_H__9EF02D24_64DB_4B01_9E6E_657BD7DC26BF__INCLUDED_)
