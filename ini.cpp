#include "StdAfx.h"
#include "ini.h"

CIniFile::CIniFile(const CString& path)
{
	filepath=path;
}

/*bool CIniFile::ReadFile()
{
	CFile f;
	if(!f.Open(filepath,CFile::modeRead|CFile::shareDenyWrite))
	{
		return false;
	}
	size_t l,kl;
	int i,j;
	CString key,name,v;
	MapKey *mk;
	try
	{
		CArchive ar(&f,CArchive::load);
		ar >> l;
		for (i=0;i<l;i++)
		{
			ar >> key;
			mk=&values[key];
			ar >> kl;
			for (j=0;j<kl;j++)
			{
				ar >> name;
				ar >> v;
				(*mk)[name]=v;
			}
		}
		ar.Close();
	}
	catch (CException*)
	{
		values.clear();
		//MessageBox(NULL,_T("oneline.dat文件已损坏！所有设置将重置。"),_T("配置文件损坏"),MB_OK|MB_ICONERROR);
		return false;
	}
	
	return true;
}

bool CIniFile::WriteFile()
{
	MapTotal::iterator it;
	MapKey::iterator itk;
	CFile f;
	if (!f.Open(filepath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive))
	{
		//MessageBox(NULL,_T("无法保存设置到INI文件！"),_T("一行小说阅读器"),MB_OK);
		return false;
	}
	try
	{
		CArchive ar(&f,CArchive::store);
		ar << values.size();
		for(it=values.begin();it!=values.end();it++)
		{
			ar << it->first;
			ar << it->second.size();
			for (itk=it->second.begin();itk!=it->second.end();itk++)
			{
				ar << itk->first << itk->second;
			}
		}
	}
	catch (CException*)
	{
		return false;
	}
	return true;
}*/

bool CIniFile::ReadFile()
{
	CStdioFile f;
	if(!f.Open(filepath,CFile::modeRead|CFile::shareDenyWrite))
	{
		return false;
	}
	CString line,name,data;
	int pos,pos2;
	MapKey *mk=NULL;
	//std::pair<MapTotal::iterator,bool> rpair;
	while (f.ReadString(line))
	{
		pos=line.FindOneOf(_T("=[;"));
		if (pos!=-1)
		{
			switch(line[pos])
			{
			case _T('['):
				pos2=line.ReverseFind(_T(']'));
				if (pos2>pos)
				{
					pos++;
					name=line.Mid(pos,pos2-pos);
					mk=&values[name];
				}
				break;
			case _T('='):
				if (mk)
				{
					name=line.Left(pos);
					data=line.Mid(pos+1);
					(*mk)[name]=data;
				}
				break;
			}
		}
	}
	f.Close();
	return true;
}

bool CIniFile::WriteFile()
{
	CStdioFile f;
	if (!f.Open(filepath,CFile::modeCreate|CFile::modeWrite|CFile::shareExclusive))
	{
		//MessageBox(NULL,_T("无法保存设置到INI文件！"),_T("一行小说阅读器"),MB_OK);
		return false;
	}
	MapTotal::iterator it;
	MapKey::iterator itk;
	//CString ts;
	for(it=values.begin();it!=values.end();it++)
	{
		f.WriteString(_T("[")+it->first+_T("]\n"));
		for (itk=it->second.begin();itk!=it->second.end();itk++)
		{
			f.WriteString(itk->first+_T("=")+itk->second+_T("\n"));
		}
	}
	return true;
}

const CString CIniFile::GetValue(const CString& keyname,const CString& valuename,const CString& v)
{
	MapTotal::iterator it;
	MapKey::iterator itk;
	it=values.find(keyname);
	if (it==values.end())
	{
		return v;
	}
	itk=it->second.find(valuename);
	if (itk==it->second.end())
	{
		return v;
	}
	else
	{
		return itk->second;
	}
}

const long CIniFile::GetValueI(const CString& keyname,const CString& valuename,const long& v)
{
	MapTotal::iterator it;
	MapKey::iterator itk;
	it=values.find(keyname);
	if (it==values.end())
	{
		return v;
	}
	itk=it->second.find(valuename);
	if (itk==it->second.end())
	{
		return v;
	}
	else
	{
		return _ttol(itk->second);
	}
}

const bool CIniFile::GetValueB(const CString& keyname,const CString& valuename,const bool& v)
{
	MapTotal::iterator it;
	MapKey::iterator itk;
	it=values.find(keyname);
	if (it==values.end())
	{
		return v;
	}
	itk=it->second.find(valuename);
	if (itk==it->second.end())
	{
		return v;
	}
	else
	{
		return _ttoi(itk->second)!=0;
	}
}

const double CIniFile::GetValueF(const CString& keyname,const CString& valuename,const double& v/* =0.0 */)
{
	MapTotal::iterator it;
	MapKey::iterator itk;
	it=values.find(keyname);
	if (it==values.end())
	{
		return v;
	}
	itk=it->second.find(valuename);
	if (itk==it->second.end())
	{
		return v;
	}
	else
	{
		TCHAR *stop;
		return _tcstod(itk->second,&stop);
	}
}

void CIniFile::SetValue(const CString& keyname,const CString& valuename,const CString& v)
{
	values[keyname][valuename]=v;
}

void CIniFile::SetValueI(const CString& keyname,const CString& valuename,const long& v)
{
	TCHAR s[12];
	values[keyname][valuename]=_ltot(v,s,10);
}

void CIniFile::SetValueB(const CString& keyname,const CString& valuename,const bool& v)
{
	if (v)
	{
		values[keyname][valuename]=_T("1");
	}
	else
	{
		values[keyname][valuename]=_T("0");
	}
}

void CIniFile::SetValueF(const CString& keyname,const CString& valuename,const double& v)
{
	CString s;
	s.Format(_T("%f"),v);
	values[keyname][valuename]=s;
}