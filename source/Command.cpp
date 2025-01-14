// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include "Wg/Command.hpp"

using namespace Wg;

// ======================================================================
// Command

Command::Command(CommandId id)
  : m_id(id)
  // , m_invoker(NULL)
{
}

Command::~Command()
= default;

// ======================================================================
// CommandsClient

CommandsClient::CommandsClient()
= default;

CommandsClient::~CommandsClient()
{
  std::vector<Command*>::iterator it;
  for (it = m_commands.begin(); it != m_commands.end(); ++it)
    delete *it;
}

/**
   Adds a command.

   @warning The cmd will be deleted automatically.

   @see @ref page_tn_010
*/
void CommandsClient::addCommand(Command* cmd)
{
  m_commands.push_back(cmd);
}

Command* CommandsClient::removeCommand(Command* cmd)
{
  remove_from_container(m_commands, cmd);
  return cmd;
}

Command* CommandsClient::getCommandById(CommandId id) const
{
  for (auto cmd : m_commands) {
    if (cmd->getId() == id)
      return cmd;
  }
  return nullptr;
}
