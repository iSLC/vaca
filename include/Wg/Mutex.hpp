// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/NonCopyable.hpp"

namespace Wg {

/**
   An object to synchronize threads using mutual exclusion of critical
   sections.

   This kind of mutex can be used to synchronize multiple threads of
   the same process. No multiple processes!

   @win32
     This is a @msdn{CRITICAL_SECTION} wrapper.
   @endwin32

   @see ScopedLock, ConditionVariable, Thread,
	@wikipedia{Critical_section, Critical Section in Wikipedia}
	@wikipedia{Mutex, Mutex in Wikipedia}
*/
class VACA_DLL Mutex : private NonCopyable {
    class MutexImpl;

    MutexImpl *m_impl;

public:

    Mutex();

    ~Mutex();

    void lock();

    bool tryLock();

    void unlock();

};

} // namespace Wg
