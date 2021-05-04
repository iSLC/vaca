// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/Thread.hpp"
#include "VACA/Debug.hpp"
#include "VACA/Frame.hpp"
#include "VACA/Signal.hpp"
#include "VACA/Timer.hpp"
#include "VACA/Mutex.hpp"
#include "VACA/ScopedLock.hpp"
#include "VACA/Slot.hpp"
#include "VACA/TimePoint.hpp"

#include <vector>
#include <algorithm>
#include <memory>

using namespace Wg;

// ======================================================================

// TODO
// - replace with some C++0x's Thread-Local Storage
// - use __thread in GCC, and __declspec( thread ) in MSVC
// - use TlsAlloc

struct ThreadData
{
  /**
     The ID of this thread.
  */
  ThreadId threadId;

  /**
     Visible frames in this thread. A frame is an instance of Frame
     class.
  */
  std::vector<Frame*> frames;

  TimePoint updateIndicatorsMark;
  bool updateIndicators : 1;

  /**
     True if the message-loop must be stopped.
  */
  bool breakLoop : 1;

  /**
     Widget used to call createHandle.
  */
  Widget* outsideWidget;

  ThreadData(ThreadId id) {
    threadId = id;
    breakLoop = false;
    updateIndicators = true;
    outsideWidget = nullptr;
  }

};

static Mutex data_mutex;
static std::vector<ThreadData*> dataOfEachThread;

static ThreadData* get_thread_data()
{
  ScopedLock hold(data_mutex);
  std::vector<ThreadData*>::iterator it, end = dataOfEachThread.end();
  ThreadId id = ::GetCurrentThreadId();

  // first of all search the thread-data in the list "dataOfEachThread"...
  for (it=dataOfEachThread.begin();
       it!=end;
       ++it) {
    if ((*it)->threadId == id)	// it's already created...
      return *it;		// return it
  }

  // create the data for the this thread
  auto* data = new ThreadData(id);
  VACA_TRACE("new data-thread %d\n", id)

  // add it to the list
  dataOfEachThread.push_back(data);

  // return the allocated data
  return data;
}

// ======================================================================

static DWORD WINAPI ThreadProxy(LPVOID slot)
{
  // Force the creation of a message queue in this new thread...
  {
    MSG msg;
    PeekMessage(&msg, nullptr, WM_USER, WM_USER, PM_NOREMOVE);
  }

  std::unique_ptr<Slot0<void> > slot_ptr(reinterpret_cast<Slot0<void>*>(slot));
  (*slot_ptr)();
  return 0;
}

Thread::Thread()
{
  m_handle = ::GetCurrentThread();
  m_id = ::GetCurrentThreadId();

  VACA_TRACE("current Thread (%p, %d)\n", this, m_id)
}

/**
   Creates a new thread to execute the specified slot in that thread.

   @throw CreateThreadException
     If the thread could not be created.

   @internal
*/
void Thread::Thread_(const Slot0<void>& slot)
{
  Slot0<void>* slotclone = slot.clone();
  DWORD id;

  m_handle = CreateThread(nullptr, 0,
			  ThreadProxy,
			  // clone the slot
			  reinterpret_cast<LPVOID>(slotclone),
			  CREATE_SUSPENDED, &id);
  if (!m_handle) {
    delete slotclone;
    throw CreateThreadException();
  }

  m_id = id;

  VACA_TRACE("new Thread (%p, %d)\n", this, m_id)
  ResumeThread(m_handle);
}

Thread::~Thread()
{
  if (isJoinable() && m_handle != nullptr) {
     CloseHandle(reinterpret_cast<HANDLE>(m_handle));
     m_handle = nullptr;
  }

  VACA_TRACE("delete Thread (%p, %d)\n", this, m_id)
}

/**
   Returns the thread ID.

   @win32
     This is equal to @msdn{GetCurrentThreadId} for the current
     thread or the ID returned by @msdn{CreateThread}.
   @endwin32
*/
ThreadId Thread::getId() const
{
  return m_id;
}

/**
   Waits the thread to finish, and them closes it.
*/
void Thread::join()
{
  assert(m_handle != NULL);
  assert(isJoinable());

  WaitForSingleObject(reinterpret_cast<HANDLE>(m_handle), INFINITE);
  CloseHandle(reinterpret_cast<HANDLE>(m_handle));
  m_handle = nullptr;

  VACA_TRACE("join Thread (%p, %d)\n", this, m_id)
}

