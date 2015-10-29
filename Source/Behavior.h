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
#include <vector>

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
    typedef std::unique_ptr<Behavior> Child;
    typedef std::vector<Child>::iterator Children;

    typedef std::function<void(State)> EnterHook;
    typedef std::function<void(State)> IterateHook;
    typedef std::function<void(State)> ExitHook;

    class Behavior {
    public:
        /**
        Creates a default Behavior with an empty Parent (nullptr)
        and no Child behaviors.
        The Behavior starts in an INVALID State.
        */
        Behavior();
        ~Behavior();

        /**
        Adds a new Child to the end of the list of Children.

        newChild: The Behavior to add to the list of Children.
        If it's a nullptr, then no action is taken.
        */
        virtual void addChild(Behavior* newChild);

        /**
        Sets a new Parent.

        newParent: The Behavior to set as the new Parent.
        If it's a nullptr, then the current Parent is
        removed.
        */
        void setParent(Behavior* newParent);

        /**
        Retrieves the current Parent.
        */
        Parent parent();

        /**
        Retrieves an iterator to the first Child.
        */
        Children children();

        /**
        Retrieves an iterator to the end of the list of Children.
        */
        Children children_end();

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

        /**
        Notifies this Behavior of the given State change.
        */
        virtual void notify(Behavior * who, State newState) = 0;

    protected:
        Parent m_parent;
        std::vector<Child> m_children;

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

    private:

    };
}

#endif