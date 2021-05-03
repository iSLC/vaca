// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"

namespace vaca {

/**
   Handles the global clipboard.
*/
class VACA_DLL Clipboard
{
  Widget* m_owner;

public:

  Clipboard(Widget* owner = NULL);
  virtual ~Clipboard();

  bool isText() const;

  String getString() const;
  void setString(const String& str);

};

} // namespace vaca
