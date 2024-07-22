/**
 * \file   TecplotLoader.cpp
 * \author L. Nagy 
 *
 * Copyright [2016] Lesleis Nagy. All rights reserved.
 */

#include "TecplotLoader.h"
#include "DebugMacros.h"

#include <cstdio>

TecplotLoader::TecplotLoader() {
  using std::string;
  using std::stringstream;

  /* Regular expression strings */
  string strFloat    = "[-+]?[0-9]*.?[0-9]+([eE][-+]?[0-9]+)?";
  string strInt      = "[-+]?[0-9]+";
  string strUint     = "[0-9]+";
  string strZeroOne  = "[0-1]+";

  /* Strings holding complete regular expressions */
  stringstream ssTitle;
  stringstream ssZone;
  stringstream ssVars;
  stringstream ssFem;
  stringstream ssVertAndField;
  stringstream ssTet;
  stringstream ssF;
  stringstream ssZoneAndFem;

  /* Create the strings holding regular expressions */
  ssTitle << "^[[:space:]]*TITLE[[:space:]]*=[[:space:]]*(.+)$";

  ssZone  << "^[[:space:]]*ZONE[[:space:]]+"
          << "T[[:space:]]*=[[:space:]]*\"(.+)\"[[:space:]]*"
          << "N[[:space:]]*=[[:space:]]*([0-9]+)[[:space:]]*,[[:space:]]*"
          << "E[[:space:]]*=[[:space:]]*([0-9]+)[[:space:]]*$";

  ssVars  << "^[[:space:]]*VARIABLES[[:space:]]*=[[:space:]]*(.+)[[:space:]]*$";

  ssFem   << "^[[:space:]]*F[[:space:]]*=[[:space:]]*FEPOINT,"
          << "[[:space:]]+ET[[:space:]]*=[[:space:]]*TETRAHEDRON[[:space:]]*$";

  ssZoneAndFem << "^[[:space:]]*ZONE[[:space:]]+"
               << "T[[:space:]]*=[[:space:]]*\"(.+)\"[[:space:]]*,?[[:space:]]*"
               << "N[[:space:]]*=[[:space:]]*([0-9]+)[[:space:]]*,?[[:space:]]*"
               << "E[[:space:]]*=[[:space:]]*([0-9]+)[[:space:]]*,?[[:space:]]*"
               << "F[[:space:]]*=[[:space:]]*FEPOINT[[:space:]]*,?[[:space:]]*"
               << "[[:space:]]+ET[[:space:]]*=[[:space:]]*TETRAHEDRON[[:space:]]*$";

  //std::cout << "Zone and FEM: " << ssZoneAndFem.str() << std::endl;

  ssVertAndField << "^[[:space:]]*";
  ssVertAndField << "(" << strFloat << ")[[:space:]]+";
  ssVertAndField << "(" << strFloat << ")[[:space:]]+";
  ssVertAndField << "(" << strFloat << ")[[:space:]]+";
  ssVertAndField << "(" << strFloat << ")[[:space:]]+";
  ssVertAndField << "(" << strFloat << ")[[:space:]]+";
  ssVertAndField << "(" << strFloat << ")[[:space:]]*$";

  //std::cout << "Vert and field: " << ssVertAndField.str() << std::endl;

  ssTet   << "^[[:space:]]*";
  ssTet   << "(" << strUint << ")[[:space:]]+";
  ssTet   << "(" << strUint << ")[[:space:]]+";
  ssTet   << "(" << strUint << ")[[:space:]]+";
  ssTet   << "(" << strUint << ")[[:space:]]*$";

  ssF     << "^[[:space:]]*";
  ssF     << "(" << strFloat << ")[[:space:]]+";
  ssF     << "(" << strFloat << ")[[:space:]]+";
  ssF     << "(" << strFloat << ")[[:space:]]*$";

  // Regular expression objects.
  int retiTitle = regcomp(&m_regexTitle, ssTitle.str().c_str(), REG_EXTENDED);
  if (retiTitle) {
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexTitle");
  }

  int retiZone = regcomp(&m_regexZone, ssZone.str().c_str(), REG_EXTENDED);
  if (retiZone) {
    // Delete all regexs before.
    regfree(&m_regexTitle);
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexZone");
  }

  int retiVars = regcomp(&m_regexVars, ssVars.str().c_str(), REG_EXTENDED);
  if (retiVars) {
    // Delete all regexs before.
    regfree(&m_regexTitle);
    regfree(&m_regexZone);
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexVars");
  }

  int retiFem = regcomp(&m_regexFem, ssFem.str().c_str(), REG_EXTENDED);
  if (retiFem) {
    // Delete all regexs before.
    regfree(&m_regexTitle);
    regfree(&m_regexZone);
    regfree(&m_regexVars);
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexFem");
  }

  int retiVandF = regcomp(&m_regexVertAndField, ssVertAndField.str().c_str(),
                          REG_EXTENDED);
  if (retiVandF) {
    // Delete all regexs before.
    regfree(&m_regexTitle);
    regfree(&m_regexZone);
    regfree(&m_regexVars);
    regfree(&m_regexFem);
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexVandF");
  }

  int retiTet = regcomp(&m_regexTet, ssTet.str().c_str(), REG_EXTENDED);
  if (retiTet) {
    // Delete all regexs before.
    regfree(&m_regexTitle);
    regfree(&m_regexZone);
    regfree(&m_regexVars);
    regfree(&m_regexFem);
    regfree(&m_regexVertAndField);
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexTet");
  }

  int retiF = regcomp(&m_regexF, ssF.str().c_str(), REG_EXTENDED);
  if (retiF) {
    // Delete all regexs before.
    regfree(&m_regexTitle);
    regfree(&m_regexZone);
    regfree(&m_regexVars);
    regfree(&m_regexFem);
    regfree(&m_regexVertAndField);
    regfree(&m_regexTet);
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexF");
  }

  int retiZoneAndFem = regcomp(&m_regexZoneAndFem, ssZoneAndFem.str().c_str(), REG_EXTENDED);
  if (retiZoneAndFem) {
    // Delete all regexs before.
    regfree(&m_regexTitle);
    regfree(&m_regexZone);
    regfree(&m_regexVars);
    regfree(&m_regexFem);
    regfree(&m_regexVertAndField);
    regfree(&m_regexTet);
    regfree(&m_regexF);
    // Throw exception.
    throw TecplotRegexCompilationException("m_regexZoneAndFem");
  }

}

