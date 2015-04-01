#if !defined(AFX_DIALOGVOICE_H__D5524203_BBFA_4EA6_B5BD_DA33193F911E__INCLUDED_)
#define AFX_DIALOGVOICE_H__D5524203_BBFA_4EA6_B5BD_DA33193F911E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogVoice.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogVoice dialog

class CDialogVoice : public CDialog
{
// Construction
public:
	CDialogVoice(CWnd* pParent = NULL);   // standard constructor
	void saveVar();
// Dialog Data
	//{{AFX_DATA(CDialogVoice)
	enum { IDD = IDD_DIALOG_VOICE };
	CComboBox	m_combo;
	CSliderCtrl	m_sVolume;
	CSliderCtrl	m_sRate;
	CString	m_staticRate;
	CString	m_staticVolume;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogVoice)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HWND hCombo;
	void setStaticRate();
	void setStaticVolume();
	CComPtr<ISpVoice> pVoice;
	// Generated message map functions
	//{{AFX_MSG(CDialogVoice)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnCustomdrawSliderRate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderVolume(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnButtonListen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGVOICE_H__D5524203_BBFA_4EA6_B5BD_DA33193F911E__INCLUDED_)
