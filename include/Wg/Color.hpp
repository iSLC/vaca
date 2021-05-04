// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"

namespace Wg {

/**
   A color that can be used to create pens or brushes.
*/
class VACA_DLL Color {
public:

    static const Color White;
    static const Color LightGray;
    static const Color Gray;
    static const Color DarkGray;
    static const Color Black;
    static const Color Red;
    static const Color Pink;
    static const Color Orange;
    static const Color Yellow;
    static const Color Green;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Blue;

    Color();

    Color(const Color &color);

    Color(int r, int g, int b) noexcept;

    virtual ~Color();

    [[nodiscard]] int getR() const;

    [[nodiscard]] int getG() const;

    [[nodiscard]] int getB() const;

    [[nodiscard]] Color negative() const;

    [[nodiscard]] Color toBlackAndWhite() const;

    Color &operator=(const Color &color);

    bool operator==(const Color &color) const;

    bool operator!=(const Color &color) const;

    Color operator+(const Color &color) const;

    Color operator-(const Color &color) const;

    Color operator*(int value) const;

    Color operator/(int value) const;

    Color operator*(double value) const;

    Color operator/(double value) const;

private:
    unsigned int m_raw;
};

} // namespace Wg
