// onelineDlg.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "onelineDlg.h"
#include <AFXADV.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <AFXMT.H>

extern COnelineApp theApp;
extern COnelineDlg *dlg;
CCriticalSection csblock;

#include <sphelper.h>
/////////////////////////////////////////////////////////////////////////////
// COnelineDlg dialog

COnelineDlg::COnelineDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COnelineDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COnelineDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	ignoreClick=false;
	needReDraw=true;
	isSizeMove=false;
	isPopupMenu=false;
	moveDirection=0;
	rolling=false;
	//lastMenuTime=0;
	flagNotMove=false;
	dAbout=NULL;
	wordOffset=0;
	dJumpto=NULL;
	isReading=false;
	dSearch=NULL;
	cacheArea=-1;
	dIndex=NULL;
	checkMenuInsideClick=false;
}

void COnelineDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COnelineDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(COnelineDlg, CDialog)
	//{{AFX_MSG_MAP(COnelineDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_NCHITTEST()
	ON_MESSAGE(WM_DISPLAYCHANGE,OnDisplayChange)
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCRBUTTONUP()
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	ON_MESSAGE(WM_TRAYEVENT,OnTrayEvent)
	ON_WM_DESTROY()
	ON_COMMAND(ID_MENU_SHOWTRAYICON, OnMenuShowtrayicon)
	ON_COMMAND(ID_MENU2_SHOW, OnMenu2Show)
	ON_COMMAND(ID_MENU_ABOUT, OnMenuAbout)
	ON_COMMAND(ID_MENU_OPEN, OnMenuOpen)
	ON_COMMAND(ID_MENU_AUTODETECT, OnMenuAutodetect)
	ON_WM_SIZE()
	ON_WM_NCMOUSEMOVE()
	ON_COMMAND(ID_MENU_SETTINGS, OnMenuSettings)
	ON_WM_MOUSEWHEEL()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_MENU_READ, OnMenuRead)
	ON_WM_GETDLGCODE()
	ON_WM_ERASEBKGND()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MENU_MOVEFORWARD, OnMenuMoveforward)
	ON_COMMAND(ID_MENU_MOVEBACKWARD, OnMenuMovebackward)
	ON_COMMAND(ID_MENU_AUTOROLL, OnMenuAutoroll)
	ON_WM_NCLBUTTONDBLCLK()
	ON_COMMAND(ID_MENU_JUMP, OnMenuJump)
	ON_COMMAND(ID_MENU_FIND, OnMenuFind)
	ON_WM_GETMINMAXINFO()
	ON_COMMAND(ID_MENU_INDEX, OnMenuIndex)
	ON_COMMAND(ID_MENU2_SHOWTRAYICON, OnMenuShowtrayicon)
	ON_COMMAND(ID_MENU2_EXIT, OnMenuExit)
	ON_COMMAND(ID_MENU2_READ, OnMenuRead)
	ON_COMMAND(ID_MENU2_SETTINGS, OnMenuSettings)
	ON_WM_NCACTIVATE()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(ID_MENU_ANSI,ID_MENU_UNICODE,OnMenuCodes)
	ON_MESSAGE(WM_WAKEUP,OnWakeUp)
	ON_COMMAND_RANGE(ID_MENU_RECENTFILE,ID_MENU_RECENTFILE+9,OnMenuRecentFile)
	ON_MESSAGE(WM_HOTKEY,OnHotKey)
	ON_MESSAGE(WM_ENTERSIZEMOVE,OnEnterSizeMove)
	ON_MESSAGE(WM_EXITSIZEMOVE,OnExitSizeMove)
	ON_MESSAGE(WM_TTSEVENT,OnTTSEvent)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnelineDlg message handlers

BOOL COnelineDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	screenX=GetSystemMetrics(SM_CXSCREEN);
	screenY=GetSystemMetrics(SM_CYSCREEN);
	isHide=false;
	hideFlag=false;
	timerSet=false;
	lButtonDown=false;
	dSettings=NULL;
	blocksize=0;
	remainSpace=0;
	youjian.LoadMenu(IDR_MENU_POPUP);
	popupMenu=youjian.GetSubMenu(0);
	trayMenu=youjian.GetSubMenu(1);
	if (theApp.showTrayIcon)
	{
		createTrayData();
		Shell_NotifyIcon(NIM_ADD,trayData);
	}
	else
	{
		trayData=NULL;
		popupMenu->CheckMenuItem(ID_MENU_SHOWTRAYICON,MF_UNCHECKED);
		trayMenu->CheckMenuItem(ID_MENU2_SHOWTRAYICON,MF_UNCHECKED);
	}
	ModifyStyleEx( WS_EX_APPWINDOW, 0 );
	if (theApp.winx<0||theApp.winx>screenX)
	{
		theApp.winx=0;
	}
	if (theApp.winy<0||theApp.winy>screenY)
	{
		theApp.winy=0;
	}
	if (theApp.winwidth>screenX)
	{
		theApp.winwidth=screenX;
	}
	SetWindowPos(&CWnd::wndTopMost,theApp.winx,theApp.winy,theApp.winwidth,theApp.winheight,0);
	fileOpened=false;
	pOldBitmap=NULL;
	MemDC.CreateCompatibleDC(NULL);
	MemDC.SetBkColor(theApp.bgcolor);
	MemDC.SetTextColor(theApp.fcolor);
	pOldFont=MemDC.SelectObject(&(theApp.font));
	this->SetWindowText(_T("一行小说阅读器"));
	//CoInitialize(NULL);
	TCHAR buf[2];
	CString ts;
	for (int i=0;i<10;i++)
	{
		_itot(i,buf,10);
		ts=theApp.iniFile->GetValue(_T("recentfile"),buf,_T(""));
		if (ts==_T(""))
		{
			break;
		}
		recentFiles.push_back(ts);
		speechBufLength=new unsigned short[100];
	}
	drawMenu();
	if (theApp.hasTTS)
	{
		if(!RegisterHotKey(this->m_hWnd,1,MOD_CONTROL,VK_F12))
		{
			MessageBox(_T("注册热键Ctrl+F12失败！"));
		}
		ULONGLONG interest=SPFEI(SPEI_END_INPUT_STREAM)|SPFEI(SPEI_WORD_BOUNDARY);
		theApp.cVoice->SetInterest(interest,interest);
		theApp.cVoice->SetNotifyWindowMessage(this->m_hWnd,WM_TTSEVENT,0,0);
	}
	std::list<CString>::iterator it;
	it=recentFiles.begin();
	if (it!=recentFiles.end())
	{
		openNovel(*it);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COnelineDlg::OnPaint() 
{
	CRect rect;
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		//CDialog::OnPaint();
		if(GetUpdateRect(&rect))
		{
			PAINTSTRUCT ps;
			CDC *pDC=BeginPaint(&ps);
			if(isHide)
			{
				pDC->FillSolidRect(rect,theApp.bgcolor);
			}
			else if (needReDraw)
			{
				needReDraw=false;
				CRect r;
				GetWindowRect(r);
				ScreenToClient(r);
				if(pOldBitmap!=NULL)
				{
					MemDC.SelectObject(pOldBitmap);
					MemBitmap.DeleteObject();
				}
				MemBitmap.CreateCompatibleBitmap(pDC,r.Width(),r.Height());
				pOldBitmap=MemDC.SelectObject(&MemBitmap);
				MemDC.FillSolidRect(r,theApp.bgcolor);
				if (fileOpened)
				{
					getOutput();
				}
				else
				{
					outputWord=_T("一行小说阅读器 By:念千");
					MemDC.TextOut(theApp.sleft,theApp.stop,outputWord);
				}
				pDC->BitBlt(0,0,r.Width(),r.Height(),&MemDC,0,0,SRCCOPY);
			}
			else
				pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&MemDC,rect.left,rect.top,SRCCOPY);
			EndPaint(&ps);
		}
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COnelineDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

UINT COnelineDlg::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//return CDialog::OnNcHitTest(point);
	CRect r;
	this->GetWindowRect(&r);
	if (point.x>r.right-5)
	{
		return HTRIGHT;
	}
	else if (point.x<r.left+5)
	{
		return HTLEFT;
	}
	else
		return HTCAPTION;
}

