// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Widget.hpp"

namespace vaca {

/**
   Base for every button.

   @win32
     It's a wrapper for the @msdn{BUTTON} class.
   @endwin32
*/
class VACA_DLL ButtonBase : public Widget
{
public:

  ButtonBase(Widget* parent, Style style);
  explicit ButtonBase(HWND handle);
  virtual ~ButtonBase();

  bool isSelected();
  void setSelected(bool state);

  // Signals
  Signal1<void, Event&> Click; ///< @see onClick

protected:
  // Events
  virtual void onPreferredSize(PreferredSizeEvent& ev);

  // New events
  virtual void onClick(Event& ev);

  // Reflected notifications
  virtual bool onReflectedCommand(int id, int code, LRESULT& lResult);
};

} // namespace vaca
