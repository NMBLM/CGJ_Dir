#pragma once

namespace engine {

	struct vec2 {
		float x, y;
		// Constructor

		// transform to diff dimension

		// Comparison

		// Sum,sub, mult by scalar


		// input output

	};

	struct vec3 {
		float x, y, z;

		// Constructor
		// transform to diff dimension


		// Comparison

		// Sum,sub, mult by scalar

		// cross,dot product

		// input output

	};


	struct vec4 {
		float x, y, z, w;
		
		// Constructor

		// assignment, Comparison

		// Sum,sub, mult by scalar

		// cross,dot product

		// input output
	};


	struct mat2 {
		float mat[4];

		//constructor
		// transform to diff dimension

		
		//assignment, comparison
		
		//input/output

		//sum, subtraction 
		
		//multiplication (by another matrix, a scalar, or a vector) 
		
		//transpose, determinant and inverse (except for 4x4 matrices)


	};

	struct mat3 {
		float mat[9];

		//constructor
		// transform to diff dimension


		//assignment, comparison

		//input/output

		//sum, subtraction 

		//multiplication (by another matrix, a scalar, or a vector) 

		//transpose, determinant and inverse (except for 4x4 matrices)
	};

	struct mat4 {
		float mat[16];

		//constructor

		//assignment, comparison

		//input/output

		//sum, subtraction 

		//multiplication (by another matrix, a scalar, or a vector) 

		//transpose, determinant and inverse (except for 4x4 matrices)
	};


	struct MatrixFactory {

		// identity
		// scale ( x, y, z);
		// translation (x,y,z) revisit this
		// rotation ( angle )
		// dual is for cross Product
		// dual ( vec2 )
		// dual ( vec3 );
		// dual ( vec4 );

		//Shear/Sheer?
	};


}
