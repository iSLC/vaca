// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/CloseEvent.hpp"
#include "Wg/Frame.hpp"

using namespace Wg;

/**
   Creates a new CloseEvent for the specified Frame.

   @param source
      Frame that is being closed.
*/
CloseEvent::CloseEvent(Frame* source)
  : CancelableEvent(source)
{
}

/**
   Destroys the CloseEvent.
*/
CloseEvent::~CloseEvent()
= default;
