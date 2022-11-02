#ifndef MMATH_H
#define MMATH_H

#include "vec3.h"
#include <cmath>
#include <vector>
#include <iterator>

//matrix math library
namespace math
{


	class mat3
	{
	private:
		double** m_matrix;
	public:

		mat3()
		{
			//DEFAULT CONSTRUCTOR
			m_matrix = new double* [3];
			m_matrix[0] = new double[3];
			m_matrix[1] = new double[3];
			m_matrix[2] = new double[3];
			for (int j = 0; j < 3; j++)
				for (int i = 0; i < 3; i++)
					m_matrix[j][i] = 0;
		}

		mat3(const vec3& col1, const vec3& col2, const vec3& col3)
		{
			m_matrix = new double* [3];
			m_matrix[0] = new double[3];
			m_matrix[1] = new double[3];
			m_matrix[2] = new double[3];

			for (int i = 0; i < 3; i++)
				m_matrix[0][i] = col1[i];
			for (int i = 0; i < 3; i++)
				m_matrix[1][i] = col2[i];
			for (int i = 0; i < 3; i++)
				m_matrix[2][i] = col3[i];
		}

		~mat3()
		{
			delete[] m_matrix[0];
			delete[] m_matrix[1];
			delete[] m_matrix[2];
			delete[] m_matrix;
		}

		inline vec3 operator[](const int index) const
		{

			if ((index > 2) || (index < 0))
			{
				std::cout << "\nERROR: Vector index out of bound\n";
				exit(0);
			}
			if (index == 0)
			{
				return { m_matrix[0][0],m_matrix[0][1] ,m_matrix[0][2] };
			}
			if (index == 1)
			{
				return { m_matrix[1][0], m_matrix[1][1], m_matrix[1][2] };
			}

			if (index == 2)
			{
				return { m_matrix[2][0], m_matrix[2][1], m_matrix[2][2] };
			}
			return {};
		}

	};

	inline mat3 operator+(const mat3& first,const mat3& second)
	{
		vec3 col1 = first[0] + second[0];
		vec3 col2 = first[1] + second[1];
		vec3 col3 = first[2] + second[2];

		return { col1,col2,col3 };
	}

	mat3 operator*(const mat3& first,const mat3& second)
	{
		const double c11 = first[0][0] * second[0][0] + first[1][0] * second[0][1] + first[2][0] * second[0][2];
		const double c12 = first[0][0] * second[1][0] + first[1][0] * second[1][1] + first[2][0] * second[1][2];
		const double c13 = first[0][0] * second[2][0] + first[1][0] * second[2][1] + first[2][0] * second[2][2];

		const double c21 = first[0][1] * second[0][0] + first[1][1] * second[0][1] + first[2][1] * second[0][2];
		const double c22 = first[0][1] * second[1][0] + first[1][1] * second[1][1] + first[2][1] * second[1][2];
		const double c23 = first[0][1] * second[2][0] + first[1][1] * second[2][1] + first[2][1] * second[2][2];

		const double c31 = first[0][2] * second[0][0] + first[1][2] * second[0][1] + first[2][2] * second[0][2];
		const double c32 = first[0][2] * second[1][0] + first[1][2] * second[1][1] + first[2][2] * second[1][2];
		const double c33 = first[0][2] * second[2][0] + first[1][2] * second[2][1] + first[2][2] * second[2][2];

		return {{c11,c21,c31},{c12,c22,c32},{c13,c23,c33}};

	}

	mat3 operator*(const mat3& in,const double scalar)
	{
		vec3 col1 = in[0];
		vec3 col2 = in[1];
		vec3 col3 = in[2];

		return { {col1[0]*scalar,col1[1] * scalar,col1[2] * scalar},{col2[0] * scalar,col2[1] * scalar,col2[2] * scalar},{col3[0] * scalar,col3[1] * scalar,col3[2] * scalar}};
	}


	mat3 operator*(const double scalar,const mat3& in)
	{
		vec3 col1 = in[0];
		vec3 col2 = in[1];
		vec3 col3 = in[2];

		return { {col1[0] * scalar,col1[1] * scalar,col1[2] * scalar},{col2[0] * scalar,col2[1] * scalar,col2[2] * scalar},{col3[0] * scalar,col3[1] * scalar,col3[2] * scalar} };
	}

	vec3 operator*(const mat3& lm,const vec3& v)
	{
		return {lm[0][0]*v[0]+lm[1][0]*v[1]+lm[2][0]*v[2],lm[0][1] * v[0] + lm[1][1] * v[1] + lm[2][1] * v[2] ,lm[0][2] * v[0] + lm[1][2] * v[1] + lm[2][2] * v[2] };
	}

	inline bool operator==(const mat3& first, const mat3& second)
	{
		if( (first[0]==second[0]) && (first[1] == second[1]) && (first[2] == second[2]) )
		{
			return true;
		}else
		{
			return false;
		}
	}

	double determinant(const mat3& matrix)
	{
		const double aei = matrix[0][0] * matrix[1][1] * matrix[2][2];
		const double bfg = matrix[1][0] * matrix[2][1] * matrix[0][2];
		const double cdh = matrix[2][0] * matrix[0][1] * matrix[1][2];
		const double ceg = matrix[2][0] * matrix[1][1] * matrix[0][2];
		const double bdi = matrix[1][0] * matrix[0][1] * matrix[2][2];
		const double afh = matrix[0][0] * matrix[2][1] * matrix[1][2];

		return (aei + bfg + cdh - ceg - bdi - afh);
	}

	mat3 transpose(const mat3& in)
	{
		return { {in[0][0],in[1][0],in[2][0] }, {in[0][1],in[1][1],in[2][1] }, {in[0][2],in[1][2],in[2][2] } };
	}

	mat3 inverse(const mat3& matrix)
	{
		double det = determinant(matrix);
		if(!are_doubles_equal(det,0))
		{
			//compute adjugate of matrix
			mat3 adj = { {matrix[1][1] * matrix[2][2] - matrix[2][1] * matrix[1][2],matrix[2][1] * matrix[0][2] - matrix[0][1] * matrix[2][2],matrix[0][1] * matrix[1][2] - matrix[1][1] * matrix[0][2]},
							{matrix[2][0] * matrix[1][2] - matrix[1][0] * matrix[2][2],matrix[0][0] * matrix[2][2] - matrix[2][0] * matrix[0][2],matrix[1][0] * matrix[0][2] - matrix[0][0] * matrix[1][2]},
							{matrix[1][0] * matrix[2][1] - matrix[2][0] * matrix[1][1],matrix[2][0] * matrix[0][1] - matrix[0][0] * matrix[2][1],matrix[0][0] * matrix[1][1] - matrix[1][0] * matrix[0][1]}};

			return adj * (1 / (determinant(matrix)));
		}else
		{
			//matrix is singular
			exit(-1);
		}

	}



}
#endif