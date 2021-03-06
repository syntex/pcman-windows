#include "Address.h"

#include <afxinet.h>

CAddress::CAddress()
	: valid_(false)
{
}

CAddress::CAddress(const CString& url)
{
	Parse(url);
}

bool CAddress::Parse(const CString& url)
{
	url_ = url;

	int pos = url_.Find(_T("://"));
	if (pos < 0) {
		valid_ = false;
		return false;
	}
	protocol_ = url_.Left(pos).MakeLower();

	// Special handle for file. AfxParseURLEx has bugs unescaping.
	if (protocol_ == _T("file")) {
		TCHAR path[MAX_PATH];
		DWORD len = MAX_PATH;
		valid_ = S_OK == PathCreateFromUrl(url, path, &len, NULL);
		if (!valid_) {
			return false;
		}
		path[len] = 0;
		path_ = path;
	} else {
		CString user, pass;
		DWORD service;
		valid_ = 0 != AfxParseURLEx(url_, service, server_, path_, port_, user, pass, 0);
	}
	return true;
}

CAddress::~CAddress()
{
}

CString CAddress::Description() const
{
	CString s;
	s.Format(_T("[CAddress protocol=%s server=%s port=%d path=%s url=%s]"),
		protocol_, server_, port_, path_, url_);
	return s;
}

unsigned short CAddress::DefaultPort() const
{
	const CString& protocol = Protocol();
	if (protocol == "telnet" || protocol == "bbs")
		return 23;
	else if (protocol == "http" || protocol == "ws")
		return 80;
	else if (protocol == "https" || protocol == "wss")
		return 443;
	else
		return 0;
}
