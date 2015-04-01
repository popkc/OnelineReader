#ifndef ini_h
#define ini_h

#include "StdAfx.h"

#include <map>

typedef std::map<CString,CString> MapKey;
typedef std::map<CString,MapKey> MapTotal;

class CIniFile
{
protected:
	CString filepath;
public:
	MapTotal values;
	CIniFile(const CString& path);
	void SetValue(const CString& keyname,const CString& valuename,const CString& v);
	void SetValueI(const CString& keyname,const CString& valuename,const long& v);
	void SetValueB(const CString& keyname,const CString& valuename,const bool& v);
	void SetValueF(const CString& keyname,const CString& valuename,const double& v);
	bool ReadFile();
	bool WriteFile();
	const CString GetValue(const CString& keyname,const CString& valuename,const CString& v=_T(""));
	const long GetValueI(const CString& keyname,const CString& valuename,const long& v=0);
	const bool GetValueB(const CString& keyname,const CString& valuename,const bool& v=false);
	const double GetValueF(const CString& keyname,const CString& valuename,const double& v=0.0);
};

#endif