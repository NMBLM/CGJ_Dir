#include <ctime>
#include "vector.h"
#include "matrix.h"
/*


reg E [-1,1]

"static_cast<float>(rand()/ static_cast<float>(RAND.MAX/2.0f) -1.0f"

*/

#define PI 3.14159265f

using namespace engine;
int test()
{
	srand(static_cast<unsigned> (time(0)));



	// EXERCISE 1
	vec3 i, j, k;
	i = vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f,
			 static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f,
			 static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f);
	j = vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f,
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f,
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f);
	k = vec3(static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f,
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f,
			static_cast<float>(rand()) / static_cast<float>(RAND_MAX / 2.0f) - 1.0f);

	std::cout << " EXERCISE 1 " << std::endl;

	std::cout << " i x j x k = "			 <<		i.cross(j.cross(k))				<< std::endl;
	std::cout << " j * (i.k) - k * (i.j) = " <<		j * (i.dot(k)) - k * (i.dot(j)) << std::endl;

	std::cout << " i x j x k == j*(i.k) - k*(i.j) ";
	if (	i.cross(j.cross(k)) == j * i.dot(k) - k * i.dot(j)	 ) {
		std::cout << "True" << std::endl;
	}
	else {
		std::cout << "False" << std::endl;
	}


	// EXERCISE 2

	vec3 XX = vec3(2, 0, 0);
	vec3 YY = vec3(0, 3, 0);
	vec3 ZZ = vec3(0, 0, 4);
	std::cout << " EXERCISE 2"<< std::endl;


	std::cout << " XX = " << XX << std::endl;
	std::cout << " YY = " << YY << std::endl;
	std::cout << " ZZ = " << ZZ << std::endl;

	std::cout << " Rotate  90 axis XX" << std::endl << MatrixFactory::createRotationMatrix3(PI / 2, XX) << std::endl;
	std::cout << " Rotate  -90 axis XX" << std::endl << MatrixFactory::createRotationMatrix3(-PI / 2, XX) << std::endl;

	std::cout << " YY rotated 90 on axis XX"  << MatrixFactory::createRotationMatrix3(PI / 2, XX) * YY << std::endl;
	std::cout << " YY rotated -90 on axis XX" << MatrixFactory::createRotationMatrix3(-PI / 2, XX) * YY << std::endl;
	std::cout << " ZZ rotated 90 on axis XX"  << MatrixFactory::createRotationMatrix3(PI / 2, XX)  * ZZ << std::endl;
	std::cout << " ZZ rotated -90 on axis XX" << MatrixFactory::createRotationMatrix3(-PI / 2, XX) * ZZ << std::endl;
	return 0;
}