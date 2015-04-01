#if !defined(AFX_DIALOGJUMPTO_H__B5AB8094_9D40_4906_8F0A_2762B713F308__INCLUDED_)
#define AFX_DIALOGJUMPTO_H__B5AB8094_9D40_4906_8F0A_2762B713F308__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogJumpto.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogJumpto dialog

class CDialogJumpto : public CDialog
{
// Construction
public:
	CDialogJumpto(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogJumpto)
	enum { IDD = IDD_DIALOG_JUMPTO };
	CEdit	m_editJumpto;
	CSliderCtrl	m_sliderJumpto;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogJumpto)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	bool skipOne;
	// Generated message map functions
	//{{AFX_MSG(CDialogJumpto)
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawSliderJumpto(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonJumpto();
	afx_msg void OnChangeEditJumpto();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGJUMPTO_H__B5AB8094_9D40_4906_8F0A_2762B713F308__INCLUDED_)
