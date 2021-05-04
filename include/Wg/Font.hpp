// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/SharedPtr.hpp"
#include "Wg/GdiObject.hpp"

namespace Wg {

// ======================================================================

/**
   @see FontStyle
*/
struct FontStyleEnumSet {
    enum enumeration {
        Regular = 0,
        Bold = 1,
        Italic = 2,
        Underline = 4,
        Strikeout = 8,
    };
};

/**
   A FontStyle can be one of the following values:
   @li FontStyle::Regular
   @li FontStyle::Bold
   @li FontStyle::Italic
   @li FontStyle::Underline
   @li FontStyle::Strikeout
*/
typedef EnumSet<FontStyleEnumSet> FontStyle;

// ======================================================================

/**
   Dimensions of a Font.

   @see Graphics#getFontMetrics
*/
class VACA_DLL FontMetrics {
    friend class Graphics;

    TEXTMETRIC m_textMetric{};

public:
    FontMetrics() = default;

    FontMetrics(const FontMetrics &fm) = default;

    [[nodiscard]] int getHeight() const { return m_textMetric.tmHeight; }

    [[nodiscard]] int getAscent() const { return m_textMetric.tmAscent; }

    [[nodiscard]] int getDescent() const { return m_textMetric.tmDescent; }

    [[nodiscard]] int getAverageCharWidth() const { return m_textMetric.tmAveCharWidth; }

    [[nodiscard]] int getMaximumCharWidth() const { return m_textMetric.tmMaxCharWidth; }

    [[nodiscard]] int getLeading() const { return m_textMetric.tmInternalLeading; }
};

/**
   A shared pointer to a font that can be used in Graphics or Widget to
   draw text.

   This is a SharedPtr, so if you copy instances of fonts they will be
   referencing to the same place. You can't clone fonts because you
   can't modify them.

   @win32
     This is a @msdn{HFONT} wrapper.
   @endwin32

   @see Graphics#setFont, Graphics#drawString
*/
class VACA_DLL Font : private SharedPtr<GdiObject<HFONT> > {
    friend class Application;

public:

    Font();

    Font(const Font &font);

    Font(const Font &font, FontStyle style);

    Font(const String &familyName, int size, FontStyle style = FontStyle::Regular);

    explicit Font(HFONT hfont);

    Font(LPLOGFONT lplf);

    ~Font() override;

    [[nodiscard]] int getPointSize() const;

    [[nodiscard]] FontStyle getStyle() const;

    Font &operator=(const Font &font);

    [[nodiscard]] HFONT getHandle() const;

    bool getLogFont(LPLOGFONT lplf) const;

private:
    void assign(LPLOGFONT lplf);
};

} // namespace Wg
