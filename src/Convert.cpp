/**
 * \file   Convert.cxx 
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

#include "Convert.h"


int Convert::nanoMeterToInt(QString size, bool & status)
{
  QRegularExpressionMatch match = mRegexSizeNanoMeter.match(size);
  bool exactMatch = match.hasMatch()
      && (match.capturedStart() == 0)
      && (match.capturedLength() == size.length());

  if (exactMatch) {
    QString sz = match.captured(1);
    status = true;
    return sz.toInt();
  }
  status = false;
  return 0;
}

int Convert::microMeterToInt(QString size, bool & status) 
{
  QRegularExpressionMatch match = mRegexSizeMicroMeter.match(size);
  bool exactMatch = match.hasMatch()
      && (match.capturedStart() == 0)
      && (match.capturedLength() == size.length());

  if (exactMatch) {
    QString sz = match.captured(1);
    status = true;
    return sz.toInt();
  }
  status = false;
  return 0;
}

int Convert::celsiusToInt(QString temp, bool & status)
{
  QRegularExpressionMatch match = mRegexTempCelsius.match(temp);
  bool exactMatch = match.hasMatch()
      && (match.capturedStart() == 0)
      && (match.capturedLength() == temp.length());

  if (exactMatch) {
    QString sz = match.captured(1);;
    status = true;
    return sz.toInt();
  }
  status = false;
  return 0;
}

int Convert::farenheitToInt(QString temp, bool & status)
{
  QRegularExpressionMatch match = mRegexTempFarenheit.match(temp);
  bool exactMatch = match.hasMatch()
      && (match.capturedStart() == 0)
      && (match.capturedLength() == temp.length());

  if (exactMatch) {
    QString sz = match.captured(1);
    status = true;
    return sz.toInt();
  }
  status = false;
  return 0;
}

int Convert::kelvinToInt(QString temp, bool & status)
{
  QRegularExpressionMatch match = mRegexTempKelvin.match(temp);
  bool exactMatch = match.hasMatch()
      && (match.capturedStart() == 0)
      && (match.capturedLength() == temp.length());

  if (exactMatch) {
    QString sz = match.captured(1);
    status = true;
    return sz.toInt();
  }
  status = false;
  return 0;
}

QString Convert::nanoMeterToString(int size, bool & status) {
  status = true;
  return QString::number(size) + "nm";
}

QString Convert::microMeterToString(int size, bool & status) {
  status = true;
  return QString::number(size) + "um";
}

QString Convert::celsiusToString(int size, bool & status) {
  status = true;
  return QString::number(size) + "C";
}

QString Convert::farenheitToString(int size, bool & status) {
  status = true;
  return QString::number(size) + "F";
}

QString Convert::kelvinToString(int size, bool & status) {
  status = true;
  return QString::number(size) + "K";
}
