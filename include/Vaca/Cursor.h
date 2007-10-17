// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005, 2006, David A. Capello
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in
//   the documentation and/or other materials provided with the
//   distribution.
// * Neither the name of the Vaca nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#ifndef VACA_CURSOR_H
#define VACA_CURSOR_H

#include "Vaca/base.h"
#include "Vaca/Cursor.h"

namespace Vaca {

/**
 * System mouse cursors.
 *
 * @see #Vaca::Cursor::Cursor(SysCursor)
 */
enum SysCursor {
  NoCursor,
  ArrowCursor,
  CrosshairCursor,
  HandCursor,
  HelpCursor,
  TextCursor,
  ForbiddenCursor,
  MoveCursor,
  SizeECursor,
  SizeNCursor,
  SizeNECursor,
  SizeNWCursor,
  SizeSCursor,
  SizeSECursor,
  SizeSWCursor,
  SizeWCursor,
  UpArrowCursor,
  WaitCursor,
  WaitBgCursor
};

class String;

/**
 * A cursor (HCURSOR wrapper).
 */
class VACA_DLL Cursor
{
  HCURSOR m_HCURSOR;
  bool m_autoDelete;

public:

  explicit Cursor(int cursorId);
  explicit Cursor(SysCursor cursor);
  explicit Cursor(const String &fileName);
  Cursor(const Cursor &cursor);
  virtual ~Cursor();

  HCURSOR getHCURSOR();

};

} // namespace Vaca

#endif
