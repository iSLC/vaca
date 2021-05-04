// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/HttpRequest.hpp"
#include "Wg/String.hpp"

#include <memory>
#include <cassert>

using namespace Wg;
using namespace std;

// ======================================================================
// HttpRequest::InetHandle

/**
   @throw HttpRequestException
*/
HttpRequest::InetHandle::InetHandle(HINTERNET handle,
				    const String& errorMsg)
  : handle(handle)
{
  if (!handle)
    throw HttpRequestException(errorMsg);
}

HttpRequest::InetHandle::~InetHandle()
{
  if (handle)
    InternetCloseHandle(handle);
}

// ======================================================================
// HttpRequest::InetConnection

HttpRequest::InetConnection::InetConnection(const String& agentName)
  : InetHandle(InternetOpen(agentName.c_str(),
			    INTERNET_OPEN_TYPE_DIRECT,
			    nullptr, nullptr, 0),
	       L"Can't connect to Internet")
{
}

// ======================================================================
// HttpRequest::InetSession

HttpRequest::InetSession::InetSession(InetConnection& conn,
				      const String& host)
  : InetHandle(InternetConnect(conn.handle,
			       host.c_str(),
			       INTERNET_DEFAULT_HTTP_PORT,
			       nullptr, nullptr, INTERNET_SERVICE_HTTP, 0, 0),
	       String(L"Can't open a HTTP session with ") + host)
{
}

// ======================================================================
// HttpRequest::InetRequest

HttpRequest::InetRequest::InetRequest(InetSession& ses,
				      const String& method,
				      const String& object)
  : InetHandle(HttpOpenRequest(ses.handle,
			       method.c_str(),
			       object.c_str(),
			       nullptr, nullptr, nullptr,
			       INTERNET_FLAG_NO_UI, 0),
	       L"Can't send a " + method + L" request to " + object)
{
}

// ======================================================================
// HttpRequest

HttpRequest::HttpRequest(const String& url,
			 const String& method)
    : conn(L"Mozilla/4.0 (compatible; MSIE 7.0; Windows NT 5.1)")
    , ses(conn, url_host(url))
    , req(ses, method, url_object(url))
{
}

HttpRequest::~HttpRequest()
= default;

/**
   Sends the HTTP request.

   @return
     The status code of the HTTP response that comes from the
     server.
*/
int HttpRequest::send(const String& headers, const char* body)
{
  if (HttpSendRequest(req.handle,
		      headers.empty() ? nullptr: headers.c_str(),
                      static_cast<DWORD>(headers.size()),
		      reinterpret_cast<LPVOID>(const_cast<char*>(body)),
                      static_cast<DWORD>(body ? strlen(body) : 0))) {
    return getStatusCode();
  }
  return 0;
}

/**
   Reads the specified amount of bytes from the stream of bytes.

   @param buf
     Where to put the read data. Must have at least @a length bytes.

   @param length
     How many bytes to read. The size of the buffer.

   @return
     The number of bytes that were read.

   @throw HttpRequestException
*/
size_t HttpRequest::read(char* buf, size_t length)
{
  assert(buf != NULL);
  assert(length > 0);

  DWORD bytes = 0;
  if (!InternetReadFile(req.handle, buf, static_cast<DWORD>(length), &bytes))
    throw HttpRequestException();

  return bytes;
}

int HttpRequest::getStatusCode()
{
  DWORD statusCode = 0;
  DWORD bufLength = sizeof(DWORD);
  DWORD index = 0;

  if (HttpQueryInfo(req.handle,
		    HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER,
		    &statusCode,
		    &bufLength,
		    &index)) {
    return static_cast<int>(statusCode);
  }
  else
    return 0;
}

size_t HttpRequest::getContentLength()
{
  DWORD contentLength = 0;
  DWORD bufLength = sizeof(DWORD);
  DWORD index = 0;

  if (HttpQueryInfo(req.handle,
		    HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER,
		    &contentLength,
		    &bufLength,
		    &index)) {
    return contentLength;
  }
  else
    return 0;
}

bool HttpRequest::hasHeader(const String& headerName)
{
  auto bufLength = static_cast<DWORD>(headerName.size() + 1);
  std::unique_ptr<char[]> buf(new char[bufLength]);
  DWORD index = 0;

  strcpy(buf.get(), convert_to<std::string>(headerName).c_str());

  if (HttpQueryInfo(req.handle, HTTP_QUERY_CUSTOM,
		    buf.get(), &bufLength, &index)) {
    return true;
  }
  else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
    return true;
  else
    return false;
}

/**
   Returns the value of the specified header.

   @return
     The value of the specified header, or an empty string if the
     header is not found.
*/
String HttpRequest::getHeader(const String& headerName)
{
  auto bufLength = static_cast<DWORD>(headerName.size() + 1);
  std::unique_ptr<char[]> buf(new char[bufLength]);
  DWORD index = 0;

  while (true) {
    // copy the header's name in the buffer 'buf'
    strcpy(buf.get(), convert_to<std::string>(headerName).c_str());

    // get the header's value
    if (HttpQueryInfo(req.handle, HTTP_QUERY_CUSTOM,
		      buf.get(), &bufLength, &index)) {
      // ok
      return convert_to<String, const char*>(buf.get());
    }
    else if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
      // the old value is deleted by auto_ptr::operator=
      buf = std::unique_ptr<char[]>(new char[bufLength]);
      continue;
    }
    else
      return String(L"");
  }
}
