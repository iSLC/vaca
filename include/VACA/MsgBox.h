// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#ifndef VACA_MSGBOX_H
#define VACA_MSGBOX_H

#include "VACA/Base.h"
#include "VACA/Color.h"
#include "VACA/Component.h"
#include "VACA/Exception.h"
#include "VACA/Graphics.h"
#include "VACA/Rect.h"
#include "VACA/ScrollInfo.h"
#include "VACA/Signal.h"
#include "VACA/Size.h"
#include "VACA/Style.h"

#include <vector>

namespace vaca {

/**
   Class to show a message box.
*/
class VACA_DLL MsgBox
{
public:

  struct TypeEnum
  {
    enum enumeration {
      Ok,
      OkCancel,
      YesNo,
      YesNoCancel,
      RetryCancel,
      CancelRetryContinue,
    };
    static const enumeration default_value = Ok;
  };

  struct IconEnum
  {
    enum enumeration {
      None,
      Error,
      Warning,
      Question,
      Information,
    };
    static const enumeration default_value = None;
  };

  struct ResultEnum
  {
    enum enumeration {
      Ok, Cancel,
      Yes, No,
      Retry, Continue,
    };
    static const enumeration default_value = Ok;
  };

  typedef Enum<TypeEnum> Type;
  typedef Enum<IconEnum> Icon;
  typedef Enum<ResultEnum> Result;

public:

  static Result show(Widget* parent,
		     const String& title,
		     const String& text,
		     Type type = Type::Ok,
		     Icon icon = Icon::None,
		     int default_button = -1);

  static void showException(Widget* parent,
			    const Exception& exception);

};

} // namespace vaca

#endif // VACA_MSGBOX_H
