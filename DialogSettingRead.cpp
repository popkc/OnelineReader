// DialogSettingRead.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogSettingRead.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSettingRead dialog
#include "onelineDlg.h"
extern COnelineApp theApp;
extern COnelineDlg *dlg;

#pragma warning(disable : 4355)
CDialogSettingRead::CDialogSettingRead(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSettingRead::IDD, pParent),
	cfd(NULL,CF_SCREENFONTS|CF_INITTOLOGFONTSTRUCT,NULL,this),
	fccd(theApp.fcolor,CC_RGBINIT,this),
	bgccd(theApp.bgcolor,CC_RGBINIT,this)
{
	//{{AFX_DATA_INIT(CDialogSettingRead)
	m_eSLeft=theApp.sleft;
	m_eSRight=theApp.sright;
	m_eSTop=theApp.stop;
	m_eSBottom=theApp.sbottom;
	m_eWordspace=theApp.wordspace;
	m_checkSmartSeg = theApp.smartSeg;
	//}}AFX_DATA_INIT
	
}


void CDialogSettingRead::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSettingRead)
	DDX_Control(pDX, IDC_SPIN_WORDSPACE, m_sWordspace);
	DDX_Control(pDX, IDC_SPIN_STOP, m_sSTop);
	DDX_Control(pDX, IDC_SPIN_SRIGHT, m_sSRight);
	DDX_Control(pDX, IDC_SPIN_SLEFT, m_sSLeft);
	DDX_Control(pDX, IDC_SPIN_SBOTTOM, m_sSBottom);
	DDX_Text(pDX, IDC_EDIT_SLEFT, m_eSLeft);
	DDV_MinMaxUInt(pDX, m_eSLeft, 0, 500);
	DDX_Text(pDX, IDC_EDIT_SRIGHT, m_eSRight);
	DDV_MinMaxUInt(pDX, m_eSRight, 0, 500);
	DDX_Text(pDX, IDC_EDIT_STOP, m_eSTop);
	DDV_MinMaxUInt(pDX, m_eSTop, 0, 500);
	DDX_Text(pDX, IDC_EDIT_SBOTTOM, m_eSBottom);
	DDV_MinMaxUInt(pDX, m_eSBottom, 0, 500);
	DDX_Text(pDX, IDC_EDIT_WORDSPACE, m_eWordspace);
	DDV_MinMaxUInt(pDX, m_eWordspace, 0, 100);
	DDX_Check(pDX, IDC_CHECK_SMARTSEG, m_checkSmartSeg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSettingRead, CDialog)
	//{{AFX_MSG_MAP(CDialogSettingRead)
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_BGCOLOR, OnButtonBgcolor)
	ON_BN_CLICKED(IDC_BUTTON_FCOLOR, OnButtonFcolor)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSettingRead message handlers

void CDialogSettingRead::saveVar()
{
	theApp.sleft=m_eSLeft;
	theApp.sright=m_eSRight;
	theApp.stop=m_eSTop;
	theApp.sbottom=m_eSBottom;
	theApp.wordspace=m_eWordspace;
	LOGFONT *lf;
	lf=cfd.m_cf.lpLogFont;
	theApp.fontbold=lf->lfWeight;
	theApp.fontsize=lf->lfHeight;
	theApp.fontitalic=lf->lfItalic;
	theApp.fontname=lf->lfFaceName;
	theApp.fcolor=fccd.GetColor();
	theApp.bgcolor=bgccd.GetColor();
	theApp.smartSeg=m_checkSmartSeg!=0;
	theApp.iniFile->SetValueB(_T("settings"),_T("smartseg"),theApp.smartSeg);
	theApp.iniFile->SetValueI(_T("settings"),_T("fcolor"),theApp.fcolor);
	theApp.iniFile->SetValueI(_T("settings"),_T("bgcolor"),theApp.bgcolor);
	theApp.iniFile->SetValueI(_T("settings"),_T("fontbold"),lf->lfWeight);
	theApp.iniFile->SetValueI(_T("settings"),_T("fontsize"),lf->lfHeight);
	theApp.iniFile->SetValueI(_T("settings"),_T("fontitalic"),lf->lfItalic);
	theApp.iniFile->SetValue(_T("settings"),_T("fontname"),lf->lfFaceName);
	theApp.iniFile->SetValueI(_T("settings"),_T("sleft"),m_eSLeft);
	theApp.iniFile->SetValueI(_T("settings"),_T("sright"),m_eSRight);
	theApp.iniFile->SetValueI(_T("settings"),_T("stop"),m_eSTop);
	theApp.iniFile->SetValueI(_T("settings"),_T("sbottom"),m_eSBottom);
	theApp.iniFile->SetValueI(_T("settings"),_T("wordspace"),m_eWordspace);
}

BOOL CDialogSettingRead::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_sSLeft.SetRange(0,500);
	m_sSRight.SetRange(0,500);
	m_sSBottom.SetRange(0,500);
	m_sSTop.SetRange(0,500);
	m_sWordspace.SetRange(0,100);
	theApp.font.GetLogFont(cfd.m_cf.lpLogFont);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CDialogSettingRead::OnButtonFont() 
{
	// TODO: Add your control notification handler code here
	cfd.DoModal();
}

void CDialogSettingRead::OnButtonBgcolor() 
{
	// TODO: Add your control notification handler code here
	bgccd.DoModal();
}

void CDialogSettingRead::OnButtonFcolor() 
{
	// TODO: Add your control notification handler code here
	fccd.DoModal();
}
