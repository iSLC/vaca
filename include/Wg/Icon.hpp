// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Size.hpp"
#include "Wg/ResourceId.hpp"
#include "Wg/SharedPtr.hpp"
#include "Wg/GdiObject.hpp"

namespace Wg {

/**
   Used to destroy the HICON handle from GdiObject.

   @internal
*/
struct Win32DestroyIcon {
    /**
       @win32
       Calls @msdn{DestroyIcon}.
       @endwin32
    */
    static void destroy(HICON handle) {
        ::DestroyIcon(handle);
    }
};

/**
   A shared pointer to an icon.

   This is a SharedPtr, so if you copy instances of icons they will be
   referencing to the same place. You can't clone icons because you can't
   modify them.

   Example
   @code
   Icon icon1(Size(32, 32));
   Icon icon2 = icon1;       // icon1 and icon2 references to the same Icon
   assert(icon1 == icon2);
   @endcode

   @win32
     This is a @msdn{HICON} wrapper.
   @endwin32
*/
class VACA_DLL Icon : private SharedPtr<GdiObject<HICON, Win32DestroyIcon> > {
public:
    Icon();

    Icon(const Icon &icon);

    explicit Icon(const ResourceId &iconId, const Size &sz = Size(0, 0));

    explicit Icon(const String &fileName, const Size &sz = Size(0, 0));

    explicit Icon(HICON handle);

    ~Icon() override;

    [[nodiscard]] HICON getHandle() const;
};

} // namespace Wg
