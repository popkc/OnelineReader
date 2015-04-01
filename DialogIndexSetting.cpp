// DialogIndexSetting.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogIndexSetting.h"
#include <boost/regex.hpp>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogIndexSetting dialog



extern COnelineApp theApp;

CDialogIndexSetting::CDialogIndexSetting(CDialogIndex* pParent /*=NULL*/)
	: CDialog(CDialogIndexSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogIndexSetting)
	m_maxchar = 0;
	//}}AFX_DATA_INIT
	par=pParent;
}


void CDialogIndexSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogIndexSetting)
	DDX_Control(pDX, IDC_EDIT_MATCH, m_editMatch);
	DDX_Control(pDX, IDC_LIST_REGEX, m_listRegex);
	DDX_Control(pDX, IDC_EDIT_REGEX, m_editRegex);
	DDX_Control(pDX, IDC_SPIN_MAXCHAR, m_spinMaxchar);
	DDX_Text(pDX, IDC_EDIT_MAXCHAR, m_maxchar);
	DDV_MinMaxUInt(pDX, m_maxchar, 5, 999999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogIndexSetting, CDialog)
	//{{AFX_MSG_MAP(CDialogIndexSetting)
	ON_LBN_SELCHANGE(IDC_LIST_REGEX, OnSelchangeListRegex)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_DEFAULT, OnButtonDefault)
	ON_BN_CLICKED(IDC_BUTTON_MATCH, OnButtonMatch)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogIndexSetting message handlers

BOOL CDialogIndexSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_maxchar=theApp.regexMaxChar;
	m_spinMaxchar.SetRange(5,999999);
	m_listRegex.ResetContent();
	for (int i=0;i<theApp.rxs.size();i++)
	{
		m_listRegex.AddString(theApp.rxs[i]);
	}
	resetListH();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogIndexSetting::OnSelchangeListRegex() 
{
	// TODO: Add your control notification handler code here

	int i=m_listRegex.GetCurSel();
	CString s;
	m_listRegex.GetText(i,s);
	m_editRegex.SetWindowText(s);
}

void CDialogIndexSetting::resetListH()
{
	CString str;
	CSize   sz;
	int     dx=0;
	CDC*    pDC = m_listRegex.GetDC();
	for (int i=0;i < m_listRegex.GetCount();i++)
	{
		m_listRegex.GetText( i, str );
		sz = pDC->GetTextExtent(str);
		
		if (sz.cx > dx)
			dx = sz.cx;
	}
	m_listRegex.ReleaseDC(pDC);
	
	// Set the horizontal extent only if the current extent is not large enough.
	if (m_listRegex.GetHorizontalExtent() != dx+150)
	{
		m_listRegex.SetHorizontalExtent(dx+150);
		//ASSERT(pmyListBox->GetHorizontalExtent() == dx);
	}

}

void CDialogIndexSetting::OnButtonAdd() 
{
	// TODO: Add your control notification handler code here
	CString s;
	m_editRegex.GetWindowText(s);
	if (!testIsValidRegex(s))
	{
		return;
	}
	m_listRegex.AddString(s);
	resetListH();
}

void CDialogIndexSetting::OnButtonDelete() 
{
	// TODO: Add your control notification handler code here
	int i=m_listRegex.GetCurSel();
	if (i==LB_ERR)
	{
		MessageBox(_T("请选择要删除的正则表达式！"));
		return;
	}
	m_listRegex.DeleteString(i);
	resetListH();
}

void CDialogIndexSetting::OnButtonModify() 
{
	// TODO: Add your control notification handler code here
	int i=m_listRegex.GetCurSel();
	if (i==LB_ERR)
	{
		MessageBox(_T("请选择要修改的正则表达式！"));
		return;
	}
	m_listRegex.DeleteString(i);
	CString s;
	m_editRegex.GetWindowText(s);
	if (!testIsValidRegex(s))
	{
		return;
	}
	m_listRegex.InsertString(i,s);
	m_listRegex.SetCurSel(i);
	resetListH();
}

void CDialogIndexSetting::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	if (!UpdateData(TRUE))
	{
		return;
	}
	theApp.regexMaxChar=m_maxchar;
	theApp.iniFile->SetValueI(_T("find"),_T("regexmaxchar"),theApp.regexMaxChar);
	theApp.rxs.clear();
	CString s,sr;
	MapKey* mk=&theApp.iniFile->values[_T("find")];
	mk->erase(mk->lower_bound(_T("r")),mk->upper_bound(_T("r9999999")));
	TCHAR ts[10];
	for (int i=0;i<m_listRegex.GetCount();i++)
	{
		m_listRegex.GetText(i,s);
		theApp.rxs.push_back(s);
		_itot(i,ts,10);
		sr=_T('r');
		sr+=ts;
		(*mk)[sr]=s;
	}
	par->makeRegex();
	theApp.regexCount=m_listRegex.GetCount();
	theApp.iniFile->SetValueI(_T("find"),_T("regexcount"),theApp.regexCount);
	EndDialog(0);
}

void CDialogIndexSetting::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CDialogIndexSetting::OnButtonDefault() 
{
	// TODO: Add your control notification handler code here
	m_maxchar=50;
	m_listRegex.ResetContent();
	for (int i=0;i<3;i++)
	{
		m_listRegex.AddString(theApp.defaultRegex(i));
	}
	UpdateData(FALSE);
	resetListH();
}


void CDialogIndexSetting::OnButtonMatch() 
{
	// TODO: Add your control notification handler code here
	CString text;
	m_editMatch.GetWindowText(text);
	if (text.IsEmpty())
	{
		MessageBox(_T("请输入要匹配的字符串"));
		return;
	}
	int l=m_listRegex.GetCount();
	if (!l)
	{
		MessageBox(_T("没有有效的正则表达式"));
		return;
	}
	CString s;
	for (int i=0;i<l;i++)
	{
		m_listRegex.GetText(i,s);
		boost::wregex r(s);
		if (boost::regex_search((LPCTSTR)text,r))
		{
			MessageBox(_T("匹配成功！"));
			return;
		}
	}
	MessageBox(_T("匹配失败。"));
	return;
}

bool CDialogIndexSetting::testIsValidRegex(CString &s)
{
	try
	{
		boost::wregex r(s);
	}
	catch (const boost::regex_error &)
	{
		MessageBox(_T("不是有效的正则表达式！"));
		return false;
	}
	//	::MessageBox(NULL,_T("不是有效的正则表达式！"),_T("错误"),MB_OK);
	return true;
}