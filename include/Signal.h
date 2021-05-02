// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#ifndef VACA_SIGNAL_H
#define VACA_SIGNAL_H

#include "vaca/base.h"
#include "vaca/Slot.h"

#include <vector>

namespace vaca {

/**
   @defgroup signal_group Signal Classes
   @{
 */

// ======================================================================
// Signal0_base<R>

/**
   Base class for signals which call functions without parameters.
*/
template<typename R>
class Signal0_base
{
public:
  typedef R ReturnType;
  typedef Slot0<R> SlotType;
  typedef std::vector<SlotType*> SlotList;

protected:
  SlotList m_slots;

public:
  Signal0_base() { }
  Signal0_base(const Signal0_base<R>& s)
  {
    copy(s);
  }
  ~Signal0_base()
  {
    disconnectAll();
  }

  SlotType* addSlot(SlotType* slot)
  {
    m_slots.push_back(slot);
    return slot;
  }

  template<typename F>
  SlotType* connect(const F& f)
  {
    return addSlot(new Slot0_fun<R, F>(f));
  }

  template<class T>
  SlotType* connect(R (T::*m)(), T* t)
  {
    return addSlot(new Slot0_mem<R, T>(m, t));
  }

  const SlotList& getSlots() const
  {
    return m_slots;
  }

  void disconnect(SlotType* slot)
  {
    remove_from_container(m_slots, slot);
  }

  void disconnectAll()
  {
    typename SlotList::iterator end = m_slots.end();
    for (typename SlotList::iterator
	   it = m_slots.begin(); it != end; ++it)
      delete *it;
    m_slots.clear();
  }

  bool empty() const
  {
    return m_slots.empty();
  }

  Signal0_base& operator=(const Signal0_base<R>& s) {
    copy(s);
    return *this;
  }

private:

  void copy(const Signal0_base<R>& s)
  {
    typename SlotList::const_iterator end = s.m_slots.end();
    for (typename SlotList::const_iterator
	   it = s.m_slots.begin(); it != end; ++it) {
      m_slots.push_back((*it)->clone());
    }
  }

};

// ======================================================================
// Signal0<R>

template<typename R>
class Signal0 : public Signal0_base<R>
{
public:
  Signal0() { }

  Signal0(const Signal0<R>& s)
    : Signal0_base<R>(s) { }

  R operator()(R default_result = R())
  {
    R result(default_result);
    typename Signal0_base<R>::SlotList::iterator end = Signal0_base<R>::m_slots.end();
    for (typename Signal0_base<R>::SlotList::iterator
	   it = Signal0_base<R>::m_slots.begin(); it != end; ++it) {
      typename Signal0_base<R>::SlotType* slot = *it;
      result = (*slot)();
    }
    return result;
  }

  template<typename Merger>
  R operator()(R default_result, const Merger& m)
  {
    R result(default_result);
    Merger merger(m);
    typename Signal0_base<R>::SlotList::iterator end = Signal0_base<R>::m_slots.end();
    for (typename Signal0_base<R>::SlotList::iterator
	   it = Signal0_base<R>::m_slots.begin(); it != end; ++it) {
      typename Signal0_base<R>::SlotType* slot = *it;
      result = merger(result, (*slot)());
    }
    return result;
  }

};

// ======================================================================
// Signal0<void>

template<>
class Signal0<void> : public Signal0_base<void>
{
public:
  Signal0() { }

  Signal0(const Signal0<void>& s)
    : Signal0_base<void>(s) { }

  void operator()()
  {
    SlotList::iterator end = m_slots.end();
    for (SlotList::iterator
	   it = m_slots.begin(); it != end; ++it) {
      SlotType* slot = *it;
      (*slot)();
    }
  }

};

// ======================================================================
// Signal1_base<R, A1>

/**
   Base class for signals which call functions with one parameter.
*/
template<typename R, typename A1>
class Signal1_base
{
public:
  typedef R ReturnType;
  typedef Slot1<R, A1> SlotType;
  typedef std::vector<SlotType*> SlotList;

protected:
  SlotList m_slots;

public:
  Signal1_base() { }
  Signal1_base(const Signal1_base<R, A1>& s)
  {
    copy(s);
  }
  ~Signal1_base()
  {
    disconnectAll();
  }