/**
   Returns true if this thread can be waited by the current thread.
*/
bool Thread::isJoinable() const
{
  return m_id != ::GetCurrentThreadId();
}

/**
   Sets the priority of the thread.

   Thread priority is relative to the other threads of the same
   process. If you want to change the priority of the entire process
   (respecting to all other processes) you have to use the
   Application#setProcessPriority member function. In other words,
   you should use this member function only if you have more than one thread in
   your application and you want to make run faster one thread than other.

   @param priority
     Can be one of the following values:
     One of the following values:
     @li ThreadPriority::Idle
     @li ThreadPriority::Lowest
     @li ThreadPriority::Low
     @li ThreadPriority::Normal
     @li ThreadPriority::High
     @li ThreadPriority::Highest
     @li ThreadPriority::TimeCritical

   @see Application#setProcessPriority
*/
void Thread::setThreadPriority(ThreadPriority priority)
{
  assert(m_handle != NULL);

  int nPriority;
  switch (priority) {
    case ThreadPriority::Idle:         nPriority = THREAD_PRIORITY_IDLE; break;
    case ThreadPriority::Lowest:       nPriority = THREAD_PRIORITY_LOWEST; break;
    case ThreadPriority::Low:          nPriority = THREAD_PRIORITY_BELOW_NORMAL; break;
    case ThreadPriority::Normal:       nPriority = THREAD_PRIORITY_NORMAL; break;
    case ThreadPriority::High:         nPriority = THREAD_PRIORITY_ABOVE_NORMAL; break;
    case ThreadPriority::Highest:      nPriority = THREAD_PRIORITY_HIGHEST; break;
    case ThreadPriority::TimeCritical: nPriority = THREAD_PRIORITY_TIME_CRITICAL; break;
    default:
      assert(false);	      // TODO throw invalid argument exception
      return;
  }

  ::SetThreadPriority(m_handle, nPriority);
}

void Thread::enqueueMessage(const Message& message) const
{
  MSG const* msg = (MSG const*)message;

  // Post the message
  while (::PostThreadMessage(m_id, msg->message, msg->wParam, msg->lParam) == 0) {
    // ...If the routine fail is because the thread does not have a message queue yet...
    // Wait some time and retry...
    ::Sleep(10);
  }
}

// ======================================================================
// CurrentThread

ThreadId CurrentThread::getId()
{
  return ::GetCurrentThreadId();
}

void CurrentThread::enqueueMessage(const Message& message)
{
  Thread currentThread;
  currentThread.enqueueMessage(message);
}

/**
   Does the message loop while there are
   visible @link Wg::Frame frames@endlink.

   @see Frame::setVisible
*/
void Wg::CurrentThread::doMessageLoop()
{
  // message loop
  Message msg;
  while (getMessage(msg))
    processMessage(msg);
}

/**
   Does the message loop until the @a widget is hidden.
*/
void Wg::CurrentThread::doMessageLoopFor(Widget* widget)
{
  // get widget HWND
  HWND hwnd = widget->getHandle();
  assert(::IsWindow(hwnd));

  // get parent HWND
  HWND hparent = widget->getParentHandle();

  // disable the parent HWND
  if (hparent != nullptr)
    ::EnableWindow(hparent, FALSE);

  // message loop
  Message message;
  while (widget->isVisible() && getMessage(message))
    processMessage(message);

  // enable the parent HWND
  if (hparent)
    ::EnableWindow(hparent, TRUE);
}

void Wg::CurrentThread::pumpMessageQueue()
{
  Message msg;
  while (peekMessage(msg))
    processMessage(msg);
}

void Wg::CurrentThread::breakMessageLoop()
{
  get_thread_data()->breakLoop = true;
  ::PostThreadMessage(::GetCurrentThreadId(), WM_NULL, 0, 0);
}

void Wg::CurrentThread::yield()
{
  ::Sleep(0);
}

void Wg::CurrentThread::sleep(int msecs)
{
  ::Sleep(static_cast<DWORD>(msecs));
}

