#include "StdAfx.h"
#include <Windows.h>
#include "StringConvert.h"

StringConvert::StringConvert(void)
{
}


StringConvert::~StringConvert(void)
{
}


void StringConvert::gb2312ToUtf8(const string& src, string& result)
{
	wstring strWideChar;
	gb2312ToUnicode(src, strWideChar);
	unicodeToUTF8(strWideChar, result);
}


void StringConvert::unicodeToUTF8(const wstring& src, string& result)
{
	int n = WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, 0, 0, 0, 0);
	result.resize(n);
	::WideCharToMultiByte(CP_UTF8, 0, src.c_str(), -1, (char*)result.c_str(), result.length(), 0, 0);
}
void StringConvert::gb2312ToUnicode(const string& src, wstring& result)
{
	int n = MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, NULL, 0);
	result.resize(n);
	::MultiByteToWideChar(CP_ACP, 0, src.c_str(), -1, (LPWSTR)result.c_str(), result.length());
}
