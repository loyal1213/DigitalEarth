#pragma once
#include <string>
using namespace std;
class StringConvert
{
public:
	StringConvert(void);
	~StringConvert(void);
	static void gb2312ToUtf8(const string& src, string& result);
	static void unicodeToUTF8(const wstring& src, string& result);
	static void gb2312ToUnicode(const string& src, wstring& result);
};

