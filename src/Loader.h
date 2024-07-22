/**
 * \file   Loader.h
 * \author L. Nagy
 *
 * Copyright [2016] Lesleis Nagy. All rights reserved.
 */

#ifndef LOADER_H_
#define LOADER_H_

/**
 * \brief Parent class of all loaders.
 *
 * The Loader class is the parent class of all Loader* objects. It simply
 * defines the type of indexing used in the source file being read.
 */
class Loader {
 public:
    /**
     * Enumeration for source file indexing denotes the type of indexing 
     * used in the source file being read.
     */
    enum SourceFileIndexing {
      /// Used if the source file has indices starting at zero.
      ZERO_INDEXING,
      /// Used if the source file has indices starting at one.
      ONE_INDEXING
    };
};

#endif  // LOADER_H_
