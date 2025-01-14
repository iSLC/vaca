// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Mutex.hpp"
#include "Wg/NonCopyable.hpp"

namespace Wg {

/**
   An object to safely lock and unlock mutexes.

   The constructor of ScopedLock locks the mutex, the destructor
   unlocks the mutex. In this way you can safely use ScopedLock
   inside a try/catch block without worrying about to the lock
   state of the mutex.

   For example:
   @code
   try {
     ScopedLock hold(mutex);
     throw Exception();
   }
   catch (...) {
     // the mutex is unlocked here
   }
   // if you don't throw a exception, the mutex is unlocked here too
   @endcode

   @see Mutex, ConditionVariable
*/
class ScopedLock : private NonCopyable {
    Mutex &m_mutex;

    // not defined
    ScopedLock() = delete;

public:

    /**
       Creates the ScopedLock locking the specified mutex.

       @param mutex
         Mutex to be hold by the ScopedLock's life-time.
    */
    ScopedLock(Mutex &mutex)
            : m_mutex(mutex) {
        m_mutex.lock();
    }

    /**
       Destroys the ScopedLock unlocking the held mutex.
    */
    ~ScopedLock() {
        m_mutex.unlock();
    }

    /**
       Returns which mutex is being held.
    */
    [[nodiscard]] Mutex &getMutex() const {
        return m_mutex;
    }

};

} // namespace Wg
