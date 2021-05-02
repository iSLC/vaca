// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#ifndef VACA_COLORDIALOG_H
#define VACA_COLORDIALOG_H

#include "VACA/Base.h"
#include "VACA/CommonDialog.h"
#include "VACA/Color.h"

namespace vaca {

/**
   A common dialog box to select colors.
*/
class VACA_DLL ColorDialog : public CommonDialog
{
  Color m_color;
  COLORREF m_customColors[16];

public:

  ColorDialog(const Color& color, Widget* parent);
  virtual ~ColorDialog();

  virtual bool doModal();

  Color getColor() const;

};

} // namespace vaca

#endif // VACA_COLORDIALOG_H
