// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#include <utility>

#include "VACA/TreeViewEvent.hpp"
#include "VACA/TreeView.hpp"

using namespace Wg;

TreeViewEvent::TreeViewEvent(TreeView* treeView, TreeNode* treeNode, String label)
  : CancelableEvent(treeView)
  , m_treeNode(treeNode)
  , m_label(std::move(label))
{
}

TreeViewEvent::~TreeViewEvent()
= default;

TreeNode* TreeViewEvent::getTreeNode()
{
  return m_treeNode;
}

String TreeViewEvent::getLabel()
{
  return m_label;
}

