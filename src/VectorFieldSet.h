/**
 * \file   VectorFieldSet.h 
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

#ifndef VECTOR_FIELD_SET_H_
#define VECTOR_FIELD_SET_H_

#include <memory>
#include <algorithm>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>

#include <vtkActor.h>
#include <vtkArrayCalculator.h>
#include <vtkArrowSource.h>
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
#include <vtkPointData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkQtTableView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkUnstructuredGrid.h>

#include <QProgressDialog>

#include "VectorField.h"

std::vector<std::string> split(const std::string &s, char delim);

class VectorFieldSet
{
public:
  VectorFieldSet(
      std::vector<std::string>      modelPaths,
      vtkSmartPointer<vtkRenderer>  renderer,
      double                        arrowFieldScale);

  VectorFieldSet(
      std::vector<std::string>       modelPaths,
      vtkSmartPointer<vtkRenderer>   renderer,
      QProgressDialog              & progress,
      size_t                         offset,
      double                         arrowFieldScale);

  VectorFieldSet() {}

  std::string currentDisplayName();

  void display(const std::string & name);

  void toggleGeometry();

  void toggleIsosurface();

  void nextModel();

  void previousModel();

  void firstModel();

  void lastModel();

  void setArrowScale(double arrowScale);

  inline size_t size() const { return mFields.size(); }

  std::shared_ptr<VectorField> & operator[] (size_t idx);

  std::shared_ptr<VectorField> & operator[] (const std::string & name);

  std::string currentName() const { return mCurrentName; }

private:
  vtkSmartPointer<vtkRenderer>                                    mRenderer;
  std::unordered_map< std::string, std::shared_ptr<VectorField> > mFields;
  std::unordered_map< std::string, size_t>                        mNameToIdx;
  std::vector<std::string>                                        mIdxToName;
  std::string                                                     mCurrentName;
  double                                                          mHmin;
  double                                                          mHmax;
  size_t                                                          mNLut;
  vtkSmartPointer<vtkLookupTable>                                 mLut;
  bool withGeometry;
  bool withIsosurface;

  std::shared_ptr<VectorField> field(const std::string & name);
  void buildLut();
};

#endif  // VECTOR_FIELD_SET_H_
