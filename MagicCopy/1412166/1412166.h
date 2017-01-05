#include "resource.h"
#include <string>

void doRemoveMouseHook();
void doInstallMouseHook();
void cut();
void copy();
void paste();
void showMenuPopup(POINT p);
TCHAR* getContentFromClipboard();
void CancelPopupMenu();
std::wstring UrlEncode(std::wstring url);
std::wstring getLinkSearch(TCHAR* text);