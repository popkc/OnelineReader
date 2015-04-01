// DialogVoice.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogVoice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include <spuihelp.h>
/////////////////////////////////////////////////////////////////////////////
// CDialogVoice dialog

extern COnelineApp theApp;
CDialogVoice::CDialogVoice(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogVoice::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogVoice)
	m_staticRate = _T("");
	m_staticVolume = _T("");
	//}}AFX_DATA_INIT
}


void CDialogVoice::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogVoice)
	DDX_Control(pDX, IDC_COMBO_VOICE, m_combo);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_sVolume);
	DDX_Control(pDX, IDC_SLIDER_RATE, m_sRate);
	DDX_Text(pDX, IDC_STATIC_RATE, m_staticRate);
	DDX_Text(pDX, IDC_STATIC_VOLUME, m_staticVolume);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogVoice, CDialog)
	//{{AFX_MSG_MAP(CDialogVoice)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_RATE, OnCustomdrawSliderRate)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_VOLUME, OnCustomdrawSliderVolume)
	ON_BN_CLICKED(IDC_BUTTON_LISTEN, OnButtonListen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogVoice message handlers

BOOL CDialogVoice::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_COMBO_VOICE,&hCombo);
	SpInitTokenComboBox(hCombo,SPCAT_VOICES);
	m_sRate.SetRange(-10,10);
	m_sVolume.SetRange(0,100);
	m_sRate.SetTicFreq(1);
	m_sRate.SetPos(theApp.voiceRate);
	m_sVolume.SetPos(theApp.voiceVolume);
	ISpObjectToken *t2;
	t2=theApp.cToken;
	//theApp.cVoice->GetVoice(&t2);
	if (t2!=NULL)
	{
		int l=m_combo.GetCount();
		ISpObjectToken *tToken;
		WCHAR *id1,*id2;
		t2->GetId(&id1);
		for (int i=0;i<l;i++)
		{
			tToken=(ISpObjectToken*)m_combo.GetItemDataPtr(i);
			tToken->GetId(&id2);
			if (!wcscmp(id1,id2))
			{
				m_combo.SetCurSel(i);
				break;
			}
		}
	}
	int r=theApp.voiceRate;
	_itot(r,m_staticRate.GetBuffer(4),10);
	m_staticRate.ReleaseBuffer();
	setStaticVolume();
	pVoice.CoCreateInstance(CLSID_SpVoice);
	//DWORD t=m_combo.GetItemData(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogVoice::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	SpDestroyTokenComboBox(hCombo);
	pVoice.Release();
}

void CDialogVoice::OnCustomdrawSliderRate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	setStaticRate();
	*pResult = 0;
}

void CDialogVoice::setStaticRate()
{
	int r=m_sRate.GetPos();
	_itot(r,m_staticRate.GetBuffer(4),10);
	m_staticRate.ReleaseBuffer();
	UpdateData(FALSE);
}

void CDialogVoice::setStaticVolume()
{
	int v=m_sVolume.GetPos();
	_itot(v,m_staticVolume.GetBuffer(4),10);
	m_staticVolume.ReleaseBuffer();
	UpdateData(FALSE);
}

void CDialogVoice::OnCustomdrawSliderVolume(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	setStaticVolume();
	*pResult = 0;
}

void CDialogVoice::saveVar()
{
	theApp.voiceRate=m_sRate.GetPos();
	theApp.voiceVolume=m_sVolume.GetPos();
	ISpObjectToken* pt=SpGetCurSelComboBoxToken(hCombo);
	WCHAR* pw;
	pt->GetId(&pw);
	//int l=WideCharToMultiByte(CP_ACP,0,pw,-1,NULL,0,NULL,NULL);
	//WideCharToMultiByte(CP_ACP,0,pw,-1,theApp.voiceToken.GetBuffer(l),l,NULL,NULL);
	//theApp.voiceToken.ReleaseBuffer();
	theApp.voiceToken=pw;
	theApp.cVoice->SetVoice(pt);
	theApp.cVoice->SetRate(theApp.voiceRate);
	theApp.cVoice->SetVolume(theApp.voiceVolume);
	theApp.cToken=pt;
	theApp.iniFile->SetValue(_T("voice"),_T("token"),theApp.voiceToken);
	theApp.iniFile->SetValueI(_T("voice"),_T("volume"),theApp.voiceVolume);
	theApp.iniFile->SetValueI(_T("voice"),_T("rate"),theApp.voiceRate);
}

void CDialogVoice::OnButtonListen() 
{
	// TODO: Add your control notification handler code here
	//GetDlgItem(IDC_BUTTON_LISTEN)->EnableWindow(FALSE);
	ISpObjectToken* pt=SpGetCurSelComboBoxToken(hCombo);
	pVoice->SetVoice(pt);
	pVoice->SetRate(m_sRate.GetPos());
	pVoice->SetVolume(m_sVolume.GetPos());
	pVoice->Speak(NULL,SPF_PURGEBEFORESPEAK,NULL);
	pVoice->Speak(L"欢迎使用一行小说阅读器。Thank you!",SPF_ASYNC|SPF_IS_NOT_XML,NULL);
}
