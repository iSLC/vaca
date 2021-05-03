// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/Clipboard.hpp"
#include "VACA/Widget.hpp"
#include "VACA/System.hpp"
#include "VACA/String.hpp"

using namespace vaca;

Clipboard::Clipboard(Widget* owner)
{
  m_owner = owner;
}

Clipboard::~Clipboard()
= default;

bool Clipboard::isText()
{
  return IsClipboardFormatAvailable(CF_TEXT) ? true: false;
}

String Clipboard::getString() const
{
  HWND hwndOwner = m_owner ? m_owner->getHandle(): nullptr;
  String str;

  if (!IsClipboardFormatAvailable(CF_TEXT) &&
      !IsClipboardFormatAvailable(CF_UNICODETEXT))
    return str;

  if (!OpenClipboard(hwndOwner)) 
    return str;

  HGLOBAL hglobal = GetClipboardData(CF_UNICODETEXT);
  if (hglobal != nullptr) {
    auto lpwstr = static_cast<LPWSTR>(GlobalLock(hglobal));
    if (lpwstr != nullptr) {
      str = lpwstr;
      GlobalUnlock(hglobal);
    }
  }
  else {
    hglobal = GetClipboardData(CF_TEXT);
    if (hglobal != nullptr) {
      auto lpstr = static_cast<LPSTR>(GlobalLock(hglobal));
      if (lpstr != nullptr) {
	str = convert_to<String>(lpstr);
	GlobalUnlock(hglobal);
      }
    }
  }

  CloseClipboard();

  return str;
}

void Clipboard::setString(const String& str)
{
  HWND hwndOwner = m_owner ? m_owner->getHandle(): nullptr;

  if (!OpenClipboard(hwndOwner))
    return;

  EmptyClipboard();

  int len = static_cast<int>(str.size());
  HGLOBAL hglobal = GlobalAlloc(GMEM_MOVEABLE, sizeof(Char)*(len+1));
  auto lptstr = static_cast<LPTSTR>(GlobalLock(hglobal));
  copy_string_to(str, lptstr, len+1);
  GlobalUnlock(hglobal);
 
#ifdef _UNICODE
  SetClipboardData(CF_UNICODETEXT, hglobal);
#else
  SetClipboardData(CF_TEXT, hglobal);
#endif

  CloseClipboard();
}
