// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#define VACA_STRING_CPP

#include "Wg/String.hpp"
#include "Wg/Debug.hpp"
#include "Wg/Exception.hpp"
#include <cstdarg>
#include <cstdlib>
#include <cctype>
#include <vector>
#include <iterator>
#include <wininet.h>

#include <algorithm>
#include <memory>

using namespace Wg;

String Wg::format_string(const Char* fmt, ...)
{
  std::unique_ptr<Char[]> buf;
  int size = 512;

  while (true) {
    buf = std::unique_ptr<Char[]>(new Char[size <<= 1]);

    va_list ap;
    va_start(ap, fmt);
    int written = _vsnwprintf(buf.get(), static_cast<size_t>(size), fmt, ap);
    va_end(ap);

    if (written == size) {
      if (buf.get()[size] == 0)
	break;
    }
    else if (written >= 0 && written < size) {
      assert(buf.get()[written] == 0);
      break;
    }
    // else continue growing the buffer...
  }

  return String(buf.get());
}

String Wg::trim_string(const String& str)
{
  return trim_string(str.c_str());
}

String Wg::trim_string(const Char* str)
{
  assert(str != NULL);

  String res(str);
  while (!res.empty() && std::isspace(res.at(0)))
    res.erase(res.begin());
  while (!res.empty() && std::isspace(res.at(res.size()-1)))
    res.erase(res.end()-1);

  return res;
}

std::string Wg::to_utf8(const String& string)
{
  int required_size =
    WideCharToMultiByte(CP_UTF8, 0,
                        string.c_str(), static_cast<int>(string.size()),
                        nullptr, 0, nullptr, nullptr);

  if (required_size == 0)
    return std::string();

  std::vector<char> buf(static_cast<unsigned long long int>(++required_size));

  WideCharToMultiByte(CP_UTF8, 0,
                      string.c_str(), static_cast<int>(string.size()),
                      &buf[0], required_size,
                      nullptr, nullptr);

  return std::string(&buf[0]);
}

String Wg::from_utf8(const std::string& string)
{
  int required_size =
    MultiByteToWideChar(CP_UTF8, 0,
                        string.c_str(), static_cast<int>(string.size()),
                        nullptr, 0);

  if (required_size == 0)
    return String();

  std::vector<wchar_t> buf(static_cast<unsigned long long int>(++required_size));

  MultiByteToWideChar(CP_UTF8, 0,
                      string.c_str(), static_cast<int>(string.size()),
                      &buf[0], required_size);

  return String(&buf[0]);
}

namespace {
  struct is_separator
  {
    const String* separators;
    is_separator(const String* separators) : separators(separators) { }
    bool operator()(Char chr) const
    {
      for (wchar_t separator : *separators) {
	if (chr == separator)
	  return true;
      }
      return false;
    }
  };
}

void Wg::split_string(const String& string, std::vector<String>& parts, const String& separators)
{
  auto elements = static_cast<size_t>(1 + std::count_if(string.begin(), string.end(), is_separator(&separators)));
  parts.resize(elements);

  size_t beg = 0, end;
  while (true) {
    end = string.find_first_of(separators);
    if (end != String::npos) {
      parts.push_back(string.substr(beg, end - beg));
      beg = end+1;
    }
    else {
      parts.push_back(string.substr(beg));
      break;
    }
  }
}

template<> std::string Wg::convert_to(const Char* const& from)
{
  int len = static_cast<int>(std::wcslen(from) + 1);
  std::unique_ptr<char[]> ansiBuf(new char[len]);
  int ret = WideCharToMultiByte(CP_ACP, 0, from, len, ansiBuf.get(), len, nullptr, nullptr);
  if (ret == 0)
    return "";
  else
    return std::string(ansiBuf.get());
}

template<> std::string Wg::convert_to(const String& from)
{
  int len = static_cast<int>(from.size() + 1);
  std::unique_ptr<char[]> ansiBuf(new char[len]);
  int ret = WideCharToMultiByte(CP_ACP, 0, from.c_str(), len, ansiBuf.get(), len, nullptr, nullptr);
  if (ret == 0)
    return "";
  else
    return std::string(ansiBuf.get());
}

