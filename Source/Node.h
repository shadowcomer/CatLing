#ifndef NODE_H
#define NODE_H

#include <memory>
#include <vector>

namespace BT {

    class Node;

    typedef std::shared_ptr<Node> Parent;
    typedef std::unique_ptr<Node> Child;
    typedef std::vector<Child>::iterator Children;

    class Node {
    public:
        Node();

        ~Node();

        virtual void addChild(Node* newChild);
        void setParent(Node* newParent);
        Parent parent();
        Children children();
        Children children_end();

    private:
        Parent m_parent;
        std::vector<Child> m_children;

    };
}

#endif