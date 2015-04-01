// DialogIndex.cpp : implementation file
//

#include "stdafx.h"
#include "oneline.h"
#include "DialogIndex.h"
#include <boost/regex.hpp>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogIndex dialog
#include "onelineDlg.h"

extern COnelineApp theApp;
extern COnelineDlg *dlg;

std::vector<boost::wregex> regs;

CDialogIndex::CDialogIndex(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogIndex::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogIndex)
	//}}AFX_DATA_INIT
	id=0;
	changed=false;
	makeRegex();
	dIS=NULL;
}

void CDialogIndex::makeRegex()
{
	regs.clear();
	int i,l;
	l=theApp.rxs.size();
	for (i=0;i<l;i++)
	{
		regs.push_back(boost::wregex(theApp.rxs[i]));
	}
}

void CDialogIndex::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogIndex)
	DDX_Control(pDX, IDC_LIST_INDEX, m_listIndex);
	DDX_Control(pDX, IDC_PROGRESS_INDEX, m_progressIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogIndex, CDialog)
	//{{AFX_MSG_MAP(CDialogIndex)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_INDEX, OnDblclkListIndex)
	ON_BN_CLICKED(IDC_BUTTON_JUMP, OnButtonJump)
	ON_COMMAND(ID_MENU_INDEX_BUILD, OnMenuIndexBuild)
	ON_COMMAND(ID_MENU_INDEX_CLEAR, OnMenuIndexClear)
	ON_COMMAND(ID_MENU_INDEX_CONTINUE, OnMenuIndexContinue)
	ON_COMMAND(ID_MENU_INDEX_DELETE, OnMenuIndexDelete)
	ON_COMMAND(ID_MENU_INDEX_SETTING, OnMenuIndexSetting)
	ON_COMMAND(ID_MENU_INDEX_STOP, OnMenuIndexStop)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_INDEX, OnKeydownListIndex)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_SEARCHTHREAD,OnThreadMSG)
	ON_NOTIFY_EX(TTN_NEEDTEXT,0,OnToolTipsNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogIndex message handlers

BOOL CDialogIndex::OnInitDialog() 
{
	CDialog::OnInitDialog();
	changed=false;
	// TODO: Add extra initialization here
	m_listIndex.DeleteAllItems();
	m_listIndex.InsertColumn(0,_T("位置"));
	m_listIndex.InsertColumn(1,_T("名称"));
	m_listIndex.SetColumnWidth(0,100);
	m_listIndex.SetColumnWidth(1,500);
	m_listIndex.SetExtendedStyle(m_listIndex.GetExtendedStyle()|LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	int row,i;
	MapTotal::iterator it;
	it=theApp.iniFile->values.find(dlg->txtFile.GetFilePath());
	if (it!=theApp.iniFile->values.end())
	{
		MapKey::iterator itk;
		itk=it->second.begin();
		long pos;
		MapIndex indexes;
		while(itk!=it->second.end())
		{
			if (itk->first[0]==_T('z'))
			{
				do 
				{
					pos=_ttol((LPCTSTR)(itk->first.Mid(1)));
					indexes[pos]=itk->second;
					itk++;
				} while (itk!=it->second.end());
				break;
			}
			itk++;
		}
		MapIndex::iterator iti;
		iti=indexes.begin();
		TCHAR spos[11];
		i=0;
		while (iti!=indexes.end())
		{
			_itot(iti->first,spos,10);
			row=m_listIndex.InsertItem(i,spos);
			m_listIndex.SetItemData(row,iti->first);
			m_listIndex.SetItemText(row,1,iti->second);
			iti++;
			i++;
		}
	}
	m_toolbar.CreateEx(this,TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_ALIGN_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS);
	m_toolbar.LoadToolBar(IDR_TOOLBAR_INDEX);
	/*CBitmap bmp;
	bmp.LoadBitmap(IDR_TOOLBAR_INDEX);
	iml.Create(16,16,ILC_COLOR16,4,4);
	iml.Add(&bmp,(CBitmap*)NULL);
	m_toolbar.GetToolBarCtrl().SetImageList(&iml);*/
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
	m_toolbar.GetToolBarCtrl().EnableButton(ID_MENU_INDEX_STOP,FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogIndex::OnDblclkListIndex(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	LPNMITEMACTIVATE pnm=(LPNMITEMACTIVATE)pNMHDR;
	if (pnm->iItem!=-1)
	{
		long pos=m_listIndex.GetItemData(pnm->iItem);
		dlg->filePos=pos;
		dlg->needReDraw=true;
		dlg->Invalidate();
	}
	
	*pResult = 0;
}

void CDialogIndex::OnButtonJump() 
{
	// TODO: Add your control notification handler code here
	POSITION pos=m_listIndex.GetFirstSelectedItemPosition();
	if (!pos)
	{
		MessageBox(_T("请选择要跳转的位置。"));
		return;
	}
	long f=m_listIndex.GetItemData(m_listIndex.GetNextSelectedItem(pos));
	dlg->filePos=f;
	dlg->needReDraw=true;
	dlg->Invalidate();
}

void CDialogIndex::indexStart()
{
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_STOP,MF_ENABLED);
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_CONTINUE,MF_DISABLED);
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_BUILD,MF_DISABLED);
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_SETTING,MF_DISABLED);
	m_toolbar.GetToolBarCtrl().EnableButton(ID_MENU_INDEX_STOP,TRUE);
	m_toolbar.GetToolBarCtrl().EnableButton(ID_MENU_INDEX_BUILD,FALSE);
	m_toolbar.GetToolBarCtrl().EnableButton(ID_MENU_INDEX_CONTINUE,FALSE);
	m_progressIndex.ShowWindow(TRUE);
}

