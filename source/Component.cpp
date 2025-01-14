// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Component.hpp"
#include "Wg/SharedPtr.hpp"
#include "Wg/Property.hpp"
#include "Wg/Debug.hpp"

using namespace Wg;

/**
   Creates a new component.

   With the debug version of the library, you will get in the
   @ref page_debug_log a line specifying when the component
   was created.
*/
Component::Component()
{
  VACA_TRACE("new Component (%p)\n", this)
}

/**
   Destroys the component.

   With the debug version of the library, you will get in the
   @ref page_debug_log a line specifying when the component
   was destroyed.
*/
Component::~Component()
{
  VACA_TRACE("delete Component (%p)\n", this)
}

PropertyPtr Component::getProperty(const String& name)
{
  auto it = m_properties.find(name);
  if (it != m_properties.end())
    return it->second;
  else
    return PropertyPtr();
}

void Component::setProperty(const PropertyPtr& property)
{
  m_properties[property->getName()] = property;
}

bool Component::hasProperty(const String& name)
{
  auto it = m_properties.find(name);
  return it != m_properties.end();
}

void Component::removeProperty(const String& name)
{
  auto it = m_properties.find(name);
  if (it != m_properties.end())
    m_properties.erase(it);
}

const Component::Properties& Component::getProperties() const
{
  return m_properties;
}
