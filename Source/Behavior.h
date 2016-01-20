#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include <memory>

#include <functional>

namespace bt {

    enum class State {
        INVALID,
        RUNNING,
        SUCCESS,
        FAILURE,
        ABORTED,
    };

    class Behavior;

    typedef std::function<void(Behavior *)> BehaviorMonitor;

    class Behavior {
    public:
/*
A Behavior that does not have a parent is a root Behavior.
If monitorization is not required, the given BehaviorMonitor
should have an empty implementation.
*/
        Behavior(Behavior * parent, BehaviorMonitor monitor);
        virtual ~Behavior();

/*
A default constructor does not give any value to the constructed
Behavior.
*/
        Behavior() = delete;

        State currentState() const;

/*
Interface for iteration over Behaviors. Each implementation of a
Behavior must determine which Behavior is next.
*/
        virtual Behavior * nextBehavior() = 0;

/*
Interface for invocation of a single iteration over this Behavior.
*/
        virtual void tick() = 0;

/**
Set the current parent behavior. If nullptr is given, the parent
is cleared.
*/
        void setParent(Behavior* newParent);

/**
Set the current monitor.
*/
        void setMonitor(BehaviorMonitor newMonitor);

    protected:
        Behavior * m_parentBehavior;
        BehaviorMonitor m_monitor;
        State m_currentState;
    };
}

#endif