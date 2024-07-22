/**
 * \file   DirectoryDatabase.cxx 
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

#include "DirectoryDatabase.h"

//const QRegExp StartEndPair::reDatFile = QRegExp("([0-9]+)nm_([0-9]+)C_mag_([0-9]{4})\\.dat");
//const QRegExp StartEndPair::reTecFile = QRegExp("([0-9]+)nm_([0-9]+)C_mag_([0-9]{4})_mult\\.tec");

const QString DirectoryDatabase::lems = "lems";

const QString DirectoryDatabase::strRegexFieldFile = 
    "([0-9]+)nm_([0-9]+)C_mag_([0-9]{4})_mult\\.tec";

const QString DirectoryDatabase::strRegexStdoutFile =
    "stdout_lcl_([0-9]+)nm_([0-9]+)C_([0-9]{4})";

const QString FieldDataHasher::strRegexFieldFile = 
    "([0-9]+)nm_([0-9]+)C_mag_([0-9]{4})_mult\\.tec";

const QString FieldDataHasher::strRegexStdoutFile =
    "stdout_lcl_([0-9]+)nm_([0-9]+)C_([0-9]{4})";

const QString DirectoryDatabase::strRegexStartEndFile = "START_END(_([0-9]+))?";

///////////////////////////////////////////////////////////////////////////////
// Function materialList().                                                  //
///////////////////////////////////////////////////////////////////////////////

QStringList DirectoryDatabase::materialList() const
{
  QStringList output;

  QDir parentDir(mRootDir);
  parentDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

  for (auto d : parentDir.entryList()) {
    output.append(d);
  }

  return output;
}

///////////////////////////////////////////////////////////////////////////////
// Function geometryList().                                                  //
///////////////////////////////////////////////////////////////////////////////

QStringList DirectoryDatabase::geometryList(QString material) const
{
  QStringList output;

  QDir parentDir(mRootDir);
  parentDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState == false) {
    ERROR("Could not change to material '"
        << material.toUtf8().constData() << "' directory");
  }

  for (auto d : parentDir.entryList()) {
    output.append(d);
  }

  return output;
}

///////////////////////////////////////////////////////////////////////////////
// Function sizeList()                                                       //
///////////////////////////////////////////////////////////////////////////////

QStringList DirectoryDatabase::sizeList(
    QString material,
    QString geometry) const
{
  QStringList output;

  QDir parentDir(mRootDir);
  parentDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState == false) {
    ERROR("Could not change to material '"
        << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '"
        << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(lems);
  DEBUG(parentDir.absolutePath().toUtf8().constData());
  if (cdState == false) {
    ERROR("Could not change to 'lem' directory");
  }

  for (auto d : parentDir.entryList()) {
    output.append(d);
  }

  return output;
}

///////////////////////////////////////////////////////////////////////////////
// Function temperatureList()                                                //
///////////////////////////////////////////////////////////////////////////////

QStringList DirectoryDatabase::temperatureList(
    QString material,
    QString geometry,
    QString size) const
{
  QStringList output;

  QDir parentDir(mRootDir);
  parentDir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState == false) {
    ERROR("Could not change to material '"
        << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '"
        << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(lems);
  if (cdState == false) {
    ERROR("Could not change to 'lem' directory");
  }

  cdState = false;
  cdState = parentDir.cd(size);
  if (cdState == false) {
    ERROR("Could not change to size '"
        << size.toUtf8().constData() << "' directory");
  }

  for (auto d : parentDir.entryList()) {
    output.append(d);
  }

  return output;
}

///////////////////////////////////////////////////////////////////////////////
// Function fieldList()                                                      //
///////////////////////////////////////////////////////////////////////////////

QStringList DirectoryDatabase::fieldFileList(
      QString material,
      QString geometry,
      QString size,
      QString temperature) const
{
  QStringList output;

  QDir parentDir(mRootDir);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState==false) {
    ERROR("Could not change to material '"
             << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '" 
             << geometry.toUtf8().constData() << "' directory");
  }
  
  cdState = false;
  cdState = parentDir.cd(lems);
  if (cdState == false) {
    ERROR("Could not change to lems directory '" 
             << lems.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(size);
  if (cdState == false) {
    ERROR("Could not change to size '" 
             << size.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(temperature);
  if (cdState == false) {
    ERROR("Could not change to temperature '" 
             << temperature.toUtf8().constData() << "' directory");
  }

  QStringList files = parentDir.entryList();

  for (auto f : files) {
    if (mRegexFieldFile.exactMatch(f)) {
      QString absFilePath = mRootDir.canonicalPath() + QDir::separator()
                            + material               + QDir::separator()
                            + geometry               + QDir::separator()
                            + lems                   + QDir::separator()
                            + size                   + QDir::separator()
                            + temperature            + QDir::separator()
                            + f;

      QFile fabsFilePath(absFilePath);

      if (fabsFilePath.exists()) {
        output.append(absFilePath);
      } else {
        ERROR("The file '" << absFilePath.toUtf8().constData() 
            << "' doesn't exist!");
      }
    }
  }

  return output;
}

///////////////////////////////////////////////////////////////////////////////
// Function hasPathStartEndPair()                                            //
///////////////////////////////////////////////////////////////////////////////

/*
bool DirectoryDatabase::hasPathStartEndPair(
    QString material,
    QString geometry,
    QString size,
    QString temperature) const
{
  //QStringList output;

  QDir parentDir(mRootDir);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState==false) {
    ERROR("Could not change to material '"
             << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '" 
             << geometry.toUtf8().constData() << "' directory");
  }
  
  cdState = false;
  cdState = parentDir.cd(lems);
  if (cdState == false) {
    ERROR("Could not change to lems directory '" 
             << lems.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(size);
  if (cdState == false) {
    ERROR("Could not change to size '" 
             << size.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(temperature);
  if (cdState == false) {
    ERROR("Could not change to temperature '" 
             << temperature.toUtf8().constData() << "' directory");
  }

  QString absFilePath = 
    parentDir.canonicalPath() + QDir::separator() + fileStartEnd;

  QFile fabsFilePath(absFilePath);

  return fabsFilePath.exists();
  return false;
}
*/

