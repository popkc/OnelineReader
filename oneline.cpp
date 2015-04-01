// oneline.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "oneline.h"
#include "onelineDlg.h"
COnelineDlg *dlg;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include <locale.h>

/////////////////////////////////////////////////////////////////////////////
// COnelineApp

BEGIN_MESSAGE_MAP(COnelineApp, CWinApp)
	//{{AFX_MSG_MAP(COnelineApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COnelineApp construction

COnelineApp::COnelineApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only COnelineApp object

COnelineApp theApp;

/////////////////////////////////////////////////////////////////////////////
// COnelineApp initialization

BOOL COnelineApp::InitInstance()
{
	CWnd *pWin=CWnd::FindWindow(_T("#32770"),_T("一行小说阅读器"));
	if (pWin)
	{
		pWin->PostMessage(WM_WAKEUP);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	initUservar();
	CFrameWnd	*parentWnd = new CFrameWnd();
	parentWnd->Create( 0, 0, WS_OVERLAPPEDWINDOW );

	dlg=new COnelineDlg(parentWnd);
	m_pMainWnd = dlg;
	dlg->DoModal();
	//_CrtDumpMemoryLeaks();
	return FALSE;
}

void COnelineApp::initUservar()
{
	setlocale(LC_CTYPE,"chs");
	CString fn;
	GetCurrentDirectory(1024,fn.GetBuffer(1024));
	fn.ReleaseBuffer();
	fn+=_T("\\oneline.ini");
	iniFile=new CIniFile(fn);
	iniFile->ReadFile();
	fontsize=iniFile->GetValueI(_T("settings"),_T("fontsize"),32);
	fontname=iniFile->GetValue(_T("settings"),_T("fontname"),_T("黑体"));
	fontbold=iniFile->GetValueI(_T("settings"),_T("fontbold"),FW_BOLD);
	fontitalic=iniFile->GetValueI(_T("settings"),_T("fontitalic"),FALSE);
	calfontheight();
	fcolor=iniFile->GetValueI(_T("settings"),_T("fcolor"),RGB(255,255,255));
	bgcolor=iniFile->GetValueI(_T("settings"),_T("bgcolor"),RGB(0,0,0));
	sleft=iniFile->GetValueI(_T("settings"),_T("sleft"),10);
	sright=iniFile->GetValueI(_T("settings"),_T("sright"),10);
	stop=iniFile->GetValueI(_T("settings"),_T("stop"),5);
	sbottom=iniFile->GetValueI(_T("settings"),_T("sbottom"),5);
	wordspace=iniFile->GetValueI(_T("settings"),_T("wordspace"),0);
	maxblock=iniFile->GetValueI(_T("settings"),_T("maxblock"),20000);
	winheight=fontheight+stop+sbottom;
	winwidth=iniFile->GetValueI(_T("window"),_T("width"),800);
	winx=iniFile->GetValueI(_T("window"),_T("x"),100);
	winy=iniFile->GetValueI(_T("window"),_T("y"),500);
	hidePos=iniFile->GetValueI(_T("window"),_T("hidepos"),0);
	showTrayIcon=iniFile->GetValueB(_T("settings"),_T("showtrayicon"),true);
	voiceRate=iniFile->GetValueI(_T("voice"),_T("rate"),0);
	voiceVolume=iniFile->GetValueI(_T("voice"),_T("volume"),100);
	voiceToken=iniFile->GetValue(_T("voice"),_T("token"),_T(""));
	timeToRoll=iniFile->GetValueI(_T("settings"),_T("timetoroll"),10);
	rollPixels=iniFile->GetValueI(_T("settings"),_T("rollpixels"),1);
	smartSeg=iniFile->GetValueB(_T("settings"),_T("smartseg"),false);
	isDown=iniFile->GetValueB(_T("find"),_T("isdown"),true);
	isNearBy=iniFile->GetValueB(_T("find"),_T("isnearby"),true);
	withSmartSeg=iniFile->GetValueB(_T("find"),_T("withsmartseg"),false);
	CoInitialize(NULL);
	HRESULT hr=cVoice.CoCreateInstance(CLSID_SpVoice);
	if (hr==S_OK)
	{
		hasTTS=true;
		if (voiceToken!=_T(""))
		{
			hr=SpGetTokenFromId(voiceToken,&cToken);
			if (hr==S_OK)
			{
				hr=cVoice->SetVoice(cToken);
			}
		}
		cVoice->SetVolume(voiceVolume);
		cVoice->SetRate(voiceRate);
	}
	else
		hasTTS=false;
	//this->AddToRecentFileList(_T("abc"));
	TCHAR ws[10];
	int i;
	regexCount=iniFile->GetValueI("find","regexcount",3);
	for (i=0;i<regexCount;i++)
	{
		_itot(i,ws,10);
		fn=_T("r");
		fn+=ws;
		fn=iniFile->GetValue("find",fn,defaultRegex(i));
		if (fn!=_T(""))
		{
			rxs.push_back(fn);
		}
	}
	regexMaxChar=iniFile->GetValueI("find","regexmaxchar",50);
}

CString COnelineApp::defaultRegex(int n)
{
	switch (n)
	{
	case 1:
		return _T("(^|[\\s　])第?\\s*((\\d+)|[两零一二三四五六七八九壹贰叁肆伍陆柒捌玖十百千万拾佰仟〇]+|序)([篇章节集卷部\\s　\\.\\-—]|$)");
	case 2:
		return _T("(^|[\\s　])[篇章节卷部集]\\s*((\\d)|[两零一二三四五六七八九壹贰叁肆伍陆柒捌玖十百千万拾佰仟〇])");
	case 0:
		return _T("(^|[\\s　])(正文|作品相关|前传|后传|外传|引子|锲子)([\\s　]|$)");
	default:
		return _T("");
	}
}

void COnelineApp::calfontheight()
{
	HDC tDC;
	TEXTMETRIC tme;
	HFONT oldfont;
	tDC=GetDC(NULL);
	//font.CreatePointFont(fontsize,fontname);
	font.CreateFont(fontsize,0,0,0,fontbold,fontitalic,FALSE,0,GB2312_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,fontname);
	oldfont=(HFONT)SelectObject(tDC,font);
	GetTextMetrics(tDC,&tme);
	fontheight=tme.tmHeight;
	SIZE s;
	GetTextExtentPoint(tDC,_T("我"),2,&s);
	fontwidth=s.cx*2;
	SelectObject(tDC,oldfont);
}

BOOL IsTextUTF8(char* str,int len)
{
	DWORD nBytes=0;//UFT8可用1-6个字节编码,ASCII用一个字节
	UCHAR chr;
	BOOL bAllAscii=TRUE; //如果全部都是ASCII, 说明不是UTF-8
	for (int i=0;i<len;i++)
	{
		chr = (UCHAR)str[i];
		if( (chr&0x80) != 0 ) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8,ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx
			bAllAscii= FALSE;
		if(nBytes==0) //如果不是ASCII码,应该是多字节符,计算字节数
		{
			if(chr>=0x80)
			{
				if (chr>=0xf5)
					return FALSE;
				else if(chr>=0xF0)
					nBytes=3;
				else if(chr>=0xE0)
					nBytes=2;
				else if(chr>=0xC0)
					nBytes=1;
				else
				{
					return FALSE;
				}
			}
		}
		else //多字节符的非首字节,应为 10xxxxxx
		{
			if( (chr&0xC0) != 0x80 )
			{
				return FALSE;
			}
			nBytes--;
		}
	}
	/*if( nBytes > 0 ) //违返规则
	{
		return FALSE;
	}*/
	if( bAllAscii ) //如果全部都是ASCII, 说明不是UTF-8
	{
		return FALSE;
	}
	return TRUE;

}
