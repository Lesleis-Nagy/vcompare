/**
 * \file   VCompare.h
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
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 **/

#ifndef UI_VCOMPARE_H_
#define UI_VCOMPARE_H_

#include <memory>
#include <algorithm>
#include <utility>

#include <vtkActor.h>
#include <vtkArrayCalculator.h>
#include <vtkArrowSource.h>
#include <vtkAxesActor.h>
#include <vtkCamera.h>
#include <vtkCellData.h>
#include <vtkColorTransferFunction.h>
#include <vtkCubeSource.h>
#include <vtkDataObjectToTable.h>
#include <vtkDataSetMapper.h>
#include <vtkDoubleArray.h>
#include <vtkElevationFilter.h>
#include <vtkFloatArray.h>
#include <vtkGlyph3D.h>
#include <vtkGradientFilter.h>
#include <vtkLookupTable.h>
#include <vtkMaskPoints.h>
#include <vtkOrientationMarkerWidget.h>
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRendererCollection.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkUnstructuredGrid.h>

#include <QMainWindow>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QProgressDialog>
#include <QRadioButton>

#include <boost/multi_array.hpp>
#include <boost/format.hpp>
#include <boost/bind.hpp>

#include "ui_VCompare.h"

#include "DebugMacros.h"
#include "DirectoryDatabase.h"
#include "TreeModel.h"
#include "Validate.h"
#include "VectorFieldSet.h"


class TableRadioButton : public QRadioButton
{
  Q_OBJECT
public:
    TableRadioButton(const QString & info, QWidget *parent=0);

    QString info() { return mInfo; }
private:
    QString mInfo;
};

class VCompare : public QMainWindow, private Ui::VCompare
{
  Q_OBJECT
public:

  // Constructor/Destructor
  VCompare(); 
  ~VCompare() {};

public slots:
  void slotBtnChangeCurrentPathClicked();
  void slotBtnLoadModelsClicked();
  void slotBtnAutoStartEndClicked();
  void slotAddStartEndButtonClicked();
  void slotDeleteStartEndButtonClicked();

  void slotLeftModelFirstButtonClicked();
  void slotLeftModelLastButtonClicked();
  void slotLeftModelForwardButtonClicked();
  void slotLeftModelBackButtonClicked();

  void slotRightModelFirstButtonClicked();
  void slotRightModelLastButtonClicked();
  void slotRightModelForwardButtonClicked();
  void slotRightModelBackButtonClicked();

  void slotLeftAlignAxisXButtonClicked();
  void slotLeftAlignAxisYButtonClicked();
  void slotLeftAlignAxisZButtonClicked();

  void slotRightAlignAxisXButtonClicked();
  void slotRightAlignAxisYButtonClicked();
  void slotRightAlignAxisZButtonClicked();

  void slotLeftToggleGeometryButtonClicked();
  void slotRightToggleGometryButtonClicked();

  void slotLeftToggleIsosurfaceButtonClicked();
  void slotRightToggleIsosurfaceButtonClicked();

  void slotArrowScaleChanged();

  void slotPathFirstButtonGroupClicked(QAbstractButton *id);
  void slotPathLastButtonGroupClicked(QAbstractButton *id);

  void slotRevertStartEndButtonClicked();
  void slotUpdateStartEndButtonClicked();
  void slotPathStartEndPointsListBoxChanged(int);

private:

  // This is the absolute path to the current temperature being used.
  QString mCurrentTemperatureAbsolutePath;

  // This is the current material directory
  QString mCurrentMaterialDir;

  // This is the current geometry directory
  QString mCurrentGeometryDir;

  // This is the current size directory
  QString mCurrentSizeDir;

  // This is the current temperature directory
  QString mCurrentTemperatureDir;

  // This is the current node selected in the tree.
  QModelIndex mCurrentTreeTemperatureIndex;

  // This is the actor that will hold some geometry.
  vtkSmartPointer<vtkActor> geometryActor;

  // This is the left renderer.
  vtkSmartPointer<vtkRenderer> leftRenderer;

  // This is the right renderer.
  vtkSmartPointer<vtkRenderer> rightRenderer;

  vtkSmartPointer<vtkAxesActor> mLeftAxesActor;
  vtkSmartPointer<vtkAxesActor> mRightAxesActor;

  vtkSmartPointer<vtkOrientationMarkerWidget> mLeftMarkerWidget;
  vtkSmartPointer<vtkOrientationMarkerWidget> mRightMarkerWidget;

  // The models.
  VectorFieldSet mLeftFields;
  VectorFieldSet mRightFields;

  // The energy data for the models.
  EnergyEvaluationsLookup mEnergyEvaluationsLookup;

  // These values are to do with navigation.
  // mCurrentDatabaseLineEdit     : QLineEdit 
  //   stores the current database being used.
  // mCurrentDatabaseChangeButton : QPushButton
  //   changes the current databse
  //   updates: mCurrentDatabaseLineEdit
  //            mModelsTreeView
  // mCurrentDatabaseLabel        : QLabel

  // The file system model used for treeModels.
  //std::shared_ptr<QFileSystemModel> mModelsFileSystemModel; 
  std::shared_ptr<TreeModel> mModelsTreeModel;

  // The database to do with the
  DirectoryDatabase mDatabase;

  // The Validation object.
  Validate mValidate;

  // Floating point regular expression
  QRegExp mRegexFloat;

  // The initial (left view) camera.
  vtkSmartPointer<vtkCamera> mInitialLeftCamera;

  // The current (left view) camera.
  vtkSmartPointer<vtkCamera> mCurrentLeftCamera;

  // The initial (right view) camera.
  vtkSmartPointer<vtkCamera> mInitialRightCamera;

  // The current (right view) camera.
  vtkSmartPointer<vtkCamera> mCurrentRightCamera;

  QButtonGroup *mFirstGroup;
  QButtonGroup *mLastGroup;
  QList<QTableWidgetItem*> mTableIndexColumn;
  QList<TableRadioButton*> mFirstGroupButtons;
  QList<TableRadioButton*> mLastGroupButtons;
  QList<QTableWidgetItem*> mMxColumn;
  QList<QTableWidgetItem*> mMyColumn;
  QList<QTableWidgetItem*> mMzColumn;
  QList<QTableWidgetItem*> mMMagColumn;
  QList<QTableWidgetItem*> mHelicityColumn;
  QList<QTableWidgetItem*> mHandednessColumn;
  QList<QTableWidgetItem*> mRelativeEnergyColumn;

  QList<StartEndPair> mStartEndPairs;
  int mCurrentSelectedStartEndPairIndex;
  int getMaxStartEndNameIndex();

  void setFirstButtonGroupActive(const QString & item);
  void setLastButtonGroupActive(const QString & item);
  void clearOverviewTable();

  /////////////////////////////////////////////////////////////////////////////
  // Private functions.                                                      //
  /////////////////////////////////////////////////////////////////////////////
  
  void updateTreeModel();

  void updateRenderers(
      QString material,
      QString geometry,
      QString size,
      QString temperature);

  void setLeftView(double x, double y, double z);
  void setRightView(double x, double y, double z);

  double stringToDouble(const QString & strDouble, bool & status);


};

#endif  // UI_VCOMPARE_H_
