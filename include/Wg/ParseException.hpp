// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Exception.hpp"

namespace Wg {

/**
   This exception is thrown when a routine that parse a string has
   failed due ill-formed input.

   @see Bix#parse
*/
class ParseException : public Exception {
    int m_line;
    int m_column;
    int m_index;
public:

    /**
       Creates a ParserException without more information.
    */
    ParseException() noexcept
            : Exception(), m_line(-1), m_column(-1), m_index(-1) {
    }

    /**
       Creates a ParserException with full information: error message,
       line and column number where the error was found inside the
       parsed-string.

       @param message Error message.
       @param line Line number (1 should be the first line).
       @param column Column number (0 should be the first column).
       @param index Index of the character where the error was found (starting in 0).
    */
    ParseException(const String &message, int line = -1, int column = -1, int index = -1) noexcept
            : Exception(message), m_line(line), m_column(column), m_index(index) {
    }

    /**
       Destroys the exception.
    */
    ~ParseException() noexcept override
    = default;

    /**
       Returns the line number where the error was found.

       @warning 1 should be the first line.

       @see #getColumn
    */
    [[nodiscard]] int getLine() const {
        return m_line;
    }

    /**
       Returns the column number where the error was found.

       @warning 0 should be the first column.

       @see #getLine
    */
    [[nodiscard]] int getColumn() const {
        return m_column;
    }

    /**
       Returns the index of the character within the string where the
       error was found.

       @warning 0 should be the first character.
    */
    [[nodiscard]] int getIndex() const {
        return m_index;
    }

};

} // namespace Wg
