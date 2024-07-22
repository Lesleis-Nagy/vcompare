/**
 * \file   Convert.h 
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

#ifndef CONVERT_H_
#define CONVERT_H_

#include <QString>
#include <QRegularExpression>

#include "Validate.h"

class Convert
{
public:

  Convert() :
    mRegexSizeNanoMeter  (Validate::strRegexSizeNanoMeter),
    mRegexSizeMicroMeter (Validate::strRegexSizeMicroMeter),
    mRegexTempCelsius    (Validate::strRegexTempCelsius),
    mRegexTempFarenheit  (Validate::strRegexTempFarenheit),
    mRegexTempKelvin     (Validate::strRegexTempKelvin) 
  {}

  int nanoMeterToInt  (QString size, bool & status);
  int microMeterToInt (QString size, bool & status);
  int celsiusToInt    (QString temp, bool & status);
  int farenheitToInt  (QString temp, bool & status);
  int kelvinToInt     (QString temp, bool & status);

  QString nanoMeterToString  (int size, bool & status);
  QString microMeterToString (int size, bool & status);
  QString celsiusToString    (int temp, bool & status);
  QString farenheitToString  (int temp, bool & status);
  QString kelvinToString     (int temp, bool & status);

private:
  QRegularExpression mRegexSizeNanoMeter;
  QRegularExpression mRegexSizeMicroMeter;
  QRegularExpression mRegexTempCelsius;
  QRegularExpression mRegexTempFarenheit;
  QRegularExpression mRegexTempKelvin;
};


#endif  // CONVERT_H_