void TecplotLoader::header(std::string const & fileName,
                           size_t            & nvert,
                           size_t            & nelem,
                           size_t            & nzone) {
  using std::string;
  using std::ifstream;

  string line;

  nzone = 0;

  ifstream file(fileName.c_str());
  bool foundHeader = false;
  if (file.is_open()) {
    while (file) {
      getline(file, line);
      if (parseZone(line, nvert, nelem)) {
        foundHeader = true;
        (nzone)++;
      }
    }
    file.close();
    if (!foundHeader) {
      throw TecplotHeaderNotFoundException();
    }
  } else {
    throw TecplotFileAccessException();
  }
}

// fileIndexing is the type of indexing in used in the file (zero/one indexing).
// Whatever the type, this class will convert to zero indexing internally, it
// is then the responsibility of external classes to correct the indexing.
void TecplotLoader::load(std::string        const & fileName,
                         SourceFileIndexing         fileIndexing,
                         VertexField3d            & vcoord,
                         ConnectIndices4          & eindex,
                         VectorFields3d           & fields) {
  using std::string;
  using std::vector;
  using std::ifstream;

  loaderStatus state = START;

  string line;

  // Retrive no of vertices and elements.
  size_t nvert = 0, nelem = 0, nzone = 0;

  /* Open file */
  ifstream file(fileName.c_str());

  vector<double> vcl;
  vector<uint>   til;
  vector<double> fld;

  if (file.is_open()) {
    double vx, vy, vz, fx, fy, fz;
    uint n0, n1, n2, n3;
    string title, vars;

    nzone = 0;
    while (file) {
      getline(file, line);

      if (!line.empty()) {
        switch (state) {
          case START:
            if (parseVars(line, vars)) {
              // Everything is OK - do nothing.
            } else if (parseTitle(line, title)) {
              // Everything is OK - do nothing.
            } else if (parseZone(line, nvert, nelem)) {
              // Everything is OK - switch state
              state = ZONE1;
              nzone = nzone + 1;
            } else {
              // In state 'START' but matched something that shouldn't be there.
              ERROR("In state 'START' but matched something that shouldn't be there.");
              ERROR("Failing line '" << line << "'");
              throw TecplotFileParseException("218");
            }
            break;
          case ZONE1:
            if (parseFem(line)) {
              // Everything is OK - switch state
              state = FEM1;
            } else {
              ERROR("In state 'ZONE1' but matched something that shouldn't be there.");
              ERROR("Failing line '" << line << "'");
              throw TecplotFileParseException("228");
            }
            break;
          case FEM1:
            if (parseVertAndField(line, vx, vy, vz, fx, fy, fz)) {
              // Everything is OK, push data.
              vcl.push_back(vx);
              vcl.push_back(vy);
              vcl.push_back(vz);

              fld.push_back(fx);
              fld.push_back(fy);
              fld.push_back(fz);
            } else if (parseTet(line, n0, n1, n2, n3)) {
              // Everything is OK, push data.
              switch (fileIndexing) {
                case ZERO_INDEXING:
                  til.push_back(n0);
                  til.push_back(n1);
                  til.push_back(n2);
                  til.push_back(n3);
                  break;
                case ONE_INDEXING:
                  til.push_back(n0-1);
                  til.push_back(n1-1);
                  til.push_back(n2-1);
                  til.push_back(n3-1);
                  break;
                default:
                  throw TecplotFileParseException("257");
                  break;
              }
            } else if (parseZone(line, nvert, nelem)) {
              // Everything is OK, switch state.
              state = ZONE2;
              nzone = nzone + 1;
            } else {
              // In state 'FEM1' but matched something that shouldn't be there.
              ERROR("In state 'FEM1' but matched something that shouldn't be there.");
              ERROR("Failing line '" << line << "'");
              throw TecplotFileParseException("268");
            }
            break;
          case ZONE2:
            if (parseFem(line)) {
              // Everything is OK, switch state.
              state = FEM2;
            } else {
              // In state 'ZONE2' but matched something that shouldn't be there.
              ERROR("In state 'ZONE2' but matched something that shouldn't be there.");
              ERROR("Failing line '" << line << "'");
              throw TecplotFileParseException("278");
            }
            break;
          case FEM2:
            if (parseF(line, vx, vy, vz)) {
              // Everything is OK, push data.
              fld.push_back(vx);
              fld.push_back(vy);
              fld.push_back(vz);
            } else if (parseZone(line, nvert, nelem)) {
              // Everything is OK, switch state.
              state = ZONE2;
              nzone = nzone + 1;
            } else {
              // In state 'FEM2' but matched something that shouldn't be there.
              ERROR("In state 'FEM2' but matched something that shouldn't be there.");
              ERROR("Failing line '" << line << "'");
              throw TecplotFileParseException("294");
            }
            break;
          default:
            // Entered an unknown state.
            ERROR("Entered an unknown state.");
            ERROR("Failing line '" << line << "'");
            throw TecplotFileParseException("300");
            break;
        }
      }
    }
    file.close();

    // Check that we've parsed the correct number of vertices.
    if (vcl.size() != (size_t)(3*nvert)) {
      throw TecplotVertexMismatchException();
    }

    // Check that we've parsed the correct number of feld points.
    if (fld.size() != (size_t)(3*nvert*nzone)) {
      throw TecplotFieldMismatchException();
    }

    // Check that we've parsed the correct number of elements.
    if (til.size() != (size_t)(4*nelem)) {
      throw TecplotElementMismatchException();
    }

    // Copy over the data in to the output arrays.
    vcoord.resize(nvert);
    eindex.resize(nelem);
    fields.resize(nzone);
    for (auto field : fields) {
      field.resize(nvert);
    }

    for (uint i = 0; i < nvert; ++i) {
      vcoord.at(i).x = vcl[3*i + 0];
      vcoord.at(i).y = vcl[3*i + 1];
      vcoord.at(i).z = vcl[3*i + 2];
    }
    for (uint k = 0; k < nzone; ++k) {
      for (uint i = 0; i < nvert; ++i) {
        fields.at(k).at(i).x = fld[3*nvert*k + 3*i + 0];
        fields.at(k).at(i).y = fld[3*nvert*k + 3*i + 1];
        fields.at(k).at(i).z = fld[3*nvert*k + 3*i + 2];
      }
    }
    for (uint i = 0; i < nelem; ++i) {
      eindex.at(i).n0 = til[4*i + 0];
      eindex.at(i).n1 = til[4*i + 1];
      eindex.at(i).n2 = til[4*i + 2];
      eindex.at(i).n3 = til[4*i + 3];
    }
  } else {
    throw TecplotFileAccessException();
  }
}

