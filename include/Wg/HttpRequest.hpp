// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/NonCopyable.hpp"
#include "Wg/Exception.hpp"

#include <wininet.h>

namespace Wg {

/**
   This exception is thrown when HttpRequest generates an error due
   connectivity problems.
*/
class VACA_DLL HttpRequestException : public Exception {
public:
    HttpRequestException() : Exception() {}

    HttpRequestException(const String &message) : Exception(message) {}

    ~HttpRequestException() noexcept override = default;
};

/**
   Sends a HTTP request to the specified URL to get its content.
*/
class VACA_DLL HttpRequest {
    class InetHandle {
    public:
        HINTERNET handle;

        InetHandle(HINTERNET handle, const String &errorMsg);

        virtual ~InetHandle();
    };

    class InetConnection : public InetHandle {
    public:
        InetConnection(const String &agentName);
    };

    class InetSession : public InetHandle {
    public:
        InetSession(InetConnection &conn, const String &host);
    };

    class InetRequest : public InetHandle {
    public:
        InetRequest(InetSession &ses,
                    const String &method,
                    const String &object);
    };

    InetConnection conn;
    InetSession ses;
    InetRequest req;

public:
    HttpRequest(const String &url, const String &method = L"GET");

    virtual ~HttpRequest();

    int send(const String &headers = L"", const char *body = nullptr);

    std::size_t read(char *buf, std::size_t length);

    int getStatusCode();

    std::size_t getContentLength();

    bool hasHeader(const String &headerName);

    String getHeader(const String &headerName);

};

} // namespace Wg
