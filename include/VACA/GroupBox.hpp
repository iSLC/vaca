// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Widget.hpp"

namespace vaca {

/**
   An edge (with a label optionally) that can be used to group
   sub-widgets.
*/
class VACA_DLL GroupBox : public Widget
{
public:

  struct VACA_DLL Styles {
    static const Style Default;
  };

  GroupBox(const String& text, Widget* parent, Style style = Styles::Default);
  virtual ~GroupBox();

  Size getNonClientSize();

protected:

  // Events
  virtual void onPreferredSize(PreferredSizeEvent& ev);
  virtual void onLayout(LayoutEvent& ev);

  virtual bool wndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT& lResult);

};

} // namespace vaca