void CDialogIndex::indexEnd()
{
	id++;
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_STOP,MF_DISABLED);
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_CONTINUE,MF_ENABLED);
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_BUILD,MF_ENABLED);
	this->GetMenu()->EnableMenuItem(ID_MENU_INDEX_SETTING,MF_ENABLED);
	m_toolbar.GetToolBarCtrl().EnableButton(ID_MENU_INDEX_STOP,FALSE);
	m_toolbar.GetToolBarCtrl().EnableButton(ID_MENU_INDEX_BUILD,TRUE);
	m_toolbar.GetToolBarCtrl().EnableButton(ID_MENU_INDEX_CONTINUE,TRUE);
	m_progressIndex.ShowWindow(FALSE);
}

LRESULT CDialogIndex::OnThreadMSG(WPARAM wParam,LPARAM lParam)
{
	if ((wParam&0xffff0000)>>16!=id)
	{
		return 0;
	}
	int code=wParam&0x0000ffff;
	switch(code)
	{
	case 0:
		m_progressIndex.SetPos(lParam);
		break;
	case 1:
		{
			indexInfo *pi=(indexInfo*)lParam;
			TCHAR spos[11]; 
			_ltot(pi->pos,spos,10);
			int row=m_listIndex.InsertItem(m_listIndex.GetItemCount(),spos);
			m_listIndex.SetItemText(row,1,(LPCTSTR)(pi->name));
			m_listIndex.SetItemData(row,pi->pos);
			delete pi;
			changed=true;
		}
		break;
	case 2:
		indexEnd();
		MessageBox(_T("索引建立完毕！"));
		break;
	}
	return 0;
}

bool detectIndex(CString &line)
{
	for (int i=0;i<regs.size();i++)
	{
		if(boost::regex_search((LPCTSTR)line,regs[i]))
		{
			return true;
		}
	}
	return false;
}

