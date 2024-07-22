/**
 * \file   TreeItem.cxx
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

#include "TreeItem.h"

TreeItem::TreeItem(const QList<QVariant> &data, TreeItem *parent):
  mActive(false)
{
  parentItem = parent;
  itemData = data;
}

TreeItem::~TreeItem()
{
  qDeleteAll(childItems);
}

void TreeItem::appendChild(TreeItem *item)
{
  childItems.append(item);
}

TreeItem *TreeItem::child(int row)
{
  return childItems.value(row);
}

int TreeItem::childCount() const
{
  return childItems.count();
}

int TreeItem::columnCount() const
{
  return itemData.count();
}

QVariant TreeItem::data(int column) const
{
  return itemData.value(column);
}

TreeItem *TreeItem::parent()
{
  return parentItem;
}

int TreeItem::row() const
{
  if (parentItem) {
    return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));
  }

  return 0;
}

bool TreeItem::active() const
{
  return mActive;
}

void TreeItem::active(bool value)
{
  mActive = value;
}


