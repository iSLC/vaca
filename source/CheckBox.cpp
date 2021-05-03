// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/CheckBox.hpp"

using namespace vaca;

CheckBox::CheckBox(const String& text, Widget* parent, Style style)
  : ButtonBase(parent, style)
{
  setText(text);
}

CheckBox::~CheckBox()
{
}
