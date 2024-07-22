/**
 * \file   VectorField.h
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

#ifndef VECTOR_FIELD_H_
#define VECTOR_FIELD_H_

#include <algorithm>
#include <string>
#include <utility>
#include <vector>

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
#include <vtkContourGrid.h>

#include "TecplotLoader.h"
#include "Utilities.h"
#include "DebugMacros.h"

class VectorField
{
public:
  VectorField(std::string file, double arrowScale);

  std::vector<std::string> split(const std::string &s, char delim);

  std::string displayName();

  std::string name() const { return mName; }

  std::string nameIndex() const;
  
  double hmin() const { return mHmin; }

  double hmax() const { return mHmax; }

  double hmid() const { return (mHmin+mHmax)/2.0; }

  double mx() const { return mMx; }

  double my() const { return mMy; }

  double mz() const { return mMz; }

  double mmag() const { return mMmag; }

  std::string handedness() const {
    double hm = hmid();
    if (hm < 0.0) {
      return "Left";
    } else {
      return "Right";
    }
  }

  void setArrowLut(vtkSmartPointer<vtkLookupTable> arrowLut)
  {
    mArrowGlyphPolyDataMapper->SetLookupTable(arrowLut);
    mArrowGlyphPolyDataMapper->SetScalarRange(mHmin, mHmax);
    mArrowGlyphPolyDataMapper->Update();
  }

  vtkSmartPointer<vtkActor> arrows()     { return mArrowActor; }
  vtkSmartPointer<vtkActor> geometry()   { return mGeometryActor; }
  vtkSmartPointer<vtkActor> isosurface() { DEBUG("Returning isosurface"); return mIsosurfaceActor; }

  void setArrowScale(double arrowScale) { 
    if (fabs(mArrowScale - arrowScale) > 1E-9) {
      mArrowScale = arrowScale; 
      mArrowGlyph->SetScaleFactor(mArrowScale);
      mArrowGlyph->Update();
    }
  }

  void setIsosurfaceHelicity(double helicity);

  double isoSurfaceHelicity() { return mIsosurfaceHelicity; }

private:
  std::string                                 mName;
  double                                      mHmin;
  double                                      mHmax;
  double                                      mMx;
  double                                      mMy;
  double                                      mMz;
  double                                      mMmag;

  double                                      mArrowScale;

  vtkSmartPointer<vtkUnstructuredGrid>        mUGrid;

  vtkSmartPointer<vtkDataSetMapper>           mGeometryDataMapper;
  vtkSmartPointer<vtkActor>                   mGeometryActor;

  vtkSmartPointer<vtkArrowSource>             mArrowSource;
  vtkSmartPointer<vtkTransform>               mArrowTransform;
  vtkSmartPointer<vtkTransformPolyDataFilter> mArrowTransformFilter;
  vtkSmartPointer<vtkGlyph3D>                 mArrowGlyph;
  vtkSmartPointer<vtkPolyDataMapper>          mArrowGlyphPolyDataMapper;
  vtkSmartPointer<vtkActor>                   mArrowActor;

  double                                      mIsosurfaceHelicity;
  vtkSmartPointer<vtkContourGrid>             mIsosurface;
  vtkSmartPointer<vtkPolyDataMapper>          mIsosurfacePolyDataMapper;
  vtkSmartPointer<vtkActor>                   mIsosurfaceActor;

  void setGrid(
      const VertexField3d   & vert, 
      const ConnectIndices4 & conn);

  void setMagnetisation(const VectorField3d & field);

  void setHelicity();

  void setGeometry();

  void setArrows();

  void setIsosurface();

  std::string tail(std::string const& source, size_t const length) const;

};

#endif  // VECTOR_FIELD_H_
