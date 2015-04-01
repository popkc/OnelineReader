// DialogAbout.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogAbout.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogAbout dialog


CDialogAbout::CDialogAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogAbout)
	m_about =_T("这是一款用来在办公室偷偷看小说的软件，软件目前只支持TXT格式。\r\n\r\n使用时建议把软件拖到屏幕边缘，通过单击鼠标或使用鼠标滚轮进行翻页。\r\n\r\n鼠标移开后软件就会自动隐藏到屏幕边缘，什么老板键太落伍了啊！\r\n\r\n自动滚动功能会在软件隐藏时自动暂停，等你把鼠标移回来时才会继续！\r\n\r\n按CTRL+F12开启语音朗读功能，戴上耳机，神不知鬼不觉地读小说！\r\n\r\n键盘上下方向键：调整滚动速度。\r\n\r\n左右方向键：手动的逐字滚动。\r\n\r\nPage Up和Page Down键：翻页。\r\n\r\nInsert键和DEL键：同目录里有多个TXT文件时，可以用这两个键在不同文件间切换。");
	//}}AFX_DATA_INIT
}


void CDialogAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogAbout)
	DDX_Text(pDX, IDC_EDIT_ABOUT, m_about);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogAbout, CDialog)
	//{{AFX_MSG_MAP(CDialogAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogAbout message handlers
