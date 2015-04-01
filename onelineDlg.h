// onelineDlg.h : header file
//

#if !defined(AFX_ONELINEDLG_H__DE3B94A3_E07E_464F_9119_5AD0C5ECC345__INCLUDED_)
#define AFX_ONELINEDLG_H__DE3B94A3_E07E_464F_9119_5AD0C5ECC345__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// COnelineDlg dialog
#define CODE_ANSI 1
#define CODE_UTF8 2
#define CODE_UNICODE 3
#define ID_MENU_RECENTFILE 30000
#include <map>
#include "DialogSettings.h"
#include "DialogAbout.h"
#include "DialogJumpto.h"
#include "DialogSearch.h"
#include "DialogIndex.h"

struct fileBlock
{
	char *buf;
	bool valid;
};

void CALLBACK EXPORT MyOnTimer(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime);
void CALLBACK EXPORT OnTimerRoll(HWND hWnd,UINT nMsg,UINT nIDEvent,DWORD dwTime);
class COnelineDlg : public CDialog
{
// Construction
public:
	COnelineDlg(CWnd* pParent = NULL);	// standard constructor
	bool timerSet;
	void hideToEdge();
	void settingsChanged();
	CDialogSettings *dSettings;
	bool isSizeMove,isPopupMenu;
	void moveWordNext();
	void moveWordPixels(int);
	bool isHide;
	long nextWordPos,nextLinePos;
	LONG filePos;
	CFile txtFile;
	BYTE bomSize;
	bool needReDraw;
	BYTE fileCode;
	int screenX,screenY;
	void randomMove(long);
	char getOneByte(const long &pos);
	int getBackWord(const long&,WCHAR *);
	int readOneWord(const long &,WCHAR *);
// Dialog Data
	//{{AFX_DATA(COnelineDlg)
	enum { IDD = IDD_ONELINE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COnelineDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	bool hideFlag;
	void showFromEdge();
	CMenu youjian,*popupMenu,*trayMenu;
	NOTIFYICONDATA *trayData;
	bool fileOpened;
	void createTrayData();
	void openNovel(const CString &filename);
	
	std::map<long,fileBlock*> blocks;
	char* blockAvailable(const long &n);
	void createFileBuf(const long &n,fileBlock* fb);
	void clearBlock(const long &cb);
	int blocksize;
	
	CDC MemDC;
	CBitmap MemBitmap,*pOldBitmap;
	
	void getOutput();
	void closeNovel();
	int remainSpace;
	
	void moveLineNext();
	void moveLineBack();
	void moveWordBack();
	CPoint clickPoint;
	bool lButtonDown;
	
	CFont *pOldFont;
	bool ignoreClick;
	void drawMenu();
	void addToRecentFiles(const CString&);
	std::list<CString> recentFiles;
	void hideToShow();
	int isAnsiEmpty(char *);
	int isUtf8Empty(char *);
	bool isUnicodeEmpty(char*);
	
	bool rolling;
	//clock_t lastMenuTime;
	bool checkMenuOutsideClick,checkMenuInsideClick;
	bool flagNotMove;
	CDialogAbout *dAbout;
	int wordOffset,wordWidth;
	void adjustRollRate(int);
	void switchFile(bool isNext);
	CDialogJumpto *dJumpto;
	bool isReading;
	CString speechBuf;
	unsigned short *speechBufLength;
	void speechMoveNextWord(int);
	long speechPos;
	int speechWordCount;
	long nextSentencePos;
	void prepareNextSentence();
	bool isStopWord(const WCHAR &);
	bool isEndWord(const WCHAR &);
	CDialogSearch *dSearch;
	char *bufCache;
	long cacheArea;
	CDialogIndex *dIndex;
	CString outputWord;
	char moveDirection;
	// Generated message map functions
	//{{AFX_MSG(COnelineDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg UINT OnNcHitTest(CPoint point);
	afx_msg LRESULT OnDisplayChange(WPARAM, LPARAM);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnNcRButtonUp(UINT nHitTest, CPoint point);
	afx_msg void OnMenuExit();
	afx_msg LRESULT OnTrayEvent(WPARAM,LPARAM);
	afx_msg void OnDestroy();
	afx_msg void OnMenuShowtrayicon();
	afx_msg void OnMenu2Show();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuOpen();
	afx_msg void OnMenuAutodetect();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnMenuSettings();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMenuRead();
	afx_msg UINT OnGetDlgCode();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnMenuMoveforward();
	afx_msg void OnMenuMovebackward();
	afx_msg void OnMenuAutoroll();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnMenuJump();
	afx_msg void OnMenuFind();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
	afx_msg void OnMenuIndex();
	afx_msg BOOL OnNcActivate(BOOL bActive);
	//}}AFX_MSG
	afx_msg void OnMenuCodes(UINT);
	afx_msg void OnWakeUp();
	afx_msg void OnMenuRecentFile(UINT);
	afx_msg LRESULT OnHotKey(WPARAM,LPARAM);
	afx_msg LRESULT OnEnterSizeMove(WPARAM,LPARAM);
	afx_msg LRESULT OnExitSizeMove(WPARAM,LPARAM);
	afx_msg LRESULT OnTTSEvent(WPARAM,LPARAM);
	//afx_msg LRESULT OnUninitMenuPopup(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ONELINEDLG_H__DE3B94A3_E07E_464F_9119_5AD0C5ECC345__INCLUDED_)
