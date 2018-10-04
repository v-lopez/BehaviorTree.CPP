/* Copyright (C) 2015-2018 Michele Colledanchise -  All Rights Reserved
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

#ifndef FALLBACKNODE_H
#define FALLBACKNODE_H

#include "behavior_tree_core/control_node.h"

namespace BT
{

/**
 * @brief The FallbackNode is used when you want to evaluate different "strategies".
 *
 * This control node ticks its children UNTIL one of them returns SUCCESS.
 *
 * In that case it return SUCCESS, otherwise it return FAILURE.
 * If it finds a child returning RUNNING, it returns RUNNING and it restarts from the first child.
 *
 * If you don't want to restart from the first child, use FallbackNodeWithMemory instead.
 *
 * Example: three children, A, B and C
 *
 * 1) A returns FAILURE. Continue.
 * 2) B returns RUNNING, Restart from A
 * 3) A returns FAILURE again but B return SUCCESS. Stop and return SUCCESS.
 *
 */
class FallbackNode : public ControlNode
{
  public:
    FallbackNode(const std::string& name);

    virtual ~FallbackNode() override = default;

  private:
    virtual BT::NodeStatus tick() override;
};
}

#endif