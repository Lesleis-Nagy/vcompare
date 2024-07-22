/**
 * \file   data.h
 * \author L. Nagy
 *
 * This file contains the basic data structures used by CUMerrill.
 * 
 * Copyright [2016] Lesleis Nagy. All rights reserved.
 */

#ifndef DATA_H_
#define DATA_H_

#include <cmath>

#include <vector>
#include <unordered_set>

#include "Types.h"

#include "Utilities.h"

inline void compare_swap(uint & a, uint & b) {
  if (a > b) {
    uint tmp = a;
    a = b;
    b = tmp;
  }
}

template <class T> struct CanonicalSimplexHash;
template <class T> struct CanonicalSimplexOrder;
template <class T> struct CanonicalSimplexEquality;

/**
 * Structure defining a connectivity for two indices (e.g. a line).
 */
struct Connect2 {
  /// First index.
  unsigned int n0;
  /// Second index.
  unsigned int n1;
};
inline bool operator == (const Connect2 & lhs, const Connect2 & rhs) {
  return lhs.n0 == rhs.n0 && lhs.n1 == rhs.n1;
}
template <> struct CanonicalSimplexHash<Connect2> {
  size_t operator() (Connect2 const& s) const {
    size_t seed = 0;

    uint n0 = s.n0;
    uint n1 = s.n1;

    // Sorting network for two values.
    compare_swap(n0, n1);

    // Build hash
    hash_combine(seed, n0);
    hash_combine(seed, n1);

    return seed;
  }
};
template <> struct CanonicalSimplexOrder<Connect2> {
  bool operator () (const Connect2 &lhs, const Connect2 &rhs) const {
    uint ln0 = lhs.n0, ln1 = lhs.n1;
    uint rn0 = rhs.n0, rn1 = rhs.n1;

    // Sort left hand side and right hand side indices in to canonical order
    compare_swap(ln0, ln1);

    compare_swap(rn0, rn1);

    // Perform lexographical ordering on the canonical ordering.
    if (ln0 < rn0) {
      return true;
    }
    if (rn0 < ln0) {
      return false;
    }

    if (ln1 < rn1) {
      return true;
    }

    return false;
  }
};
template <> struct CanonicalSimplexEquality<Connect2> {
  bool operator() (const Connect2 &lhs, const Connect2 &rhs) const {
    uint ln0 = lhs.n0, ln1 = lhs.n1;
    uint rn0 = rhs.n0, rn1 = rhs.n1;

    // Sort left hand side and right hand side indices in to canonical order
    compare_swap(ln0, ln1);

    compare_swap(rn0, rn1);

    // Perform equality.
    return ln0 == rn0 && ln1 == rn1;
  }
};

/**
 * Structure defining connectivity for three indices (e.g. a triangle).
 */
