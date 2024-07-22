/**
 * \file   VCompare.cxx
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

#include "VCompare.h"

TableRadioButton::TableRadioButton(const QString &info, QWidget *parent):
  QRadioButton(parent), mInfo(info)
{
}

// Constructor
VCompare::VCompare() : 
  mRegexFloat("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?")
{
  this->setupUi(this);

  // Left VTK renderer.
  leftRenderer = vtkSmartPointer<vtkRenderer>::New();
  // Add the renderer to QT's VTK renderer.
  mDisplayVTKLeft->GetRenderWindow()->AddRenderer(leftRenderer);

  // Right VTK Renderer.
  rightRenderer = vtkSmartPointer<vtkRenderer>::New();
  // Add the renderer to QT's VTK renderer.
  mDisplayVTKRight->GetRenderWindow()->AddRenderer(rightRenderer);

  // By Default present the user with a cube.
  vtkSmartPointer<vtkCubeSource> cubeSource = 
      vtkSmartPointer<vtkCubeSource>::New();
  cubeSource->Update();

  vtkSmartPointer<vtkPolyDataMapper> polyDataMapper = 
    vtkSmartPointer<vtkPolyDataMapper>::New();
  polyDataMapper->SetInputConnection(cubeSource->GetOutputPort());

  geometryActor = vtkSmartPointer<vtkActor>::New();

  geometryActor->SetMapper(polyDataMapper);

  mLeftAxesActor = vtkSmartPointer<vtkAxesActor>::New();
  mLeftMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  mLeftMarkerWidget->SetInteractor(leftRenderer->GetRenderWindow()->GetInteractor());
  mLeftMarkerWidget->SetDefaultRenderer(leftRenderer);
  mLeftMarkerWidget->SetOrientationMarker(mLeftAxesActor);
  mLeftMarkerWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
  mLeftMarkerWidget->SetEnabled(1);
  mLeftMarkerWidget->InteractiveOn();

  mRightAxesActor = vtkSmartPointer<vtkAxesActor>::New();
  mRightMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  mRightMarkerWidget = vtkSmartPointer<vtkOrientationMarkerWidget>::New();
  mRightMarkerWidget->SetInteractor(rightRenderer->GetRenderWindow()->GetInteractor());
  mRightMarkerWidget->SetDefaultRenderer(rightRenderer);
  mRightMarkerWidget->SetOrientationMarker(mRightAxesActor);
  mRightMarkerWidget->SetViewport(0.0, 0.0, 0.2, 0.2);
  mRightMarkerWidget->SetEnabled(1);
  mRightMarkerWidget->InteractiveOn();
  
  // Add default actors to geometry.
  leftRenderer->AddActor(geometryActor);
  rightRenderer->AddActor(geometryActor);

  // Create the fileSystemTreeModels.
  //mModelsFileSystemModel = std::make_shared<QFileSystemModel>();
  //mModelsFileSystemModel->setRootPath("/");
  //mModelsTreeView->setModel(mModelsFileSystemModel.get());

  // Hide the columns of the directory tree that we don't want.
  mModelsTreeView->setColumnHidden(1, true);
  mModelsTreeView->setColumnHidden(2, true);
  mModelsTreeView->setColumnHidden(3, true);
  mModelsTreeView->setColumnHidden(4, true);

  mModelsTreeModel = std::make_shared<TreeModel>();
  mModelsTreeView->setModel(mModelsTreeModel.get());

  //-------------------------------------------------------------------------//
  // Connect signals and slots                                               //
  //-------------------------------------------------------------------------//

  connect(mCurrentDatabaseChangeButton , SIGNAL(clicked()), 
          this                         , SLOT(slotBtnChangeCurrentPathClicked()));

  connect(mLoadModels                  , SIGNAL(clicked()),
          this                         , SLOT(slotBtnLoadModelsClicked()));

  connect(mLoadModels                  , SIGNAL(clicked()),
          this                         , SLOT(slotBtnAutoStartEndClicked()));

  connect(mAddStartEndButton           , SIGNAL(clicked()),
          this                         , SLOT(slotAddStartEndButtonClicked()));

  connect(mDeleteStartEndButton        , SIGNAL(clicked()),
          this                         , SLOT(slotDeleteStartEndButtonClicked()));

  connect(mLeftModelFirstButton        , SIGNAL(clicked()),
          this                         , SLOT(slotLeftModelFirstButtonClicked()));

  connect(mLeftModelLastButton         , SIGNAL(clicked()),
          this                         , SLOT(slotLeftModelLastButtonClicked()));

  connect(mLeftModelForwardButton      , SIGNAL(clicked()),
          this                         , SLOT(slotLeftModelForwardButtonClicked()));

  connect(mLeftModelBackButton         , SIGNAL(clicked()),
          this                         , SLOT(slotLeftModelBackButtonClicked()));

  connect(mRightModelFirstButton       , SIGNAL(clicked()),
          this                         , SLOT(slotRightModelFirstButtonClicked()));

  connect(mRightModelLastButton        , SIGNAL(clicked()),
          this                         , SLOT(slotRightModelLastButtonClicked()));

  connect(mRightModelForwardButton     , SIGNAL(clicked()),
          this                         , SLOT(slotRightModelForwardButtonClicked()));

  connect(mRightModelBackButton        , SIGNAL(clicked()),
          this                         , SLOT(slotRightModelBackButtonClicked()));

  connect(mLeftAlignAxisX              , SIGNAL(clicked()),
          this                         , SLOT(slotLeftAlignAxisXButtonClicked()));

  connect(mLeftAlignAxisY              , SIGNAL(clicked()),
          this                         , SLOT(slotLeftAlignAxisYButtonClicked()));

  connect(mLeftAlignAxisZ              , SIGNAL(clicked()),
          this                         , SLOT(slotLeftAlignAxisZButtonClicked()));

  connect(mRightAlignAxisX             , SIGNAL(clicked()),
          this                         , SLOT(slotRightAlignAxisXButtonClicked()));

  connect(mRightAlignAxisY             , SIGNAL(clicked()),
          this                         , SLOT(slotRightAlignAxisYButtonClicked()));

  connect(mRightAlignAxisZ             , SIGNAL(clicked()),
          this                         , SLOT(slotRightAlignAxisZButtonClicked()));

  connect(mLeftToggleGeometryButton    , SIGNAL(clicked()),
          this                         , SLOT(slotLeftToggleGeometryButtonClicked()));

  connect(mRightToggleGeometryButton   , SIGNAL(clicked()),
          this                         , SLOT(slotRightToggleGometryButtonClicked()));

  connect(mLeftToggleIsosurfaceButton  , SIGNAL(clicked()),
          this                         , SLOT(slotLeftToggleIsosurfaceButtonClicked()));

  connect(mRightToggleIsosurfaceButton , SIGNAL(clicked()),
          this                         , SLOT(slotRightToggleIsosurfaceButtonClicked()));

  connect(mArrowScale                  , SIGNAL(editingFinished()),
          this                         , SLOT(slotArrowScaleChanged()));

  connect(mRevertStartEndButton        , SIGNAL(clicked()),
          this                         , SLOT(slotRevertStartEndButtonClicked()));

  connect(mUpdateStartEndButton        , SIGNAL(clicked()),
          this                         , SLOT(slotUpdateStartEndButtonClicked()));

  connect(mPathStartEndPointsListBox   , SIGNAL(currentRowChanged(int)),
          this                         , SLOT(slotPathStartEndPointsListBoxChanged(int)));

  //---------------------------------------------------------------------------
  // Copy over initial cameras.
  //---------------------------------------------------------------------------
  mInitialLeftCamera = vtkSmartPointer<vtkCamera>::New();
  vtkRenderer *leftRen = 
    mDisplayVTKLeft->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
  leftRen->GetActiveCamera()->DeepCopy(mInitialLeftCamera);
  slotLeftAlignAxisXButtonClicked();

  mInitialRightCamera = vtkSmartPointer<vtkCamera>::New();
  vtkRenderer *rightRen =
    mDisplayVTKRight->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
  rightRen->GetActiveCamera()->DeepCopy(mInitialRightCamera);
  slotRightAlignAxisXButtonClicked();

  //----------------
  //
  //----------------
  
  QPalette palette = mOverviewTable->palette();
  palette.setBrush(QPalette::Highlight,QBrush(Qt::white));
  palette.setBrush(QPalette::HighlightedText,QBrush(Qt::black));
  mOverviewTable->setPalette(palette);

  mOverviewTable->setRowCount(10);

  mFirstGroup = new QButtonGroup(this);
  mLastGroup  = new QButtonGroup(this);

  connect(mFirstGroup, SIGNAL(buttonClicked(QAbstractButton*)), 
      this, SLOT(slotPathFirstButtonGroupClicked(QAbstractButton*)));
  connect(mLastGroup,  SIGNAL(buttonClicked(QAbstractButton*)), 
      this, SLOT(slotPathLastButtonGroupClicked(QAbstractButton*)));


  //----------------
  //
  //----------------

  mCurrentSelectedStartEndPairIndex = -1;
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] Function slotBtnChangeCurrentPathClicked()                         //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotBtnChangeCurrentPathClicked()
{
  // Let the user select a directory.
  QString dirName = QFileDialog::getExistingDirectory(
      this, 
      tr("Select database"), 
      "/Users/Les", 
      QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  mCurrentDatabaseLineEdit->setText(dirName);
  // QFileInfo fileInfo(dirName);
  updateTreeModel();
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] Function slotBtnLoadModelsClicked()                                //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotBtnLoadModelsClicked()
{
  QModelIndexList selected = 
    mModelsTreeView->selectionModel()->selectedIndexes();

  // This holds the path from the selected temperature directory to the root
  QList<QVariant> path;

  unsigned int selectedCount = 0;
  QModelIndex newSelection;
  for (auto idx : selected) {
    DEBUG("idx: " << idx.data().toString().toUtf8().constData());
    newSelection = idx;
    path = mModelsTreeModel->pathToParent(idx); 
    selectedCount++;
  }  

  // There should be only one of these selected.
  if (selectedCount == 1) {
    if (mValidate.isValidTempCelsius(path[0].toString())) {
      // Update the renderers with the necessary data.
      updateRenderers(
          path[3].toString(), 
          path[2].toString(), 
          path[1].toString(), 
          path[0].toString());

      INFO("Setting mCurrentTemperatureAbsolutePath");
      mCurrentTemperatureAbsolutePath = QString("%1%2%3%4%5%6%7%8%9%10%11")
        .arg(mDatabase.rootDir()).arg(QDir::separator())
        .arg(path[3].toString()).arg(QDir::separator())
        .arg(path[2].toString()).arg(QDir::separator())
        .arg("lems").arg(QDir::separator())
        .arg(path[1].toString()).arg(QDir::separator())
        .arg(path[0].toString());

      INFO("Setting mCurrentMaterialDir");
      mCurrentMaterialDir    = path[3].toString();
      INFO("Setting mCurrentGeometryDir");
      mCurrentGeometryDir    = path[2].toString();
      INFO("Setting mCurrentSizeDir");
      mCurrentSizeDir        = path[1].toString();
      INFO("Setting mCurrentTemperatureDir");
      mCurrentTemperatureDir = path[0].toString();

      DEBUG("mCurrentTemperatureAbsolutePath: " 
            << mCurrentTemperatureAbsolutePath.toUtf8().constData());
      DEBUG("mCurrentMaterialDir:             " 
            << mCurrentMaterialDir.toUtf8().constData());
      DEBUG("mCurrentGeometryDir:             " 
            << mCurrentGeometryDir.toUtf8().constData());
      DEBUG("mCurrentSizeDir:                 " 
            << mCurrentSizeDir.toUtf8().constData());
      DEBUG("mCurrentTemperatureDir:          " 
            << mCurrentTemperatureDir.toUtf8().constData());

      INFO("Set the current active tree node");
      mModelsTreeModel->active(mCurrentTreeTemperatureIndex, false);
      mCurrentTreeTemperatureIndex = newSelection;
      mModelsTreeModel->active(mCurrentTreeTemperatureIndex, true);
    }
  } else {
    if (selectedCount != 0) {
      ERROR("I've got a non-unit number of selections");
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] Function slotBtnAutoStartEndClicked()                              //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotBtnAutoStartEndClicked()
{
  INFO("Setting mCurrentTemperatureAbsolutePath");
  
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] Function slotAddStartEndButtonClicked()                            //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotAddStartEndButtonClicked()
{
  INFO("slotAddStartEndButtonClicked()");
  DEBUG("mCurrentSelectedStartEndPairIndex: " << mCurrentSelectedStartEndPairIndex);

  unsigned int newNameIndex = getMaxStartEndNameIndex() + 1;
  StartEndPair newStartEndPair (mLeftCurrentModelName->text(),
                                mRightCurrentModelName->text(),
                                newNameIndex,
                                mCurrentTemperatureAbsolutePath);

  mStartEndPairs.push_back(newStartEndPair);

  mPathStartEndPointsListBox->addItem(newStartEndPair.name());

  newStartEndPair.writeToFile();

  mPathStartEndPointsListBox->setCurrentRow(mPathStartEndPointsListBox->count()-1);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] Function slotDeleteStartEndButtonClicked()                         //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotDeleteStartEndButtonClicked()
{
  INFO("slotDeleteStartEndButtonClicked()");
  DEBUG("mCurrentSelectedStartEndPairIndex: " << mCurrentSelectedStartEndPairIndex);

  if (mCurrentSelectedStartEndPairIndex == -1) {
    return;
  }

  mStartEndPairs[mCurrentSelectedStartEndPairIndex].deleteFile();
  mStartEndPairs.removeAt(mCurrentSelectedStartEndPairIndex);
  mPathStartEndPointsListBox->takeItem(mCurrentSelectedStartEndPairIndex);
  mPathStartEndPointsListBox->setCurrentRow(mPathStartEndPointsListBox->count()-1);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftModelFirstButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftModelFirstButtonClicked()
{
  mLeftFields.firstModel();
  mLeftCurrentModelName->setText(mLeftFields.currentDisplayName().c_str());
  mLeftStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mLeftFields.currentDisplayName()).str());
  setFirstButtonGroupActive(mLeftFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftModelLastButtonClicked()                                   //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftModelLastButtonClicked()
{
  mLeftFields.lastModel();
  mLeftCurrentModelName->setText(mLeftFields.currentDisplayName().c_str());
  mLeftStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mLeftFields.currentDisplayName()).str());
  setFirstButtonGroupActive(mLeftFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftModelForwardButtonClicked()                                //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftModelForwardButtonClicked()
{
  mLeftFields.nextModel();
  mLeftCurrentModelName->setText(mLeftFields.currentDisplayName().c_str());
  mLeftStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mLeftFields.currentDisplayName()).str());
  setFirstButtonGroupActive(mLeftFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftModelBackButtonClicked()                                   //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftModelBackButtonClicked()
{
  mLeftFields.previousModel();
  mLeftCurrentModelName->setText(mLeftFields.currentDisplayName().c_str());
  mLeftStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mLeftFields.currentDisplayName()).str());
  setFirstButtonGroupActive(mLeftFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightModelFirstButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightModelFirstButtonClicked()
{
  mRightFields.firstModel();
  mRightCurrentModelName->setText(mRightFields.currentDisplayName().c_str());
  mRightStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mRightFields.currentDisplayName()).str());
  setLastButtonGroupActive(mRightFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightModelLastButtonClicked()                                   //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightModelLastButtonClicked()
{
  mRightFields.lastModel();
  mRightCurrentModelName->setText(mRightFields.currentDisplayName().c_str());
  mRightStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mRightFields.currentDisplayName()).str());
  setLastButtonGroupActive(mRightFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightModelForwardButtonClicked()                                //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightModelForwardButtonClicked()
{
  mRightFields.nextModel();
  mRightCurrentModelName->setText(mRightFields.currentDisplayName().c_str());
  mRightStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mRightFields.currentDisplayName()).str());
  setLastButtonGroupActive(mRightFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightModelBackButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightModelBackButtonClicked()
{
  mRightFields.previousModel();
  mRightCurrentModelName->setText(mRightFields.currentDisplayName().c_str());
  mRightStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mRightFields.currentDisplayName()).str());
  setLastButtonGroupActive(mRightFields.currentName().c_str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftAlignAxisXButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftAlignAxisXButtonClicked()
{
  setLeftView(1.0, 0.0, 0.0);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftAlignAxisYButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftAlignAxisYButtonClicked()
{
  setLeftView(0.0, 1.0, 0.0);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftAlignAxisZButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftAlignAxisZButtonClicked()
{
  setLeftView(0.0, 0.0, 1.0);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightAlignAxisXButtonClicked()                                 //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightAlignAxisXButtonClicked()
{
  setRightView(1.0, 0.0, 0.0);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightAlignAxisYButtonClicked()                                 //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightAlignAxisYButtonClicked()
{
  setRightView(0.0, 1.0, 0.0);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightAlignAxisZButtonClicked()                                 //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightAlignAxisZButtonClicked()
{
  setRightView(0.0, 0.0, 1.0);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftToggleGeometryButtonClicked()                              //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftToggleGeometryButtonClicked()
{
  mLeftFields.toggleGeometry();
  mDisplayVTKLeft->update();
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightToggleGometryButtonClicked()                              //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightToggleGometryButtonClicked()
{
  mRightFields.toggleGeometry();
  mDisplayVTKRight->update();
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotLeftToggleIsosurfaceButtonClicked()                            //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotLeftToggleIsosurfaceButtonClicked()
{
  mLeftFields.toggleIsosurface();
  mDisplayVTKLeft->update();
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRightToggleIsosurfaceButtonClicked()                            //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRightToggleIsosurfaceButtonClicked()
{
  mRightFields.toggleIsosurface();
  mDisplayVTKRight->update();
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotArrowScaleChanged()                                            //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotArrowScaleChanged()
{
  bool   status = false;
  double scale  = stringToDouble(mArrowScale->text(), status);

  if (status != false) {
    mLeftFields.setArrowScale(scale);
    mRightFields.setArrowScale(scale);
    DEBUG("Set arrow scale to: " << scale);
  }

  mDisplayVTKLeft->update();
  mDisplayVTKRight->update();

}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotPathFirstButtonGroupClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotPathFirstButtonGroupClicked(QAbstractButton *id)
{
  TableRadioButton *ptrButton = static_cast<TableRadioButton *>(id);
  INFO("slotPathFirstButtonGroupClicked(): " << ptrButton->info().toUtf8().constData());
  mLeftFields.display(
      ptrButton->info().toUtf8().constData());
  mLeftCurrentModelName->setText(mLeftFields.currentDisplayName().c_str());
  mLeftStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mLeftFields.currentDisplayName()).str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotPathLastButtonGroupClicked()                                   //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotPathLastButtonGroupClicked(QAbstractButton *id)
{
  TableRadioButton *ptrButton = static_cast<TableRadioButton *>(id);
  INFO("slotPathLastButtonGroupClicked(): " << ptrButton->info().toUtf8().constData());
  mRightFields.display(
      ptrButton->info().toUtf8().constData());
  mRightCurrentModelName->setText(mRightFields.currentDisplayName().c_str());
  mRightStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mRightFields.currentDisplayName()).str());
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotRevertStartEndButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotRevertStartEndButtonClicked()
{
  INFO("slotRevertStartEndButtonClicked()");
  DEBUG("mCurrentSelectedStartEndPairIndex: " << mCurrentSelectedStartEndPairIndex);

  slotPathStartEndPointsListBoxChanged(mCurrentSelectedStartEndPairIndex);
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotUpdateStartEndButtonClicked()                                  //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotUpdateStartEndButtonClicked()
{
  INFO("slotUpdateStartEndButtonClicked()");
  DEBUG("mCurrentSelectedStartEndPairIndex: " << mCurrentSelectedStartEndPairIndex);

  StartEndPair & sep = mStartEndPairs[mCurrentSelectedStartEndPairIndex];

  sep.updateStartEndPair(mLeftCurrentModelName->text(),
                         mRightCurrentModelName->text());
  sep.writeToFile();
}

///////////////////////////////////////////////////////////////////////////////
// [SLOT] slotPathStartEndPointsListBoxChanged(int)                          //
///////////////////////////////////////////////////////////////////////////////

void VCompare::slotPathStartEndPointsListBoxChanged(int value)
{
  INFO("slotPathStartEndPointsListBoxChanged(): " << value);
  mCurrentSelectedStartEndPairIndex = value;

  if ((mCurrentSelectedStartEndPairIndex < 0) ||
      (mCurrentSelectedStartEndPairIndex >= mPathStartEndPointsListBox->count())) {
    return;
  }

  if (mStartEndPairs.size() > 0) {

    // Update vtk views
    StartEndPair & sep = mStartEndPairs[mCurrentSelectedStartEndPairIndex];
    mLeftFields.display(sep.startAsTecFile(true).toUtf8().constData());
    mRightFields.display(sep.endAsTecFile(true).toUtf8().constData());

    // Update text boxes
    mLeftCurrentModelName->setText(sep.startAsTecFile());
    mRightCurrentModelName->setText(sep.endAsTecFile());

    // Update energy data
    mLeftStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mLeftFields.currentDisplayName()).str());
    mRightStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mRightFields.currentDisplayName()).str());

    // Update table
    setFirstButtonGroupActive(sep.startAsTecFile(true));
    setLastButtonGroupActive(sep.endAsTecFile(true));
  }
}

///////////////////////////////////////////////////////////////////////////////
// Function getMaxStartEndNameIndex()                                        //
///////////////////////////////////////////////////////////////////////////////
int VCompare::getMaxStartEndNameIndex()
{
  int maxIndex = 0;
  for (auto sep : mStartEndPairs) {
    if (maxIndex < sep.nameIndex()) {
      maxIndex = sep.nameIndex();
    }
  }

  return maxIndex;
}

///////////////////////////////////////////////////////////////////////////////
// Function setFirstButtonGroupActive()                                      //
///////////////////////////////////////////////////////////////////////////////

void VCompare::setFirstButtonGroupActive(const QString & item)
{
  QList<QAbstractButton *> buttons = mFirstGroup->buttons();
  for (QAbstractButton * btn : buttons) {
    TableRadioButton *tblBtn = static_cast<TableRadioButton *>(btn);
    if (tblBtn->info() == item) {
      btn->setChecked(true);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Function setLastButtonGroupActive()                                       //
///////////////////////////////////////////////////////////////////////////////

void VCompare::setLastButtonGroupActive(const QString & item)
{
  INFO("Checking if '" << item.toUtf8().constData() << "' is same as");
  QList<QAbstractButton *> buttons = mLastGroup->buttons();
  for (QAbstractButton * btn : buttons) {
    TableRadioButton *tblBtn = static_cast<TableRadioButton *>(btn);
    INFO("   '" << tblBtn->info().toUtf8().constData() << "'");
    if (tblBtn->info() == item) {
      btn->setChecked(true);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// Function clearOverviewTable()                                             //
///////////////////////////////////////////////////////////////////////////////

void VCompare::clearOverviewTable()
{
  for (QTableWidgetItem * index : mTableIndexColumn) {
    delete index;
  }
  mTableIndexColumn.clear();

  for (TableRadioButton * btn : mFirstGroupButtons) {
    mFirstGroup->removeButton(btn);
    delete btn;
  }
  mFirstGroupButtons.clear();

  for (TableRadioButton * btn : mLastGroupButtons) {
    mLastGroup->removeButton(btn);
    delete btn;
  }
  mLastGroupButtons.clear();

  for (QTableWidgetItem * mx : mMxColumn) {
    delete mx;
  }
  mMxColumn.clear();

  for (QTableWidgetItem * my : mMyColumn) {
    delete my;
  }
  mMyColumn.clear();

  for (QTableWidgetItem * mz : mMzColumn) {
    delete mz;
  }
  mMzColumn.clear();

  for (QTableWidgetItem * mMag : mMMagColumn) {
    delete mMag;
  }
  mMMagColumn.clear();

  for (QTableWidgetItem * h : mHelicityColumn) {
    delete h;
  }
  mHelicityColumn.clear();

  for (QTableWidgetItem * hand : mHandednessColumn) {
    delete hand;
  }
  mHandednessColumn.clear();

  for (QTableWidgetItem * relE : mRelativeEnergyColumn) {
    delete relE;
  }
  mRelativeEnergyColumn.clear();

  mOverviewTable->setRowCount(0);
}

///////////////////////////////////////////////////////////////////////////////
// Function updateTreeModel()                                                //
///////////////////////////////////////////////////////////////////////////////

void VCompare::updateTreeModel()
{
  QString cpath = mCurrentDatabaseLineEdit->text();
  mDatabase.setRootDir(cpath);

  mModelsTreeModel = std::make_shared<TreeModel>(mDatabase);

  mModelsTreeView->setModel(mModelsTreeModel.get());
  mDatabase.setRootDir(cpath);
}

///////////////////////////////////////////////////////////////////////////////
// Function updateRenderers()                                                //
///////////////////////////////////////////////////////////////////////////////

void VCompare::updateRenderers(
    QString material,
    QString geometry,
    QString size,
    QString temperature)
{
  INFO("Clear overview table data");
  clearOverviewTable();

  INFO("Get field list");
  QStringList fieldList = mDatabase.fieldFileList(
      material, geometry, size, temperature);

  if (fieldList.size() == 0) {
    INFO("Field list is empty, do nothing");
    return;
  }

  INFO("Get the field files");
  std::vector<std::string> fieldFiles;
  for (auto f : fieldList) {
    fieldFiles.push_back(f.toUtf8().constData());
  }

  INFO("Prepare dialogs for reading");
  QProgressDialog progress("Reading models", "Abort", 0, fieldFiles.size()*2, this);
  progress.setWindowModality(Qt::WindowModal);
  progress.setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
  //progress.setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowTitleHint);

  bool   status = false;
  double scale  = stringToDouble(mArrowScale->text(), status);

  INFO("Read vector fields");
  mLeftFields  = VectorFieldSet(fieldFiles, leftRenderer,  progress, 0,                 scale);
  mRightFields = VectorFieldSet(fieldFiles, rightRenderer, progress, fieldFiles.size(), scale);

  INFO("Retreiving energy evaluation data");
  mEnergyEvaluationsLookup = mDatabase.getEnergyEvaluations(
      material, geometry, size, temperature);

  INFO("Populate tabular data ");
  // NOTE: we use mLeftFields as it contains a complete list of models, we
  //       could just of easily use mRightFields - as long as we're consistent.
  mOverviewTable->setRowCount(mLeftFields.size());
  for (size_t i = 0; i < mLeftFields.size(); ++i) {
    INFO("Inserting table row data " << mLeftFields[i]->name());
    QTableWidgetItem * index = new QTableWidgetItem(
        mLeftFields[i]->nameIndex().c_str());
    mTableIndexColumn.push_back(index);
    mOverviewTable->setItem(i, 0, index);

    TableRadioButton *firstRadio = new TableRadioButton(
        mLeftFields[i]->name().c_str());
    mFirstGroupButtons.push_back(firstRadio);
    mOverviewTable->setCellWidget(i, 1, firstRadio);
    mFirstGroup->addButton(firstRadio);

    TableRadioButton *lastRadio = new TableRadioButton(
        mLeftFields[i]->name().c_str());
    mLastGroupButtons.push_back(lastRadio);
    mOverviewTable->setCellWidget(i, 2, lastRadio);
    mLastGroup->addButton(lastRadio);

    QTableWidgetItem *Mx = new QTableWidgetItem(QString::number(mLeftFields[i]->mx()));
    Mx->setTextAlignment(Qt::AlignRight);
    mMxColumn.push_back(Mx);
    mOverviewTable->setItem(i, 3, Mx);

    QTableWidgetItem *My = new QTableWidgetItem(QString::number(mLeftFields[i]->my()));
    My->setTextAlignment(Qt::AlignRight);
    mMxColumn.push_back(My);
    mOverviewTable->setItem(i, 4, My);

    QTableWidgetItem *Mz = new QTableWidgetItem(QString::number(mLeftFields[i]->mz()));
    Mz->setTextAlignment(Qt::AlignRight);
    mMxColumn.push_back(Mz);
    mOverviewTable->setItem(i, 5, Mz);

    QTableWidgetItem *MMag = new QTableWidgetItem(QString::number(mLeftFields[i]->mmag()));
    MMag->setTextAlignment(Qt::AlignRight);
    mMxColumn.push_back(MMag);
    mOverviewTable->setItem(i, 6, MMag);

    QTableWidgetItem *helicity = new QTableWidgetItem(QString::number(mLeftFields[i]->hmid()));
    helicity->setTextAlignment(Qt::AlignRight);
    mMxColumn.push_back(helicity);
    mOverviewTable->setItem(i, 7, helicity);

    QTableWidgetItem *handedness = new QTableWidgetItem(mLeftFields[i]->handedness().c_str());
    mMxColumn.push_back(handedness);
    mOverviewTable->setItem(i, 8, handedness);


    std::pair<double, bool> rele = mEnergyEvaluationsLookup.value(mLeftFields[i]->displayName()).lastEnergy();
    QString releColVal;
    if (rele.second == true) {
        releColVal = QString::number(rele.first);
    } else {
        releColVal = "N/A";
    }
    QTableWidgetItem *relEnergy = new QTableWidgetItem(releColVal);
    mRelativeEnergyColumn.push_back(relEnergy);
    mOverviewTable->setItem(i, 9, relEnergy);
  }

  INFO("Set end value for progress box (terminates progress box).");
  progress.setValue(fieldFiles.size()*2);

  INFO("Finding start/end data");
  QString start = fieldList[0];
  QString end   = fieldList[0];

  mStartEndPairs = mDatabase.getStartEndPairs(
    material, geometry, size, temperature);

  INFO("Clearing path start/end points list box");
  while(mPathStartEndPointsListBox->count() > 0) {
    mPathStartEndPointsListBox->takeItem(0);
  }
  INFO("Clearing path start/end points list box ... done!");

  bool startEndFirstPass = true;
  if (mStartEndPairs.size() != 0) {
    INFO("Populating path start/end points data");
    for (auto sep : mStartEndPairs) {
      mPathStartEndPointsListBox->addItem(sep.name());
      if (startEndFirstPass) {
        startEndFirstPass = false;
        mCurrentSelectedStartEndPairIndex = 0;
        mPathStartEndPointsListBox->setCurrentRow(mCurrentSelectedStartEndPairIndex);
        start = sep.startAsTecFile(true);
        end   = sep.endAsTecFile(true);
      }
    }
  }

  INFO("Start model: " << start.toUtf8().constData());
  INFO("End model:   " << end.toUtf8().constData());

  mLeftFields.display(start.toUtf8().constData());
  mRightFields.display(end.toUtf8().constData());
  slotArrowScaleChanged();

  mLeftCurrentModelName->setText(mLeftFields.currentDisplayName().c_str());
  mRightCurrentModelName->setText(mRightFields.currentDisplayName().c_str());

  mLeftStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mLeftFields.currentDisplayName()).str());
  mRightStructureSummary->setPlainText(
      mEnergyEvaluationsLookup.value(mRightFields.currentDisplayName()).str());

  setFirstButtonGroupActive(start);
  setLastButtonGroupActive(end);
}

///////////////////////////////////////////////////////////////////////////////
// Function setLeftView()                                                    //
///////////////////////////////////////////////////////////////////////////////

void VCompare::setLeftView(double x, double y, double z)
{
  mCurrentLeftCamera = vtkSmartPointer<vtkCamera>::New();
  mCurrentLeftCamera->DeepCopy(mInitialLeftCamera);
  mCurrentLeftCamera->SetPosition(x, y, z);

  if ( (z == -1.0) || (z == 1.0) ) {
    mCurrentLeftCamera->SetViewUp(0.0, 1.0, 0.0);
  } else {
    mCurrentLeftCamera->SetViewUp(0.0, 0.0, 1.0);
  }

  vtkRenderer *ren = 
    mDisplayVTKLeft->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
  ren->SetActiveCamera(mCurrentLeftCamera);
  ren->ResetCamera();
  ren->ResetCameraClippingRange();

  mDisplayVTKLeft->update();
}

///////////////////////////////////////////////////////////////////////////////
// Function setRightView()                                                   //
///////////////////////////////////////////////////////////////////////////////

void VCompare::setRightView(double x, double y, double z)
{
  mCurrentRightCamera = vtkSmartPointer<vtkCamera>::New();
  mCurrentRightCamera->DeepCopy(mInitialRightCamera);
  mCurrentRightCamera->SetPosition(x, y, z);

  if ( (z == -1.0) || (z == 1.0) ) {
    mCurrentRightCamera->SetViewUp(0.0, 1.0, 0.0);
  } else {
    mCurrentRightCamera->SetViewUp(0.0, 0.0, 1.0);
  }

  vtkRenderer *ren = 
    mDisplayVTKRight->GetRenderWindow()->GetRenderers()->GetFirstRenderer();
  ren->SetActiveCamera(mCurrentRightCamera);
  ren->ResetCamera();
  ren->ResetCameraClippingRange();

  mDisplayVTKRight->update();
}

///////////////////////////////////////////////////////////////////////////////
// Function stringToDouble()                                                 //
///////////////////////////////////////////////////////////////////////////////

double VCompare::stringToDouble(const QString & strDouble, bool & status)
{
  if (mRegexFloat.exactMatch(strDouble)) {
    status = true;
    return strDouble.toDouble();
  }

  status = false;
  return 0.0;
}