LRESULT COnelineDlg::OnDisplayChange(WPARAM wParam, LPARAM lParam)
{
	short newx,newy;
	char* p;
	p=(char*)&lParam;
	memcpy(&newy,p,2);
	memcpy(&newx,p+2,2);
	screenX=newx;
	screenY=newy;
	CRect r;
	GetWindowRect(&r);
	if (isHide)
	{
		CPoint np;
		np=r.TopLeft();
		switch(theApp.hidePos)//移动以避免被屏幕挡住
		{
		case 1:
		case 3:
			if (r.bottom>screenY)
			{
				np.y=screenY-r.Height()-1;
			}
			break;
		case 2:
		case 4:
			if (r.right>screenX)
			{
				np.x=screenX-r.Width();
			}
			break;
		}
		if (np!=r.TopLeft())
		{
			if (r.Width()>screenX)
			{
				SetWindowPos(NULL,0,np.y,screenX,r.Height(),SWP_NOZORDER);
			}
			else
			{
				SetWindowPos(NULL,np.x,np.y,0,0,SWP_NOZORDER|SWP_NOSIZE);
			}
		}
	}
	else
	{
		hideToShow();
	}
	return 0;
}

void COnelineDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (ignoreClick)
	{
		ignoreClick=false;
	}
	else
	{
		if (checkMenuInsideClick)
		{
			checkMenuInsideClick=false;
			if (!checkMenuOutsideClick)
			{
				flagNotMove=true;
			}
		}
		clickPoint=point;
		lButtonDown=true;
	}
	CDialog::OnNcLButtonDown(nHitTest, point);
}

void COnelineDlg::hideToEdge()
{
	hideFlag=true;
	CRect r;
	GetWindowRect(&r);
	int x,y;
	
	switch(theApp.hidePos)
	{
	case 1:
		x=1-r.Width();
		y=r.top;
		break;
	case 2:
		x=r.left;
		y=1-r.Height();
		break;
	case 3:
		x=screenX-1;
		y=r.top;
		break;
	case 4:
		x=r.left;
		y=screenY-1;
		break;
	default:
		return;
	}
	isHide=true;
	SetWindowPos(NULL,x,y,0,0,SWP_NOSIZE|SWP_NOZORDER);
}

void COnelineDlg::showFromEdge()
{
	//RECT r;
	//GetWindowRect(&r);
	isHide=false;
	hideToShow();
}

void CALLBACK EXPORT MyOnTimer(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime)
{
	if (dlg->isSizeMove||dlg->isPopupMenu)
	{
		return;
	}
	POINT point;
	GetCursorPos(&point);
	RECT r;
	dlg->GetWindowRect(&r);
	if (!PtInRect(&r,point))
	{
		dlg->KillTimer(1);
		dlg->timerSet=false;
		switch(theApp.hidePos)
		{
		case 1:
			if (r.left>0) return;
			break;
		case 3:
			if(r.right<dlg->screenX) return;
			break;
		}
		dlg->hideToEdge();
	}
}

void COnelineDlg::OnNcRButtonUp(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	isPopupMenu=true;
	checkMenuInsideClick=true;
	checkMenuOutsideClick=false;
	popupMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
	//lastMenuTime=clock();
	isPopupMenu=false;
	CDialog::OnNcRButtonUp(nHitTest, point);
}

void COnelineDlg::OnMenuExit() 
{
	// TODO: Add your command handler code here
	PostQuitMessage(0);
}

LRESULT COnelineDlg::OnTrayEvent(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
	case WM_RBUTTONUP:
		POINT point;
		GetCursorPos(&point);
		SetForegroundWindow();
		trayMenu->TrackPopupMenu(TPM_LEFTALIGN,point.x,point.y,this);
		break;
	case WM_LBUTTONDOWN:
		OnMenu2Show();
		break;
	}
	return 0;
}

void COnelineDlg::createTrayData()
{
	trayData=new NOTIFYICONDATA;
	trayData->cbSize=(DWORD)sizeof(NOTIFYICONDATA);
	trayData->hWnd=this->m_hWnd;
	trayData->uID=IDR_MAINFRAME;
	trayData->uFlags=NIF_ICON|NIF_MESSAGE|NIF_TIP;
	trayData->uCallbackMessage=WM_TRAYEVENT;
	trayData->hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));
	_tcscpy(trayData->szTip,_T("一行小说阅读器 By:念千"));
}

void COnelineDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	CRect r;
	GetWindowRect(&r);
	int w=r.Width();
	if (isHide)
	{
		switch(theApp.hidePos)
		{
		case 1:
			r.left=0;
			break;
		case 2:
			r.top=0;
			break;
		case 3:
			r.left=screenX-r.Width();
			break;
		case 4:
			r.top=screenY-r.Height();
			break;
		}
	}
	theApp.iniFile->SetValueI(_T("window"),_T("x"),r.left);
	theApp.iniFile->SetValueI(_T("window"),_T("y"),r.top);
	theApp.iniFile->SetValueI(_T("window"),_T("width"),w);
	theApp.iniFile->SetValueI(_T("window"),_T("hidepos"),theApp.hidePos);
	if (fileOpened)
	{
		theApp.iniFile->SetValueI(txtFile.GetFilePath(),_T("pos"),filePos);
	}
	std::list<CString>::iterator it;
	it=recentFiles.begin();
	int l=recentFiles.size();
	TCHAR buf[2];
	for (w=0;w<l;w++)
	{
		_itot(w,buf,10);
		theApp.iniFile->SetValue(_T("recentfile"),buf,*it);
		it++;
	}
	theApp.iniFile->WriteFile();
	Shell_NotifyIcon(NIM_DELETE,trayData);
	//ExitProcess(0);
}

