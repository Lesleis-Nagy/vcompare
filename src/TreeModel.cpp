/**
 * \file   TreeModel.cxx 
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
#include "TreeModel.h"

TreeModel::TreeModel(const DirectoryDatabase & db, QObject *parent)
  : QAbstractItemModel(parent)
{
  QList<QVariant> rootData;
  rootData << "Materials";
  rootItem = new TreeItem(rootData);
  setupModelData(db, rootItem);
}

TreeModel::TreeModel(QObject *parent)
  : QAbstractItemModel(parent)
{
  QList<QVariant> rootData;
  rootData << "Materials";
  rootItem = new TreeItem(rootData);
}

TreeModel::~TreeModel()
{
  delete rootItem;
}

int TreeModel::columnCount(const QModelIndex &parent) const
{
  if (parent.isValid())
    return static_cast<TreeItem*>(parent.internalPointer())->columnCount();
  else
    return rootItem->columnCount();
}

QVariant TreeModel::data(const QModelIndex &index, int role) const
{
  if (index.isValid() && role == Qt::ForegroundRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    if (item->active()) {
      return QVariant(QColor(Qt::green));
    } else {
      return QVariant(QColor(Qt::black));
    }
  }

  if (index.isValid() && role == Qt::DisplayRole) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    return item->data(index.column());
  }

  return QVariant();
}

QList<QVariant> TreeModel::pathToParent(const QModelIndex & index) const
{
  QList<QVariant> output;
  if (!index.isValid()) {
    return output;
  }

  TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
  while (item != 0) {
    output.append(item->data(index.column()));
    item = item->parent();
  }

  return output;
}

void TreeModel::active(const QModelIndex & index, bool value)
{
  if (index.isValid()) {
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
    item->active(value);
  }
}

Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
    int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    return rootItem->data(section);
  }

  return QVariant();
}

QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent)
  const
{
  if (!hasIndex(row, column, parent))
    return QModelIndex();

  TreeItem *parentItem;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  TreeItem *childItem = parentItem->child(row);
  if (childItem)
    return createIndex(row, column, childItem);
  else
    return QModelIndex();
}

QModelIndex TreeModel::parent(const QModelIndex &index) const
{
  if (!index.isValid())
    return QModelIndex();

  TreeItem *childItem = static_cast<TreeItem*>(index.internalPointer());
  TreeItem *parentItem = childItem->parent();

  if (parentItem == rootItem)
    return QModelIndex();

  return createIndex(parentItem->row(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex &parent) const
{
  TreeItem *parentItem;
  if (parent.column() > 0)
    return 0;

  if (!parent.isValid())
    parentItem = rootItem;
  else
    parentItem = static_cast<TreeItem*>(parent.internalPointer());

  return parentItem->childCount();
}

void TreeModel::setupModelData(const DirectoryDatabase & db, TreeItem *parent)
{
  // Materials.
  for (auto mat : db.materialList()) {
    QList<QVariant> material;
    material << mat;
    
    TreeItem *pMatItem = new TreeItem(material, parent);
    parent->appendChild(pMatItem);

    for (auto geom : db.geometryList(mat)) {
      QList<QVariant> geometry;
      geometry << geom;

      TreeItem *pGeomItem = new TreeItem(geometry, pMatItem);
      pMatItem->appendChild(pGeomItem);

      for (auto sz : db.sizeList(mat, geom)) {
        QList<QVariant> size;
        size << sz;

        TreeItem *pSizeItem = new TreeItem(size, pGeomItem);
        pGeomItem->appendChild(pSizeItem);

        for (auto temp : db.temperatureList(mat, geom, sz)) {
          QList<QVariant> temperature;
          temperature << temp;

          TreeItem *pTemperatureItem = new TreeItem(temperature, pSizeItem);
          pSizeItem->appendChild(pTemperatureItem);

          //if (db.hasPathStartEndPair(mat, geom, sz, temp)) {
          //  pTemperatureItem->hasStartEndTrue();
          //}
        }
      }
    }
  }

  /*
  while (number < lines.count()) {
    int position = 0;
    while (position < lines[number].length()) {
      if (lines[number].mid(position, 1) != " ")
        break;
      position++;
    }

    QString lineData = lines[number].mid(position).trimmed();

    if (!lineData.isEmpty()) {
      // Read the column data from the rest of the line.
      QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
      QList<QVariant> columnData;
      for (int column = 0; column < columnStrings.count(); ++column)
        columnData << columnStrings[column];

      if (position > indentations.last()) {
        // The last child of the current parent is now the new parent
        // unless the current parent has no children.

        if (parents.last()->childCount() > 0) {
          parents << parents.last()->child(parents.last()->childCount()-1);
          indentations << position;
        }
      } else {
        while (position < indentations.last() && parents.count() > 0) {
          parents.pop_back();
          indentations.pop_back();
        }
      }

      // Append a new item to the current parent's list of children.
      parents.last()->appendChild(new TreeItem(columnData, parents.last()));
    }

    number++;
  }
  */
}
