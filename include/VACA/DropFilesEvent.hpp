// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/Event.hpp"

#include <vector>

namespace vaca {

class VACA_DLL DropFilesEvent : public Event
{
  std::vector<String>& m_files;

public:

  DropFilesEvent(Widget* source, std::vector<String>& files);
  ~DropFilesEvent() override;

  std::vector<String> getFiles();

};

} // namespace vaca
