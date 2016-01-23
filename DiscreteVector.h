
/**
 * This file is part of YetAnotherRubiksCube game source code
 *
 * Copyright (C) 2010  Aleksey "Adaon" Fedotov
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef _DISCRETE_VECTOR_H_
#define _DISCRETE_VECTOR_H_



class DiscreteVector
{
public:
  int i, j, k;

  DiscreteVector(): i(0), j(0), k(0) {}
  DiscreteVector(int _i, int _j, int _k): i(_i), j(_j), k(_k) {}

  /// Addition operator
  DiscreteVector operator +(const DiscreteVector &v) const
  {
    return DiscreteVector(i + v.i, j + v.j, k + v.k);
  }

  /// Subtraction operator
  DiscreteVector operator -(const DiscreteVector &v) const
  {
    return DiscreteVector(i - v.i, j - v.j, k - v.k);
  }

  /// Multiplication by scalar
  DiscreteVector operator *(const int s) const
  {
    return DiscreteVector(i * s, j * s, k * s);
  }

  /// Normalization
  void normalize()
  {
    // i
    if (i > 0)
      i = 1;
    else if (i == 0)
      i = 0;
    else if (i < 0)
      i = -1;

    // j
    if (j > 0)
      j = 1;
    else if (j == 0)
      j = 0;
    else if (j < 0)
      j = -1;

    // k
    if (k > 0)
      k = 1;
    else if (k == 0)
      k = 0;
    else if (k < 0)
      k = -1;
  }
};



#endif
