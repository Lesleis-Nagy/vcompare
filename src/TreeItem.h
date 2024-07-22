/**
 * \file   TreeItem.h
 * \author L. Nagy
 * 
 * MIT License
 *
 * Copyright (c) [2016] Lesleis Nagy
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef TREEITEM_H_
#define TREEITEM_H_

#include <QList>
#include <QStringList>
#include <QVariant>

#include "DebugMacros.h"

class TreeItem
{
public:
  TreeItem(const QList<QVariant> &data, TreeItem *parent = 0);
  ~TreeItem();

  void appendChild(TreeItem *child);

  TreeItem *child(int row);
  int childCount() const;
  int columnCount() const;
  QVariant data(int column) const;
  int row() const;
  TreeItem *parent();

  bool active() const;
  void active(bool value);

private:
  QList<TreeItem*>  childItems;
  QList<QVariant>   itemData;
  bool              mActive;
  TreeItem         *parentItem;
};

#endif  // TREEITEM_H_
