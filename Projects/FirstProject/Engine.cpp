#include "Engine.h"
using namespace engine;

const mat3 engine::MatrixFactory::Matrice3Identity()
{
	return mat3(1, 0, 0,
				0, 1, 0,
				0, 0, 1 );
}