void COnelineDlg::OnMenuShowtrayicon() 
{
	// TODO: Add your command handler code here
	if(theApp.showTrayIcon)
	{
		theApp.showTrayIcon=false;
		theApp.iniFile->SetValueB(_T("settings"),_T("showtrayicon"),false);
		popupMenu->CheckMenuItem(ID_MENU_SHOWTRAYICON,MF_UNCHECKED);
		trayMenu->CheckMenuItem(ID_MENU2_SHOWTRAYICON,MF_UNCHECKED);
		Shell_NotifyIcon(NIM_DELETE,trayData);
	}
	else
	{
		if (!trayData)
		{
			createTrayData();
		}
		theApp.showTrayIcon=true;
		theApp.iniFile->SetValueB(_T("settings"),_T("showtrayicon"),true);
		popupMenu->CheckMenuItem(ID_MENU_SHOWTRAYICON,MF_CHECKED);
		trayMenu->CheckMenuItem(ID_MENU2_SHOWTRAYICON,MF_CHECKED);
		Shell_NotifyIcon(NIM_ADD,trayData);
	}
}

void COnelineDlg::OnMenu2Show() 
{
	// TODO: Add your command handler code here
	if (isHide)
	{
		showFromEdge();
	}
}

void COnelineDlg::OnMenuAbout() 
{
	// TODO: Add your command handler code here
	if (!dAbout)
	{
		dAbout=new CDialogAbout(this);
	}
	dAbout->DoModal();
}

void COnelineDlg::OnMenuOpen() 
{
	// TODO: Add your command handler code here
	CFileDialog *cfd=new CFileDialog(TRUE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_HIDEREADONLY,_T("文本文件(*.txt)|*.txt|所有文件(*)|*||"),this);
	if (cfd->DoModal()==IDOK)
	{
		addToRecentFiles(cfd->GetPathName());
		openNovel(cfd->GetPathName());
	}
	delete cfd;
}

void COnelineDlg::openNovel(const CString &filename)
{
	if (fileOpened)
	{
		closeNovel();
	}
	CFileException fe;
	if (!txtFile.Open(filename,CFile::modeRead|CFile::shareDenyWrite,&fe))
	{
		CString emsg(_T("打开文件“"));
		emsg+=fe.m_strFileName;
		emsg+=_T("”时发生错误。\n错误原因：");
		TCHAR etext[100];
		fe.GetErrorMessage(etext,100);
		emsg+=etext;
		MessageBox(emsg,_T("错误"),MB_ICONERROR|MB_OK);
		popupMenu->EnableMenuItem(3,MF_BYPOSITION|MF_GRAYED);
		popupMenu->EnableMenuItem(4,MF_BYPOSITION|MF_GRAYED);
		popupMenu->EnableMenuItem(5,MF_BYPOSITION|MF_GRAYED);
		popupMenu->EnableMenuItem(6,MF_BYPOSITION|MF_GRAYED);
		trayMenu->EnableMenuItem(2,MF_BYPOSITION|MF_GRAYED);
		return;
	}
	fileOpened=true;
	filePos=theApp.iniFile->GetValueI(filename,_T("pos"),0);
	bomSize=theApp.iniFile->GetValueI(filename,_T("bomsize"),0);
	fileCode=theApp.iniFile->GetValueI(filename,_T("code"),0);
	if (fileCode<=0||fileCode>3)
	{
		OnMenuAutodetect();
	}
	else
	{
		if(filePos<bomSize) filePos=bomSize;
		int i=ID_MENU_ANSI-1+fileCode;
		for(int j=ID_MENU_ANSI;j<=ID_MENU_UNICODE;j++)
		{
			if(j!=i)
				popupMenu->CheckMenuItem(j,MF_UNCHECKED);
			else
				popupMenu->CheckMenuItem(j,MF_CHECKED);
		}
		needReDraw=true;
		Invalidate();
	}
	popupMenu->EnableMenuItem(3,MF_BYPOSITION|MF_ENABLED);
	popupMenu->EnableMenuItem(4,MF_BYPOSITION|MF_ENABLED);
	popupMenu->EnableMenuItem(6,MF_BYPOSITION|MF_ENABLED);
	if (theApp.hasTTS)
	{
		popupMenu->EnableMenuItem(5,MF_BYPOSITION|MF_ENABLED);
		trayMenu->EnableMenuItem(2,MF_BYPOSITION|MF_ENABLED);
	}
}

void COnelineDlg::OnMenuCodes(UINT i) 
{
	// TODO: Add your command handler code here
	if(!fileOpened) return;
	fileCode=i-ID_MENU_ANSI+1;
	if (fileCode==2&&bomSize!=3)
	{
		bomSize=0;
	}
	else if (fileCode==3&&bomSize!=2)
	{
		bomSize=0;
	}
	else if (fileCode==0)
	{
		bomSize=0;
	}
	if(filePos<bomSize) filePos=bomSize;
	theApp.iniFile->SetValueI(txtFile.GetFilePath(),_T("code"),fileCode);
	theApp.iniFile->SetValueI(txtFile.GetFilePath(),_T("bomsize"),bomSize);
	UINT j;
	for(j=ID_MENU_ANSI;j<=ID_MENU_UNICODE;j++)
	{
		if(j!=i)
			popupMenu->CheckMenuItem(j,MF_UNCHECKED);
		else
			popupMenu->CheckMenuItem(j,MF_CHECKED);
	}
	needReDraw=true;
	Invalidate();
}

void COnelineDlg::OnMenuAutodetect() 
{
	// TODO: Add your command handler code here
	if(!fileOpened) return;
	char buf[3]={0};
	csblock.Lock();
	txtFile.SeekToBegin();
	txtFile.Read(buf,3);
	csblock.Unlock();
	if (!memcmp(buf,"\xff\xfe",2))
	{
		bomSize=2;
		OnMenuCodes(ID_MENU_UNICODE);
		return;
	}
	else if (!memcmp(buf,"\xef\xbb\xbf",3))
	{
		bomSize=3;
		OnMenuCodes(ID_MENU_UTF8);
		return;
	}
	bomSize=0;
	int l;
	l=txtFile.GetLength();
	if (l>4096)
	{
		l=4096;
	}
	cacheArea=-1;
	char* bbuf=blockAvailable(0);
	BOOL isutf8=IsTextUTF8(bbuf,l);
	if (isutf8)
	{
		OnMenuCodes(ID_MENU_UTF8);
	}
	else
	{
		OnMenuCodes(ID_MENU_ANSI);
	}
}

char* COnelineDlg::blockAvailable(const long &n)
{
	csblock.Lock();
	std::map<long,fileBlock*>::iterator it;
	it=blocks.find(n);
	fileBlock *fb;
	if (it==blocks.end())
	{
		fb=new fileBlock;
		createFileBuf(n,fb);
		blocks.insert(std::pair<long,fileBlock*>(n,fb));
	}
	else
	{
		fb=it->second;
	}
	if (!fb->valid)
	{
		createFileBuf(n,fb);
	}
	char *o=fb->buf;
	csblock.Unlock();
	return o;
}

void COnelineDlg::createFileBuf(const long &n,fileBlock* fb)
{
	fb->buf=new char[4096];
	txtFile.Seek(n*4096,CFile::begin);
	txtFile.Read(fb->buf,4096);
	fb->valid=true;
	blocksize++;
	clearBlock(n);
}

