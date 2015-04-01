// DialogJumpto.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogJumpto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogJumpto dialog
#include "onelineDlg.h"
#include <math.h>
extern COnelineDlg *dlg;

CDialogJumpto::CDialogJumpto(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogJumpto::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogJumpto)
	//}}AFX_DATA_INIT
	skipOne=false;
}


void CDialogJumpto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogJumpto)
	DDX_Control(pDX, IDC_EDIT_JUMPTO, m_editJumpto);
	DDX_Control(pDX, IDC_SLIDER_JUMPTO, m_sliderJumpto);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogJumpto, CDialog)
	//{{AFX_MSG_MAP(CDialogJumpto)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_JUMPTO, OnCustomdrawSliderJumpto)
	ON_BN_CLICKED(IDC_BUTTON_JUMPTO, OnButtonJumpto)
	ON_EN_CHANGE(IDC_EDIT_JUMPTO, OnChangeEditJumpto)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogJumpto message handlers

BOOL CDialogJumpto::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_sliderJumpto.SetRange(0,10000);
	double percent;
	percent=dlg->filePos;
	percent/=dlg->txtFile.GetLength();
	percent*=10000;
	percent=floor(percent);
	m_sliderJumpto.SetPos(percent);
	CString s;
	s.Format(_T("%.2f"),percent/100);
	m_editJumpto.SetWindowText(s);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogJumpto::OnCustomdrawSliderJumpto(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	if (skipOne)
	{
		skipOne=false;
		return;
	}
	double d=m_sliderJumpto.GetPos();
	d/=100;
	CString s;
	s.Format(_T("%.2f"),d);
	m_editJumpto.SetWindowText(s);
}

void CDialogJumpto::OnButtonJumpto() 
{
	// TODO: Add your control notification handler code here
	long pos;
	CString s;
	m_editJumpto.GetWindowText(s);
	TCHAR *stop;
	double d=_tcstod(s,&stop);
	pos=dlg->txtFile.GetLength();
	pos*=d/100;
	dlg->randomMove(pos);
}

void CDialogJumpto::OnChangeEditJumpto() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	CString s;
	m_editJumpto.GetWindowText(s);
	TCHAR *stop;
	double d=_tcstod(s,&stop);
	if (d<0)
	{
		d=0;
	}
	else if (d>100)
	{
		d=100;
	}
	d*=100;
	skipOne=true;
	m_sliderJumpto.SetPos(d);
}
