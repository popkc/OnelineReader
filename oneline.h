// oneline.h : main header file for the ONELINE application
//

#if !defined(AFX_ONELINE_H__625C5560_F57B_427A_8342_453B37F19E47__INCLUDED_)
#define AFX_ONELINE_H__625C5560_F57B_427A_8342_453B37F19E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ini.h"
#include <spuihelp.h>
#include <atlbase.h>
#include <list>
#include <VECTOR>

/////////////////////////////////////////////////////////////////////////////
// COnelineApp:
// See oneline.cpp for the implementation of this class
//
#define WM_TRAYEVENT (WM_USER+1)
#define WM_WAKEUP (WM_USER+2)
#define WM_TTSEVENT (WM_USER+3)
#define WM_SEARCHTHREAD (WM_USER+4)

class COnelineApp : public CWinApp
{
public:
	COnelineApp();
	CFont font;
	int fontsize;
	CString fontname;
	COLORREF bgcolor,fcolor;
	int sleft,sright,stop,sbottom;
	int wordspace;
	int maxblock;
	void calfontheight();
	int fontheight,fontwidth;
	CIniFile *iniFile;
	int winwidth,winheight,winx,winy;
	BYTE hidePos;
	bool showTrayIcon;
	BYTE fontitalic;
	int fontbold;
	int voiceRate,voiceVolume;
	CString voiceToken;
	CComPtr<ISpVoice> cVoice;
	CComPtr<ISpObjectToken> cToken;
	bool hasTTS;
	int timeToRoll,rollPixels;
	bool smartSeg;
	bool isDown,isNearBy,withSmartSeg;
	std::vector<CString> rxs;
	int regexCount,regexMaxChar;
	CString defaultRegex(int n);
private:
	void initUservar();
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnelineApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(COnelineApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

BOOL IsTextUTF8(char* str,int len);
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONELINE_H__625C5560_F57B_427A_8342_453B37F19E47__INCLUDED_)