// fileIndexing is the type of indexing in used in the file (zero/one indexing).
// Whatever the type, this class will convert to zero indexing internally, it
// is then the responsibility of external classes to correct the indexing.
void TecplotLoader::load(std::string        const & fileName,
                         SourceFileIndexing         fileIndexing,
                         VertexField3d            & vcoord,
                         ConnectIndices4          & eindex,
                         VectorField3d            & field) {
  using std::string;
  using std::vector;
  using std::ifstream;

  loaderStatus state = START;

  string line;

  // Retrive no of vertices and elements.
  size_t nvert = 0, nelem = 0; //, nzone = 0;

  /* Open file */
  ifstream file(fileName.c_str());

  vector<double> vcl;
  vector<uint>   til;
  vector<double> fld;

  if (file.is_open()) {
    double vx, vy, vz, fx, fy, fz;
    uint n0, n1, n2, n3;
    string title, vars;

    // nzone = 0;
    while (file) {
      getline(file, line);

      //std::cout << "About to parse line: " << line << std::endl;

      if (!line.empty()) {
        switch (state) {
          case START:
            if (parseVars(line, vars)) {
              // Everything is OK - do nothing.
              //std::cout << "Parsed variables: " << line << std::endl;
            } else if (parseTitle(line, title)) {
              // Everything is OK - do nothing.
              //std::cout << "Parsed title: " << line << std::endl;
            } else if (parseZoneAndFem(line, nvert, nelem)) {
              // Everything is OK - switch state
              //std::cout << "Parsed zone and FEM (switching state): " << line << std::endl;
              state = FEM1;
            } else {
              // In state 'START' but matched something that shouldn't be there.
              ERROR("In state 'START' but matched something that shouldn't be there.");
              ERROR("Failing line '" << line << "'");
              throw TecplotFileParseException("218");
            }
            break;
          case FEM1:
            if (parseVertAndField(line, vx, vy, vz, fx, fy, fz)) {
              // Everything is OK, push data.
              vcl.push_back(vx);
              vcl.push_back(vy);
              vcl.push_back(vz);

              fld.push_back(fx);
              fld.push_back(fy);
              fld.push_back(fz);

              /*
              std::cout << "r: (" << vx << ", " << vy << ", " << vz 
                        << ") <-->"
                        << "f: (" << fx << ", " << fy << ", " << fz
                        << ")"
                        << std::endl;
               */

            } else if (parseTet(line, n0, n1, n2, n3)) {
              // Everything is OK, push data.
              switch (fileIndexing) {
                case ZERO_INDEXING:
                  til.push_back(n0);
                  til.push_back(n1);
                  til.push_back(n2);
                  til.push_back(n3);
                  break;
                case ONE_INDEXING:
                  til.push_back(n0-1);
                  til.push_back(n1-1);
                  til.push_back(n2-1);
                  til.push_back(n3-1);
                  break;
                default:
                  throw TecplotFileParseException("257");
                  break;
              }
            } else {
              // In state 'FEM1' but matched something that shouldn't be there.
              ERROR("In state 'FEM1' but matched something that shouldn't be there.");
              ERROR("Failing line '" << line << "'");
              throw TecplotFileParseException("268");
            }
            break;
          default:
            // Entered an unknown state.
            ERROR("Entered an unknown state.");
            ERROR("Failing line '" << line << "'");
            throw TecplotFileParseException("300");
            break;
        }
      }
    }
    file.close();

    // Check that we've parsed the correct number of vertices.
    if (vcl.size() != (size_t)(3*nvert)) {
      throw TecplotVertexMismatchException();
    }

    // Check that we've parsed the correct number of feld points.
    if (fld.size() != (size_t)(3*nvert)) {
      throw TecplotFieldMismatchException();
    }

    // Check that we've parsed the correct number of elements.
    if (til.size() != (size_t)(4*nelem)) {
      throw TecplotElementMismatchException();
    }

    // Copy over the data in to the output arrays.
    vcoord.resize(nvert);
    field.resize(nvert);
    eindex.resize(nelem);

    for (uint i = 0; i < nvert; ++i) {
      vcoord.at(i).x = vcl[3*i + 0];
      vcoord.at(i).y = vcl[3*i + 1];
      vcoord.at(i).z = vcl[3*i + 2];
    }
    for (uint i = 0; i < nvert; ++i) {
      field.at(i).x = fld[3*i + 0];
      field.at(i).y = fld[3*i + 1];
      field.at(i).z = fld[3*i + 2];
    }
    for (uint i = 0; i < nelem; ++i) {
      eindex.at(i).n0 = til[4*i + 0];
      eindex.at(i).n1 = til[4*i + 1];
      eindex.at(i).n2 = til[4*i + 2];
      eindex.at(i).n3 = til[4*i + 3];
    }
  } else {
    throw TecplotFileAccessException();
  }
}

