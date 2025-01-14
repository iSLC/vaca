// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/CheckBox.hpp"

using namespace Wg;

CheckBox::CheckBox(const String& text, Widget* parent, const Style& style)
  : ButtonBase(parent, style)
{
  setText(text);
}

CheckBox::~CheckBox()
= default;
