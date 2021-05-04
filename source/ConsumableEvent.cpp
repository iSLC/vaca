// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/ConsumableEvent.hpp"

using namespace Wg;

ConsumableEvent::ConsumableEvent(Component* source)
  : Event(source)
  , m_consumed(false)
{
}

ConsumableEvent::~ConsumableEvent()
= default;

/**
   Consumes the event.
*/
void ConsumableEvent::consume()
{
  m_consumed = true;
}

/**
   The event was consumed.
*/
bool ConsumableEvent::isConsumed() const
{
  return m_consumed;
}