/**
   Gets a message waiting for it: locks the execution of the program
   until a message is received from the operating system.

   @return
     True if the @a message parameter was filled (because a message was received)
     or false if there aren't more visible @link Frame frames@endlink
     to dispatch messages.
*/
bool Wg::CurrentThread::getMessage(Message& message)
{
  ThreadData* data = get_thread_data();

  // break this loop? (explicit break or no-more visible frames)
  if (data->breakLoop || data->frames.empty())
    return false;

  // we have to update indicators?
  if (data->updateIndicators &&
      data->updateIndicatorsMark.elapsed() > 0.1) {
    data->updateIndicators = false;

    // for each registered frame we should call updateIndicators to
    // update the state of all visible indicators (like top-level
    // items in the menu-bar and buttons in the tool-bar)
    for (auto & frame : data->frames) {
      frame->updateIndicators();
    }
  }

  // get the message from the queue
  auto msg = (LPMSG)message;
  msg->hwnd = nullptr;
  BOOL bRet = ::GetMessage(msg, nullptr, 0, 0);

  // WM_QUIT received?
  if (bRet == 0)
    return false;

  // WM_NULL message... maybe Timers or CallInNextRound
  if (msg->message == WM_NULL)
    Timer::pollTimers();

  return true;
}

/**
   Gets a message without waiting for it, if the queue is empty, this
   member function returns false.

   The message is removed from the queue.

   @return
     Returns true if the @a msg parameter was filled with the next message
     in the queue or false if the queue was empty.
*/
bool CurrentThread::peekMessage(Message& message)
{
  auto msg = (LPMSG)message;
  msg->hwnd = nullptr;
  return ::PeekMessage(msg, nullptr, 0, 0, PM_REMOVE) != FALSE;
}

void CurrentThread::processMessage(Message& message)
{
  auto msg = (LPMSG)message;

  if (!CurrentThread::details::preTranslateMessage(message)) {
    // Send preTranslateMessage to the active window (useful for
    // modeless dialogs). WARNING: Don't use GetForegroundWindow
    // because it returns windows from other applications
    HWND hactive = GetActiveWindow();
    if (hactive != nullptr && hactive != msg->hwnd) {
      Widget* activeWidget = Widget::fromHandle(hactive);
      if (activeWidget != nullptr && activeWidget->preTranslateMessage(message))
	return;
    }

    //if (!TranslateAccelerator(msg->hwnd, hAccelTable, msg))
    //{
    ::TranslateMessage(msg);
    ::DispatchMessage(msg);
    //}
  }
}

// ======================================================================
// Vaca internals

/**
   Pretranslates the message. The main function is to retrieve the
   Widget pointer (using Widget::fromHandle()) and then (if it isn't
   NULL), call its Widget#preTranslateMessage.
*/
bool CurrentThread::details::preTranslateMessage(Message& message)
{
  auto msg = (LPMSG)message;

  // TODO process messages that produce a update-indicators event
  if ((msg->message == WM_ACTIVATE) ||
      (msg->message == WM_CLOSE) ||
      (msg->message == WM_SETFOCUS) ||
      (msg->message == WM_KILLFOCUS) ||
      (msg->message >= WM_LBUTTONDOWN && msg->message <= WM_MBUTTONDBLCLK) ||
      (msg->message >= WM_KEYDOWN && msg->message <= WM_DEADCHAR)) {
    ThreadData* data = get_thread_data();
    data->updateIndicators = true;
    data->updateIndicatorsMark = TimePoint();
  }

  if (msg->hwnd != nullptr) {
    Widget* widget = Widget::fromHandle(msg->hwnd);
    if (widget && widget->preTranslateMessage(message))
      return true;
  }

  return false;
}

/**
    @internal
 */
Widget* CurrentThread::details::getOutsideWidget()
{
  return get_thread_data()->outsideWidget;
}

/**
   @internal
 */
void CurrentThread::details::setOutsideWidget(Widget* widget)
{
  get_thread_data()->outsideWidget = widget;
}

/**
   @internal
 */
void CurrentThread::details::addFrame(Frame* frame)
{
  get_thread_data()->frames.push_back(frame);
}

/**
   @internal
 */
void CurrentThread::details::removeFrame(Frame* frame)
{
  remove_from_container(get_thread_data()->frames, frame);

  // when this thread doesn't have more Frames to continue we must to
  // break the current message loop
  if (get_thread_data()->frames.empty())
    CurrentThread::breakMessageLoop();
}

void details::removeAllThreadData()
{
  ScopedLock hold(data_mutex);
  std::vector<ThreadData*>::iterator it, end = dataOfEachThread.end();

  for (it=dataOfEachThread.begin(); it!=end; ++it) {
    VACA_TRACE("delete data-thread %d\n", (*it)->threadId)
    delete *it;
  }

  dataOfEachThread.clear();
}
