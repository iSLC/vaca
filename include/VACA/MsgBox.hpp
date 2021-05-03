// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Color.hpp"
#include "VACA/Component.hpp"
#include "VACA/Exception.hpp"
#include "VACA/Graphics.hpp"
#include "VACA/Rect.hpp"
#include "VACA/ScrollInfo.hpp"
#include "VACA/Signal.hpp"
#include "VACA/Size.hpp"
#include "VACA/Style.hpp"

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
