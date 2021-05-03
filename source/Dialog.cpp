// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "VACA/Dialog.hpp"
#include "VACA/Debug.hpp"
#include "VACA/Application.hpp"
#include "VACA/CloseEvent.hpp"
#include "VACA/WidgetClass.hpp"
#include "VACA/CommandEvent.hpp"

#define WIN32_LEAN_AND_MEAN
#include <winuser.h>

using namespace vaca;

/**
   Creates a dialog using the default DialogClass.

   @see doModal(), DialogStyle
*/
Dialog::Dialog(const String& title, Widget* parent, const Style& style)
  : Frame(WidgetClassName::None, title, parent, style)
{
  setDefWndProc(DefDlgProc);

  create(DialogClass::getClassName(), parent, style);
  setText(title);

  SetWindowLongPtr(getHandle(), DWLP_DLGPROC,
		   reinterpret_cast<LONG_PTR>(Dialog::globalDlgProc));

  m_state = false;
}

/**
   Creates a dialog with a custom WNDCLASS. @a className can be NULL
   if you want to call Widget::create() by your self.
*/
Dialog::Dialog(const WidgetClassName& className, const String& title, Widget* parent, const Style& style)
  : Frame(WidgetClassName::None, title, parent, style)
{
  setDefWndProc(DefDlgProc);

  if (className != WidgetClassName::None) {
    create(className, parent, style);
    setText(title);
  }

  m_state = false;
}

Dialog::Dialog(ResourceId dialogId, Widget* parent)
  : Frame(::CreateDialog(Application::getHandle(),
			 dialogId.toLPTSTR(),
			 parent ? parent->getHandle() : nullptr,
			 Dialog::globalDlgProc))
{
  m_state = false;
}

Dialog::Dialog(HWND handle)
  : Frame(handle)
{
  m_state = false;
}

Dialog::~Dialog()
= default;

/**
   You can use this to set the doModal()'s return value.
*/
void Dialog::setReturnState(bool state)
{
  m_state = state;
}

/**
   Executes the dialog in a local message loop, disabling the parent
   widget.

   @return Should returns true if the user press the OK button, or
	   false if the user press the Cancel/Close buttons.
*/
bool Dialog::doModal()
{
  setVisible(true);

  CurrentThread::doMessageLoopFor(this);

  return m_state;
}

/**
   Calls Win32's IsDialogMessage.
*/
bool Dialog::preTranslateMessage(Message& message)
{
  if (Frame::preTranslateMessage(message))
    return true;

  if (::IsDialogMessage(getHandle(), (LPMSG)message))
    return true;

  return false;
}

Widget* Dialog::getNextFocusableWidget(Widget* widget)
{
  assert(::IsWindow(getHandle()));

  HWND hwnd = GetNextDlgTabItem(getHandle(),
				widget != nullptr ? widget->getHandle(): nullptr,
				FALSE);

  return hwnd != nullptr ? Widget::fromHandle(hwnd): nullptr;
}

Widget* Dialog::getPreviousFocusableWidget(Widget* widget)
{
  assert(::IsWindow(getHandle()));

  HWND hwnd = GetNextDlgTabItem(getHandle(),
				widget ? widget->getHandle(): nullptr,
				TRUE);

  return hwnd != nullptr ? Widget::fromHandle(hwnd): nullptr;
}

/**
   Hides the dialog, but before changes the return state
   (#setReturnState) to true, so #doModal returns true.

   You can use this to bind the OK button action (Button#Click).
   Also, this function is automatically called when the Id#Ok command
   is processed by the dialog through #onCommand event.

   Example:
   @code
     Dialog dlg(...);
     Button ok("&OK", &dlg);
     ok.Click.connect(Bind(&Dialog::onOk, &dlg));
   @endcode
   Or:
   @code
     Dialog dlg(...);
     Button ok("&OK", Dialog::Id::Ok, &dlg);
   @endcode

   @see #onCancel
*/
void Dialog::onOk()
{
  Ok();
  setReturnState(true);
  setVisible(false);
}

/**
   Generates a #onClose event for the dialog, but before changes the
   return state (setReturnState) to false, so #doModal returns false
   too.

   You can use this member function to bind the Cancel button action
   (Button#Click). Anyway this function is automatically
   called when the Id#Cancel command is processed by the dialog
   through #onCommand event.

   Example:
   @code
     Dialog dlg(...);
     Button cancel("&Cancel", &dlg);
     cancel.Click.connect(Bind(&Dialog::onCancel, &dlg));
   @endcode
   Or:
   @code
     Dialog dlg(...);
     Button cancel("&Cancel", Dialog::Id::Cancel, &dlg);
   @endcode

   @see #onOk, #onClose
*/
void Dialog::onCancel()
{
  Cancel();
  setReturnState(false);
  sendMessage(WM_CLOSE, 0, 0); // cancel is like if the user close the window
}

void Dialog::onCommand(CommandEvent& ev)
{
  if (!ev.isConsumed()) {
    switch (ev.getCommandId()) {

      case Dialog::Id::Ok:
	onOk();
	ev.consume();
	break;

      case Dialog::Id::Cancel:
	onCancel();
	ev.consume();
	break;
    }
  }
  Frame::onCommand(ev);
}

INT_PTR CALLBACK Dialog::globalDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  // do default behaviour
  return FALSE;
}