void COnelineDlg::clearBlock(const long &cb)
{
	int j=blocksize-theApp.maxblock;
	if (j>0)
	{
		long nowblock=filePos/4096;
		std::map<long,fileBlock*>::iterator it,it2;
		long first,last;
		for (int i=0;i<j;i++)
		{
			it=blocks.begin();
			for(;;)
			{
				if (it->second->valid==true&&it->first!=cb&&it->first!=nowblock)
				{
					first=it->first;
					break;
				}
				it++;
			}
			it2=blocks.end();
			for(;;)
			{
				it2--;
				if (it2->second->valid==true&&it2->first!=cb&&it2->first!=nowblock)
				{
					last=it2->first;
					break;
				}
			}
			if (nowblock-first<last-nowblock)
			{
				it=it2;
			}
			delete[] it->second->buf;
			it->second->valid=false;
			blocksize--;
		}
	}
}

void COnelineDlg::closeNovel()
{
	csblock.Lock();
	fileOpened=false;
	theApp.iniFile->SetValueI(txtFile.GetFilePath(),_T("pos"),filePos);
	txtFile.Close();
	std::map<long,fileBlock*>::iterator it;
	it=blocks.begin();
	while(it!=blocks.end())
	{
		if (it->second->valid)
		{
			delete[] it->second->buf;
		}
		delete it->second;
		it++;
	}
	blocks.clear();
	blocksize=0;
	csblock.Unlock();
	if (rolling)
	{
		OnMenuAutoroll();
	}
	if (isReading)
	{
		OnMenuRead();
	}
}

int COnelineDlg::readOneWord(const long &pos,WCHAR *ot)
{
	int i=0;
	unsigned char buf[4];
	buf[0]=getOneByte(pos);
	bool skipEndl=false;
	bool isSpace;
	WCHAR w;
	switch(fileCode)
	{
	case 1://ansi
	{
		switch (buf[0])
		{
		case 0x0d:
		case 0x0a:
			if (theApp.smartSeg)
			{
				getBackWord(pos,&w);
				if (!isEndWord(w))
				{
					skipEndl=true;
					isSpace=false;
				}
			}
		case 0x09:
			for (i=1;;)
			{
				buf[0]=getOneByte(pos+i);
				switch(buf[0])
				{
				case 0x0d:
				case 0x0a:
					isSpace=false;
					i++;
					break;
				case 0x20:
				case 0x09:
					i++;
					isSpace=true;
					break;
				case 0xa1:
					buf[1]=getOneByte(pos+i+1);
					if (buf[1]==0xa1)
					{
						i+=2;
						isSpace=true;
					}
					else
					{
						goto posOver;
					}
					break;
				default:
					goto posOver;
				}
			}
posOver:
			if (skipEndl&&!isSpace)
			{
				break;
			}
			*ot=0;
			return i;
		}
		if (buf[0]<0x80)
		{
			MultiByteToWideChar(CP_ACP,0,(char*)buf,1,ot,1);
			i+=1;
		}
		else
		{
			buf[1]=getOneByte(pos+i+1);
			MultiByteToWideChar(CP_ACP,0,(char*)buf,2,ot,1);
			i+=2;
		}
		return i;
	}
	case 2://utf8
	{
		switch (buf[0])
		{
		case 0x0d:
		case 0x0a:
			if (theApp.smartSeg)
			{
				getBackWord(pos,&w);
				if (!isEndWord(w))
				{
					skipEndl=true;
					isSpace=false;
				}
			}
		case 0x09:
			for (i=1;;)
			{
				buf[0]=getOneByte(pos+i);
				switch(buf[0])
				{
				case 0x0d:
				case 0x0a:
					isSpace=false;
					i++;
					break;
				case 0x20:
				case 0x09:
					isSpace=true;
					i++;
					break;
				case 0xe3:
					buf[1]=getOneByte(pos+i+1);
					if (buf[1]==0x80)
					{
						buf[2]=getOneByte(pos+i+2);
						if (buf[2]==0x80)
						{
							i+=3;
							isSpace=true;
							continue;
						}
					}
					goto posOverUtf8;
				default:
					goto posOverUtf8;
				}
			}
posOverUtf8:
			if (skipEndl&&!isSpace)
			{
				break;
			}
			*ot=0;
			return i;
		}
		const char _len[]={1,1,2,3};
		if (buf[0]<0xC0||buf[0]>=0xf5)
		{
			MultiByteToWideChar(CP_UTF8,0,(char*)buf,1,ot,1);
			return 1;
		}
		char len=_len[(buf[0]&0x30)>>4];
		for (int j=1;j<=len;j++)
		{
			buf[j]=getOneByte(pos+i+j);
		}
		MultiByteToWideChar(CP_UTF8,0,(char*)buf,len+1,ot,1);
		return i+j;
	}
	case 3://unicode
	{
		buf[1]=getOneByte(pos+1);
		switch (*((WCHAR*)buf))
		{
		case 0x000d:
		case 0x000a:
			if (theApp.smartSeg)
			{
				getBackWord(pos,&w);
				if (!isEndWord(w))
				{
					skipEndl=true;
					isSpace=false;
				}
			}
		case 0x0009:
			for (i=2;;)
			{
				buf[0]=getOneByte(pos+i);
				buf[1]=getOneByte(pos+i+1);
				switch(*((WCHAR*)buf))
				{
				case 0x000d:
				case 0x000a:
					isSpace=false;
					i+=2;
					break;
				case 0x0020:
				case 0x0009:
				case 0x3000:
					isSpace=true;
					i+=2;
					break;
				default:
					goto posOverUnicode;
				}
			}
posOverUnicode:
			if (skipEndl&&!isSpace)
			{
				break;
			}
			*ot=0;
			return i;
		}
		memcpy(ot,buf,2);
		i+=2;
		return i;
	}
	}
	return 0;
}

char COnelineDlg::getOneByte(const long &pos)
{
	if (!fileOpened||pos>=txtFile.GetLength()||pos<bomSize)
	{
		return 0;
	}
	long n=pos>>12;
	int bpos=pos&0xfff;
	if (n==cacheArea)
	{
		return bufCache[bpos];
	}
	cacheArea=n;
	bufCache=blockAvailable(n);
	return bufCache[bpos];
}

