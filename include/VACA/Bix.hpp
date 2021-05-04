// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Layout.hpp"

namespace vaca {

enum {
    // flags: type
    BixTypeMask = 3,
    BixRow = 1,            // row vector (horizontal, x axis)
    BixCol = 2,            // column vector (vertical, y axis)
    BixMat = 3,            // matrix (both axis)
    // flags: modifiers
    BixEvenX = 4,
    BixEvenY = 8,
    BixEven = BixEvenX | BixEvenY,
    BixFillX = 16,
    BixFillY = 32,
    BixFill = BixFillX | BixFillY,
};

/**
   A Bix is a <em>layout manager</em> which allows you to arrange widgets
   in a matricial way or horizontally/vertically (like a row/column vector
   respectively).

   All matrices and vectors can be inner one inside the other.
*/
class VACA_DLL Bix : public Layout {

    struct Element;
    struct SubBixElement;
    struct WidgetElement;
    struct Matrix;

    typedef std::vector<Element *> Elements;

    int m_flags;
    int m_cols;
    int m_border;
    int m_childSpacing;
    Elements m_elements;

public:

    Bix(int flags, int matrixColumns = 0);

    ~Bix() override;

    [[nodiscard]] bool isRow() const;

    [[nodiscard]] bool isCol() const;

    [[nodiscard]] bool isMat() const;

    [[nodiscard]] bool isEvenX() const;

    [[nodiscard]] bool isEvenY() const;

    [[nodiscard]] bool isEven() const;

    [[nodiscard]] bool isFillX() const;

    [[nodiscard]] bool isFillY() const;

    [[nodiscard]] bool isFill() const;

    [[nodiscard]] int getBorder() const;

    void setBorder(int border);

    [[nodiscard]] int getChildSpacing() const;

    void setChildSpacing(int childSpacing);

    [[nodiscard]] int getMatrixColumns() const;

    void setMatrixColumns(int matrixColumns);

    Bix *add(int flags, int matrixColumns = 0);

    void add(Widget *child, int flags = 0);

    void remove(Bix *subbix);

    void remove(Widget *child);

    Size getPreferredSize(Widget *parent, WidgetList &widgets, const Size &fitIn) override;

    static Bix *parse(const Char *fmt, ...);

protected:

    void layout(Widget *parent, WidgetList &widgets, const Rect &rc) override;

private:

    Size getPreferredSize(const Size &fitIn);

    Size getPreferredSize(Matrix &mat) const;

    void layout(WidgetsMovement &movement, Bix *parentBix, const Rect &rc);

    Size getMatrixDimension();

    void fillMatrix(Matrix &mat);

};

} // namespace vaca
