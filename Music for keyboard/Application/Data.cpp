// Data.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "Data.h"


void split(const wstring &s, wchar_t delim, vector<wstring> &elems) {
	wstringstream ss;
	ss.str(s);
	wstring item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


vector<wstring> split(const wstring &s, wchar_t delim) {
	vector<wstring> elems;
	split(s, delim, elems);
	return elems;
}
bool Init(map<int, std::wstring> &setting_1, wchar_t *filename)
{
	wstring line;
	wifstream myfile;
	myfile.open(filename, ios::in);
	if (myfile.is_open())
	{
		while (getline(myfile, line))
		{
			wcout << line << endl;
			vector<wstring> v = split(line, ' ');
			setting_1.insert(pair<int, wstring>(stoi(v[0]), MAINPATH + v[1]));
		}
		myfile.close();
		return 1;
	}
	return 0;
}

wstring GetLink(map<int, std::wstring> &setting_1, int key)
{
	wstring a = setting_1[key];
	return a;
}

void Update(map<int, std::wstring> &setting_1, int key, wstring value)
{
	setting_1[key] = value;
}

bool Save(map<int, std::wstring> setting_1, wchar_t *filename)
{
	wstring line;
	wofstream myfile;
	myfile.open(filename, ios::out);
	if (myfile.is_open())
	{
		map<int, wstring>::iterator temp;
		for (temp = setting_1.begin(); temp != setting_1.end(); temp++)
		{
			wchar_t t[256];
			swprintf_s(t, L"%d %s", temp->first, temp->second.c_str());
			myfile << t << std::endl;
		}
		myfile.close();
		return 1;
	}
	return 0;
}

wstring ExePath() {
	WCHAR buffer[500];
	DWORD size = GetModuleFileNameW(NULL, buffer, 500);
	wstring::size_type pos = wstring(buffer).find_last_of(L"\\/");

	wstring result = wstring(buffer).substr(0, pos);
	pos = result.find_last_of(L"\\/");
	return result.substr(0, pos);
}

