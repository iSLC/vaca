// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "Wg/Base.hpp"
#include "Wg/NonCopyable.hpp"
#include "Wg/Signal.hpp"

#include <vector>

namespace Wg {

/**
   A command, action or function of the application.

   @see CommandId
*/
class VACA_DLL Command : private NonCopyable {
    CommandId m_id;

public:
    Command(CommandId id);

    virtual ~Command();

    [[nodiscard]] CommandId getId() const { return m_id; }

    virtual void execute() = 0;

    virtual bool isEnabled() { return true; }

    virtual bool isChecked() { return false; }
};

/**
   Specialization of Command class to handle the
   Command#execute and Command#isEnabled
   as @link page_mess_signals signals@endlink.
*/
class SignalCommand : public Command {
public:

    /**
       Creates a new SignalCommand without slots in signals.

       \param id
         Command ID that will be handled by this SignalCommand.
     */
    SignalCommand(CommandId id) : Command(id) {}

    /**
       Creates a new SignalCommand with the specified functor .

       \param id
         Command ID that will be handled by this SignalCommand.

       \param f
         Functor to be called when the command is executed.
     */
    template<typename F>
    SignalCommand(CommandId id, const F &f) : Command(id) {
        Execute.connect(f);
    }

    /**
       Creates a new SignalCommand with the specified functor .

       \param id
         Command ID that will be handled by this SignalCommand.

       \param m
         Member function of the T class to be called when the command is executed.

       \param t
         Instance of T class to be used when the member function is called.
     */
    template<class T>
    SignalCommand(CommandId id, void (T::*m)(), T *t) : Command(id) {
        Execute.connect(m, t);
    }

    ~SignalCommand() override = default;

    void execute() override { Execute(); }

    bool isEnabled() override {
        return Enabled(true);    // true by default
    }

    bool isChecked() override {
        return Checked(false);    // false by default
    }

    Signal0<void> Execute;
    Signal0<bool> Enabled;
    Signal0<bool> Checked;
};

/**
   An object that contains commands (Application).
*/
class VACA_DLL CommandsClient {
    std::vector<Command *> m_commands;

public:
    CommandsClient();

    virtual ~CommandsClient();

    void addCommand(Command *cmd);

    Command *removeCommand(Command *cmd);

    [[nodiscard]] Command *getCommandById(CommandId id) const;
};

} // namespace Wg
