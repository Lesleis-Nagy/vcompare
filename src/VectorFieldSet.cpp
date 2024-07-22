/**
 * \file   VectorFieldSet.cxx 
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

//#include "DebugMacros.h"
#include "VectorFieldSet.h"

///////////////////////////////////////////////////////////////////////////////
// Split utility function                                                    //
// TODO: move this splitting out of this file/class and just return the      //
//       mCurrentName then do the splitting (which if only for display       // 
//       purposes) in VCompare class.                                        //
///////////////////////////////////////////////////////////////////////////////

std::vector<std::string> split(const std::string &s, char delim) {
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> tokens;
  while (getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}

///////////////////////////////////////////////////////////////////////////////
// Constructor                                                               //
///////////////////////////////////////////////////////////////////////////////

VectorFieldSet::VectorFieldSet(
    std::vector<std::string>     modelPaths, 
    vtkSmartPointer<vtkRenderer> renderer,
    double                       arrowFieldScale) :
  mRenderer(renderer), mNLut(1000), withGeometry(false), withIsosurface(false)
{
  mHmin =  1E12;
  mHmax = -1E12;
  size_t i = 0;
  for (auto p : modelPaths) {
    std::shared_ptr<VectorField> f = std::make_shared<VectorField>(p, arrowFieldScale);
    mFields.insert({p, f});

    mNameToIdx.insert({p, i});
    mIdxToName.push_back(p);

    mHmin = (f->hmin() < mHmin) ? f->hmin() : mHmin;
    mHmax = (f->hmax() > mHmax) ? f->hmax() : mHmax;

    i = i + 1;
  }
  std::sort(mIdxToName.begin(), mIdxToName.end());

  // Build the colour LUT
  buildLut();

  // For each arrow model, update the colour LUT to be this colour LUT.
  for (auto kv : mFields) {
    kv.second->setArrowLut(mLut);
  }
}

VectorFieldSet::VectorFieldSet(
    std::vector<std::string>       modelPaths,
    vtkSmartPointer<vtkRenderer>   renderer,
    QProgressDialog              & progress,
    size_t                         offset,
    double                         arrowFieldScale) :
  mRenderer(renderer), mNLut(1000), withGeometry(false), withIsosurface(false)
{
  mHmin =  1E12;
  mHmax = -1E12;
  size_t i = 0;
  for (auto p : modelPaths) {

    progress.setValue(i+offset);
    
    std::shared_ptr<VectorField> f = std::make_shared<VectorField>(p, arrowFieldScale);
    mFields.insert({p, f});

    progress.setValue(i+offset);

    mNameToIdx.insert({p, i});
    mIdxToName.push_back(p);

    mHmin = (f->hmin() < mHmin) ? f->hmin() : mHmin;
    mHmax = (f->hmax() > mHmax) ? f->hmax() : mHmax;

    i = i + 1;
  }

  std::sort(mIdxToName.begin(), mIdxToName.end());

  // Build the colour LUT
  buildLut();

  // For each arrow model, update the colour LUT to be this colour LUT.
  for (auto kv : mFields) {
    kv.second->setArrowLut(mLut);
  }
}


std::string VectorFieldSet::currentDisplayName()
{
  auto tokens = split(mCurrentName, '/');
  return tokens.back();
}


void VectorFieldSet::display(const std::string & name)
{
  auto actor    = field(name)->arrows();
  auto gactor   = field(name)->geometry();
  auto isoactor = field(name)->isosurface();

  if (actor != NULL) {
    mRenderer->RemoveAllViewProps();

    mRenderer->AddActor(actor);

    if (withGeometry) {
      if (gactor != NULL) {
        mRenderer->AddActor(gactor);
      }
    }

    if (withIsosurface) {
      if (isoactor != NULL) {
        mRenderer->AddActor(isoactor);
        //DEBUG("Added isosurface actor to renderer");
      }
    }
      
    mRenderer->ResetCamera();
    mRenderer->GetRenderWindow()->Render();
    mCurrentName = name;
  }
}

void VectorFieldSet::toggleGeometry()
{
  size_t currentIdx = mNameToIdx[mCurrentName];

  if (withGeometry == false) {
    withGeometry = true;
  } else {
    withGeometry = false;
  }

  display(mIdxToName[currentIdx]);
}

void VectorFieldSet::toggleIsosurface()
{
  size_t currentIdx = mNameToIdx[mCurrentName];

  if (withIsosurface == false) {
    withIsosurface = true;
  } else {
    withIsosurface = false;
  }

  display(mIdxToName[currentIdx]);
}

void VectorFieldSet::nextModel()
{
  size_t currentIdx = mNameToIdx[mCurrentName];

  if (currentIdx+1 == mIdxToName.size()) {
    // Cannot go beyond the maximal model (loop round?).
    return;
  }

  display(mIdxToName[currentIdx+1]);
}

void VectorFieldSet::previousModel()
{
  int currentIdx = mNameToIdx[mCurrentName];

  if (currentIdx-1 == -1) {
    // Cannot go beyond before first model (loop round?).
    return;
  }

  display(mIdxToName[currentIdx-1]);
}

void VectorFieldSet::firstModel()
{
  display(*mIdxToName.begin());
}

void VectorFieldSet::lastModel()
{
  display(mIdxToName.back());
}

void VectorFieldSet::setArrowScale(double arrowScale)
{
  for (auto kv : mFields) {
    auto key   = kv.first;
    auto field = kv.second;

    field->setArrowScale(arrowScale);
  }
}

std::shared_ptr<VectorField> & VectorFieldSet::operator[] (size_t idx)
{
  return mFields[mIdxToName[idx]];
}

std::shared_ptr<VectorField> & VectorFieldSet::operator[] (const std::string & name) 
{
  return mFields[name];
}

///////////////////////////////////////////////////////////////////////////////
// Function field()                                                          //
///////////////////////////////////////////////////////////////////////////////

std::shared_ptr<VectorField> VectorFieldSet::field(const std::string & name)
{
  if (mFields.find(name) != mFields.end()) {
    return mFields[name];
  }
  return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Function buildLut()                                                       //
///////////////////////////////////////////////////////////////////////////////

void VectorFieldSet::buildLut()
{
  mLut = vtkSmartPointer<vtkLookupTable>::New();

  vtkSmartPointer<vtkColorTransferFunction> ctf = 
    vtkSmartPointer<vtkColorTransferFunction>::New();

  double ds = fabs(mHmax - mHmin)/(double)mNLut;
  
  mLut->SetNumberOfTableValues(mNLut);

  ctf->AddRGBSegment(
      (mHmin)          , 0.0, 0.0, 1.0, 
      (mHmin+mHmax)/2.0, 1.0, 1.0, 1.0);
  ctf->AddRGBSegment(
      (mHmin+mHmax)/2.0, 1.0, 1.0, 1.0,
      (mHmax)          , 1.0, 0.0, 0.0);

  for (size_t i = 0; i < mNLut; ++i) {
    double s    = mHmin+ ds*(double)i;

    double *rgb = ctf->GetColor(s);

    mLut->SetTableValue(i, rgb[0], rgb[1], rgb[2], 1.0);
  }
}