struct Connect3 {
  /// First index.
  unsigned int n0;
  /// Second index.
  unsigned int n1;
  /// Third index.
  unsigned int n2;
};
inline bool operator == (const Connect3 & lhs, const Connect3 & rhs) {
  return lhs.n0 == rhs.n0 && lhs.n1 == rhs.n1 && lhs.n2 == rhs.n2;
}
template <> struct CanonicalSimplexHash<Connect3> {
  size_t operator() (Connect3 const& s) const {
    size_t seed = 0;

    uint n0 = s.n0;
    uint n1 = s.n1;
    uint n2 = s.n2;

    // Sorting network for 3 values.
    compare_swap(n0, n1);
    compare_swap(n0, n2);
    compare_swap(n1, n2);

    // Build hash.
    hash_combine(seed, n0);
    hash_combine(seed, n1);
    hash_combine(seed, n2);

    return seed;
  }
};
template <> struct CanonicalSimplexOrder<Connect3> {
  bool operator() (const Connect3 &lhs, const Connect3 &rhs) const {
    uint ln0 = lhs.n0, ln1 = lhs.n1, ln2 = lhs.n2;
    uint rn0 = rhs.n0, rn1 = rhs.n1, rn2 = rhs.n2;

    // Sort left hand side and right hand side indices in to canonical order
    compare_swap(ln0, ln1);
    compare_swap(ln0, ln2);
    compare_swap(ln1, ln2);

    compare_swap(rn0, rn1);
    compare_swap(rn0, rn2);
    compare_swap(rn1, rn2);

    // Perform lexographical ordering on the canonically ordered indices.
    if (ln0 < rn0) {
      return true;
    }

    if (rn0 < ln0) {
      return false;
    }

    // ln0 and rn0 are equal
    if (ln1 < rn1) {
      return true;
    }

    if (rn1 < ln1) {
      return false;
    }

    // ln1 and rn1 are equal
    if (ln2 < rn2) {
      return true;
    }

    return false;
  }
};
template <> struct CanonicalSimplexEquality<Connect3> {
  bool operator() (const Connect3 &lhs, const Connect3 &rhs) const {
    uint ln0 = lhs.n0, ln1 = lhs.n1, ln2 = lhs.n2;
    uint rn0 = rhs.n0, rn1 = rhs.n1, rn2 = rhs.n2;

    // Sort left hand side and right hand side indices in to canonical order
    compare_swap(ln0, ln1);
    compare_swap(ln0, ln2);
    compare_swap(ln1, ln2);

    compare_swap(rn0, rn1);
    compare_swap(rn0, rn2);
    compare_swap(rn1, rn2);

    // printf("Equality test: %i, %i, %i == %i, %i, %i\n", ln0, ln1, ln2, rn0, rn1, rn2);

    return ln0 == rn0 && ln1 == rn1 && ln2 == rn2;
  }
};

/**
 * Structure definining connectivity for four indices (e.g. a tetrahedron).
 */
struct Connect4 {
  /// First index.
  unsigned int n0;
  /// Second index.
  unsigned int n1;
  /// Third index.
  unsigned int n2;
  /// Fourth index.
  unsigned int n3;
};
inline bool operator == (const Connect4 & lhs, const Connect4 & rhs) {
  return lhs.n0 == rhs.n0 && lhs.n1 == rhs.n1
      && lhs.n2 == rhs.n2 && lhs.n3 == rhs.n3;
}
template <> struct CanonicalSimplexHash<Connect4> {
  size_t operator() (Connect4 const & s) const {
    size_t seed = 0;

    uint n0 = s.n0;
    uint n1 = s.n1;
    uint n2 = s.n2;
    uint n3 = s.n3;

    // Sorting network for 4 values.
    compare_swap(n0, n2);
    compare_swap(n1, n3);
    compare_swap(n0, n1);
    compare_swap(n2, n3);
    compare_swap(n1, n2);

    // Build hash.
    hash_combine(seed, n0);
    hash_combine(seed, n1);
    hash_combine(seed, n2);
    hash_combine(seed, n3);

    return seed;
  }
};
template <> struct CanonicalSimplexOrder<Connect4> {
  bool operator() (const Connect4 &lhs, const Connect4 &rhs) const {
    uint ln0 = lhs.n0, ln1 = lhs.n1, ln2 = lhs.n2, ln3 = lhs.n3;
    uint rn0 = rhs.n0, rn1 = rhs.n1, rn2 = rhs.n2, rn3 = rhs.n3;

    // Sort left hand side and right hand side indices in to canonical order
    compare_swap(ln0, ln2);
    compare_swap(ln1, ln3);
    compare_swap(ln0, ln1);
    compare_swap(ln2, ln3);
    compare_swap(ln1, ln2);

    compare_swap(rn0, rn2);
    compare_swap(rn1, rn3);
    compare_swap(rn0, rn1);
    compare_swap(rn2, rn3);
    compare_swap(rn1, rn2);

    // Perform lexographical ordering on the canonically ordered indices.
    if (ln0 < rn0) {
      return true;
    }

    if (rn0 < ln0) {
      return false;
    }

    // ln0 and rn0 are equal
    if (ln1 < rn1) {
      return true;
    }

    if (rn1 < ln1) {
      return false;
    }

    // ln1 and rn1 are equal
    if (ln2 < rn2) {
      return true;
    }

    if (rn2 < ln2) {
      return false;
    }

    // ln2 and rn2 are equal
    if (ln3 < rn3) {
      return true;
    }

    return false;
  }
};
template <> struct CanonicalSimplexEquality<Connect4> {
  bool operator() (const Connect4 &lhs, const Connect4 &rhs) const {
    uint ln0 = lhs.n0, ln1 = lhs.n1, ln2 = lhs.n2, ln3 = lhs.n3;
    uint rn0 = rhs.n0, rn1 = rhs.n1, rn2 = rhs.n2, rn3 = rhs.n3;

    // Sort left hand side and right hand side indices in to canonical order
    compare_swap(ln0, ln2);
    compare_swap(ln1, ln3);
    compare_swap(ln0, ln1);
    compare_swap(ln2, ln3);
    compare_swap(ln1, ln2);

    compare_swap(rn0, rn2);
    compare_swap(rn1, rn3);
    compare_swap(rn0, rn1);
    compare_swap(rn2, rn3);
    compare_swap(rn1, rn2);

    // Perform equality.
    return ln0 == rn0 && ln1 == rn1 && ln2 == rn2 && ln3 == rn3;
  }
};

