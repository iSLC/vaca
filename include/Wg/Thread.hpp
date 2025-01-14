// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Exception.hpp"
#include "Wg/Message.hpp"
#include "Wg/NonCopyable.hpp"
#include "Wg/Slot.hpp"

namespace Wg {

// ======================================================================

/**
   It's like a namespace for ThreadPriority.

   @see ThreadPriority
*/
struct ThreadPriorityEnum {
    enum enumeration {
        Idle,
        Lowest,
        Low,
        Normal,
        High,
        Highest,
        TimeCritical,
    };
    static const enumeration default_value = Normal;
};

/**
   Thread priority.

   One of the following values:
   @li ThreadPriority::Idle
   @li ThreadPriority::Lowest
   @li ThreadPriority::Low
   @li ThreadPriority::Normal
   @li ThreadPriority::High
   @li ThreadPriority::Highest
   @li ThreadPriority::TimeCritical
*/
typedef Enum<ThreadPriorityEnum> ThreadPriority;

// ======================================================================

/**
   This exception is thrown when a new Thread couldn't be created.
*/
class CreateThreadException : public Exception {
public:

    CreateThreadException() : Exception() {}

    CreateThreadException(const String &message) : Exception(message) {}

    ~CreateThreadException() noexcept override = default;

};

/**
   A thread of execution.

   Each thread that has widgets has a queue of messages, each message
   represent an event that the user generated with the input devices
   or a signal from the operating system (OS). The #doMessageLoop is
   the member function that gets messages from the OS and distributes
   them to widgets.

   @see doMessageLoop
*/
class VACA_DLL Thread : public NonCopyable {
    friend class Frame;

private:

    HANDLE m_handle;
    ThreadId m_id;

public:

    Thread();

    ~Thread();

    /**
       Creates a new thread running the specified function or functor @a f.

       @throw CreateThreadException
         If the thread couldn't be created by Win32's @c CreateThread.
    */
    template<typename F>
    explicit Thread(F f) {
        Thread_(Slot0_fun<void, F>(f));
    }

    [[nodiscard]] ThreadId getId() const;

    void join();

    [[nodiscard]] bool isJoinable() const;

    void setThreadPriority(ThreadPriority priority);

    // ===============================================================
    // MESSAGES
    // ===============================================================

    void enqueueMessage(const Message &message) const;

private:
    void Thread_(const Slot0<void> &slot);

};

namespace CurrentThread {
VACA_DLL ThreadId getId();

VACA_DLL void enqueueMessage(const Message &message);

VACA_DLL void doMessageLoop();

VACA_DLL void doMessageLoopFor(Widget *widget);

VACA_DLL void pumpMessageQueue();

VACA_DLL void breakMessageLoop();

VACA_DLL void yield();

VACA_DLL void sleep(int msecs);

VACA_DLL bool getMessage(Message &msg);

VACA_DLL bool peekMessage(Message &msg);

VACA_DLL void processMessage(Message &msg);

namespace details {
VACA_DLL bool preTranslateMessage(Message &message);

VACA_DLL Widget *getOutsideWidget();

VACA_DLL void setOutsideWidget(Widget *widget);

VACA_DLL void addFrame(Frame *frame);

VACA_DLL void removeFrame(Frame *frame);
}

}

namespace details {
VACA_DLL void removeAllThreadData();
}

} // namespace Wg
