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
	m_about =_T("����һ�������ڰ칫��͵͵��С˵����������Ŀǰֻ֧��TXT��ʽ��\r\n\r\nʹ��ʱ���������ϵ���Ļ��Ե��ͨ����������ʹ�������ֽ��з�ҳ��\r\n\r\n����ƿ�������ͻ��Զ����ص���Ļ��Ե��ʲô�ϰ��̫�����˰���\r\n\r\n�Զ��������ܻ����������ʱ�Զ���ͣ�����������ƻ���ʱ�Ż������\r\n\r\n��CTRL+F12���������ʶ����ܣ����϶�������֪�����ض�С˵��\r\n\r\n�������·���������������ٶȡ�\r\n\r\n���ҷ�������ֶ������ֹ�����\r\n\r\nPage Up��Page Down������ҳ��\r\n\r\nInsert����DEL����ͬĿ¼���ж��TXT�ļ�ʱ�����������������ڲ�ͬ�ļ����л���");
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
