// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/GdiObject.hpp"
#include "Wg/SharedPtr.hpp"

namespace Wg {

/**
   A region, it can be simple as a rectangle, complex as any shape,
   but also can be empty.
*/
class VACA_DLL Region : private SharedPtr<GdiObject<HRGN> > {
public:

    Region();

    Region(const Region &rgn);

    explicit Region(HRGN hrgn);

    explicit Region(const Rect &rc);

    ~Region() override;

    [[nodiscard]] bool isEmpty() const;

    [[nodiscard]] bool isSimple() const;

    [[nodiscard]] bool isComplex() const;

    Region &operator=(const Region &rgn);

    [[nodiscard]] Region clone() const;

    [[nodiscard]] Rect getBounds() const;

    Region &offset(int dx, int dy);

    Region &offset(const Point &point);

    [[nodiscard]] bool contains(const Point &pt) const;

    [[nodiscard]] bool contains(const Rect &rc) const;

    bool operator==(const Region &rgn) const;

    bool operator!=(const Region &rgn) const;

    Region operator|(const Region &rgn) const;

    Region operator+(const Region &rgn) const;

    Region operator&(const Region &rgn) const;

    Region operator-(const Region &rgn) const;

    Region operator^(const Region &rgn) const;

    Region &operator|=(const Region &rgn);

    Region &operator+=(const Region &rgn);

    Region &operator&=(const Region &rgn);

    Region &operator-=(const Region &rgn);

    Region &operator^=(const Region &rgn);

    static Region fromRect(const Rect &rc);

    static Region fromEllipse(const Rect &rc);

    static Region fromRoundRect(const Rect &rc, const Size &ellipseSize);

    [[nodiscard]] HRGN getHandle() const;

};

} // namespace Wg