  SlotType* addSlot(SlotType* slot)
  {
    m_slots.push_back(slot);
    return slot;
  }

  template<typename F>
  SlotType* connect(const F& f)
  {
    return addSlot(new Slot1_fun<R, F, A1>(f));
  }

  template<class T>
  SlotType* connect(R (T::*m)(A1), T* t)
  {
    return addSlot(new Slot1_mem<R, T, A1>(m, t));
  }

  const SlotList& getSlots() const
  {
    return m_slots;
  }

  void disconnect(SlotType* slot)
  {
    remove_from_container(m_slots, slot);
  }

  void disconnectAll()
  {
    typename SlotList::iterator end = m_slots.end();
    for (typename SlotList::iterator
	   it = m_slots.begin(); it != end; ++it)
      delete *it;
    m_slots.clear();
  }

  bool empty() const
  {
    return m_slots.empty();
  }

  Signal1_base& operator=(const Signal1_base<R, A1>& s) {
    copy(s);
    return *this;
  }

private:

  void copy(const Signal1_base<R, A1>& s)
  {
    typename SlotList::const_iterator end = s.m_slots.end();
    for (typename SlotList::const_iterator
	   it = s.m_slots.begin(); it != end; ++it) {
      m_slots.push_back((*it)->clone());
    }
  }

};

// ======================================================================
// Signal1<R, A1>

template<typename R, typename A1>
class Signal1 : public Signal1_base<R, A1>
{
public:
  Signal1() { }

  Signal1(const Signal1<R, A1>& s)
    : Signal1_base<R, A1>(s) { }

  R operator()(A1 a1, R default_result = R())
  {
    R result(default_result);
    typename Signal1_base<R, A1>::SlotList::iterator end = Signal1_base<R, A1>::m_slots.end();
    for (typename Signal1_base<R, A1>::SlotList::iterator
	   it = Signal1_base<R, A1>::m_slots.begin(); it != end; ++it) {
      typename Signal1_base<R, A1>::SlotType* slot = *it;
      result = (*slot)(a1);
    }
    return result;
  }

  template<typename Merger>
  R operator()(A1 a1, R default_result, const Merger& m)
  {
    R result(default_result);
    Merger merger(m);
    typename Signal1_base<R, A1>::SlotList::iterator end = Signal1_base<R, A1>::m_slots.end();
    for (typename Signal1_base<R, A1>::SlotList::iterator
	   it = Signal1_base<R, A1>::m_slots.begin(); it != end; ++it) {
      typename Signal1_base<R, A1>::SlotType* slot = *it;
      result = merger(result, (*slot)(a1));
    }
    return result;
  }

};

// ======================================================================
// Signal1<void, A1>

template<typename A1>
class Signal1<void, A1> : public Signal1_base<void, A1>
{
public:
  Signal1() { }

  Signal1(const Signal1<void, A1>& s)
    : Signal1_base<void, A1>(s) { }

  void operator()(A1 a1)
  {
    typename Signal1_base<void, A1>::SlotList::iterator end = Signal1_base<void, A1>::m_slots.end();
    for (typename Signal1_base<void, A1>::SlotList::iterator
	   it = Signal1_base<void, A1>::m_slots.begin(); it != end; ++it) {
      typename Signal1_base<void, A1>::SlotType* slot = *it;
      (*slot)(a1);
    }
  }

};

// ======================================================================
// Signal2_base<R, A1, A2>

/**
   Base class for signals which call functions with two parameters.
*/
template<typename R, typename A1, typename A2>
class Signal2_base
{
public:
  typedef R ReturnType;
  typedef Slot2<R, A1, A2> SlotType;
  typedef std::vector<SlotType*> SlotList;

protected:
  SlotList m_slots;

public:
  Signal2_base() { }
  Signal2_base(const Signal2_base<R, A1, A2>& s)
  {
    copy(s);
  }
  ~Signal2_base()
  {
    disconnectAll();
  }

