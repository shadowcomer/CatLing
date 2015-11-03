/**
    A Behavior is an abstract class that represents anything that
    can be executed iteratively.

    One of the main properties of a Behavior is that it can
    represent any executable logic that C++ can define, giving
    the ability to construct ordered sets of Behaviors.

    Thus, Behaviors can be interrelated to form Behavior Trees.

    This implementation of a Behavior Tree allows the user to set
    functions to be called when the Behavior is entered, iterated
    over, or exited, thus being able to monitorize it.

    How it works:
    Given a set of implementations of a Behavior, these may be
    joined to each other to compose a Behavior Tree.

    Starting at the root Behavior, the user may then call the
    'iterate' method to do exactly one iteration of the logic of
    the implementation. If its the first time this Behavior has
    been executed, then the configured enter hook function is
    called. Each iteration will call the configured iteration
    hook. If the logic of the Behavior implementation changes
    the state to a 'terminated' state, then the exit hook is
    called.
*/

#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <memory>

#include <functional>

namespace BT {

    enum class State {
        INITIAL,
        RUNNING,
        SUCCESS,
        FAILURE,
        ABORTED,
    };

    class Behavior;

    typedef std::shared_ptr<Behavior> Parent;

    typedef std::function<void(State)> EnterHook;
    typedef std::function<void(State)> IterateHook;
    typedef std::function<void(State)> ExitHook;

    class Behavior {
    public:
        /**
        Creates a default Behavior with an empty Parent (nullptr).
        The Behavior starts in an INVALID State.
        */
        Behavior();
        ~Behavior();

        /**
        Sets a new Parent.

        newParent: The Behavior to set as the new Parent.
        If it's a nullptr, then the current Parent is
        removed.
        */
        void setParent(Parent const & newParent);

        /**
        Retrieves the current Parent.
        */
        Parent parent();

        /**
        Retrieves the current state.
        */
        State state() const;

        /**
        Checks whether this Behavior is in the given State.
        */
        bool is(State state) const;

        // ----------
        // Hooks
        // ----------
        // Hook configuration methods

        /**
        Sets the Behavior enter hook.
        */
        void setEnterHook(EnterHook fn);

        /**
        Sets the Behavior iteration hook.
        */
        void setIterationHook(IterateHook fn);

        /**
        Sets the Behavior end hook.
        */
        void setExitHook(ExitHook fn);

        // ----------
        // End hooks
        // ----------

        /**
        Executes exactly one iteration.
        */
        State iterate();

        /**
        Number of iterations attempted.
        */
        int iterations() const;

        /**
        Checks for termination, which is all states that can't be
        iterated over.
        */
        bool terminated() const;

    protected:
        Parent m_parent;

        int m_iterations; // Number of iterations attempted.

        State m_state; // The current state

        // ----------
        // Hooks
        // ----------
        // Hooks to call on different events
        EnterHook m_enterHook;
        IterateHook m_iterationHook;
        ExitHook m_exitHook;

        // Calls to the currently set hooks
        /** Calls the enter hook */
        void callEnterHook();

        /** Calls the iteration hook */
        void callIterationHook();

        /** Calls the exit hook */
        void callExitHook();
        // ----------
        // End hooks
        // ----------

        /**
        This function is called by the
        public interface, 'iterate', to implement the
        child's part without removing Behavior's implementation,
        and as such must be overriden.
        */
        virtual State doIterate() = 0;

        /**
        Notifies the parent Behavior to execute its
        'treatNotification' function.
        */
        void notifyParent(State oldState);

    private:

        /**
        Implementation of what to do when the Behavior
        implementation has been notified through 'notifyParent'.
        */
        virtual void treatNotification(Behavior const * const who,
            State oldState) = 0;
    };
}

#endif