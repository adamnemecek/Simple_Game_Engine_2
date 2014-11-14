#ifndef ENGINE_FLOAT_QUAT_H
#define ENGINE_FLOAT_QUAT_H

#include <glm\vec3.hpp>

namespace Math
{
   // this data structure is my own attempt at a quaternion
   // Note: My version is unlike GLM's quaternions.  
   // GLM fquat is a specific template of the basic quat, and it explicitly uses four floats (w (scalar), x, y, and z).
   // My quat operates explicitly on a floats and a glm::vec3, and I take advantage of the vector's supported functions
   // to satisfy the quaternion math.
   struct __declspec(dllexport) F_Quat
   {
      float m_scalar;
      glm::vec3 m_vector;

      // generates a quat Q of the following form:
      // [scalar = 0, vector = <0, 0, 0>]
      // Note: For generating Real (vector = <0,0,0>), and Pure (scalar = 0) quats, please use 
      // the static generator methods of the same names
      F_Quat();

      // generates a quat Q of the follow
      F_Quat(const float scalar, const glm::vec3 &vector);

      // generates a Real quat of the following form
      // [scalar = argument, vector = <0,0,0>]
      // Note: This is useful for scalar multiplication. ??should I just have scalar multiplication and avoid use real quats altogether??
      static F_Quat generate_real_quat(const float new_scalar);

      // generates a Pure quat of the following form
      // [scalar = 0, vector = argument]
      // Note: This is useful for generating a point that can be rotated by another quat
      // or transform (rotated and translated) by a dual quat.
      static F_Quat generate_pure_quat(const glm::vec3 &new_vector);

      // generate a quat given a rotation axis and a rotation angle
      // Note: The algorithm used here was heavily influenced by the ArcSynthesis tutorial
      // on quaternions (tutorial #8).
      static F_Quat generate_rotator(const glm::vec3 &rotation_axis, const float rotation_angle_rad);

      // normalization, but don't modify the original
      static F_Quat normalize(const F_Quat &fq);


      // explicit self-assignment operator
      void operator=(const F_Quat &right);

      // self-addition
      // Note: Is used by in dual quat multiplication.
      // Note: Results in a quat of the following form:
      void operator+=(const F_Quat &right);

      // scalar self-multiplication
      // Note: Given an original quat Q, results in a quat of the following form:
      // [new scalar = Q.scalar * right, new vector = Q.vector * right]
      void operator*=(const float right);

      // self-multiplication
      // Note: For the sake of brevity, let scalar be "S" and vector be "V".  Then given two
      // quats Q1 and Q2, Q1 times Q2 results in a quat of the following form:
      // [S1*S2 - dot(V1, V2), S1*V2 + S2*V1 + cross(V1, V2)]
      void operator*=(const F_Quat &right);

      // self-conjugate
      // Note: Results in a quaterion of the following form:
      // [new scalar = scalar, new vector = (-1) * vector]
      // Note: Conjugation of a quaternion Q results in a quaternion Q* such that
      // (Q)(Q*) = [new scalar = scalar^2 + dot(vector, vector), new_vector = <0,0,0>]
      F_Quat conjugate() const;

      // self-normalization
      // Note: Results in a unit quaternion (magnitude 1) by the following calculation:
      // [new scalar = scalar / magnitude, new vector = vector / magnitude]
      void normalize();


   private:
      // Note: These functions are private for the following reasons:
      // - Magnitude: This is only useful for normalization, so I decided that it was best to
      // have a normalize() function and calculate the magnitude internally.
      // - Magnitude squared: This is only useful in calculating the inverse, so like the 
      // magnitude, I will calculate this internally when calculating the inverse.
      // - Inverse: I have no use for this function now.

      // self-magnitude
      // Note: Results in a float by the following calculation: 
      // sqrt(scalar^2 + dot(vector, vector))
      // Note: This is equivalent to sqrt((Q)(Q*)), where Q* is the conjugate of quaternion Q.
      // Note: If magnitude is 1 (or very very close to it because float circuits are not
      // always precise), then the quaternion is a unit quaternion.
      float magnitude() const;

      // self-magnitude squared
      // Note: This can be useful for quick and dirty magnitude comparisons, but most 
      // likely it will be used in calculating an inverse of the following form:
      // Q_inverse = Q_conjugate / (Q.magnitude())^2)
      float magnitude_squared() const;

      // self-inverse
      // Note: Results in a quaternion Q such that 'this' times Q = [1, <0,0,0>].
      F_Quat inverse() const;
   };

   // addition
   F_Quat operator+(const F_Quat &left, const F_Quat &right);

   // scalar multiplication
   F_Quat operator*(const float left, const F_Quat &right);
   F_Quat operator*(const F_Quat &left, const float right);

   // multiplication
   F_Quat operator*(const F_Quat &left, const F_Quat &right);
}

#endif
