/**
 * \file   TreeModel.h 
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

#ifndef TREE_MODEL_H_
#define TREE_MODEL_H_

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <QtGui>

#include "DirectoryDatabase.h"

class TreeItem;

class TreeModel : public QAbstractItemModel
{
  Q_OBJECT

  public:
    TreeModel(const DirectoryDatabase & db, QObject *parent = 0);
    TreeModel(QObject *parent = 0);

    ~TreeModel();

    QVariant data(const QModelIndex &index, int role) const;

    QList<QVariant> pathToParent(const QModelIndex & index) const;

    void active(const QModelIndex & index, bool value);

    Qt::ItemFlags flags(const QModelIndex &index) const;

    QVariant headerData(
        int section, Qt::Orientation orientation,
        int role = Qt::DisplayRole) const;

    QModelIndex index(
        int row, int column,
        const QModelIndex &parent = QModelIndex()) const;

    QModelIndex parent(const QModelIndex &index) const;

    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    int columnCount(const QModelIndex &parent = QModelIndex()) const;

  private:
    void setupModelData(const DirectoryDatabase & db, TreeItem *parent);

    TreeItem * rootItem;
};

#endif  // TREE_MODEL_H_
