#if !defined(AFX_DIALOGINDEXSETTING_H__65A0E49E_D136_4463_BF2A_ACD49A671F65__INCLUDED_)
#define AFX_DIALOGINDEXSETTING_H__65A0E49E_D136_4463_BF2A_ACD49A671F65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogIndexSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogIndexSetting dialog
#include "DialogIndex.h"
class CDialogIndex;
class CDialogIndexSetting : public CDialog
{
// Construction
public:
	CDialogIndexSetting(CDialogIndex* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogIndexSetting)
	enum { IDD = IDD_DIALOG_INDEXSETTING };
	CEdit	m_editMatch;
	CListBox	m_listRegex;
	CEdit	m_editRegex;
	CSpinButtonCtrl	m_spinMaxchar;
	UINT	m_maxchar;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogIndexSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void resetListH();
	CDialogIndex* par;
	bool testIsValidRegex(CString &);
	// Generated message map functions
	//{{AFX_MSG(CDialogIndexSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListRegex();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonModify();
	afx_msg void OnButtonOk();
	afx_msg void OnButtonCancel();
	afx_msg void OnButtonDefault();
	afx_msg void OnButtonMatch();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGINDEXSETTING_H__65A0E49E_D136_4463_BF2A_ACD49A671F65__INCLUDED_)
