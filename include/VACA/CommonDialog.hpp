// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Component.hpp"

#include <commdlg.h>

namespace vaca {

/**
   Base class for common dialogs.
*/
class VACA_DLL CommonDialog : public Component
{
  Widget* m_parent;

public:

  CommonDialog(Widget* parent);
  ~CommonDialog() override;

  Widget* getParent();
  HWND getParentHandle();

  virtual bool doModal() = 0;

};

} // namespace vaca