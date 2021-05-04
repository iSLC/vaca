// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2010 David Capello
//
// This file is distributed under the terms of the MIT license,
// please read LICENSE.txt for more information.

#pragma once

#include "VACA/Base.hpp"
#include "VACA/CancelableEvent.hpp"

namespace vaca {

/**
   Event where interact a TreeView and TreeNode.
*/
class VACA_DLL TreeViewEvent : public CancelableEvent {
    TreeNode *m_treeNode;
    String m_label;

public:

    TreeViewEvent(TreeView *treeView, TreeNode *treeNode, String label = L"");

    ~TreeViewEvent() override;

    TreeView *getTreeView();

    TreeNode *getTreeNode();

    String getLabel();

};

} // namespace vaca