void COnelineDlg::getOutput()
{
	//static long lfp=-2;
	long pos=filePos;
	int off=0;
	int width,cw;
	cw=theApp.sleft-wordOffset;
	//lfp=filePos;
	CRect r;
	remainSpace=0;
	GetWindowRect(r);
	ScreenToClient(r);
	CSize size;
	width=r.Width()-theApp.sright;
	bool firstFlag=true;
	WCHAR word;
	outputWord.Empty();
	while(pos<txtFile.GetLength())
	{
		off=readOneWord(pos,&word);
		if (word!=0)
		{
			outputWord+=word;
			size=MemDC.GetTextExtent(&word,1);
			MemDC.TextOut(cw,theApp.stop,&word,1);
			cw+=size.cx+theApp.wordspace;
			if (firstFlag)
			{
				wordWidth=size.cx+theApp.wordspace;
			}
		}
		else
		{
			cw+=theApp.fontwidth;
			if (cw>width)
			{
				remainSpace=cw-width;
			}
			if (firstFlag)
			{
				wordWidth=theApp.fontwidth;
			}
		}
		pos+=off;
		if (firstFlag)
		{
			firstFlag=false;
			if (pos<txtFile.GetLength())
			{
				nextWordPos=pos;
			}
			else
			{
				nextWordPos=-1;
				break;
			}
		}
		if (cw>width)
		{
			break;
		}
	}
	if (pos>=txtFile.GetLength()&&cw<=width)
	{
		nextLinePos=-1;
	}
	else
	{
		if (remainSpace!=0)
		{
			nextLinePos=pos;
		}
		else
			nextLinePos=pos-off;
	}
	MemDC.FillSolidRect(r.right-theApp.sright,0,theApp.sright,r.Height(),theApp.bgcolor);
	MemDC.FillSolidRect(0,0,theApp.sleft,r.Height(),theApp.bgcolor);
}

void COnelineDlg::OnSize(UINT nType, int cx, int cy) 
{
	needReDraw=true;
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	/*if (cx<50)
	{
		SetWindowPos(NULL,0,0,50,cy,SWP_NOMOVE|SWP_NOZORDER);
	}*/
	Invalidate();
}

void COnelineDlg::moveLineNext()
{
	if (nextLinePos==-1)
	{
		MessageBox(_T("已到文件结尾。"));
		return;
	}
	filePos=nextLinePos;
	wordOffset=-remainSpace;
	needReDraw=true;
	Invalidate();
}

void COnelineDlg::moveWordNext()
{
	if (nextWordPos==-1)
	{
		return;
	}
	filePos=nextWordPos;
	wordOffset=0;
	needReDraw=true;
	Invalidate();
}

void COnelineDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (isHide)
	{
		showFromEdge();
	}
	if (lButtonDown)
	{
		lButtonDown=false;
		clickPoint-=point;
		if (flagNotMove)
		{
			flagNotMove=false;
		}
		else if (fileOpened&&nHitTest==HTCAPTION&&abs(clickPoint.x)<5&&abs(clickPoint.y)<5)
		{
			moveLineNext();
		}
		hideToShow();
	}
	if (!timerSet&&theApp.hidePos>0)
	{
		SetTimer(1,50,MyOnTimer);
		timerSet=true;
	}

	if (GetForegroundWindow()!=this)
	{
		INPUT ip[2]={0};
		ip[0].type=INPUT_MOUSE;
		ip[0].mi.dwFlags=MOUSEEVENTF_LEFTDOWN;
		ip[1].type=INPUT_MOUSE;
		ip[1].mi.dwFlags=MOUSEEVENTF_LEFTUP;
		ignoreClick=true;
		SendInput(2,ip,sizeof(INPUT));
	}
	CDialog::OnNcMouseMove(nHitTest, point);
}

void COnelineDlg::OnMenuSettings() 
{
	// TODO: Add your command handler code here
	if (!dSettings)
	{
		dSettings=new CDialogSettings(this);
	}
	dSettings->DoModal();
}

void COnelineDlg::OnWakeUp()
{
	showFromEdge();
}

void COnelineDlg::settingsChanged()
{
	MemDC.SelectObject(pOldFont);
	theApp.font.DeleteObject();
	theApp.calfontheight();
	pOldFont=MemDC.SelectObject(&(theApp.font));
	MemDC.SetBkColor(theApp.bgcolor);
	MemDC.SetTextColor(theApp.fcolor);
	CRect r;
	GetWindowRect(r);
	SetWindowPos(NULL,0,0,r.Width(),theApp.stop+theApp.sbottom+theApp.fontheight,SWP_NOZORDER|SWP_NOMOVE);
	csblock.Lock();
	cacheArea=-1;
	clearBlock(filePos/4096);
	csblock.Unlock();
	needReDraw=true;
	Invalidate();
}