///////////////////////////////////////////////////////////////////////////////
// Function deletePathStartEndPair()                                            //
///////////////////////////////////////////////////////////////////////////////

/*
void DirectoryDatabase::deletePathStartEndPair(
    QString material,
    QString geometry,
    QString size,
    QString temperature)
{
  //QStringList output;

  QDir parentDir(mRootDir);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState==false) {
    ERROR("Could not change to material '"
             << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '" 
             << geometry.toUtf8().constData() << "' directory");
  }
  
  cdState = false;
  cdState = parentDir.cd(lems);
  if (cdState == false) {
    ERROR("Could not change to lems directory '" 
             << lems.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(size);
  if (cdState == false) {
    ERROR("Could not change to size '" 
             << size.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(temperature);
  if (cdState == false) {
    ERROR("Could not change to temperature '" 
             << temperature.toUtf8().constData() << "' directory");
  }

  QString absFilePath = 
    parentDir.canonicalPath() + QDir::separator() + fileStartEnd;

  QFile fabsFilePath(absFilePath);

  fabsFilePath.remove();
}
*/

///////////////////////////////////////////////////////////////////////////////
// Function getStartEndPairs()                                               //
///////////////////////////////////////////////////////////////////////////////
QList<StartEndPair> DirectoryDatabase::getStartEndPairs(
    QString   material,
    QString   geometry,
    QString   size,
    QString   temperature) const
{
  QList<StartEndPair> startEndPairs;

  QDir parentDir(mRootDir);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState==false) {
    ERROR("Could not change to material '"
             << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '" 
             << geometry.toUtf8().constData() << "' directory");
  }
  
  cdState = false;
  cdState = parentDir.cd(lems);
  if (cdState == false) {
    ERROR("Could not change to lems directory '" 
             << lems.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(size);
  if (cdState == false) {
    ERROR("Could not change to size '" 
             << size.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(temperature);
  if (cdState == false) {
    ERROR("Could not change to temperature '" 
             << temperature.toUtf8().constData() << "' directory");
  }

  QStringList files = parentDir.entryList();

  for (auto f : files) {
    if (mRegexStartEndFile.exactMatch(f)) {
      QString absFilePath =
        parentDir.canonicalPath() + QDir::separator() + f;
      DEBUG("Processing start/end file: " << absFilePath.toUtf8().constData());
      QFile fabsFilePath(absFilePath);

      if (fabsFilePath.exists()) {
        if (fabsFilePath.open(QFile::ReadOnly)) {
          QTextStream fin(&fabsFilePath);
          QString start, end;
          fin >> start;
          fin >> end;

          startEndPairs.push_back(StartEndPair(start, end, f, parentDir.canonicalPath()));
        }
      }
    }
  }

  return startEndPairs;
}

///////////////////////////////////////////////////////////////////////////////
// Function setStartEndPair()                                                //
///////////////////////////////////////////////////////////////////////////////
/*
void DirectoryDatabase::setStartEndPair(
    QString material,
    QString geometry,
    QString size,
    QString temperature,
    QString startPath,
    QString endPath)
{
  QDir parentDir(mRootDir);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState==false) {
    ERROR("Could not change to material '"
             << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '" 
             << geometry.toUtf8().constData() << "' directory");
  }
  
  cdState = false;
  cdState = parentDir.cd(lems);
  if (cdState == false) {
    ERROR("Could not change to lems directory '" 
             << lems.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(size);
  if (cdState == false) {
    ERROR("Could not change to size '" 
             << size.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(temperature);
  if (cdState == false) {
    ERROR("Could not change to temperature '" 
             << temperature.toUtf8().constData() << "' directory");
  }

  QString absFilePath = 
    parentDir.canonicalPath() + QDir::separator() + fileStartEnd;

  QFile fabsFilePath(absFilePath);

  startPath.chop(9);
  endPath.chop(9);

  startPath = startPath + ".dat";
  endPath   = endPath   + ".dat";

  if (fabsFilePath.open(QFile::WriteOnly | QFile::Truncate)) {
    QTextStream fout(&fabsFilePath);

    fout << startPath;
    fout << "\n";
    fout << endPath;
    fout << "\n";
  }
  fabsFilePath.close();

}
*/

///////////////////////////////////////////////////////////////////////////////
// Function getEnergyEvaluations()                                           //
///////////////////////////////////////////////////////////////////////////////
EnergyEvaluationsLookup DirectoryDatabase::getEnergyEvaluations(
    QString material,
    QString geometry,
    QString size,
    QString temperature) const
{
  EnergyEvaluationsLookup output;

  QDir parentDir(mRootDir);

  bool cdState;

  cdState = false;
  cdState = parentDir.cd(material);
  if (cdState==false) {
    ERROR("Could not change to material '"
             << material.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(geometry);
  if (cdState == false) {
    ERROR("Could not change to geometry '" 
             << geometry.toUtf8().constData() << "' directory");
  }
  
  cdState = false;
  cdState = parentDir.cd(lems);
  if (cdState == false) {
    ERROR("Could not change to lems directory '" 
             << lems.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(size);
  if (cdState == false) {
    ERROR("Could not change to size '" 
             << size.toUtf8().constData() << "' directory");
  }

  cdState = false;
  cdState = parentDir.cd(temperature);
  if (cdState == false) {
    ERROR("Could not change to temperature '" 
             << temperature.toUtf8().constData() << "' directory");
  }

  QStringList files = parentDir.entryList();

  for (auto f : files) {
    if (mRegexStdoutFile.exactMatch(f)) {
      QString absFilePath = mRootDir.canonicalPath() + QDir::separator()
                            + material               + QDir::separator()
                            + geometry               + QDir::separator()
                            + lems                   + QDir::separator()
                            + size                   + QDir::separator()
                            + temperature            + QDir::separator()
                            + f;

      QFile fabsFilePath(absFilePath);

      if (fabsFilePath.exists()) {
        output.insert(absFilePath, EnergyEvaluations(absFilePath));
      } else {
        ERROR("The file '" << absFilePath.toUtf8().constData() 
            << "' doesn't exist!");
      }
    }
  }

  return output;
}

///////////////////////////////////////////////////////////////////////////////
// absPath()                                                                 //
///////////////////////////////////////////////////////////////////////////////
QString DirectoryDatabase::absPathTecplot(
    QString      material,
    QString      geometry,
    unsigned int size,
    unsigned int temperature,
    unsigned int index) const
{
  QString strSize  = QString("%inm").arg(size);
  QString strTemp  = QString("%iC").arg(temperature);
  
  QString tecFile  = QString("%inm_$iC_mag_%04i_mult.tec").arg(size).arg(temperature).arg(index);

  QDir parentDir(mRootDir);

  parentDir.cd(material);
  parentDir.cd(geometry);
  parentDir.cd(strSize);
  parentDir.cd(strTemp);

  return parentDir.canonicalPath() + QDir::separator() + tecFile; 
}