  SlotType* addSlot(SlotType* slot)
  {
    m_slots.push_back(slot);
    return slot;
  }

  template<typename F>
  SlotType* connect(const F& f)
  {
    return addSlot(new Slot2_fun<R, F, A1, A2>(f));
  }

  template<class T>
  SlotType* connect(R (T::*m)(A1, A2), T* t)
  {
    return addSlot(new Slot2_mem<R, T, A1, A2>(m, t));
  }

  const SlotList& getSlots() const
  {
    return m_slots;
  }

  void disconnect(SlotType* slot)
  {
    remove_from_container(m_slots, slot);
  }

  void disconnectAll()
  {
    typename SlotList::iterator end = m_slots.end();
    for (typename SlotList::iterator
	   it = m_slots.begin(); it != end; ++it)
      delete *it;
    m_slots.clear();
  }

  bool empty() const
  {
    return m_slots.empty();
  }

  Signal2_base& operator=(const Signal2_base<R, A1, A2>& s) {
    copy(s);
    return *this;
  }

private:

  void copy(const Signal2_base<R, A1, A2>& s)
  {
    typename SlotList::const_iterator end = s.m_slots.end();
    for (typename SlotList::const_iterator
	   it = s.m_slots.begin(); it != end; ++it) {
      m_slots.push_back((*it)->clone());
    }
  }

};

// ======================================================================
// Signal2<R, A1>

template<typename R, typename A1, typename A2>
class Signal2 : public Signal2_base<R, A1, A2>
{
public:
  Signal2() { }

  Signal2(const Signal2<R, A1, A2>& s)
    : Signal2_base<R, A1, A2>(s) { }

  R operator()(A1 a1, A2 a2, R default_result = R())
  {
    R result(default_result);
    typename Signal2_base<R, A1, A2>::SlotList::iterator end = Signal2_base<R, A1, A2>::m_slots.end();
    for (typename Signal2_base<R, A1, A2>::SlotList::iterator
	   it = Signal2_base<R, A1, A2>::m_slots.begin(); it != end; ++it) {
      typename Signal2_base<R, A1, A2>::SlotType* slot = *it;
      result = (*slot)(a1, a2);
    }
    return result;
  }

  template<typename Merger>
  R operator()(A1 a1, A2 a2, R default_result, const Merger& m)
  {
    R result(default_result);
    Merger merger(m);
    typename Signal2_base<R, A1, A2>::SlotList::iterator end = Signal2_base<R, A1, A2>::m_slots.end();
    for (typename Signal2_base<R, A1, A2>::SlotList::iterator
	   it = Signal2_base<R, A1, A2>::m_slots.begin(); it != end; ++it) {
      typename Signal2_base<R, A1, A2>::SlotType* slot = *it;
      result = merger(result, (*slot)(a1, a2));
    }
    return result;
  }

};

// ======================================================================
// Signal2<void, A1>

template<typename A1, typename A2>
class Signal2<void, A1, A2> : public Signal2_base<void, A1, A2>
{
public:
  Signal2() { }

  Signal2(const Signal2<void, A1, A2>& s)
    : Signal2_base<void, A1, A2>(s) { }

  void operator()(A1 a1, A2 a2)
  {
    typename Signal2_base<void, A1, A2>::SlotList::iterator end = Signal2_base<void, A1, A2>::m_slots.end();
    for (typename Signal2_base<void, A1, A2>::SlotList::iterator
	   it = Signal2_base<void, A1, A2>::m_slots.begin(); it != end; ++it) {
      typename Signal2_base<void, A1, A2>::SlotType* slot = *it;
      (*slot)(a1, a2);
    }
  }

};

/** @} */

} // namespace vaca

#endif // VACA_SIGNAL_H