bool TecplotLoader::parseTitle(std::string const & line,
                               std::string       & title) {
  using std::vector;

  size_t nmatches = 10;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexTitle, cline, nmatches, &matches[0], 0);
  if (!reti) {
    title = extract_string(cline, &matches[1]);

    return true;
  }

  return false;
}

bool TecplotLoader::parseZone(std::string const & line,
                              size_t            & nvert,
                              size_t            & nelem) {
  using std::vector;

  size_t nmatches = 10;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexZone, cline, nmatches, &matches[0], 0);
  if (!reti) {
    nvert = extract_size_t(cline, &matches[2]);
    nelem = extract_size_t(cline, &matches[3]);

    return true;
  }

  return false;
}

bool TecplotLoader::parseVars(std::string const & line,
                              std::string       & vars) {
  using std::vector;

  size_t nmatches = 10;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexVars, cline, nmatches, &matches[0], 0);
  if (!reti) {
    vars = extract_string(cline, &matches[1]);

    return true;
  }

  return false;
}

bool TecplotLoader::parseFem(std::string const &line) {
  using std::vector;

  size_t nmatches = 10;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexFem, cline, nmatches, &matches[0], 0);
  if (!reti) {
    return true;
  }

  return false;
}

bool TecplotLoader::parseVertAndField(std::string const & line,
                                      double            & vx,
                                      double            & vy,
                                      double            & vz,
                                      double            & fx,
                                      double            & fy,
                                      double            & fz) {
  using std::vector;

  size_t nmatches = 20;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexVertAndField, cline, nmatches, &matches[0], 0);
  //std::cout << "nmatches: " << nmatches << std::endl;

  if (!reti) {
    //std::cout << "the bad line: " << cline << std::endl;
    vx = extract_double(cline, &matches[ 1]);
    vy = extract_double(cline, &matches[ 3]);
    vz = extract_double(cline, &matches[ 5]);
    fx = extract_double(cline, &matches[ 7]);
    fy = extract_double(cline, &matches[ 9]);
    fz = extract_double(cline, &matches[11]);

    return true;
  }

  return false;
}

