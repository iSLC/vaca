// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/FindFiles.hpp"
#include "Wg/String.hpp"

using namespace Wg;

FindFiles::FindFiles(const String& pattern)
{
  m_pattern = pattern;
  m_handle = nullptr;
}

FindFiles::~FindFiles()
{
  if (m_handle)
    FindClose(m_handle);
 }

bool FindFiles::next()
{
  if (!m_handle) {
    m_handle = FindFirstFile(m_pattern.c_str(), &m_data);
    return m_handle != nullptr;
  }
  else {
    return FindNextFile(m_handle, &m_data) ? true: false; 
  }
}

String FindFiles::getFileName() const
{
    return m_data.cFileName;
}

String FindFiles::getFullFileName() const
{
  return file_path(m_pattern) / m_data.cFileName;
}

bool FindFiles::isFile() const
{
    return (m_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool FindFiles::isDirectory() const
{
  return (m_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
}
