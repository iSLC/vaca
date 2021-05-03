// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/ColorDialog.hpp"
#include "VACA/Application.hpp"
#include "VACA/Win32.hpp"

using namespace vaca;

ColorDialog::ColorDialog(const Color& color, Widget* parent)
  : CommonDialog(parent)
  , m_color(color)
{
  for (unsigned long & m_customColor : m_customColors)
    m_customColor = RGB(255, 255, 255);
}

ColorDialog::~ColorDialog()
= default;

bool ColorDialog::doModal()
{
  CHOOSECOLOR cc;

  cc.lStructSize = sizeof(CHOOSECOLOR);
  cc.hwndOwner = getParentHandle();
  cc.hInstance = nullptr;
  cc.rgbResult = convert_to<COLORREF>(m_color);
  cc.lpCustColors = m_customColors;
  cc.Flags = 0
    | CC_ANYCOLOR
    | CC_FULLOPEN
    | CC_RGBINIT
    | CC_SOLIDCOLOR
    ;
//   cc.lCustData;
//   cc.lpfnHook;
//   cc.lpTemplateName;

  if (ChooseColor(&cc)) {
    m_color = convert_to<Color>(cc.rgbResult);
    return true;
  }
  else
    return false;
}

Color ColorDialog::getColor() const
{
  return m_color;
}
