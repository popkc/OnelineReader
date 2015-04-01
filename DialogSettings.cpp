// DialogSettings.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogSettings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings dialog
#include "onelineDlg.h"
extern COnelineApp theApp;
extern COnelineDlg *dlg;

CDialogSettings::CDialogSettings(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSettings::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSettings)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	
}


void CDialogSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSettings)
	DDX_Control(pDX, IDC_TAB_SETTINGS, m_tSettings);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSettings, CDialog)
	//{{AFX_MSG_MAP(CDialogSettings)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_SETTINGS, OnSelchangingTabSettings)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SETTINGS, OnSelchangeTabSettings)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSettings message handlers

BOOL CDialogSettings::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_tSettings.InsertItem(0,_T("ÔÄ¶Á"));
	m_tSettings.InsertItem(1,_T("ÓïÒô"));
	m_tSettings.InsertItem(2,_T("¸ß¼¶"));
	m_tSettings.GetClientRect(windowRect);
	windowRect.top+=30;
	windowRect.bottom-=10;
	windowRect.left+=5;
	windowRect.right-=5;
	h=GetDlgItem(IDC_TAB_SETTINGS);
	pageRead.Create(IDD_DIALOG_SETTING_READ,h);
	pageRead.MoveWindow(windowRect);
	pageRead.ShowWindow(TRUE);
	pageCreated[0]=true;
	pageCreated[1]=false;
	pageCreated[2]=false;
	pages[0]=&pageRead;
	pages[1]=&pageVoice;
	pages[2]=&pageAdvance;
	m_tSettings.SetCurSel(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSettings::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	if (!currentPageUpdateOK())
	{
		return;
	}
	pageRead.saveVar();
	if (pageCreated[1])
	{
		pageVoice.saveVar();
	}
	if (pageCreated[2])
	{
		pageAdvance.saveVar();
	}
	dlg->settingsChanged();
	EndDialog(1);
}

bool CDialogSettings::currentPageUpdateOK()
{
	/*CDialog *td;
	switch(m_tSettings.GetCurSel())
	{
	case 0:
		td=&pageRead;
		break;
	case 1:
		td=&pageVoice;
		break;
	case 2:
		td=&pageAdvance;
		break;
	}*/
	if (!pages[m_tSettings.GetCurSel()]->UpdateData(TRUE))
	{
		return false;
	}
	else
		return true;
}

void CDialogSettings::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
}

void CDialogSettings::OnSelchangingTabSettings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if (currentPageUpdateOK())
	{
		*pResult=0;
	}
	else
		*pResult = 1;
}

void CDialogSettings::OnSelchangeTabSettings(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int sel=m_tSettings.GetCurSel();
	for (int i=0;i<3;i++)
	{
		if (pageCreated[i])
		{
			pages[i]->ShowWindow(FALSE);
		}
	}
	switch(sel)
	{
	case 1:
		if (!pageCreated[1])
		{
			pageVoice.Create(IDD_DIALOG_VOICE,h);
			pageVoice.MoveWindow(windowRect);
			pageCreated[1]=true;
		}
		break;
	case 2:
		if(!pageCreated[2])
		{
			pageAdvance.Create(IDD_DIALOG_SETTING_ADVANCE,h);
			pageAdvance.MoveWindow(windowRect);
			pageCreated[2]=true;
		}
		break;
	}
	pages[sel]->ShowWindow(TRUE);
	*pResult = 0;
}
