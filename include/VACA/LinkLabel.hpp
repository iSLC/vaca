// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/CustomLabel.hpp"

namespace vaca {

/**
   A link to Internet (or whatever you want).
*/
class VACA_DLL LinkLabel : public CustomLabel
{
  enum State {
    Outside,
    Inside,
    Hover
  };

  State m_state;
  String m_url;
  Font m_underlineFont;
  Image* m_image;

public:

  struct VACA_DLL Styles {
    static const Style Default;
  };

  LinkLabel(const String& urlOrText, Widget* parent, Style style = Styles::Default);
  LinkLabel(const String& url, const String& text, Widget* parent, Style style = Styles::Default);
  LinkLabel(const String& url, Image& image, Widget* parent, Style style = Styles::Default);
  virtual ~LinkLabel();

  virtual void setFont(Font font);

  virtual Color getLinkColor();
  virtual Color getHoverColor();

  Signal1<void, Event&> Click; ///< @see onClick

protected:

  // Events
  virtual void onPreferredSize(PreferredSizeEvent& ev);
  virtual void onPaint(PaintEvent& ev);
  virtual void onMouseEnter(MouseEvent& ev);
  virtual void onMouseMove(MouseEvent& ev);
  virtual void onMouseLeave(MouseEvent& ev);
  virtual void onMouseDown(MouseEvent& ev);
  virtual void onSetCursor(SetCursorEvent& ev);
//   virtual void onResize(ResizeEvent& ev);
  virtual void onFocusEnter(FocusEvent& ev);
  virtual void onFocusLeave(FocusEvent& ev);
  virtual void onKeyDown(KeyEvent& ev);

  // New events
  virtual void onClick(Event& ev);

private:

  void init(String text, Image* image = NULL);
  void click();
  void updateFont(const Font& font);
  Rect getLinkBounds(Graphics& g);

};

} // namespace vaca
