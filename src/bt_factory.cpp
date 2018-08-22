/* Copyright (C) 2018 Michele Colledanchise -  All Rights Reserved
 * Copyright (C) 2018 Davide Faconti -  All Rights Reserved
*
*   Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"),
*   to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
*   and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
*   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
*   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
*   WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "behavior_tree_core/bt_factory.h"

namespace BT
{
BehaviorTreeFactory::BehaviorTreeFactory()
{
    registerNodeType<FallbackNode>("Fallback");
    registerNodeType<FallbackNodeWithMemory>("FallbackStar");
    registerNodeType<SequenceNode>("Sequence");
    registerNodeType<SequenceNodeWithMemory>("SequenceStar");

    registerNodeType<DecoratorNegationNode>("Negation");
    registerNodeType<DecoratorRetryNode>("RetryUntilSuccesful");
    registerNodeType<DecoratorRepeatNode>("Repeat");

    registerNodeType<DecoratorSubtreeNode>("SubTree");
}

bool BehaviorTreeFactory::unregisterBuilder(const std::string& ID)
{
    auto it = builders_.find(ID);
    if (it == builders_.end())
    {
        return false;
    }
    builders_.erase(ID);
    return true;
}

void BehaviorTreeFactory::registerBuilder(const std::string& ID, NodeBuilder builder)
{
    auto it = builders_.find(ID);
    if (it != builders_.end())
    {
        throw BehaviorTreeException("ID '" + ID + "' already registered");
    }

    builders_.insert(std::make_pair(ID, builder));
}

void BehaviorTreeFactory::registerSimpleCondition(const std::string &ID,
                                                  const std::function<NodeStatus()> &tick_functor)
{
    auto wrapper = [tick_functor](const Blackboard::Ptr&){ return tick_functor(); };
    registerSimpleCondition(ID, wrapper);
}

void BehaviorTreeFactory::registerSimpleAction(const std::string& ID,
                                               const std::function<NodeStatus()> &tick_functor)
{
    auto wrapper = [tick_functor](const Blackboard::Ptr&){ return tick_functor(); };
    registerSimpleAction(ID, wrapper);
}

void BehaviorTreeFactory::registerSimpleDecorator(const std::string& ID,
                                                  const std::function<NodeStatus(NodeStatus)> &tick_functor)
{
    auto wrapper = [tick_functor](NodeStatus status, const Blackboard::Ptr&){ return tick_functor(status); };
    registerSimpleDecorator(ID, wrapper);
}

void BehaviorTreeFactory::registerSimpleCondition(const std::string &ID,
                                                  const SimpleConditionNode::TickFunctor &tick_functor)
{
    NodeBuilder builder = [tick_functor, ID](const std::string& name, const NodeParameters&) {
        return std::unique_ptr<TreeNode>(new SimpleConditionNode(name, tick_functor));
    };

    registerBuilder(ID, builder);
    storeNodeModel<SimpleConditionNode>(ID);
}

void BehaviorTreeFactory::registerSimpleAction(const std::string& ID,
                                               const SimpleActionNode::TickFunctor& tick_functor)
{
    NodeBuilder builder = [tick_functor, ID](const std::string& name, const NodeParameters&) {
        return std::unique_ptr<TreeNode>(new SimpleActionNode(name, tick_functor));
    };

    registerBuilder(ID, builder);
    storeNodeModel<SimpleActionNode>(ID);
}

void BehaviorTreeFactory::registerSimpleDecorator(const std::string &ID,
                                                  const SimpleDecoratorNode::TickFunctor &tick_functor)
{
    NodeBuilder builder = [tick_functor, ID](const std::string& name, const NodeParameters&) {
        return std::unique_ptr<TreeNode>(new SimpleDecoratorNode(name, tick_functor));
    };

    registerBuilder(ID, builder);
    storeNodeModel<SimpleDecoratorNode>(ID);
}

std::unique_ptr<TreeNode> BehaviorTreeFactory::instantiateTreeNode(const std::string& ID, const std::string& name,
                                                                   const NodeParameters& params) const
{
    auto it = builders_.find(ID);
    if (it == builders_.end())
    {
        throw std::invalid_argument("ID '" + ID + "' not registered");
    }
    std::unique_ptr<TreeNode> node = it->second(name, params);
    node->setRegistrationName(ID);
    return node;
}

void BehaviorTreeFactory::sortTreeNodeModel()
{
    std::sort( treenode_models_.begin(), treenode_models_.end(),
               [](const TreeNodeModel& a, const TreeNodeModel& b)
    {
        int comp = std::strcmp( toStr( a.type ), toStr( b.type ));
        if( comp == 0)
        {
            return a.registration_ID < b.registration_ID;
        }
        return comp < 0;
    });
}

}   // end namespace