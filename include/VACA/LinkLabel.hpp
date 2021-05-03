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
  Image* m_image{};

public:

  struct VACA_DLL Styles {
    static const Style Default;
  };

  LinkLabel(const String& urlOrText, Widget* parent, const Style& style = Styles::Default);
  LinkLabel(const String& url, const String& text, Widget* parent, const Style& style = Styles::Default);
  LinkLabel(const String& url, Image& image, Widget* parent, const Style& style = Styles::Default);
  ~LinkLabel() override;

  void setFont(Font font) override;

  virtual Color getLinkColor();
  virtual Color getHoverColor();

  Signal1<void, Event&> Click; ///< @see onClick

protected:

  // Events
  void onPreferredSize(PreferredSizeEvent& ev) override;
  void onPaint(PaintEvent& ev) override;
  void onMouseEnter(MouseEvent& ev) override;
  void onMouseMove(MouseEvent& ev) override;
  void onMouseLeave(MouseEvent& ev) override;
  void onMouseDown(MouseEvent& ev) override;
  void onSetCursor(SetCursorEvent& ev) override;
//   virtual void onResize(ResizeEvent& ev);
  void onFocusEnter(FocusEvent& ev) override;
  void onFocusLeave(FocusEvent& ev) override;
  void onKeyDown(KeyEvent& ev) override;

  // New events
  virtual void onClick(Event& ev);

private:

  void init(const String& text, Image* image = nullptr);
  void click();
  void updateFont(const Font& font);
  Rect getLinkBounds(Graphics& g);

};

} // namespace vaca