BOOL COnelineDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	if(fileOpened)
	{
		CRect r;
		GetWindowRect(r);
		if (r.PtInRect(pt))
		{
			if (zDelta>0)
				moveLineBack();
			else
				moveLineNext();
		}
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void COnelineDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	if (fileOpened)
	{
		switch(nChar)
		{
		case VK_UP:
			adjustRollRate(-1);
			break;
		case VK_DOWN:
			adjustRollRate(1);
			break;
		case VK_LEFT:
			moveWordBack();
			break;
		case VK_RIGHT:
			moveWordNext();
			break;
		case VK_PRIOR:
			moveLineBack();
			break;
		case VK_NEXT:
			moveLineNext();
			break;
		case VK_INSERT:
			switchFile(false);
			break;
		case VK_DELETE:
			switchFile(true);
			break;
		case VK_RETURN:
			OnMenuAutoroll();
			break;
		case 'F':
			if (GetKeyState(VK_CONTROL)&0x8000 && !(GetKeyState(VK_MENU)&0x8000) && !(GetKeyState(VK_SHIFT)&0x8000))
			{
				OnMenuFind();
			}
			break;
		case 'J':
			if (GetKeyState(VK_CONTROL)&0x8000 && !(GetKeyState(VK_MENU)&0x8000) && !(GetKeyState(VK_SHIFT)&0x8000))
			{
				OnMenuJump();
			}
			break;
		case 'I':
			if (GetKeyState(VK_CONTROL)&0x8000 && !(GetKeyState(VK_MENU)&0x8000) && !(GetKeyState(VK_SHIFT)&0x8000))
			{
				OnMenuIndex();
			}
			break;
		case 'C':
			if (GetKeyState(VK_CONTROL)&0x8000 && !(GetKeyState(VK_MENU)&0x8000) && !(GetKeyState(VK_SHIFT)&0x8000))
			{
				int l=outputWord.GetLength()*2+2;
				HGLOBAL hgm=GlobalAlloc(GHND,l);
				if(!hgm)
				{
					return;
				}
				void *lpgm=GlobalLock(hgm);
				if(lpgm) memcpy(lpgm,(LPCTSTR)outputWord,l);
				GlobalUnlock(hgm);
				OpenClipboard();
				EmptyClipboard();
				SetClipboardData(CF_UNICODETEXT,hgm);
				CloseClipboard();
			}
			break;
		}
	}
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

void COnelineDlg::moveWordBack()
{
	if (filePos<=bomSize)
	{
		return;
	}
	long pos=filePos-1;
	unsigned char buf[3];
	buf[2]=getOneByte(pos);
	buf[1]=getOneByte(pos-1);
	int i;
	switch (fileCode)
	{
	case 1:
		while(i=isAnsiEmpty((char*)(buf+1)))
		{
			pos-=i;
			if (pos<=bomSize)
			{
				filePos=bomSize;
				goto posMBover;
			}
			if (i==1)
			{
				buf[2]=buf[1];
			}
			else
			{
				buf[2]=getOneByte(pos);
			}
			buf[1]=getOneByte(pos-1);
		}
		if (buf[2]<0x80)
		{
			filePos=pos;
		}
		else
		{
			filePos=pos-1;
		}
		break;
	case 2:
		buf[0]=getOneByte(pos-2);
		while(i=isUtf8Empty((char*)buf))
		{
			pos-=i;
			if (pos<=bomSize)
			{
				filePos=bomSize;
				goto posMBover;
			}
			if (i==1)
			{
				buf[2]=buf[1];
				buf[1]=buf[0];
			}
			else
			{
				buf[2]=getOneByte(pos);
				buf[1]=getOneByte(pos-1);
			}
			buf[0]=getOneByte(pos-2);
		}
		if (buf[2]<0x80)
		{
			filePos=pos;
			break;
		}
		for (i=2;i>=0;i--)
		{
			if ((buf[i]&0xc0)==0xc0)
			{
				filePos=pos-2+i;
				goto posMBover;
			}
		}
		filePos=pos-3;
		break;
	case 3:
		while(isUnicodeEmpty((char*)buf))
		{
			pos-=2;
			if (pos<=bomSize)
			{
				filePos=bomSize;
				goto posMBover;
			}
			buf[2]=getOneByte(pos);
			buf[1]=getOneByte(pos-1);
		}
		filePos=pos-1;
		break;
	}
posMBover:
	if (filePos<bomSize)
	{
		filePos=bomSize;
	}
	wordOffset=0;
	needReDraw=true;
	Invalidate();
}

bool COnelineDlg::isUnicodeEmpty(char* buf)
{
	WCHAR *w=(WCHAR*)(buf+1);
	switch(*w)
	{
	case 0x000d:
	case 0x000a:
	case 0x0020:
	case 0x0009:
	case 0x3000:
		return true;
	}
	return false;
}

int COnelineDlg::isUtf8Empty(char *buf)
{
	switch(buf[2])
	{
	case 0x09:
	case 0x0a:
	case 0x0d:
	case 0x20:
		return 1;
	}
	if (!memcmp(buf,"\xe3\x80\x80",3))
	{
		return 3;
	}
	return 0;
}

int COnelineDlg::isAnsiEmpty(char *c)
{
	switch(c[1])
	{
	case 0x09:
	case 0x0a:
	case 0x0d:
	case 0x20:
		return 1;
	}
	if(!memcmp(c,"\xa1\xa1",2))
	{
		return 2;
	}
	return 0;
}

int COnelineDlg::getBackWord(const long& cpos,WCHAR *ot)
{
	unsigned char t;
	char buf[4];
	switch (fileCode)
	{
	case 1:
		t=getOneByte(cpos-1);
		if (t<0x80)
		{
			MultiByteToWideChar(CP_ACP,0,(char*)&t,1,ot,1);
			return 1;
		}
		else
		{
			buf[1]=t;
			buf[0]=getOneByte(cpos-2);
			MultiByteToWideChar(CP_ACP,0,(char*)buf,2,ot,1);
			return 2;
		}
	case 2:
		{
			int i;
			for (i=1;cpos-i>bomSize&&i<5;i++)
			{
				t=buf[4-i]=getOneByte(cpos-i);
				if (t<0x80)
				{
					MultiByteToWideChar(CP_UTF8,0,(char*)&t,1,ot,1);
					return i;
				}
				if ((t&0xc0)==0xc0)
				{
					MultiByteToWideChar(CP_UTF8,0,(char*)buf+4-i,i,ot,1);
					return i;
				}
			}
			return 4;
		}
	case 3:
		{
			((char*)ot)[1]=getOneByte(cpos-1);
			((char*)ot)[0]=getOneByte(cpos-2);
			return 2;
		}
	}
	return 0;
}

void COnelineDlg::moveLineBack()
{
	if(filePos<=bomSize)
	{
		return;
	}
	if (filePos==bomSize+1)
	{
		filePos=bomSize;
		wordOffset=0;
		needReDraw=true;
		Invalidate();
		return;
	}
	int cw,cp,width;
	cw=theApp.sleft;
	CRect r;
	GetWindowRect(r);
	width=r.Width()-theApp.sright;
	CSize sz;
	bool flagBlanking=false,flagIsBlank;
	long startBlank,lastStartFlag;
	long pos=filePos;
	long directCountPos;
	lastStartFlag=-1;
	WCHAR word;
	bool isEndWithReturn=false;
	long lastReturnPos=-1;
	for(;;)
	{
		cp=getBackWord(pos,&word);
		pos-=cp;
		if (pos<=bomSize)
		{
			filePos=bomSize;
			goto posMLBend;
		}
		switch(word)
		{
		case 0x000d:
		case 0x000a:
			if (theApp.smartSeg)
			{
				if (!flagBlanking)
				{
					isEndWithReturn=true;
				}
				lastReturnPos=pos;
			}
		case 0x0009:
			lastStartFlag=pos;
		case _T(' '):
		case _T('　'):
			flagIsBlank=true;
			if (!flagBlanking)
			{
				flagBlanking=true;
				startBlank=pos+cp;
			}
			break;
		default:
			flagIsBlank=false;
		}
		if (flagBlanking)
		{
			if (!flagIsBlank)//结束空格模式
			{
				flagBlanking=false;
				if (lastStartFlag!=-1)//如果开始时不是090A0D，则还要单独计算
				{
					if (isEndWithReturn&&lastReturnPos==pos+cp&&!isEndWord(word))//是要跳行了
					{
						isEndWithReturn=false;
						lastStartFlag=-1;
						goto posMLBcount;
					}
					cw+=theApp.fontwidth;
					if (cw>=width)
					{
						filePos=startBlank;
						wordOffset=cw-width-theApp.fontwidth;
						goto posMLBend2;
					}
					if (lastStartFlag==pos+cp)
					{
						isEndWithReturn=false;
						lastStartFlag=-1;
						goto posMLBcount;
					}
					else
					{
						directCountPos=lastStartFlag;
					}
					lastStartFlag=-1;
				}
				else
				{
					directCountPos=startBlank;
				}
				isEndWithReturn=false;
				while(directCountPos>=pos)
				{
					cp=getBackWord(directCountPos,&word);
					sz=MemDC.GetTextExtent(&word,1);
					cw+=sz.cx;
					if (cw>=width)
					{
						filePos=directCountPos;
						goto posMLBend;
					}
					directCountPos-=cp;
				}
			}
		}
		else
		{
posMLBcount:
			sz=MemDC.GetTextExtent(&word,1);
			cw+=sz.cx+theApp.wordspace;
			if(cw>=width)
			{
				filePos=pos+cp;
				goto posMLBend;
			}
		}
	}
posMLBend:
	wordOffset=0;
posMLBend2:
	needReDraw=true;
	Invalidate();
	return;
}

void COnelineDlg::OnMenuRead() 
{
	// TODO: Add your command handler code here
	if (isReading)
	{
		isReading=false;
		theApp.cVoice->Speak(NULL,SPF_PURGEBEFORESPEAK,NULL);
		popupMenu->CheckMenuItem(ID_MENU_READ,MF_UNCHECKED);
		trayMenu->CheckMenuItem(ID_MENU2_READ,MF_UNCHECKED);
	}
	else
	{
		if (rolling)
		{
			OnMenuAutoroll();
		}
		isReading=true;
		popupMenu->CheckMenuItem(ID_MENU_READ,MF_CHECKED);
		trayMenu->CheckMenuItem(ID_MENU2_READ,MF_CHECKED);
		nextSentencePos=filePos;
		wordOffset=0;
		SPEVENT ev;
		while(theApp.cVoice->GetEvents(1,&ev,NULL)==S_OK);
		prepareNextSentence();
	}
}

void COnelineDlg::OnMenuRecentFile(UINT pos)
{
	int p=pos-ID_MENU_RECENTFILE;
	std::list<CString>::iterator it;
	it=recentFiles.begin();
	for (int i=0;i<p;i++)
	{
		it++;
	}
	CString fn=*it;
	if (i!=0)
	{
		recentFiles.erase(it);
		recentFiles.push_front(fn);
		drawMenu();
	}
	openNovel(fn);
}

void COnelineDlg::drawMenu()
{
	int s=recentFiles.size();
	if (s==0)
	{
		return;
	}
	std::list<CString>::iterator it;
	it=recentFiles.begin();
	CMenu *rm=popupMenu->GetSubMenu(1);
	int i;
	int l=rm->GetMenuItemCount();
	CString cs;
	TCHAR buf[2];
	for (i=0;i<l;i++)
	{
		_itot(i,buf,10);
		cs=_T('&');
		cs+=buf;
		cs+=_T(' ');
		cs+=*it;
		rm->ModifyMenu(i,MF_BYPOSITION,ID_MENU_RECENTFILE+i,cs);
		it++;
	}
	for (;i<s;i++)
	{
		_itot(i,buf,10);
		cs=_T('&');
		cs+=buf;
		cs+=_T(' ');
		cs+=*it;
		rm->AppendMenu(MF_STRING,ID_MENU_RECENTFILE+i,cs);
		it++;
	}
}

void COnelineDlg::addToRecentFiles(const CString& s)
{
	std::list<CString>::iterator it;
	it=recentFiles.begin();
	while(it!=recentFiles.end())
	{
		if (*it==s)
		{
			recentFiles.erase(it);
			break;
		}
		it++;
	}
	recentFiles.push_front(s);
	if (recentFiles.size()>10)
	{
		recentFiles.pop_back();
	}
	drawMenu();
}

void COnelineDlg::hideToShow()
{
	CRect r;
	GetWindowRect(&r);
	int t,newx,newy,newh;
	newh=0;
	newx=r.left;
	newy=r.top;
	if (r.left<=0)
	{
		newh=1;
		newx=0;
	}
	else
	{
		t=screenX-r.Width();
		if (r.left>=t)
		{
			newh=3;
			newx=t;
		}
	}
	if (r.top<=0)
	{
		newh=2;
		newy=0;
	}
	else
	{
		t=screenY-r.Height();
		if (r.top>=t)
		{
			newh=4;
			newy=t;
		}
	}
	theApp.hidePos=newh;
	if (newh==0)
	{
		KillTimer(1);
		timerSet=false;
	}
	if (r.left!=newx||r.top!=newy)
	{
		SetWindowPos(NULL,newx,newy,0,0,SWP_NOSIZE|SWP_NOZORDER);
	}
}

LRESULT COnelineDlg::OnHotKey(WPARAM wParam,LPARAM lParam)
{
	if (fileOpened&&theApp.hasTTS)
	{
		OnMenuRead();
	}
	return 0;
}

UINT COnelineDlg::OnGetDlgCode() 
{
	// TODO: Add your message handler code here and/or call default
	return DLGC_WANTALLKEYS|DLGC_WANTARROWS;
	//return CDialog::OnGetDlgCode();
}

BOOL COnelineDlg::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	
	//return CDialog::OnEraseBkgnd(pDC);
	return TRUE;
}

