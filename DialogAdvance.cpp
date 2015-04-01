// DialogAdvance.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogAdvance.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogAdvance dialog

extern COnelineApp theApp;
CDialogAdvance::CDialogAdvance(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAdvance::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogAdvance)
	m_eMaxBlock = theApp.maxblock;
	//}}AFX_DATA_INIT
}


void CDialogAdvance::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAdvance)
	DDX_Control(pDX, IDC_SPIN_MAXBLOCK, m_sMaxBlock);
	DDX_Text(pDX, IDC_EDIT_MAXBLOCK, m_eMaxBlock);
	DDV_MinMaxUInt(pDX, m_eMaxBlock, 5, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogAdvance, CDialog)
	//{{AFX_MSG_MAP(CDialogAdvance)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnButtonClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogAdvance message handlers

BOOL CDialogAdvance::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_sMaxBlock.SetRange(5,999999);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogAdvance::saveVar()
{
	theApp.maxblock=m_eMaxBlock;
	theApp.iniFile->SetValueI(_T("settings"),_T("maxblock"),m_eMaxBlock);
}

void CDialogAdvance::OnButtonClear() 
{
	// TODO: Add your control notification handler code here
	int i=MessageBox(_T("当oneline.ini文件过大时，清理文件信息可以提高软件启动速度，但也会导致所有的章节索引和阅读位置等信息失效。\n确实要清除文件信息吗？"),_T("确认"),MB_YESNO);
	if (i==IDYES)
	{
		MapTotal::iterator it,it2;
		it=theApp.iniFile->values.begin();
		while(it!=theApp.iniFile->values.end())
		{
			if (it->first!=_T("recentfile")&&it->first!=_T("settings")&&it->first!=_T("window")&&it->first!=_T("voice")&&it->first!=_T("find"))
			{
				it2=it;
				it++;
				theApp.iniFile->values.erase(it2);
			}
			else
				it++;
		}
		theApp.iniFile->WriteFile();
		MessageBox(_T("清理完毕！"));
	}
}
