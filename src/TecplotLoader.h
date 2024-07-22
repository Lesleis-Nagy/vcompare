/**
 * \file   TecplotLoader.h
 * \author L. Nagy
 *
 * Copyright [2016] Lesleis Nagy. All rights reserved.
 */

#ifndef SRC_IO_TECPLOTLOADER_H_
#define SRC_IO_TECPLOTLOADER_H_

#include <regex.h>

#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Types.h"
#include "Data.h"
#include "Loader.h"

/**
 * \brief Exeption class for file access errors.
 */
class TecplotFileAccessException : public std::exception {
 public:
    ~TecplotFileAccessException() throw() {}

    const char* what() const throw() {
      return "Could not open tecplot file.";
    }
};

/**
 * \brief Exception class for vertex mismatch (i.e. if the number of vertices 
 *        parsed from a file is not the same as the number of vertices 
 *        reported in the file).
 */  
class TecplotVertexMismatchException : public std::exception {
 public:
    ~TecplotVertexMismatchException() throw() {}

    const char* what() const throw() {
      return "No. of vertices does not match header info in file.";
    }
};

/**
 * \brief Exception class for field mismatch (i.e. if the number of field
 *        values parsed from a file is not the same as the number of vertices
 *        reported in the file).
 */
class TecplotFieldMismatchException : public std::exception {
 public:
    ~TecplotFieldMismatchException() throw() {}

    const char* what() const throw() {
      return "No. of field points does not match header/zone info in file.";
    }
};

/**
 * \brief Exception class for element mismatch (i.e. if the number of elements 
 *        parsed from a file is not the same as the number of elements 
 *        reported in the file).
 */
class TecplotElementMismatchException : public std::exception {
 public:
    ~TecplotElementMismatchException() throw() {}

    const char* what() const throw() {
      return "No. of elements does not match header info in file.";
    }
};

/**
 * \brief Exception class thrown if header information is not found.
 */
class TecplotHeaderNotFoundException : public std::exception {
 public:
    ~TecplotHeaderNotFoundException() throw() {}

    const char* what() const throw() {
      return "Tecplot header information not found.";
    }
};

/**
 * \brief Exception class thrown if there is some parsing error.
 */
class TecplotFileParseException : public std::exception {
 public:
    TecplotFileParseException():m_info("") {}
    TecplotFileParseException(std::string info):m_info(info) {}
    ~TecplotFileParseException() throw() {}

    const char* what() const throw() {
      std::string msg("Tecplot file parse exception occured.");
      msg += m_info;
      return m_info.c_str();
    }
 private:
    std::string m_info;
};

/**
 * \brief Exception class thrown if there is a regular expression parse
 *        exeption.
 */
class TecplotRegexCompilationException : public std::exception {
 public:
   
    TecplotRegexCompilationException():m_info("") {}
    TecplotRegexCompilationException(std::string info):m_info(info) {}

    ~TecplotRegexCompilationException() throw() {}

    const char * what() const throw() {
      std::string msg("Tecplot regular expression compilation failed: ");
      msg += m_info;
      return m_info.c_str();
    }
 private:
    std::string m_info;
};

/**
 * \brief Class to load a Tecplot file.
 *
 * The TecplotLoader class contais functionality to read Tecplot files and
 * extract basic data/metadata in to primitive data structures. The Tecplot
 * file may contain multiple zones (i.e. several fields corresponding with
 * vertex positions).
 */
class TecplotLoader : public Loader {
 public:
    /**
     * \brief Default constructor.
     *
     * Creates a TecplotLoader and initialises its internal file parsing state.
     */
    TecplotLoader();

    /**
     * \brief Read header information.
     * \param[in]  fileName the name of the file from which element and vertex
     *                      information is to be parsed.
     * \param[out] nvert    the number of vertices reported in the header.
     * \param[out] nelem    the number of elements reported in the header.
     * \param[out] nzone    the number of zones reported in the header.
     *
     * \return Nothing.
     */
    void header(std::string const & fileName,
                size_t            & nvert,
                size_t            & nelem,
                size_t            & nzone);

    /**
     * \brief Read vertex and connectivity information from a Tecplot file.
     *
     * \param[in]  fileName   the name of the file from which vertex and c
     *                        connectivity information is read.
     * \param[in]  fileIdxing the indexing type used in the Tecplot file (i.e. 
     *                        do vertex inices start from one or zero).
     * \param[out] vert       the array in which vertices are stored.
     * \param[out] elem       the array in which element connectivity 
     *                        information is stored.
     * \param[out] fields     the field information stored in the file, there
     *                        may be multiple fields (zones) per file.
     *
     * \return Nothing.
     */
    void load(std::string const  & fileName,
              SourceFileIndexing   fileIndexing,
              VertexField3d      & vcoord,
              ConnectIndices4    & eindex,
              VectorFields3d     & fields);