UINT threadIndex(LPVOID pParam)
{
	indexThreadInfo *info=(indexThreadInfo*)pParam;
	DWORD id=info->id;
	id<<=16;
	long pos=info->startPos;
	MSG msg;
	if(PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
	{
		delete info;
		return 0;
	}
	int fileCode=dlg->fileCode;
	long fileSize=dlg->txtFile.GetLength();
	if (info->moveNextLine&&moveToNextLine(pos,fileCode,fileSize))
	{
		delete info;
		return 0;
	}
	CString line;
	int off;
	WCHAR w;
	long area=pos>>12;
	int i=0;
	info->startPos=pos;
	while(pos<fileSize)
	{
		off=dlg->readOneWord(pos,&w);
		pos+=off;
		switch(w)
		{
		case 0x0000:
			if(PeekMessage(&msg,NULL,WM_SEARCHTHREAD,WM_SEARCHTHREAD,PM_REMOVE))
			{
				delete info;
				return 0;
			}
			if (i>0&&detectIndex(line))
			{
				indexInfo *ii=new indexInfo;
				ii->pos=info->startPos;
				ii->name=line;
				PostMessage(info->hWnd,WM_SEARCHTHREAD,id+1,(LPARAM)ii);
			}
			i=0;
			info->startPos=pos;
			line.Empty();
			if (area!=pos>>12)
			{
				area=pos>>12;
				PostMessage(info->hWnd,WM_SEARCHTHREAD,id,area);
			}
			break;
		case _T(' '):
		case _T('　'):
			if (!i)
			{
				break;
			}
		default:
			if (i>=theApp.regexMaxChar)
			{
				moveToNextLine(pos,fileCode,fileSize);
				i=0;
				info->startPos=pos;
				line.Empty();
			}
			else
			{
				line+=w;
				i++;
			}
		}
	}
	PostMessage(info->hWnd,WM_SEARCHTHREAD,id+2,0);
	delete info;
	return 0;
}

bool moveToNextLine(long & pos,int fileCode,int fileSize)
{
	char buf[2];
	if (fileCode==3)
	{
		do 
		{
			if (pos>=fileSize)
			{
				return true;
			}
			buf[0]=dlg->getOneByte(pos++);
			buf[1]=dlg->getOneByte(pos++);
		} while (*((WCHAR*)buf)!=0x000a);
	}
	else
	{
		do 
		{
			if (pos>=fileSize)
			{
				return true;
			}
			buf[0]=dlg->getOneByte(pos++);
		} while (buf[0]!=0x0a);
	}
	return false;
}

void CDialogIndex::OnMenuIndexBuild() 
{
	// TODO: Add your command handler code here
	if (regs.empty())
	{
		MessageBox(_T("没有有效的正则表达式！请在“高级”中恢复默认设置。"));
		return;
	}
	if (m_listIndex.GetItemCount())
	{
		if (MessageBox(_T("要删除现在的所有索引信息并重新建立索引吗？\n对大文件进行索引可能需要花费很长时间。"),_T("确认"),MB_YESNO)!=IDYES)
		{
			return;
		}
		m_listIndex.DeleteAllItems();
	}
	else
	{
		if (MessageBox(_T("对大文件进行索引可能需要花费很长时间。\n确实要建立索引吗？"),_T("确认"),MB_YESNO)!=IDYES)
		{
			return;
		}
	}
	indexStart();
	m_progressIndex.SetRange32(0,dlg->txtFile.GetLength()>>12);
	m_progressIndex.SetPos(0);
	indexThreadInfo *info=new indexThreadInfo;
	info->id=id;
	info->startPos=dlg->bomSize;
	info->moveNextLine=false;
	info->hWnd=this->m_hWnd;
	thd=AfxBeginThread(threadIndex,info);
}

void CDialogIndex::OnMenuIndexClear() 
{
	// TODO: Add your command handler code here
	if (m_listIndex.GetItemCount())
	{
		m_listIndex.DeleteAllItems();
		changed=true;
	}
}

void CDialogIndex::OnMenuIndexContinue() 
{
	// TODO: Add your command handler code here
	if (regs.empty())
	{
		MessageBox(_T("没有有效的正则表达式！请在“高级”中恢复默认设置。"));
		return;
	}
	int i=m_listIndex.GetItemCount();
	if (!i)
	{
		OnMenuIndexBuild();
		return;
	}
	i--;
	long pos=m_listIndex.GetItemData(i);
	indexStart();
	m_progressIndex.SetRange32(pos>>12,dlg->txtFile.GetLength()>>12);
	m_progressIndex.SetPos(pos>>12);
	indexThreadInfo *info=new indexThreadInfo;
	info->id=id;
	info->startPos=pos;
	info->moveNextLine=true;
	info->hWnd=this->m_hWnd;
	thd=AfxBeginThread(threadIndex,info);
}

void CDialogIndex::OnMenuIndexDelete() 
{
	// TODO: Add your command handler code here
	POSITION pos=m_listIndex.GetFirstSelectedItemPosition();
	if (!pos)
	{
		MessageBox(_T("请选择要删除的索引。"));
		return;
	}
	int i=m_listIndex.GetNextSelectedItem(pos);
	m_listIndex.DeleteItem(i);
	if (m_listIndex.GetItemCount())
	{
		if (i>=m_listIndex.GetItemCount())
		{
			m_listIndex.SetItemState(i-1,LVIS_SELECTED,LVIS_SELECTED);
		}
		else
		{
			m_listIndex.SetItemState(i,LVIS_SELECTED,LVIS_SELECTED);
		}
	}
	changed=true;
}

void CDialogIndex::OnMenuIndexSetting() 
{
	// TODO: Add your command handler code here
	if (!dIS)
	{
		dIS=new CDialogIndexSetting(this);
	}
	dIS->DoModal();
}

void CDialogIndex::OnMenuIndexStop() 
{
	// TODO: Add your command handler code here
	thd->PostThreadMessage(WM_SEARCHTHREAD,0,0);
	indexEnd();
}

void CDialogIndex::OnKeydownListIndex(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	if (pLVKeyDow->wVKey==VK_DELETE)
	{
		OnMenuIndexDelete();
	}
	*pResult = 0;
}


void CDialogIndex::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (changed)
	{
		int l=m_listIndex.GetItemCount();
		CString s;
		MapKey *mk=&theApp.iniFile->values[dlg->txtFile.GetFilePath()];
		mk->erase(mk->lower_bound(_T("z")),mk->end());
		for (int i=0;i<l;i++)
		{
			s=_T('z');
			s+=m_listIndex.GetItemText(i,0);
			(*mk)[s]=m_listIndex.GetItemText(i,1);
		}
	}
}

BOOL CDialogIndex::OnToolTipsNotify(UINT id,NMHDR* pNmhdr,LRESULT *pResult)
{
	TOOLTIPTEXT* pT=(TOOLTIPTEXT*)pNmhdr;
	UINT nid=pNmhdr->idFrom;
	switch(nid)
	{
	case ID_MENU_INDEX_BUILD:
		pT->lpszText=_T("建立索引");
		break;
	case ID_MENU_INDEX_CONTINUE:
		pT->lpszText=_T("继续建立索引");
		break;
	case ID_MENU_INDEX_STOP:
		pT->lpszText=_T("停止");
		break;
	}
	*pResult=0;
	//::SetWindowPos(pNmhdr->hwndFrom,HWND_TOP,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOOWNERZORDER);
	return TRUE;
}

