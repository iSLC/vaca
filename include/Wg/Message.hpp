// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/NonCopyable.hpp"
#include "Wg/Exception.hpp"

namespace Wg {

/**
   Thrown when a Message cannot be registered/created.
*/
class MessageException : public Exception {
public:

    MessageException() = default;

    MessageException(const String &message) : Exception(message) {}

    ~MessageException() noexcept override = default;

};

/**
   A message that comes from the operating system.

   @win32
     This is a wrapper for a @msdn{MSG}.
   @endwin32
*/
class VACA_DLL Message {
    friend class Thread;

    friend class Widget;

    MSG m_msg{};

public:

    Message();

    Message(const String &name);

    Message(const Message &msg, void *payload);

    virtual ~Message();

    [[nodiscard]] void *getPayload() const;

    inline bool operator==(const Message &message) const {
        return m_msg.message == message.m_msg.message;
    }

    inline operator MSG *() { return &m_msg; }

    inline operator MSG const *() const { return &m_msg; }

};

} // namespace Wg
