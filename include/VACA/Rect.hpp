// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"

namespace vaca {

/**
   A rectangle.
*/
class VACA_DLL Rect {
public:

    /**
       Horizontal position of the rectangle's upper-left corner (increase from left to right).

       @c x=0 means the beginning of the left side in the screen or
       @link Widget#getClientBounds Widget's client area@endlink.
    */
    int x;

    /**
       Vertical position of the rectangle's upper-left corner (increase from top to bottom).

       @c y=0 means the beginning of the top side in the screen or
       @link Widget#getClientBounds Widget's client area@endlink.
    */
    int y;

    /**
       Width of the rectangle (increase from left to right).

       @c w<1 means an empty rectangle.
    */
    int w;

    /**
       Height of the rectangle (increase from left to right).

       @c h<1 means an empty rectangle.
    */
    int h;

    Rect();

    Rect(int w, int h);

    explicit Rect(const Size &size);

    Rect(const Rect &rect);

    Rect(const Point &point, const Size &size);

    Rect(const Point &point1, const Point &point2);

    Rect(int x, int y, int w, int h);

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] Point getCenter() const;

    [[nodiscard]] Point getOrigin() const;

    [[nodiscard]] Point getPoint2() const;

    [[nodiscard]] Size getSize() const;

    Rect &setOrigin(const Point &pt);

    Rect &setSize(const Size &sz);

    Rect &offset(int dx, int dy);

    Rect &offset(const Point &point);

    Rect &inflate(int dw, int dh);

    Rect &inflate(const Size &size);

    Rect &enlarge(int unit);

    Rect &shrink(int unit);

    [[nodiscard]] bool contains(const Point &pt) const;

    [[nodiscard]] bool contains(const Rect &rc) const;

    [[nodiscard]] bool intersects(const Rect &rc) const;

    [[nodiscard]] Rect createUnion(const Rect &rc) const;

    [[nodiscard]] Rect createIntersect(const Rect &rc) const;

    bool operator==(const Rect &rc) const;

    bool operator!=(const Rect &rc) const;

};

} // namespace vaca
