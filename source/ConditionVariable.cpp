// -------------------- Original code from Boost --------------------
// Copyright (C) 2001-2003
// William E. Kempf
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Boost Software License - Version 1.0 - August 17th, 2003
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
// ------------------------------------------------------------------
//
// Vaca - Visual Application Components Abstraction
// Adapted by David Capello

#include "VACA/ConditionVariable.h"
#include "VACA/ScopedLock.h"

#include <limits>

using namespace vaca;

class ScopedUnlock : private NonCopyable
{
  ScopedLock& m_lock;
public:
  ScopedUnlock(ScopedLock& lock) : m_lock(lock) {
    m_lock.getMutex().unlock();
  }
  ~ScopedUnlock() {
    m_lock.getMutex().lock();
  }
};

/**
   Creates a new ConditionVariable.

   @throw CreateConditionVariableException
     If the creation of the ConditionVariable fails.
*/
ConditionVariable::ConditionVariable()
  : m_gone(0)
  , m_blocked(0)
  , m_waiting(0)
{
  m_gate = CreateSemaphore(0, 1, 1, NULL);
  m_queue = CreateSemaphore(0, 0, (std::numeric_limits<long>::max)(), NULL);
  m_mutex = CreateMutex(0, 0, NULL);

  if (!m_gate || !m_queue || !m_mutex) {
    if (m_gate) CloseHandle(m_gate);
    if (m_queue) CloseHandle(m_queue);
    if (m_mutex) CloseHandle(m_mutex);
    throw CreateConditionVariableException();
  }
}

ConditionVariable::~ConditionVariable()
{
  CloseHandle(m_gate);
  CloseHandle(m_queue);
  CloseHandle(m_mutex);
}

void ConditionVariable::notifyOne()
{
  unsigned signals = 0;

  WaitForSingleObject(m_mutex, INFINITE);
  if (m_waiting != 0) { // the m_gate is already closed
    if (m_blocked == 0) {
      ReleaseMutex(m_mutex);
      return;
    }

    ++m_waiting;
    --m_blocked;
    signals = 1;
  }
  else {
    WaitForSingleObject(m_gate, INFINITE);
    if (m_blocked > m_gone) {
      if (m_gone != 0) {
	m_blocked -= m_gone;
	m_gone = 0;
      }
      signals = m_waiting = 1;
      --m_blocked;
    }
    else
      ReleaseSemaphore(m_gate, 1, 0);
  }

  ReleaseMutex(m_mutex);
  if (signals)
    ReleaseSemaphore(m_queue, signals, 0);
}

void ConditionVariable::notifyAll()
{
  unsigned signals = 0;

  WaitForSingleObject(m_mutex, INFINITE);
  if (m_waiting != 0) { // the m_gate is already closed
    if (m_blocked == 0) {
      ReleaseMutex(m_mutex);
      return;
    }

    m_waiting += (signals = m_blocked);
    m_blocked = 0;
  }
  else {
    WaitForSingleObject(m_gate, INFINITE);
    if (m_blocked > m_gone) {
      if (m_gone != 0) {
	m_blocked -= m_gone;
	m_gone = 0;
      }
      signals = m_waiting = m_blocked;
      m_blocked = 0;
    }
    else
      ReleaseSemaphore(m_gate, 1, 0);
  }

  ReleaseMutex(m_mutex);
  if (signals)
    ReleaseSemaphore(m_queue, signals, 0);
}

void ConditionVariable::wait(ScopedLock& lock)
{
  enterWait();
  ScopedUnlock unlock(lock);

  WaitForSingleObject(m_queue, INFINITE);

  unsigned was_waiting = 0;
  unsigned was_gone = 0;

  WaitForSingleObject(m_mutex, INFINITE);
  was_waiting = m_waiting;
  was_gone = m_gone;
  if (was_waiting != 0) {
    if (--m_waiting == 0) {
      if (m_blocked != 0) {
	ReleaseSemaphore(m_gate, 1, 0); // open m_gate
	was_waiting = 0;
      }
      else if (m_gone != 0)
	m_gone = 0;
    }
  }
  else if (++m_gone == ((std::numeric_limits<unsigned>::max)() / 2)) {
    // timeout occured, normalize the m_gone count
    // this may occur if many calls to wait with a timeout are made and
    // no call to notify_* is made
    WaitForSingleObject(m_gate, INFINITE);
    m_blocked -= m_gone;
    ReleaseSemaphore(m_gate, 1, 0);
    m_gone = 0;
  }
  ReleaseMutex(m_mutex);

  if (was_waiting == 1) {
    for (; was_gone; --was_gone) {
      // better now than spurious later
      WaitForSingleObject(m_queue, INFINITE);
    }
    ReleaseSemaphore(m_gate, 1, 0);
  }
}

bool ConditionVariable::waitFor(ScopedLock& lock, double seconds)
{
  enterWait();
  ScopedUnlock unlock(lock);

  int milliseconds = static_cast<int>(seconds*1000.0);

  bool ret = (WaitForSingleObject(m_queue, milliseconds) == WAIT_OBJECT_0);

  unsigned was_waiting = 0;
  unsigned was_gone = 0;

  WaitForSingleObject(m_mutex, INFINITE);
  was_waiting = m_waiting;
  was_gone = m_gone;
  if (was_waiting != 0) {
    if (!ret) { // timeout
      if (m_blocked != 0)
	--m_blocked;
      else
	++m_gone; // count spurious wakeups
    }
    if (--m_waiting == 0) {
      if (m_blocked != 0) {
	ReleaseSemaphore(m_gate, 1, 0); // open m_gate
	was_waiting = 0;
      }
      else if (m_gone != 0)
	m_gone = 0;
    }
  }
  else if (++m_gone == ((std::numeric_limits<unsigned>::max)() / 2)) {
    // timeout occured, normalize the m_gone count
    // this may occur if many calls to wait with a timeout are made and
    // no call to notify_* is made
    WaitForSingleObject(m_gate, INFINITE);
    m_blocked -= m_gone;
    ReleaseSemaphore(m_gate, 1, 0);
    m_gone = 0;
  }
  ReleaseMutex(m_mutex);

  if (was_waiting == 1) {
    for (; was_gone; --was_gone) {
      // better now than spurious later
      WaitForSingleObject(m_queue, INFINITE);
    }
    ReleaseSemaphore(m_gate, 1, 0);
  }

  return ret;
}

void ConditionVariable::enterWait()
{
  WaitForSingleObject(m_gate, INFINITE);
  ++m_blocked;
  ReleaseSemaphore(m_gate, 1, 0);
}
