#if !defined(AFX_DIALOGSETTINGS_H__B4E830A6_7260_416D_9F5E_F09D1AFA11C1__INCLUDED_)
#define AFX_DIALOGSETTINGS_H__B4E830A6_7260_416D_9F5E_F09D1AFA11C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSettings.h : header file
//
#include "DialogSettingRead.h"
#include "DialogAdvance.h"
#include "DialogVoice.h"
/////////////////////////////////////////////////////////////////////////////
// CDialogSettings dialog

class CDialogSettings : public CDialog
{
// Construction
public:
	CDialogSettings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSettings)
	enum { IDD = IDD_DIALOG_SETTINGS };
	CTabCtrl	m_tSettings;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSettings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CDialogSettingRead pageRead;
	CDialogAdvance pageAdvance;
	CDialogVoice pageVoice;
	CDialog* pages[3];
	bool pageCreated[3];
	CRect windowRect;
	CWnd *h;
	bool currentPageUpdateOK();
	// Generated message map functions
	//{{AFX_MSG(CDialogSettings)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonCancel();
	afx_msg void OnSelchangingTabSettings(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTabSettings(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETTINGS_H__B4E830A6_7260_416D_9F5E_F09D1AFA11C1__INCLUDED_)
