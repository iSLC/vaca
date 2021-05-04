// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Frame.hpp"

namespace Wg {

/**
   ID for the first MdiChild. These IDs are used internally by Win32
   and the MdiListMenu.
*/
#define VACA_FIRST_MDICHILD 0xff00

/**
   Represents the Win32 class used by MdiChild.
*/
class MdiChildClass : public WidgetClass {
public:
    static WidgetClassName getClassName() { return WidgetClassName(L"Vaca.MdiChild"); }
};

/**
   Controls a child window for a MdiClient. Represent a Frame inside a
   MdiClient.
*/
class VACA_DLL MdiChild : public Register<MdiChildClass>, public Frame {
    void initialize();

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    MdiChild(const String &title, MdiClient *parent, const Style &style = Styles::Default);

    MdiChild(const String &title, MdiFrame *parent, const Style &style = Styles::Default);

    ~MdiChild() override;

protected:
    // Reflected notifications
    // virtual bool onReflectedCommand(int id, int code, LRESULT& lResult);

    bool wndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &lResult) override;
//   virtual LRESULT defWndProc(UINT message, WPARAM wParam, LPARAM lParam);

private:
//   virtual void destroyHWND(HWND hwnd);
};

/**
   Widget which its client area contains MdiChild windows.
*/
class VACA_DLL MdiClient : public Widget {
public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    MdiClient(Widget *parent, const Style &style = Styles::Default);

    ~MdiClient() override;

    void cascade();

    void tileHorizontal();

    void tileVertical();

    void arrangeIcons();

    MdiChild *getActive();

    void activate(MdiChild *mdiChild);

    void activateNext(MdiChild *mdiChild = nullptr);

    void activatePrevious(MdiChild *mdiChild = nullptr);

    MdiChild *getChildById(int wID);

    // void maximize(MdiChild* window);
    // void restore(MdiChild* window);

    bool preTranslateMessage(Message &message) override;

private:
    HWND createHandle(LPCTSTR className, Widget *parent, Style style) override;
};

/**
   Default implementation to use the MDI interface.

   You can use this kind of Frame when you want MDI applications.

   @win32
     It takes care of a MdiClient and uses the @msdn{DefFrameProc} of
     the Windows API as default message handler.
   @endwin32
*/
class VACA_DLL MdiFrame : public Frame {
    friend class Frame;

private:

    MdiClient *m_mdiClient;

public:

    struct VACA_DLL Styles {
        static const Style Default;
    };

    MdiFrame(const String &title, Widget *parent = nullptr, const Style &style = Styles::Default,
             bool customMdiClient = false);

//   MdiFrame(Widget* parent, Style style = ChildStyle); // TODO ChildStyle???
    ~MdiFrame() override;

    MdiClient *getMdiClient();

    MdiClient *setMdiClient(MdiClient *mdiClient);

    MenuBar *setMenuBar(MenuBar *menubar) override;

    void refreshMenuBar();

protected:
    bool wndProc(UINT message, WPARAM wParam, LPARAM lParam, LRESULT &lResult) override;
//   virtual LRESULT defWndProc(UINT message, WPARAM wParam, LPARAM lParam);
};


} // namespace Wg
