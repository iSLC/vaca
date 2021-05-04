// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/CommonDialog.hpp"
#include "Wg/Widget.hpp"
#include "Wg/Debug.hpp"
#include "Wg/Application.hpp"

using namespace Wg;

CommonDialog::CommonDialog(Widget* parent)
{
  m_parent = parent;
}

CommonDialog::~CommonDialog()
= default;

Widget* CommonDialog::getParent()
{
  return m_parent;
}

HWND CommonDialog::getParentHandle()
{
  return m_parent != nullptr ? m_parent->getHandle(): nullptr;
}
