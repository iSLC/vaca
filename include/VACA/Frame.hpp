// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Widget.hpp"
#include "VACA/WidgetClass.hpp"
#include "VACA/Register.hpp"
#include "VACA/Icon.hpp"

#include <vector>

namespace Wg {

/**
   Represents the Win32 class used by Frame.
*/
class FrameClass : public WidgetClass {
public:
    static WidgetClassName getClassName() { return WidgetClassName(L"Vaca.Frame"); }
};

/**
   A window with title-bar that can be minimized, maximized, etc.
*/
class VACA_DLL Frame : public Register<FrameClass>, public Widget {
    MenuBar *m_menuBar{};               // The menu bar.
    bool m_counted{};               // true if this Frame is counted in the frames' list
    Icon m_smallIcon;
    Icon m_bigIcon;

public:

    struct VACA_DLL Styles {
        static const Style Default;
        static const Style WithCaption;
        static const Style WithSystemMenu;
        static const Style Minimizable;
        static const Style Maximizable;
        static const Style Resizable;
        static const Style InitiallyMinimized;
        static const Style InitiallyMaximized;
    };

    Frame(const String &title, Widget *parent = nullptr, const Style &style = Styles::Default);

    Frame(const WidgetClassName &className, const String &title, Widget *parent = nullptr,
          const Style &style = Styles::Default);

    explicit Frame(HWND handle);

    ~Frame() override;

    void setVisible(bool visible) override;

    MenuBar *getMenuBar();

    virtual MenuBar *setMenuBar(MenuBar *menuBar);

    void setSmallIcon(const Icon &icon);

    void setBigIcon(const Icon &icon);

    void setIcon(const ResourceId &iconId);

    virtual Size getNonClientSize();

    [[nodiscard]] bool isLayoutFree() const override;

    virtual bool keepSynchronized();

    Signal1<void, Event &> Activate;     ///< @see onActivate
    Signal1<void, Event &> Deactivate;   ///< @see onDeactivate
    Signal1<void, CloseEvent &> Close;   ///< @see onClose
    Signal2<void, CardinalDirection, Rect &> Resizing; ///< @see onResizing

    bool preTranslateMessage(Message &message) override;

protected:
    // Events
    void onPreferredSize(PreferredSizeEvent &ev) override;

    void onResize(ResizeEvent &ev) override;

    void onCommand(CommandEvent &ev) override;

    void onUpdateIndicators() override;

    // New events
    virtual void onActivate(Event &ev);

    virtual void onDeactivate(Event &ev);

    virtual void onClose(CloseEvent &ev);

    virtual void onResizing(CardinalDirection dir, Rect &rc);

    bool wndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &lResult) override;

private:

    void initialize(const String &title);

    void updateMenuItem(MenuItem *menuItem);

    WidgetList getSynchronizedGroup();

};

} // namespace Wg
