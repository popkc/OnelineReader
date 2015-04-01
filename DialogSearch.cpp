// DialogSearch.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogSearch.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSearch dialog

#include "onelineDlg.h"
extern COnelineApp theApp;
extern COnelineDlg *dlg;

CDialogSearch::CDialogSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSearch)
	m_editSearch = _T("");
	//}}AFX_DATA_INIT
	id=0;
}


void CDialogSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSearch)
	DDX_Control(pDX, IDC_CHECK_SMARTSEG, m_checkSmartSeg);
	DDX_Control(pDX, IDC_PROGRESS_SEARCH, m_progressSearch);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_buttonCancel);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, m_buttonSearch);
	DDX_Text(pDX, IDC_EDIT_SEARCH, m_editSearch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSearch, CDialog)
	//{{AFX_MSG_MAP(CDialogSearch)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnButtonCancel)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SEARCHTHREAD,OnThreadSearchMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSearch message handlers

BOOL CDialogSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (theApp.isNearBy)
	{
		((CButton*)GetDlgItem(IDC_RADIO_NEARBY))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_ALL))->SetCheck(TRUE);
	}
	if (theApp.isDown)
	{
		((CButton*)GetDlgItem(IDC_RADIO_DOWN))->SetCheck(TRUE);
	}
	else
	{
		((CButton*)GetDlgItem(IDC_RADIO_UP))->SetCheck(TRUE);
	}
	if (theApp.smartSeg)
	{
		if (theApp.withSmartSeg)
		{
			m_checkSmartSeg.SetCheck(TRUE);
		}
	}
	else
	{
		m_checkSmartSeg.EnableWindow(FALSE);
	}
	si.text=&m_editSearch;
	si.hWnd=this->m_hWnd;
	searching=false;
	stop=false;
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSearch::OnButtonSearch() 
{
	// TODO: Add your control notification handler code here
	if (searching)
	{
		if (stop)
		{
			m_buttonSearch.SetWindowText(_T("暂停"));
			stop=false;
			thd->ResumeThread();
		}
		else
		{
			thd->SuspendThread();
			m_buttonSearch.SetWindowText(_T("继续"));
			stop=true;
		}
		return;
	}
	UpdateData(TRUE);
	if (m_editSearch!="")
	{
		BOOL isNearby=((CButton*)GetDlgItem(IDC_RADIO_NEARBY))->GetCheck();
		si.isDown=((CButton*)GetDlgItem(IDC_RADIO_DOWN))->GetCheck();
		m_buttonSearch.SetWindowText(_T("暂停"));
		searching=true;
		stop=false;
		m_buttonCancel.EnableWindow(TRUE);
		m_progressSearch.ShowWindow(TRUE);
		long area,endPos;
		if (si.isDown)
		{
			area=dlg->txtFile.GetLength()-dlg->filePos;
			endPos=dlg->txtFile.GetLength();
			if (isNearby&&endPos-dlg->filePos>65536)
			{
				endPos=dlg->filePos+65536;
			}
		}
		else
		{
			area=dlg->filePos;
			endPos=dlg->bomSize;
			if (isNearby&&area-endPos>65536)
			{
				endPos=area-65536;
			}
		}
		area>>=12;
		area++;
		if (isNearby&&area>16)
		{
			area=16;
		}
		si.endPos=endPos;
		si.id=id;
		m_progressSearch.SetRange32(0,area);
		if (theApp.smartSeg&&m_checkSmartSeg.GetCheck())
		{
			thd=AfxBeginThread(threadSearch,&si);
		}
		else
			thd=AfxBeginThread(threadSearchQuick,&si);
	}
	else
	{
		MessageBox(_T("请输入要查找的内容。"));
	}
}