bool TecplotLoader::parseTet(std::string const & line,
                             uint              & n0,
                             uint              & n1,
                             uint              & n2,
                             uint              & n3) {

  using std::vector;

  size_t nmatches = 10;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexTet, cline, nmatches, &matches[0], 0);
  if (!reti) {
    n0 = extract_uint(cline, &matches[1]);
    n1 = extract_uint(cline, &matches[2]);
    n2 = extract_uint(cline, &matches[3]);
    n3 = extract_uint(cline, &matches[4]);

    return true;
  }

  return false;
}

bool TecplotLoader::parseF(std::string const & line,
                           double            & fx,
                           double            & fy,
                           double            & fz) {
  using std::vector;

  size_t nmatches = 10;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexF, cline, nmatches, &matches[0], 0);
  if (!reti) {
    fx = extract_double(cline, &matches[1]);
    fy = extract_double(cline, &matches[3]);
    fz = extract_double(cline, &matches[5]);

    return true;
  }

  return false;
}

bool TecplotLoader::parseZoneAndFem(std::string const & line,
                                    size_t            & nvert,
                                    size_t            & nelem) {
  using std::vector;

  size_t nmatches = 10;

  vector<regmatch_t> matches(nmatches);

  const char * cline = line.c_str();

  int reti = regexec(&m_regexZoneAndFem, cline, nmatches, &matches[0], 0);
  if (!reti) {
    nvert = extract_size_t(cline, &matches[2]);
    nelem = extract_size_t(cline, &matches[3]);

    return true;
  }

  return false;
}