/**
 * Structure defining a two-component vector.
 */
struct Vector2d {
  /// The first, `x`, vector component.
  double x;
  /// The second, `y`, vector component.
  double y;
};

/**
 * Structure defining a two-component vertex.
 */
struct Vertex2d {
  /// The first, `x`, vertex component.
  double x;
  /// The second, `y`, vertex component.
  double y;
};

/**
 * Structure defining a three-dimensional vector.
 */
struct Vector3d {
  /// The first, `x`, vector component.
  double x;
  /// The second, `y`, vector component.
  double y;
  /// The third, `z`, vector component.
  double z;
};

/**
 * Structure defining a three-dimensional vector.
 */
struct Vertex3d {
  /// The first, `x`, vertex component.
  double x;
  /// The second, `y`, vertex component.
  double y;
  /// The third, `z`, vertex component.
  double z;
};

/**
 * Operators add two vertices.
 */
inline Vertex2d operator + (Vertex2d const& u, Vertex2d const& v) {
  return {.x = u.x + v.x, .y = u.y + v.y};
}

inline Vertex3d operator + (Vertex3d const& u, Vertex3d const& v) {
  return {.x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z};
}

/**
 * Scale a vertex.
 */
inline Vertex2d operator * (Vertex2d const& v, double s) {
  return {.x = v.x*s, .y = v.y*s};
}

inline Vertex2d operator * (double s, Vertex2d const& v) {
  return {.x = v.x*s, .y = v.y*s};
}

inline Vertex2d operator / (Vertex2d const& v, double s) {
  return {.x = v.x/s, .y = v.y/s};
}

inline Vertex3d operator * (Vertex3d const& v, double s) {
  return {.x = v.x*s, .y = v.y*s, .z = v.z*s};
}

inline Vertex3d operator * (double s, Vertex3d const& v) {
  return {.x = v.x*s, .y = v.y*s, .z = v.z*s};
}

inline Vertex3d operator / (Vertex3d const& v, double s) {
  return {.x = v.x/s, .y = v.y/s, .z = v.z/s};
}

inline Vertex3d project2d3d(Vertex2d const& v) {
  return {.x = v.x, .y = v.y, .z = 0.0};
}