    void load(std::string const & fileName,
              SourceFileIndexing  fileIndexing,
              VertexField3d     & vcoord,
              ConnectIndices4   & eindex,
              VectorField3d     & field);

 private:
    /// States that the loader may be in while parsing a Tecplot file.
    enum loaderStatus {
      /// The loader is in the start state.
      START,
      /// First zone state.
      ZONE1,
      /// Finite element information state 1
      FEM1,
      /// Subsequent zones state.
      ZONE2,
      /// Finite element information state 2
      FEM2, 
      /// Finite element zone and fem state.
      ZONEANDFEM
    };

    regex_t m_regexTitle;
    regex_t m_regexZone;
    regex_t m_regexVars;
    regex_t m_regexFem;
    regex_t m_regexVertAndField;
    regex_t m_regexTet;
    regex_t m_regexF;
    regex_t m_regexZoneAndFem;

    /**
     * \brief Function to parse the title line.
     * 
     * \param[in]  line  the line being parsed.
     * \param[out] title the title in the line being parsed.
     *
     * \return true if the input line was matched as a title line, otherwise
     *         returns false.
     */
    bool parseTitle(std::string const & line,
                    std::string       & title);

    /**
     * \brief Function to parse a zone line.
     * 
     * \param[in]  line  the line being parsed.
     * \param[out] nvert the number of vertices in the zone line.
     * \param[out] nelem the number of elements in the zone line.
     *
     * \return true if the input line was matched as a zone line, otherwise
     *         returns false.
     */
    bool parseZone(std::string const & line,
                   size_t            & nvert,
                   size_t            & nelem);

    /**
     * \brief Function to parse a vars line.
     * 
     * \param[in]  line  the line being parsed.
     * \param[out] vars  the vars information in the vars line.
     *
     * \return true if the input line was matched as a vars line, otherwise
     *         returns false.
     */
    bool parseVars(std::string const & line,
                   std::string       & vars);

    /**
     * \brief Function to parse a fem line.
     * 
     * \param[in]  line the line being parsed.
     *
     * \return true if the input line was matched as a fem line, otherwise
     *         returns false.
     */
    bool parseFem(std::string const & line);

    /**
     * \brief Function to parse a vertex and field line.
     * 
     * \param[in]  line  the line being parsed.
     * \param[out]   vx  the x coordinate of the vertex.
     * \param[out]   vy  the y coordinate of the vertex.
     * \param[out]   vz  the z coordinate of the vertex.
     * \param[out]   fx  the x coordinate of the field.
     * \param[out]   fy  the y coordinate of the field.
     * \param[out]   fz  the z coordinate of the field.
     *
     * \return true if the input line was matched as a vertex & field line, 
     *         otherwise returns false.
     */
    bool parseVertAndField(std::string const & line,
                           double            & vx,
                           double            & vy,
                           double            & vz,
                           double            & fx,
                           double            & fy,
                           double            & fz);

    /**
     * \brief Function to parse a tetrahedral element line.
     * 
     * \param[in]  line  the line being parsed.
     * \param[out]   n0  the first index of the tetrahedron. 
     * \param[out]   n1  the second index of the tetrahedron. 
     * \param[out]   n2  the third index of the tetrahedron. 
     * \param[out]   n3  the fourth index of the tetrahedron. 
     *
     * \return true if the input line was matched as a tetrahedral element line, 
     *         otherwise returns false.
     */
    bool parseTet(std::string const & line,
                  uint              & n0,
                  uint              & n1,
                  uint              & n2,
                  uint              & n3);

    /**
     * \brief Function to parse a field line.
     * 
     * \param[in]  line  the line being parsed.
     * \param[out]   fx  the x coordinate of the field.
     * \param[out]   fy  the y coordinate of the field.
     * \param[out]   fz  the z coordinate of the field.
     *
     * \return true if the input line was matched as a field line, 
     *         otherwise returns false.
     */
    bool parseF(std::string const & line,
                double            & fx,
                double            & fy,
                double            & fz);

    /**
     * \brief Function to parse a zone line.
     * 
     * \param[in]  line  the line being parsed.
     * \param[out] nvert the number of vertices in the zone line.
     * \param[out] nelem the number of elements in the zone line.
     *
     * \return true if the input line was matched as a zone line, otherwise
     *         returns false.
     */
    bool parseZoneAndFem(std::string const & line,
                         size_t            & nvert,
                         size_t            & nelem);

    double extract_double(const char * line,
                          regmatch_t * match);

    uint extract_uint(const char * line,
                      regmatch_t * match);

    size_t extract_size_t(const char * line,
                          regmatch_t * match);

    std::string extract_string(const char * line,
                               regmatch_t * pmatch);
};

#endif  // SRC_IO_TECPLOTLOADER_H_
