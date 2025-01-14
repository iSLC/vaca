// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/Point.hpp"

#include <vector>

namespace Wg {

/**
   Set of nodes to draw polygons and shapes in Graphics.
*/
class VACA_DLL GraphicsPath {
public:
    // values that are acceptable for Node#m_flags
    enum {
        MoveTo = 1,
        LineTo = 2,
        BezierControl1 = 3,        // first bezier's control point
        BezierControl2 = 4,        // second bezier's control point
        BezierTo = 5,        // bezier's end point
        TypeMask = 7,
        // flags
        CloseFigure = 8,
    };

    /**
       A node in a GraphicsPath.

       A node is an action to draw the GraphicsPath, it can be a node of
       movement (MoveTo), a node to draw a line (LineTo), or a curve
       (BezierTo).

       The nodes of movement don't draw lines, they just change the
       current position which we have to start to draw. For example,
       a GraphicsPath that represent just one line, has really two nodes:
       @li a node of movement (MoveTo) to go to the line's start position, and
       @li a node to draw a line (LineTo) that represent the end position of the line.
    */
    class VACA_DLL Node {
        friend class GraphicsPath;

        int m_flags;
        Point m_point;
    public:
        Node(int type, const Point &point);

        [[nodiscard]] int getType() const;

        [[nodiscard]] bool isCloseFigure() const;

        void setCloseFigure(bool closeFigure);

        Point &getPoint();

        [[nodiscard]] const Point &getPoint() const;
    };

private:
    std::vector<Node> m_nodes;

public:
    typedef std::vector<Node>::iterator iterator;
    typedef std::vector<Node>::const_iterator const_iterator;

    GraphicsPath();

    virtual ~GraphicsPath();

    iterator begin();

    iterator end();

    [[nodiscard]] const_iterator begin() const;

    [[nodiscard]] const_iterator end() const;

    void clear();

    [[nodiscard]] bool empty() const;

    [[nodiscard]] unsigned size() const;

    GraphicsPath &offset(int dx, int dy);

    GraphicsPath &offset(const Point &point);

    GraphicsPath &moveTo(const Point &pt);

    GraphicsPath &moveTo(int x, int y);

    GraphicsPath &lineTo(const Point &pt);

    GraphicsPath &lineTo(int x, int y);

    GraphicsPath &curveTo(const Point &pt1, const Point &pt2, const Point &pt3);

    GraphicsPath &curveTo(int x1, int y1, int x2, int y2, int x3, int y3);

    GraphicsPath &closeFigure();

    GraphicsPath &flatten();

    GraphicsPath &widen(const Pen &pen);

    [[nodiscard]] Region toRegion() const;

private:
    void addNode(int type, const Point &pt);

};

} // namespace Wg