template<> int Wg::convert_to(const String& from)
{
  return (int)std::wcstol(from.c_str(), nullptr, 10);
}

template<> long Wg::convert_to(const String& from)
{
  return std::wcstol(from.c_str(), nullptr, 10);
}

template<> unsigned int Wg::convert_to(const String& from)
{
  return (unsigned int)std::wcstoul(from.c_str(), nullptr, 10);
}

template<> unsigned long Wg::convert_to(const String& from)
{
  return std::wcstoul(from.c_str(), nullptr, 10);
}

template<> float Wg::convert_to(const String& from)
{
  return (float)std::wcstod(from.c_str(), nullptr);
}

template<> double Wg::convert_to(const String& from)
{
  return std::wcstod(from.c_str(), nullptr);
}

template<> String Wg::convert_to(const char* const& from)
{
  int len = static_cast<int>(strlen(from) + 1);
  std::unique_ptr<Char[]> wideBuf(new Char[len]);
  int ret = MultiByteToWideChar(CP_ACP, 0, from, len, wideBuf.get(), len);
  if (ret == 0)
    return L"";
  else
    return String(wideBuf.get());
}

template<> String Wg::convert_to(const std::string& from)
{
  int len = static_cast<int>(from.size() + 1);
  std::unique_ptr<Char[]> wideBuf(new Char[len]);
  int ret = MultiByteToWideChar(CP_ACP, 0, from.c_str(), len, wideBuf.get(), len);
  if (ret == 0)
    return L"";
  else
    return String(wideBuf.get());
}

template<> String Wg::convert_to(const int& from)
{
  return format_string(L"%d", from);
}

template<> String Wg::convert_to(const long& from)
{
  return format_string(L"%ld", from);
}

template<> String Wg::convert_to(const unsigned int& from)
{
  return format_string(L"%u", from);
}

template<> String Wg::convert_to(const unsigned long& from)
{
  return format_string(L"%lu", from);
}

template<> String Wg::convert_to(const float& from)
{
  return format_string(L"%.16g", from);
}

template<> String Wg::convert_to(const double& from)
{
  return format_string(L"%.16g", from);
}

/**
   Commondly used to give strings to Win32 API or from Win32 API (in
   structures and messages).
*/
void Wg::copy_string_to(const String& src, Char* dest, int size)
{
  std::wcsncpy(dest, src.c_str(), static_cast<size_t>(size));
  dest[size-1] = L'\0';
}

/**
   Concatenates two path components.

   It is useful if the string represent a path and we have to add a
   file name. For example:
   @code
   String path(L"C:\\myproject\\src");
   assert(path / L"main.h" == L"C:\\myproject\\src\\main.h");
   @endcode

   @param component
     The string to be added at the end of the string
     (separated by a slash).
*/
String Wg::operator/(const String& path, const String& comp)
{
  String res(path);

  if (!res.empty() && *(res.end()-1) != L'/' && *(res.end()-1) != L'\\')
    res.push_back(L'\\');

  res += comp;
  return res;
}

/**
   Adds a path component at the end of the path.

   It is useful if the string represent a path and we have to add a
   file name. For example:
   @code
   String path(L"C:\\myproject\\src");
   path /= L"main.h";
   assert(path == L"C:\\myproject\\src\\main.h");
   @endcode

   @param component
     The string to be added at the end of the string
     (separated by a slash).
*/
String& Wg::operator/=(String& path, const String& comp)
{
  if (!path.empty() && *(path.end()-1) != L'/' && *(path.end()-1) != L'\\')
    path.push_back(L'\\');

  path += comp;
  return path;
}

/**
   Returns the file path (the path of "C:\foo\main.cpp" is "C:\foo"
   without the file name).

   @see file_name
*/
String Wg::file_path(const String& fullpath)
{
  String::const_reverse_iterator rit;
  String res;

  for (rit=fullpath.rbegin(); rit!=fullpath.rend(); ++rit)
    if (*rit == L'\\' || *rit == L'/')
      break;

  if (rit != fullpath.rend()) {
    ++rit;
    std::copy(fullpath.begin(), String::const_iterator(rit.base()),
	      std::back_inserter(res));
  }

  return res;
}

