/**
 * \file   DirectoryDatabase.hpp
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

#ifndef DIRECTORY_DATABASE_HPP_
#define DIRECTORY_DATABASE_HPP_

#include <QDir>
#include <QFile>
#include <QRegularExpression>
#include <QString>
#include <QStringList>
#include <QTextStream>

#include <vector>
#include <utility>

#include "Utilities.h"
#include "DebugMacros.h"

class FieldDataHasher
{
public:
  const static QString strRegexStdoutFile;
  const static QString strRegexFieldFile;

  FieldDataHasher() :
    mRegexStdoutFile(strRegexStdoutFile),
    mRegexFieldFile(strRegexFieldFile)
  {}

  size_t operator() (const QString & file) {
    size_t seed = 0;
    bool status = false;

    seed = stdoutFile(file, status);
    if (status == true) {
      return seed;
    }

    seed = fieldFile(file, status);
    if (status == true) {
      return seed;
    }

    return seed;
  }

  size_t stdoutFile(const QString & stdoutFile, bool & status) {
    size_t seed = 0;
    status = true;

    if (stdoutFile.contains(mRegexStdoutFile)) {
      int size        = mRegexStdoutFile.captures(1).toInt();
      int temperature = mRegexStdoutFile.cap(2).toInt();
      int index       = mRegexStdoutFile.cap(3).toInt();

      hash_combine(seed, size);
      hash_combine(seed, temperature);
      hash_combine(seed, index);
    } else {
      status = false;
    }

    return seed;
  }

  size_t fieldFile(const QString & fieldFile, bool & status) {
    size_t seed = 0;
    status = true;

    if (fieldFile.contains(mRegexFieldFile)) {
      int size        = mRegexFieldFile.cap(1).toInt();
      int temperature = mRegexFieldFile.cap(2).toInt();
      int index       = mRegexFieldFile.cap(3).toInt();

      hash_combine(seed, size);
      hash_combine(seed, temperature);
      hash_combine(seed, index);
    } else { 
      status = false;
    }

    return seed;
  }

private:
  QRegularExpression mRegexStdoutFile;
  QRegularExpression mRegexFieldFile;
};

class StartEndPair
{
public:

  //const static QRegExp reDatFile;
  //const static QRegExp reTecFile;

  StartEndPair(QString start, QString end, QString name, QString absLoc = "") :
    mName(name), mAbsLoc(absLoc)
  {
    QRegExp reNameIndex("START_END(_([0-9]{4}))?");
    if (reNameIndex.exactMatch(mName)) {
      DEBUG("index: '" << reNameIndex.cap(2).toUtf8().constData() << "'");
      mNameIndex = reNameIndex.cap(2).toInt();
    } else {
      ERROR("Start/end 'name' invalid format!");
    }

    updateStartEndPair(start, end);
    DEBUG("  name        :  " << mName.toUtf8().constData());
    DEBUG("  name's index:  " << mNameIndex);
  }

  StartEndPair(QString start, QString end, unsigned int nameIndex, QString absLoc = "") :
    mNameIndex(nameIndex), mAbsLoc(absLoc)
  {
    if (nameIndex > 0) {
      mName = QString("START_END_%1").arg(mNameIndex, 4, 10, QChar('0'));
    } else {
      mName = QString("START_END");
    }

    updateStartEndPair(start, end);
    DEBUG("  name        :  " << mName.toUtf8().constData());
    DEBUG("  name's index:  " << mNameIndex);
  }

  void updateStartEndPair(QString start, QString end) {
    if (!extractDatFileNameData(start, mStartSize, mStartTemp, mStartIndex)) {
      INFO("Didn't parse 'start' as 'dat' filename, attempting 'tec'");
      if (!extractTecFileNameData(start, mStartSize, mStartTemp, mStartIndex)) {
        ERROR("Could not parse start as 'dat' or 'tec' file");
      } else {
        INFO("Parsed start as 'tec' file");
      }
    }

    if (!extractDatFileNameData(end, mEndSize, mEndTemp, mEndIndex)) {
      INFO("Didn't parse 'end' as 'dat' filename, attempting 'tec'");
      if (!extractTecFileNameData(end, mEndSize, mEndTemp, mEndIndex)) {
        ERROR("Could not parse end as 'dat' or 'tec' file");
      }else {
        INFO("Parsed end as 'tec' file");
      }
    }

    DEBUG("StartEndPair summary: ");
    DEBUG("  start size:  " << mStartSize);
    DEBUG("  start temp:  " << mStartTemp);
    DEBUG("  start index: " << mStartIndex);

    DEBUG("StartEndPair summary: ");
    DEBUG("  end size:  " << mEndSize);
    DEBUG("  end temp:  " << mEndTemp);
    DEBUG("  end index: " << mEndIndex);
  }

  void writeToFile() {
    QFile fabs(mAbsLoc + QDir::separator() + mName);
    QFileInfo fabsInfo(fabs);
    if (fabs.open(QFile::WriteOnly | QFile::Truncate)) {
      QTextStream fout(&fabs);
      INFO("Writing " << startAsDatFile().toUtf8().constData() 
           << ", " << endAsDatFile().toUtf8().constData() << " to "
           << fabsInfo.absoluteFilePath().toUtf8().constData());
      fout << startAsDatFile() << "\n";
      fout << endAsDatFile() << "\n";
    }
    fabs.close();
  }

  void deleteFile() {
    QFile fabs(mAbsLoc + QDir::separator() + mName);
    QFileInfo fabsInfo(fabs);
    if (fabs.exists()) {
      INFO("Deleting : " << fabsInfo.absoluteFilePath().toUtf8().constData());
      fabs.remove();
    } else {
      WARNING("Could not find : " <<
              fabsInfo.absoluteFilePath().toUtf8().constData() << " to delete");
    }
  }

  QString startAsDatFile(bool absPath=false) const 
  {
    if (absPath) {
      return mAbsLoc + QDir::separator() + QString("%1nm_%2C_mag_%3.dat")
        .arg(mStartSize).arg(mStartTemp).arg(mStartIndex, 4, 10, QChar('0'));
    } else {
      return QString("%1nm_%2C_mag_%3.dat")
        .arg(mStartSize).arg(mStartTemp).arg(mStartIndex, 4, 10, QChar('0'));
    }
  }

  QString startAsTecFile(bool absPath=false) const 
  {
    if (absPath) {
      return mAbsLoc + QDir::separator() + QString("%1nm_%2C_mag_%3_mult.tec")
        .arg(mStartSize).arg(mStartTemp).arg(mStartIndex, 4, 10, QChar('0'));
    } else { 
      return QString("%1nm_%2C_mag_%3_mult.tec")
        .arg(mStartSize).arg(mStartTemp).arg(mStartIndex, 4, 10, QChar('0'));
    }
  }

  QString endAsDatFile(bool absPath=false) const 
  {
    if (absPath) {
      return mAbsLoc + QDir::separator() + QString("%1nm_%2C_mag_%3.dat")
        .arg(mEndSize).arg(mEndTemp).arg(mEndIndex, 4, 10, QChar('0'));
    } else { 
      return QString("%1nm_%2C_mag_%3.dat")
        .arg(mEndSize).arg(mEndTemp).arg(mEndIndex, 4, 10, QChar('0'));
    }
  }

  QString endAsTecFile(bool absPath=false) const 
  {
    if (absPath) {
      return mAbsLoc + QDir::separator() + QString("%1nm_%2C_mag_%3_mult.tec")
        .arg(mEndSize).arg(mEndTemp).arg(mEndIndex, 4, 10, QChar('0'));
    } else {
      return QString("%1nm_%2C_mag_%3_mult.tec")
        .arg(mEndSize).arg(mEndTemp).arg(mEndIndex, 4, 10, QChar('0'));
    }
  }

  QString name() const { return mName; }

  unsigned int startSize()  const { return mStartSize; }
  unsigned int startTemp()  const { return mStartTemp; }
  unsigned int startIndex() const { return mStartIndex; }

  unsigned int endSize()  const { return mEndSize; }
  unsigned int endTemp()  const { return mEndTemp; }
  unsigned int endIndex() const { return mEndIndex; }

  unsigned int nameIndex() const {return mNameIndex; }

private:
  QString      mAbsLoc;
  QString      mName;
  unsigned int mNameIndex;

  unsigned int mStartSize;
  unsigned int mStartTemp;
  unsigned int mStartIndex;

  unsigned int mEndSize;
  unsigned int mEndTemp;
  unsigned int mEndIndex;

  bool extractDatFileNameData(const QString & filename, 
                              unsigned int  & size,
                              unsigned int  & temp,
                              unsigned int  & index)
  {
    QRegExp reDatFile("([0-9]+)nm_([0-9]+)C_mag_([0-9]{4})\\.dat");    
    DEBUG("filename: " << filename.toUtf8().constData());
    if (filename.contains(reDatFile)) {
      DEBUG("match 1: " << reDatFile.cap(1).toUtf8().constData());
      DEBUG("match 2: " << reDatFile.cap(2).toUtf8().constData());
      DEBUG("match 3: " << reDatFile.cap(3).toUtf8().constData());
      size  = reDatFile.cap(1).toInt();
      temp  = reDatFile.cap(2).toInt();
      index = reDatFile.cap(3).toInt();
      return true;
    } else {
      INFO("Input filename not dat file format");
      return false;
    }
  }

  bool extractTecFileNameData(const QString & filename, 
                              unsigned int  & size,
                              unsigned int  & temp,
                              unsigned int  & index)
  {
    QRegExp reTecFile("([0-9]+)nm_([0-9]+)C_mag_([0-9]{4})_mult\\.tec");    
    DEBUG("filename: " << filename.toUtf8().constData());
    if (filename.contains(reTecFile)) {
      DEBUG("match 1: " << reTecFile.cap(1).toUtf8().constData());
      DEBUG("match 2: " << reTecFile.cap(2).toUtf8().constData());
      DEBUG("match 3: " << reTecFile.cap(3).toUtf8().constData());
      size  = reTecFile.cap(1).toInt();
      temp  = reTecFile.cap(2).toInt();
      index = reTecFile.cap(3).toInt();
      return true;
    } else {
      INFO("Input filename not dat file format");
      return false;
    }
  }
};

class MinimizationData
{
public:
  MinimizationData(int     restartIndex, 
                   double  deltaF,
                   QString status,
                   int     ncalls,
                   double  finalEnergy) :
    mRestartIndex(restartIndex),
    mDeltaF(deltaF),
    mStatus(status),
    mNCalls(ncalls),
    mFinalEnergy(finalEnergy) 
  {}

  int     restartIndex() { return mRestartIndex; }

  double  deltaF()       { return mDeltaF; }

  QString status()       { return mStatus; }

  int     ncalls()       { return mNCalls; }

  double  finalEnergy()  { return mFinalEnergy; }

private:

  int     mRestartIndex;

  double  mDeltaF;

  QString mStatus;

  int     mNCalls;

  double  mFinalEnergy;
};

class EnergyEvaluations
{
public:

  EnergyEvaluations()
  {}

  EnergyEvaluations(QString stdoutFile)
  {
    DEBUG("PROCESSING: " << stdoutFile.toUtf8().constData());
    QRegExp reRestart("RESTART\\s*=\\s*([0-9]+)");
    QRegExp reStatusDeltaF("Delta F negligible\\s*:\\s*([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)");
    QRegExp reStatusFail("FAILED TO CONVERGE");
    QRegExp reNCalls("Energy Calls\\s*:\\s*([0-9]+)");
    QRegExp reFinalEnergy("Final Energy\\s*:\\s*([-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?)");
    QRegExp reFinished("FINISHED MINIMIZATION RESTARTS");

    int     restartIndex = -1;
    double  deltaF       = 0.0;
    QString status       = "";
    int     ncalls       = 0;
    double  finalEnergy  = 0.0;

    QFile inputFile(stdoutFile);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
         QString line = in.readLine();

         ////////////////////////
         // reRestart          //
         ////////////////////////
         if (line.contains(reRestart)) {
           // Add everything to the mRestarts list.
           if (restartIndex != -1) {
             mRestarts.push_back(
                 MinimizationData(
                   restartIndex, 
                   deltaF,
                   status,
                   ncalls,
                   finalEnergy));

             DEBUG("Inserting energy data:");
             DEBUG("  restartIndex: " << restartIndex);
             DEBUG("        deltaF: " << deltaF);
             DEBUG("        status: " << status.toUtf8().constData());
             DEBUG("        ncalls: " << ncalls);
             DEBUG("   finalEnergy: " << finalEnergy);
           }

           // Reset all restart data.
           restartIndex = reRestart.cap(1).toInt();
           deltaF       = 0.0;
           status       = "";
           ncalls       = 0;
           finalEnergy  = 0.0;

           DEBUG("MATCH: restartIndex= " << restartIndex);

           continue;
         }

         ////////////////////////
         // reStatusDeltaF     //
         ////////////////////////
         if (line.contains(reStatusDeltaF)) {
           deltaF = reStatusDeltaF.cap(1).toDouble();
           status = "CONVERGED";

           DEBUG("MATCH: deltaF=" << deltaF);
           DEBUG("MATCH: status=" << status.toUtf8().constData());

           continue;
         }

         ////////////////////////
         // reStatusFail       //
         ////////////////////////
         if (line.contains(reStatusFail)) {
           deltaF = 0.0;
           status = "FAILED";

           DEBUG("MATCH: deltaF=" << deltaF);
           DEBUG("MATCH: status=" << status.toUtf8().constData());

           continue;
         }

         ////////////////////////
         // reNCalls           //
         ////////////////////////
         if (line.contains(reNCalls)) {
           ncalls = reNCalls.cap(1).toInt();

           DEBUG("MATCH: ncalls=" << ncalls);

           continue;
         }

         ////////////////////////
         // reFinalEnergy      //
         ////////////////////////
         if (line.contains(reFinalEnergy)) {
           finalEnergy = reFinalEnergy.cap(1).toDouble();

           DEBUG("MATCH: finalEnergy=" << finalEnergy);

           continue;
         }

         ////////////////////////
         // reFinished         //
         ////////////////////////
         if (line.contains(reFinished)) {
           mRestarts.push_back(
               MinimizationData(
                 restartIndex, 
                 deltaF,
                 status,
                 ncalls,
                 finalEnergy));
           DEBUG("Inserting energy data:");
           DEBUG("  restartIndex: " << restartIndex);
           DEBUG("        deltaF: " << deltaF);
           DEBUG("        status: " << status.toUtf8().constData());
           DEBUG("        ncalls: " << ncalls);
           DEBUG("   finalEnergy: " << finalEnergy);
         }
       }
       inputFile.close();
    } else {
      ERROR("Could not open file '" << stdoutFile.toUtf8().constData() 
          << "'" << std::endl);
    }
  }

  EnergyEvaluations(const EnergyEvaluations & rhs) {
    mRestarts = rhs.mRestarts;
  }

  EnergyEvaluations& operator= (const EnergyEvaluations & rhs) {
    mRestarts = rhs.mRestarts;
    return *this;
  }

  QString str() {
    QString output;
    for (auto md : mRestarts) {
      output += "Restart: (index:" + QString::number(md.restartIndex()) + ")\n";

      if (md.status() == "CONVERGED") {
      output += "   Delta F: " + QString::number(md.deltaF()) + "\n";
      } else {
      output += "   CONVERGENCE FAILURE\n";
      }

      output += "   No. of calls: " + QString::number(md.ncalls()) + "\n";

      output += "   Final energy: " + QString::number(md.finalEnergy()) + "\n";
    }

    return output;
  }

  std::pair<double, bool> lastEnergy() {
    double energy = 0.0;
    bool valid = false;
    for (auto md : mRestarts) {
        if (md.status() == "CONVERGED") {
            energy = md.finalEnergy();
            valid = true;
        }
    }
    return std::make_pair(energy, valid);
  }

private:
  QList<MinimizationData> mRestarts;
};

class EnergyEvaluationsLookup
{
public:
  void insert(const QString & file, EnergyEvaluations evals)
  {
    mEELookup.insert(mHasher(file), evals);
  }

  EnergyEvaluations value(QString file)
  {
    return mEELookup.value(mHasher(file));
  }

  EnergyEvaluations value(std::string file)
  {
    return mEELookup.value(mHasher(QString(file.c_str())));
  }

  std::pair<double, bool> lastEnergy(QString file) 
  {
    EnergyEvaluations evals = mEELookup.value(mHasher(file));
    return evals.lastEnergy();
  }

  std::pair<double, bool> lastEnergy(std::string file)
  {
    EnergyEvaluations evals = mEELookup.value(mHasher(QString(file.c_str())));
    return evals.lastEnergy();
  }

private:
  QMap<size_t, EnergyEvaluations> mEELookup;
  FieldDataHasher                 mHasher;
};

class DirectoryDatabase
{
public:
  static const QString lems;

  static const QString strRegexFieldFile; 

  static const QString strRegexStdoutFile;

  static const QString strRegexStartEndFile;

  DirectoryDatabase() :
    mRootDir(""),
    mRegexFieldFile(strRegexFieldFile),
    mRegexStdoutFile(strRegexStdoutFile),
    mRegexStartEndFile(strRegexStartEndFile)
  {}

  QString rootDir() const { return mRootDir.canonicalPath(); }

  DirectoryDatabase(QString rootDir) :
    mRootDir(rootDir),
    mRegexFieldFile(strRegexFieldFile)
  {}

  void setRootDir(QString rootDir) {
    mRootDir = rootDir;
  }

  QStringList materialList() const;

  QStringList geometryList(
      QString material) const;

  QStringList sizeList(
      QString material,
      QString geometry) const;

  QStringList temperatureList(
      QString material,
      QString geometry,
      QString size) const;

  QStringList fieldFileList(
      QString material,
      QString geometry,
      QString size,
      QString temperature) const;

  /*
  bool hasPathStartEndPair(
      QString material,
      QString geometry,
      QString size,
      QString temperature) const;
  */

  /*
  void deletePathStartEndPair(
      QString material,
      QString geometry,
      QString size,
      QString temperature);
  */

  QList<StartEndPair> getStartEndPairs(
      QString   material,
      QString   geometry,
      QString   size,
      QString   temperature) const;

  /*
  void setStartEndPair(
      QString material,
      QString geometry,
      QString size,
      QString temperature,
      QString startPath,
      QString endPath);
  */

  EnergyEvaluationsLookup getEnergyEvaluations(
      QString material, 
      QString geometry,
      QString size,
      QString temperature) const;

  QString absPathTecplot(
      QString      material, 
      QString      geometry, 
      unsigned int size, 
      unsigned int temperature,
      unsigned int index) const;

private:
  QDir    mRootDir;
  QRegExp mRegexFieldFile;
  QRegExp mRegexStdoutFile;
  QRegExp mRegexStartEndFile;
};

#endif  // DIRECTORY_DATABASE_HPP_
