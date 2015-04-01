#if !defined(AFX_DIALOGADVANCE_H__D641299B_D5B7_4964_AFCE_D8601D825802__INCLUDED_)
#define AFX_DIALOGADVANCE_H__D641299B_D5B7_4964_AFCE_D8601D825802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogAdvance.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogAdvance dialog

class CDialogAdvance : public CDialog
{
// Construction
public:
	CDialogAdvance(CWnd* pParent = NULL);   // standard constructor
	void saveVar();
// Dialog Data
	//{{AFX_DATA(CDialogAdvance)
	enum { IDD = IDD_DIALOG_SETTING_ADVANCE };
	CSpinButtonCtrl	m_sMaxBlock;
	UINT	m_eMaxBlock;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogAdvance)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDialogAdvance)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGADVANCE_H__D641299B_D5B7_4964_AFCE_D8601D825802__INCLUDED_)