LRESULT COnelineDlg::OnEnterSizeMove(WPARAM,LPARAM)
{
	isSizeMove=true;
	return 0;
}

LRESULT COnelineDlg::OnExitSizeMove(WPARAM,LPARAM)
{
	isSizeMove=false;
	return 0;
}

void COnelineDlg::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	isPopupMenu=true;
	CRect r;
	GetWindowRect(r);
	checkMenuInsideClick=true;
	checkMenuOutsideClick=false;
	popupMenu->TrackPopupMenu(TPM_LEFTALIGN,r.left,r.top,this);
	//lastMenuTime=clock();
	isPopupMenu=false;
}

void COnelineDlg::OnMenuMoveforward() 
{
	// TODO: Add your command handler code here
	
	if (filePos>bomSize)
	{
		filePos--;
		needReDraw=true;
		Invalidate();
	}
}

void COnelineDlg::OnMenuMovebackward() 
{
	// TODO: Add your command handler code here
	if (filePos<txtFile.GetLength()-1)
	{
		filePos++;
		needReDraw=true;
		Invalidate();
	}
}

void COnelineDlg::OnMenuAutoroll() 
{
	// TODO: Add your command handler code here
	if (rolling)
	{
		rolling=false;
		KillTimer(2);
		popupMenu->CheckMenuItem(ID_MENU_AUTOROLL,MF_UNCHECKED);
	}
	else if (!isReading)
	{
		rolling=true;
		SetTimer(2,theApp.timeToRoll,OnTimerRoll);
		popupMenu->CheckMenuItem(ID_MENU_AUTOROLL,MF_CHECKED);
	}
}

void COnelineDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	moveLineNext();
	CDialog::OnNcLButtonDblClk(nHitTest, point);
}

void CALLBACK EXPORT OnTimerRoll(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime)
{
	if (!dlg->isHide&&dlg->nextWordPos!=-1)
		dlg->moveWordPixels(theApp.rollPixels);
}

void COnelineDlg::moveWordPixels(int pixels)
{
	wordOffset+=pixels;
	if (wordOffset>=wordWidth)
	{
		wordOffset-=wordWidth;
		moveWordNext();
	}
	else
	{
		needReDraw=true;
		Invalidate();
	}
}

void COnelineDlg::adjustRollRate(int x)
{
	int cf=0;
	if (x<0)
	{
		if (theApp.timeToRoll<=10)
		{
			if (theApp.rollPixels<20)
			{
				theApp.rollPixels++;
				cf=2;
			}
		}
		else
		{
			theApp.timeToRoll--;
			cf=1;
		}
	}
	else
	{
		if (theApp.rollPixels>1)
		{
			theApp.rollPixels--;
			cf=2;
		}
		else if (theApp.timeToRoll<50)
		{
			theApp.timeToRoll++;
			cf=1;
		}
	}
	if (cf==1)
	{
		theApp.iniFile->SetValueI(_T("settings"),_T("timetoroll"),theApp.timeToRoll);
		if (rolling)
		{
			KillTimer(2);
			SetTimer(2,theApp.timeToRoll,OnTimerRoll);
		}
	}
	else if(cf==2)
		theApp.iniFile->SetValueI(_T("settings"),_T("rollpixels"),theApp.rollPixels);
}

