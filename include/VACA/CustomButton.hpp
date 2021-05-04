// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Button.hpp"

namespace Wg {

/**
   Customizable button to draw it with your own Widget#onPaint member function.

   Example:
   @code
   class MyFancyButton : public CustomButton
   {
   public:
     MyFancyButton(const String& text, Widget* parent)
       : CustomButton(text, parent)
     {
       // ...
     }

     // paint event...
     virtual void onPaint(PaintEvent& ev)
     {
       // don't call parent implementation, use your own code to
       // paint your customized button
     }
   }
   @endcode

   @win32
     It is a button with the @msdn{BS_OWNERDRAW} style, the paint event
     is intercepted by the parent of the CustomButton and then reflected
     to it (see @ref page_tn_009).
   @endwin32
*/
class VACA_DLL CustomButton : public Button {
    UINT m_itemAction;
    UINT m_itemState;

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    CustomButton(const String &text, Widget *parent, const Style &style = Styles::Default);

    ~CustomButton() override;

protected:

    // Reflected notifications
    bool onReflectedDrawItem(Graphics &g, LPDRAWITEMSTRUCT lpDrawItem) override;

public:

    [[nodiscard]] bool isDrawEntire() const;

    [[nodiscard]] bool isFocusChanged() const;

    [[nodiscard]] bool isSelectionChanged() const;

    // Visual Aspect

    // bool hasCheckedVisualAspect();
    [[nodiscard]] bool hasDefaultOptionVisualAspect() const;

    [[nodiscard]] bool hasDisabledVisualAspect() const;

    [[nodiscard]] bool hasFocusVisualAspect() const;

    // bool hasGrayedVisualAspect();
    // bool hasHotLightVisualAspect();
    // bool hasInactiveVisualAspect();
    [[nodiscard]] bool hasNoAccelVisualAspect() const;

    [[nodiscard]] bool hasNoFocusRectVisualAspect() const;

    [[nodiscard]] bool hasSelectedVisualAspect() const;

};

} // namespace Wg
