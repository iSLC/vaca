// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/BoxConstraint.hpp"

using namespace Wg;

BoxConstraint::BoxConstraint(bool expansive)
  : m_expansive(expansive)
{
}

BoxConstraint::~BoxConstraint()
= default;

bool BoxConstraint::isExpansive() const
{
  return m_expansive;
}

void BoxConstraint::setExpansive(bool expansive)
{
  m_expansive = expansive;
}