void COnelineDlg::switchFile(bool isNext)
{
	CString path=txtFile.GetFilePath().Right(4);
	path.MakeLower();
	if (path!=_T(".txt"))
	{
		return;
	}
	int pos=txtFile.GetFilePath().ReverseFind(_T('\\'));
	path=txtFile.GetFilePath().Left(pos+1);
	path+=_T("*.txt");
	CFileFind cff;
	if (!cff.FindFile(path))
	{
		return;
	}
	int i;
	if (isNext)
	{
		bool isFound=false;
		do
		{
			i=cff.FindNextFile();
			if (cff.IsDirectory())
			{
				continue;
			}
			if (!isFound)
			{
				if (i==0)
				{
					return;
				}
				if (cff.GetFilePath()==txtFile.GetFilePath())
				{
					isFound=true;
				}
			}
			else
			{
				addToRecentFiles(cff.GetFilePath());
				openNovel(cff.GetFilePath());
				return;
			}
		}while(i);
	}
	else
	{
		CString lastFileName;
		do
		{
			i=cff.FindNextFile();
			if (cff.IsDirectory())
			{
				continue;
			}
			if (cff.GetFilePath()==txtFile.GetFilePath())
			{
				if (!lastFileName.IsEmpty())
				{
					addToRecentFiles(lastFileName);
					openNovel(lastFileName);
				}
				return;
			}
			lastFileName=cff.GetFilePath();
		}while(i);
	}
}

void COnelineDlg::OnMenuJump() 
{
	// TODO: Add your command handler code here
	if (!dJumpto)
	{
		dJumpto=new CDialogJumpto(this);
	}
	isPopupMenu=true;
	dJumpto->DoModal();
	isPopupMenu=false;
}

void COnelineDlg::randomMove(long pos)
{
	int i;
	unsigned char t;
	if (pos<=bomSize)
	{
		filePos=bomSize;
		goto posRMend;
	}
	if (pos>=txtFile.GetLength())
	{
		WCHAR word;
		i=getBackWord(txtFile.GetLength(),&word);
		filePos=txtFile.GetLength()-i;
		goto posRMend;
	}
	switch (fileCode)
	{
	case 1:
	{
		t=getOneByte(pos);
		if (t<0x80)
		{
			filePos=pos;
			break;
		}
		long cpos=pos-1;
		for (i=1;i<=4096;i++)
		{
			t=getOneByte(cpos);
			if (t<0x80)
				goto posMBansi;
			cpos--;
			if (cpos<=bomSize)
			{
				goto posMBansi;
			}
		}
		cpos=pos+1;
		while (cpos<txtFile.GetLength())
		{
			t=getOneByte(cpos);
			if (t<0x80)
				break;
			cpos++;
		}
		i=pos-cpos;
posMBansi:
		if (((i&1)==1)^(i>0))
		{
			filePos=pos-1;
		}
		else
		{
			filePos=pos;
		}
		break;
	}
	case 2:
		do 
		{
			t=getOneByte(pos);
			if (t<0x80||(t&0xc0)==0xc0)
			{
				filePos=pos;
				goto posRMend;
			}
			pos--;
		} while (pos>bomSize);
		filePos=bomSize;
		break;
	case 3:
		if ((pos&1)==1)
		{
			filePos=pos-1;
		}
		else
			filePos=pos;
		break;
	}
posRMend:
	wordOffset=0;
	needReDraw=true;
	Invalidate();
}

LRESULT COnelineDlg::OnTTSEvent(WPARAM,LPARAM)
{
	if (!isReading)
	{
		return 0;
	}
	CSpEvent ev;
	while(ev.GetFrom(theApp.cVoice)==S_OK)
	{
		switch(ev.eEventId)
		{
		case SPEI_END_INPUT_STREAM:
			prepareNextSentence();
			break;
		case SPEI_WORD_BOUNDARY:
			speechMoveNextWord((int)ev.lParam);
			break;
		}
	}
	return 0;
}

void COnelineDlg::speechMoveNextWord(int npos)
{
	if (npos<1)
	{
		return;
	}
	for (;speechWordCount<npos;speechWordCount++)
	{
		speechPos+=speechBufLength[speechWordCount];
	}
	filePos=speechPos;
	needReDraw=true;
	Invalidate();
}

void COnelineDlg::prepareNextSentence()
{
	long pos=nextSentencePos;
	WCHAR word;
	int l;
	while(pos<txtFile.GetLength())
	{
		l=readOneWord(pos,&word);
		pos+=l;
		if (!isStopWord(word))
		{
			filePos=speechPos=pos-l;
			needReDraw=true;
			Invalidate();
			speechBuf=word;
			speechBufLength[0]=l;
			goto posNS;
		}
	}
	MessageBox(_T("阅读完毕！"));
	OnMenuRead();
	return;
posNS:
	int ct;
	for(ct=1;ct<100;ct++)
	{
		l=readOneWord(pos,&word);
		pos+=l;
		speechBuf+=word;
		speechBufLength[ct]=l;
		if (isStopWord(word))
		{
			break;
		}
	}
	nextSentencePos=pos;
	speechWordCount=0;
	theApp.cVoice->Speak((LPCTSTR)speechBuf,SPF_ASYNC|SPF_IS_NOT_XML,NULL);
}

bool COnelineDlg::isStopWord(const WCHAR &w)
{
	switch(w)
	{
	case 0x0000:
	case _T('　'):
	case _T(','):
	case _T('.'):
	case _T('，'):
	case _T('。'):
	case _T('?'):
	case _T('？'):
	case _T('！'):
	case _T('!'):
	case _T('：'):
	case _T(':'):
	case _T('…'):
	case _T(';'):
	case _T('；'):
		return true;
	}
	return false;
}

bool COnelineDlg::isEndWord(const WCHAR &w)
{
	switch(w)
	{
	case _T('。'):
	case _T('.'):
	case _T('?'):
	case _T('？'):
	case _T('!'):
	case _T('！'):
	case _T('…'):
	case _T('\"'):
	case _T('”'):
	case _T('）'):
	case _T(')'):
	case _T('—'):
		return true;
	}
	return false;
}

void COnelineDlg::OnMenuFind() 
{
	// TODO: Add your command handler code here
	if (isReading)
	{
		MessageBox(_T("请先停止朗读！"));
		return;
	}
	if (!dSearch)
	{
		dSearch=new CDialogSearch(this);
	}
	isPopupMenu=true;
	dSearch->DoModal();
	isPopupMenu=false;
}

void COnelineDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
{
	// TODO: Add your message handler code here and/or call default
	lpMMI->ptMinTrackSize.x=30;
	CDialog::OnGetMinMaxInfo(lpMMI);
}

void COnelineDlg::OnMenuIndex() 
{
	// TODO: Add your command handler code here
	if (!dIndex)
	{
		dIndex=new CDialogIndex(this);
		dIndex->Create(IDD_DIALOG_INDEX,this);
	}
	dIndex->ShowWindow(SW_SHOW);
}

BOOL COnelineDlg::OnNcActivate(BOOL bActive) 
{
	// TODO: Add your message handler code here and/or call default
	if (!bActive&&checkMenuInsideClick)
	{
		checkMenuOutsideClick=true;
	}
	return CDialog::OnNcActivate(bActive);
}
