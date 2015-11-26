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
        Behavior(Behavior * parent, BehaviorMonitor monitor);
        virtual ~Behavior();

        Behavior() = delete;

        State currentState() const;
        virtual Behavior * nextBehavior() = 0;
        virtual void tick() = 0;

    private:
        Behavior * const m_parentBehavior;
        BehaviorMonitor m_monitor;
        State m_currentState;
    };
}

#endif