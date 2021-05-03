// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/NonCopyable.hpp"

namespace vaca {

class VACA_DLL FindFiles : public NonCopyable
{
  String m_pattern;
  HANDLE m_handle;
  WIN32_FIND_DATA m_data{};

public:

  FindFiles(const String& pattern);
  ~FindFiles();

  bool next();

  [[nodiscard]] String getFileName() const;
  [[nodiscard]] String getFullFileName() const;
  [[nodiscard]] bool isFile() const;
  [[nodiscard]] bool isDirectory() const;

};

} // namespace vaca