UINT threadSearchQuick(LPVOID pParam)
{
	MSG msg;
	if (PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
	{
		return 0;
	}
	searchInfo *si;
	si=(searchInfo*)pParam;
	BOOL isDown=si->isDown;
	CString text=*(si->text);
	HWND hWnd=si->hWnd;
	long pos,endPos;
	endPos=si->endPos;
	DWORD id=si->id;
	id<<=16;
	pos=dlg->filePos;
	long area,areaCount=0;
	area=pos>>12;
	unsigned char *pat;
	unsigned short l;
	BYTE fileCode=dlg->fileCode;
	bool ansiSure;
	int i,j,f,g;
	int bomSize;
	bomSize=dlg->bomSize;
	long fileLen;
	fileLen=dlg->txtFile.GetLength();
	switch(fileCode)
	{
	case 1:
		l=WideCharToMultiByte(CP_ACP,0,(LPCTSTR)text,-1,NULL,0,0,0);
		pat=new unsigned char[l];
		WideCharToMultiByte(CP_ACP,0,(LPCTSTR)text,-1,(char *)pat,l,0,0);
		l--;
		ansiSure=false;
		for (i=0;i<l;i++)
		{
			if ((pat[i]&0x80)!=0x80)
			{
				ansiSure=true;
				break;
			}
		}
		break;
	case 2:
		l=WideCharToMultiByte(CP_UTF8,0,(LPCTSTR)text,-1,NULL,0,0,0);
		pat=new unsigned char[l];
		WideCharToMultiByte(CP_UTF8,0,(LPCTSTR)text,-1,(char *)pat,l,0,0);
		l--;
		break;
	case 3:
		pat=(unsigned char *)((LPCTSTR)text);
		l=text.GetLength()*2;
		break;
	}

	unsigned short bmBc[256];
	unsigned short *bmGs=new unsigned short[l];
	unsigned short *suff=new unsigned short[l];
	suff=new unsigned short[l];
	for (i=0;i<256;i++)
	{
		bmBc[i]=l;
	}
	for (i = 0; i < l; ++i)
		bmGs[i] = l;
	if (isDown)
	{
		pos++;
		
		j=l-1;
		for (i=0;i<j;i++)
		{
			bmBc[pat[i]]=j-i;
		}
		
		suff[l - 1] = l;
		g = l - 1;
		for (i = l - 2; i >= 0; --i) {
			if (i > g && suff[i + l - 1 - f] < i - g)
				suff[i] = suff[i + l - 1 - f];
			else {
				if (i < g)
					g = i;
				f = i;
				while (g >= 0 && pat[g] == pat[g + l - 1 - f])
					--g;
				suff[i] = f - g;
			}
		}
		
		j = 0;
		for (i = l - 1; i >= 0; --i)
			if (suff[i] == i + 1)
				for (; j < l - 1 - i; ++j)
					if (bmGs[j] == l)
						bmGs[j] = l - 1 - i;
		for (i = 0; i <= l - 2; ++i)
			bmGs[l - 1 - suff[i]] = l - 1 - i;
	}
	else
	{
		pos-=l;

		j=l-1;
		for (i=l-1;i>0;i--)
		{
			bmBc[pat[i]]=i;
		}
		
		suff[0] = l;
		g = 0;
		for (i = 1; i < l; ++i) {
			if (i < g && suff[i - f] < g-i)
				suff[i] = suff[i - f];
			else {
				if (i > g)
					g = i;
				f = i;
				while (g < l && pat[g] == pat[g - f])
					++g;
				suff[i] = g-f;
			}
		}
		
		j = l-1;
		for (i = 0; i < l; ++i)
			if (suff[i] == l-i)
				for (; j >l-1-i; --j)
					if (bmGs[j] == l)
						bmGs[j] = i;
		for (i = l-1; i >= 1; --i)
			bmGs[suff[i]] = i;
	}
	delete[] suff;


	unsigned char t;
	if (isDown)
	{
		while(pos<endPos)
		{
			if (fileCode==3&&(pos&1)==1)
			{
				pos++;
			}
			for (i=l-1;i>=0;i--)
			{
				t=dlg->getOneByte(pos+i);
				if (pat[i]!=t)
				{
					break;
				}
			}
			if (i<0)
			{
				if (fileCode==1&&!ansiSure)
				{
					t=findAnsiPre(pos-1);
					if (t==0)
					{
						pos++;
						goto posDownEnd;
					}
					else if (t==2)
					{
						t=findAnsiNext(pos+l);
						if (t==0)
						{
							pos++;
							goto posDownEnd;
						}
					}
				}
				PostMessage(hWnd,WM_SEARCHTHREAD,id+1,pos);
				goto posFindIt;
			}
			else
			{
				pos+=max(bmGs[i],bmBc[t]-l+1+i);
			}
posDownEnd:	
			if (area!=pos>>12)
			{
				if (PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
				{
					goto posFindIt;
				}
				area=pos>>12;
				areaCount++;
				PostMessage(hWnd,WM_SEARCHTHREAD,id,areaCount);
			}
		}//end while
	}
	else
	{
		while(pos>=endPos)
		{
			if (fileCode==3&&(pos&1)==1)
			{
				pos--;
			}
			for (i=0;i<l;i++)
			{
				t=dlg->getOneByte(pos+i);
				if (pat[i]!=t)
				{
					break;
				}
			}
			if (i==l)
			{
				if (fileCode==1&&!ansiSure)
				{
					t=findAnsiPre(pos-1);
					if (t==0)
					{
						pos--;
						goto posUpEnd;
					}
					else if (t==2)
					{
						t=findAnsiNext(pos+l);
						if (t==0)
						{
							pos--;
							goto posUpEnd;
						}
					}
				}
				PostMessage(hWnd,WM_SEARCHTHREAD,id+1,pos);
				goto posFindIt;
			}
			else
			{
				pos-=max(bmGs[i],bmBc[t]-i);
				//ASSERT(max(bmGs[i],bmBc[t]-i)>0);
			}
posUpEnd:	
			if (area!=pos>>12)
			{
				if (PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
				{
					goto posFindIt;
				}
				area=pos>>12;
				areaCount++;
				PostMessage(hWnd,WM_SEARCHTHREAD,id,areaCount);
			}
		}//end while
	}
	PostMessage(hWnd,WM_SEARCHTHREAD,id+2,0);
posFindIt:
	if (fileCode!=3)
	{
		delete[] pat;
	}
	delete[] bmGs;
	return 0;
}

BYTE findAnsiPre(const long& pos)
{
	int j;
	int bomSize=dlg->bomSize;
	for (j=0;j<4096;j++)
	{
		if (pos-j<=bomSize)
		{
			j=pos-bomSize;
			goto posFindAnsi;
		}
		if ((dlg->getOneByte(pos-j)&0x80)!=0x80)
		{
posFindAnsi:
			if ((j&1)==1)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	return 2;
}

BYTE findAnsiNext(const long& pos)
{
	int j;
	int fileLen=dlg->txtFile.GetLength();
	for(j=0;j<4096;j++)
	{
		if (pos+j>=fileLen)
		{
			j=fileLen-pos;
			goto posFindAnsiNext;
		}
		if ((dlg->getOneByte(pos+j)&0x80)!=0x80)
		{
posFindAnsiNext:
			if ((j&1)==1)
			{
				return 0;
			}
			else
			{
				return 1;
			}
		}
	}
	return 2;
}

LRESULT CDialogSearch::OnThreadSearchMessage(WPARAM wParam,LPARAM lParam)
{
	if ((wParam&0xFFFF0000)>>16!=id)
	{
		return 0;
	}
	int mt=wParam&0x0000ffff;
	switch(mt)
	{
	case 0:
		m_progressSearch.SetPos(lParam);
		return 0;
	case 1:
		dlg->filePos=lParam;
		dlg->needReDraw=true;
		dlg->Invalidate();
		MessageBox(_T("找到了！"));
		break;
	case 2:
		MessageBox(_T("找不到对应的内容。"));
		break;
	case 3:
		MessageBox(_T("发生错误！"));
		break;
	}
	m_buttonSearch.SetWindowText(_T("查找"));
	searching=false;
	m_buttonCancel.EnableWindow(FALSE);
	m_progressSearch.SetPos(0);
	m_progressSearch.ShowWindow(FALSE);
	return 0;
}

void CDialogSearch::OnButtonCancel() 
{
	// TODO: Add your control notification handler code here
	id++;
	thd->PostThreadMessage(WM_SEARCHTHREAD,0,0);
	if (stop)
	{
		thd->ResumeThread();
	}
	m_buttonSearch.SetWindowText(_T("查找"));
	searching=false;
	m_buttonCancel.EnableWindow(FALSE);
	m_progressSearch.SetPos(0);
	m_progressSearch.ShowWindow(FALSE);
}

void CDialogSearch::OnDestroy() 
{
	CDialog::OnDestroy();
	if (searching)
	{
		id++;
		thd->PostThreadMessage(WM_SEARCHTHREAD,0,0);
		if (stop)
		{
			thd->ResumeThread();
		}
	}
	theApp.isDown=((CButton*)GetDlgItem(IDC_RADIO_DOWN))->GetCheck()!=0;
	theApp.isNearBy=((CButton*)GetDlgItem(IDC_RADIO_NEARBY))->GetCheck()!=0;
	theApp.withSmartSeg=m_checkSmartSeg.GetCheck()!=0;
	theApp.iniFile->SetValueB(_T("find"),_T("isdown"),theApp.isDown);
	theApp.iniFile->SetValueB(_T("find"),_T("isnearby"),theApp.isNearBy);
	theApp.iniFile->SetValueB(_T("find"),_T("withsmartseg"),theApp.withSmartSeg);
	// TODO: Add your message handler code here
	
}

UINT threadSearch(LPVOID pParam)
{
	searchInfo *si;
	si=(searchInfo*)pParam;
	BOOL isDown=si->isDown;
	CString text=*(si->text);
	HWND hWnd=si->hWnd;
	MSG msg;
	long pos,recordPos,endPos;
	endPos=si->endPos;
	DWORD id=si->id;
	id<<=16;
	int off,comparePos;
	WCHAR w;
	pos=dlg->filePos;
	long area,areaCount=0;
	area=pos>>12;
	if (PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
	{
		return 0;
	}
	if(isDown)
	{
		off=dlg->readOneWord(pos,&w);
		pos+=off;
		for (;;)
		{
			if (pos>=endPos)
			{
				goto posNotFind;
			}
			for(comparePos=0;;)
			{
				off=dlg->readOneWord(pos,&w);
				pos+=off;
				if (w==text[comparePos])
				{
					if (!comparePos)
						recordPos=pos-off;
					comparePos++;
					if (comparePos>=text.GetLength())
						goto posFind;
				}
				else
				{
					break;
				}
			}
			if (area!=pos>>12)
			{
				if (PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
				{
					return 0;
				}
				area=pos>>12;
				areaCount++;
				PostMessage(si->hWnd,WM_SEARCHTHREAD,id,areaCount);
			}
		}
	}
	else
	{
		int spos=text.GetLength()-1;
		for(;;)
		{
			if (pos<endPos)
			{
				goto posNotFind;
			}
			for(comparePos=spos;;)
			{
				off=dlg->getBackWord(pos,&w);
				pos-=off;
				if (w==text[comparePos])
				{
					comparePos--;
					if (comparePos<0)
					{
						recordPos=pos;
						goto posFind;
					}
				}
				else
				{
					break;
				}
			}
			if (area!=pos>>12)
			{
				if (PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
				{
					return 0;
				}
				area=pos>>12;
				areaCount++;
				PostMessage(si->hWnd,WM_SEARCHTHREAD,id,areaCount);
			}
		}
	}
	return 0;
posNotFind:
	PostMessage(si->hWnd,WM_SEARCHTHREAD,id+2,0);
	return 0;
posFind:
	PostMessage(si->hWnd,WM_SEARCHTHREAD,id+1,recordPos);
	return 0;
}
