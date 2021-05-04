// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Property.hpp"
#include "Wg/Debug.hpp"

using namespace Wg;

/**
   Creates a new named property.
*/
Property::Property(const String& name)
  : m_name(name)
{
}

/**
   Destroys the property.
*/
Property::~Property()
= default;

String Property::getName() const
{
  return m_name;
}
