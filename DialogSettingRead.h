#if !defined(AFX_DIALOGSETTINGREAD_H__79788D44_6DA7_44D3_B369_F4587FC78004__INCLUDED_)
#define AFX_DIALOGSETTINGREAD_H__79788D44_6DA7_44D3_B369_F4587FC78004__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSettingRead.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSettingRead dialog

class CDialogSettingRead : public CDialog
{
// Construction
public:
	CDialogSettingRead(CWnd* pParent = NULL);   // standard constructor
	void saveVar();
// Dialog Data
	//{{AFX_DATA(CDialogSettingRead)
	enum { IDD = IDD_DIALOG_SETTING_READ };
	CSpinButtonCtrl	m_sWordspace;
	CSpinButtonCtrl	m_sSTop;
	CSpinButtonCtrl	m_sSRight;
	CSpinButtonCtrl	m_sSLeft;
	CSpinButtonCtrl	m_sSBottom;
	UINT	m_eSLeft;
	UINT	m_eSRight;
	UINT	m_eSTop;
	UINT	m_eSBottom;
	UINT	m_eWordspace;
	BOOL	m_checkSmartSeg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSettingRead)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFontDialog cfd;
	CColorDialog fccd,bgccd;
	// Generated message map functions
	//{{AFX_MSG(CDialogSettingRead)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonFont();
	afx_msg void OnButtonBgcolor();
	afx_msg void OnButtonFcolor();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSETTINGREAD_H__79788D44_6DA7_44D3_B369_F4587FC78004__INCLUDED_)
