/**
 * \file   Validate.h
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

#ifndef VALIDATE_H_
#define VALIDATE_H_

#include <QString>
#include <QRegularExpression>

class Validate
{
public:
  static const QString strRegexSizeNanoMeter;
  static const QString strRegexSizeMicroMeter;
  static const QString strRegexTempCelsius;
  static const QString strRegexTempFarenheit;
  static const QString strRegexTempKelvin;

  Validate() :
    mRegexSizeNanoMeter(strRegexSizeNanoMeter),
    mRegexSizeMicroMeter(strRegexSizeMicroMeter),
    mRegexTempCelsius(strRegexTempCelsius),
    mRegexTempFarenheit(strRegexTempFarenheit),
    mRegexTempKelvin(strRegexTempKelvin) 
  {}

  bool isValidSizeNanoMeter(QString size) 
  {
    QRegularExpressionMatch match = mRegexSizeNanoMeter.match(size);
    return match.hasMatch()
      && (match.capturedStart() == 0)
      && (match.capturedLength() == size.length());
  }

  bool isValidSizeMicroMeter(QString size)
  {
    QRegularExpressionMatch match = mRegexSizeMicroMeter.match(size);
    return match.hasMatch()
        && (match.capturedStart() == 0)
        && (match.capturedLength() == size.length());
  }

  bool isValidTempCelsius(QString temp)
  {
    QRegularExpressionMatch match = mRegexTempCelsius.match(temp);
    return match.hasMatch()
        && (match.capturedStart() == 0)
        && (match.capturedLength() == temp.length());
  }

  bool isValidTempFarenheit(QString temp)
  {
    QRegularExpressionMatch match = mRegexTempFarenheit.match(temp);
    return match.hasMatch()
        && (match.capturedStart() == 0)
        && (match.capturedLength() == temp.length());
  }

  bool isValidTempKelvin(QString temp)
  {
    QRegularExpressionMatch match = mRegexTempKelvin.match(temp);
    return match.hasMatch()
        && (match.capturedStart() == 0)
        && (match.capturedLength() == temp.length());
  }

private:
  QRegularExpression mRegexSizeNanoMeter;
  QRegularExpression mRegexSizeMicroMeter;
  QRegularExpression mRegexTempCelsius;
  QRegularExpression mRegexTempFarenheit;
  QRegularExpression mRegexTempKelvin;
};

#endif  // VALIDATE_H_