inline Vertex3d project2d3d(Vertex2d const& v, double zplane) {
  return {.x = v.x, .y = v.y, .z = zplane};
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Operators take the difference between two vertices (produces a vector).
 */
inline Vector2d operator - (Vertex2d const& u, Vertex2d const& v) {
  return {.x = u.x - v.x, .y = u.y - v.y};
}

inline Vector3d operator - (Vertex3d const& u, Vertex3d const& v) {
  return {.x = u.x - v.x, .y = u.y - v.y, .z = u.z - v.z};
}

///////////////////////////////////////////////////////////////////////////////

/**
 * Operators add two vectors (produces a vector).
 */
inline Vector2d operator + (Vector2d const& u, Vector2d const& v) {
  return {.x = u.x + v.x, .y = u.y + v.y};
}

inline Vector3d operator + (Vector3d const& u, Vector3d const& v) {
  return {.x = u.x + v.x, .y = u.y + v.y, .z = u.z + v.z};
}

/**
 * Operators to subtract two vectors.
 */
inline Vector2d operator - (Vector2d const& u, Vector2d const& v) {
  return {.x = u.x - v.x, .y = u.y - v.y};
}

inline Vector3d operator - (Vector3d const& u, Vector3d const& v) {
  return {.x = u.x - v.x, .y = u.y - v.y, .z = u.z - v.z};
}

/**
 * Scale a vector.
 */
inline Vector2d operator * (Vector2d const& v, double s) {
  return {.x = v.x*s, .y = v.y*s};
}

inline Vector2d operator * (double s, Vector2d const& v) {
  return {.x = v.x*s, .y = v.y*s};
}

inline Vector2d operator / (Vector2d const& v, double s) {
  return {.x = v.x/s, .y = v.y/s};
}

inline Vector3d operator * (Vector3d const& v, double s) {
  return {.x = v.x*s, .y = v.y*s, .z = v.z*s};
}

inline Vector3d operator * (double s, Vector3d const& v) {
  return {.x = v.x*s, .y = v.y*s, .z = v.z*s};
}

inline Vector3d operator / (Vector3d const& v, double s) {
  return {.x = v.x/s, .y = v.y/s, .z = v.z/s};
}

/**
 * Take the dot product of two vectors.
 */
inline double dot(Vector2d const& u, Vector2d const& v) {
  return (u.x * v.x) + (u.y * v.y);
}

inline double dot(Vector3d const& u, Vector3d const& v) {
  return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

/**
 * Calculte the length of a vector.
 */
inline double norm(Vector2d const& v) {
  return sqrt(v.x*v.x + v.y*v.y);
}

inline double norm(Vector3d const& v) {
  return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

/**
 * Vormalize a vector.
 */
inline void normalize(Vector2d & v) {
  double length = norm(v);
  v.x /= length;
  v.y /= length;
}

inline void normalize(Vector3d & v) {
  double length = norm(v);
  v.x /= length;
  v.y /= length;
  v.z /= length;
}

/**
 * Return normalized version of a vector.
 */
inline Vector2d cnormalize(Vector2d const& v) {
  double length = norm(v);
  return v/length;
}

inline Vector3d cnormalize(Vector3d const& v) {
  double length = norm(v);
  return v/length;
}

/**
 * Take the cross product of two vectors (3d only).
 */
inline Vector3d cross(Vector3d const& lhs, Vector3d const& rhs) {
  return {.x = (lhs.y * rhs.z) - (lhs.z * rhs.y),
          .y = (lhs.z * rhs.x) - (lhs.x * rhs.z), 
          .z = (lhs.x * rhs.y) - (lhs.y * rhs.x)};
}

typedef std::vector<Connect2>                ConnectIndices2;
typedef std::vector<Connect3>                ConnectIndices3;
typedef std::vector<Connect4>                ConnectIndices4;

typedef std::vector<Vector2d>                VectorField2d;
typedef std::vector< std::vector<Vector2d> > VectorFields2d;
typedef std::vector<Vertex2d>                VertexField2d;

typedef std::vector<Vector3d>                VectorField3d;
typedef std::vector< std::vector<Vector3d> > VectorFields3d;
typedef std::vector<Vertex3d>                VertexField3d;

#endif  // DATA_H_
