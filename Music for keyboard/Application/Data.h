#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <sstream>
using namespace std;

bool Init(map<int, std::wstring> &setting_1, wchar_t *filename);
std::wstring GetLink(map<int, std::wstring> &setting_1, int key);
std::wstring ExePath();
void Update(map<int, std::wstring> &setting_1, int key, wstring value);
bool Save(map<int, std::wstring> setting_1, wchar_t *filename);

#define MAINPATH (ExePath() + L"\\")