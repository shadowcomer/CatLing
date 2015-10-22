#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>

namespace BT {

    enum class State {
        INVALID,
        RUNNING,
        SUCCESS,
        FAILED,
        ABORTED,
    };

    class Node;

    typedef std::shared_ptr<Node> Parent;
    typedef std::unique_ptr<Node> Child;
    typedef std::vector<Child>::iterator Children;

    class Node {
    public:
        /**
        Creates a default Node with an empty Parent (nullptr)
        and no Child nodes.
        */
        Node();
        ~Node();

        /**
        Adds a new Child to the end of the list of Children.

        newChild: The Node to add to the list of Children.
        If it's a nullptr, then no action is taken.
        */
        virtual void addChild(Node* newChild);

        /**
        Sets a new Parent.

        newParent: The Node to set as the new Parent.
        If it's a nullptr, then the current Parent is
        removed.
        */
        void setParent(Node* newParent);

        /**
        Retrieves the current Parent of this Node.
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

    protected:
        Parent m_parent;
        std::vector<Child> m_children;

    private:

    };
}

#endif