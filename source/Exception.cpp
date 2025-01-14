// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Exception.hpp"
#include "Wg/String.hpp"

#include <lmerr.h>
#include <wininet.h>

using namespace Wg;

/**
   Creates generic exception with an empty error message.
*/
Exception::Exception()
  : std::exception()
{
  initialize();
}

/**
   Creates an exception with the specified error message.

   @param message Error message.
*/
Exception::Exception(const String& message)
  : std::exception()
  , m_message(message)
{
  initialize();
}

/**
   Destroys the exception.
*/
Exception::~Exception() noexcept
= default;

/**
   Returns a printable C-string of the error.

   If the Exception(const String&) constructor was used to
   create the exception, this member function returns the
   same string as #getMessage but in ASCII format.
*/
const char* Exception::what() const noexcept
{
  return m_what.c_str();
}

const String& Exception::getMessage() const noexcept
{
  return m_message;
}

int Exception::getErrorCode() const
{
  return m_errorCode;
}

void Exception::initialize()
{
  HMODULE hmodule = nullptr;
  DWORD flags =
    FORMAT_MESSAGE_ALLOCATE_BUFFER |
    FORMAT_MESSAGE_FROM_SYSTEM |
    FORMAT_MESSAGE_IGNORE_INSERTS;
  LPSTR msgbuf = nullptr;

  m_errorCode = static_cast<int>(GetLastError());

  // is it an network-error?
  if (m_errorCode >= NERR_BASE && m_errorCode <= MAX_NERR) {
    hmodule = LoadLibraryEx(L"netmsg.dll",
			    nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (hmodule)
      flags |= FORMAT_MESSAGE_FROM_HMODULE;
  }
  else if (m_errorCode >= INTERNET_ERROR_BASE && m_errorCode <= INTERNET_ERROR_LAST) {
    hmodule = LoadLibraryEx(L"wininet.dll",
			    nullptr, LOAD_LIBRARY_AS_DATAFILE);
    if (hmodule)
      flags |= FORMAT_MESSAGE_FROM_HMODULE;
  }

  // get the error message in ASCII
  if (!FormatMessageA(flags,
		      hmodule,
                      static_cast<DWORD>(m_errorCode),
		      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		      reinterpret_cast<LPSTR>(&msgbuf),
		      0, nullptr)) {
    // error we can't load the string
    msgbuf = nullptr;
  }

  if (hmodule)
    FreeLibrary(hmodule);

  m_what += convert_to<std::string>(format_string(L"%d", m_errorCode));
  m_what += " - ";
  if (msgbuf) {
    m_what += msgbuf;
    LocalFree(msgbuf);
  }
  m_what += convert_to<std::string>(m_message);
}
