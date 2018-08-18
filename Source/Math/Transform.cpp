#include "Transform.h"

Matrix Transform::Inverse() const
{
	Matrix _Inverse(ToMatrix().Inverse());
	return _Inverse;
}