/**
   Returns the file name (the file name of "C:\foo\main.cpp" is
   "main.cpp", without the path).

   @see file_path, file_title
*/
String Wg::file_name(const String& fullpath)
{
  String::const_reverse_iterator rit;
  String res;

  for (rit=fullpath.rbegin(); rit!=fullpath.rend(); ++rit)
    if (*rit == L'\\' || *rit == L'/')
      break;

  std::copy(String::const_iterator(rit.base()), fullpath.end(),
	    std::back_inserter(res));

  return res;
}

/**
   Returns the file extension (the extension of "C:\foo\main.cpp" is
   "cpp", without the path and its title).

   @warning
     For a file name like "pack.tar.gz" the extension is "gz".

   @see file_path, file_title
*/
String Wg::file_extension(const String& fullpath)
{
  String::const_reverse_iterator rit;
  String res;

  // search for the first dot from the end of the string
  for (rit=fullpath.rbegin(); rit!=fullpath.rend(); ++rit) {
    if (*rit == L'\\' || *rit == L'/')
      return res;
    else if (*rit == L'.')
      break;
  }

  if (rit != fullpath.rend()) {
    std::copy(String::const_iterator(rit.base()), fullpath.end(),
	      std::back_inserter(res));
  }

  return res;
}

/**
   Returns the file title (the title of "C:\foo\main.cpp" is "main",
   without the path and without the extension).

   @warning
     For a file name like "pack.tar.gz" the title is "pack.tar".

   @see file_path, file_extension
*/
String Wg::file_title(const String& fullpath)
{
  String::const_reverse_iterator rit;
  String::const_iterator last_dot = fullpath.end();
  String res;

  for (rit=fullpath.rbegin(); rit!=fullpath.rend(); ++rit) {
    if (*rit == L'\\' || *rit == L'/')
      break;
    else if (*rit == L'.' && last_dot == fullpath.end())
      last_dot = rit.base()-1;
  }

  for (String::const_iterator it(rit.base()); it!=fullpath.end(); ++it) {
    if (it == last_dot)
      break;
    else
      res.push_back(*it);
  }

  return res;
}

String Wg::url_host(const String& url)
{
  String host;
  int len = static_cast<int>(url.size());
  for (int c=0; c<len; ++c) {
    if (url[c] == L':' && url[c+1] == L'/' && url[c+2] == L'/') {
      for (c+=3; c<len && url[c] != L'/'; ++c)
	host.push_back(url[c]);
    }
  }
  return host;
}

String Wg::url_object(const String& url)
{
  String object;
  int len = static_cast<int>(url.size());
  for (int c=0; c<len; ++c) {
    if (url[c] == ':' && url[c+1] == '/' && url[c+2] == '/') {
      for (c+=3; c<len && url[c] != '/'; ++c)
	;
      for (; c<len; ++c)
	object.push_back(url[c]);
    }
  }
  return object;
}

String Wg::encode_url(const String& url)
{
  std::unique_ptr<Char[]> buf;
  DWORD size = 1024;

  while (true) {
    buf = std::unique_ptr<Char[]>(new Char[size]);

    if (::InternetCanonicalizeUrl(url.c_str(), buf.get(), &size, 0))
      break;

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
      throw Exception();

    // else continue growing the buffer...
  }

  return String(buf.get());
}

String Wg::decode_url(const String& url)
{
  std::unique_ptr<Char[]> buf;
  DWORD size = 1024;

  while (true) {
    buf = std::unique_ptr<Char[]>(new Char[size]);

    if (::InternetCanonicalizeUrl(url.c_str(), buf.get(), &size,
				  ICU_DECODE | ICU_NO_ENCODE))
      break;

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
      throw Exception();

    // else continue growing the buffer...
  }

  return String(buf.get());
}