double TecplotLoader::extract_double(const char * line,
                                     regmatch_t * pmatch) {
  size_t   nline = 1024;
  int      i     = 0;
  int      j     = 0;
  char   * end   = NULL;

  char  str_val[nline];

  for (i = pmatch->rm_so; i < pmatch->rm_eo; ++i) {
    str_val[j] = line[i];
    j = j + 1;
  }
  str_val[j] = 0;

  return strtod(str_val, &end);
}

uint TecplotLoader::extract_uint(const char * line,
                                 regmatch_t * pmatch) {
  size_t   nline = 1024;
  int      i     = 0;
  int      j     = 0;
  char   * end   = NULL;

  char    str_val[nline];

  for (i = pmatch->rm_so; i < pmatch->rm_eo; ++i) {
    str_val[j] = line[i];
    j = j + 1;
  }
  str_val[j] = 0;

  return strtoul(str_val, &end, 10);
}

size_t TecplotLoader::extract_size_t(const char * line,
                                     regmatch_t * pmatch) {
  size_t   nline = 1024;
  int      i     = 0;
  int      j     = 0;
  char   * end   = NULL;

  char    str_val[nline];

  for (i = pmatch->rm_so; i < pmatch->rm_eo; ++i) {
    str_val[j] = line[i];
    j = j + 1;
  }
  str_val[j] = 0;

  return (size_t)strtoul(str_val, &end, 10);
}

std::string TecplotLoader::extract_string(const char * line,
                                          regmatch_t * pmatch) {
  using std::string;

  size_t   nline = 1024;
  int      i     = 0;
  int      j     = 0;

  char    str_val[nline];

  for (i = pmatch->rm_so; i < pmatch->rm_eo; ++i) {
    str_val[j] = line[i];
    j = j + 1;
  }
  str_val[j] = 0;

  return string(str_val);
}
