// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/SharedPtr.hpp"

namespace Wg {

/**
   A brush can be used to fill rectangles, ellipses, and paths.

   This is a SharedPtr, so if you copy instances of brushes they will be
   referencing to the same place. You can't clone brushes because you can't
   modify them.

   @win32
     This is a @msdn{HBRUSH} wrapper.
   @endwin32

   @see Graphics, Graphics#fillRect, Graphics#fillPath
*/
class VACA_DLL Brush {
    template<typename To, typename From>
    friend To convert_to(const From &from);

public:
    Brush();

    Brush(const Brush &brush);

    explicit Brush(const Color &color);

    virtual ~Brush();

    Brush &operator=(const Brush &brush);

    [[nodiscard]] Color getColor() const;

private:
    class BrushImpl;

    SharedPtr<BrushImpl> m_impl;
};

} // namespace Wg
