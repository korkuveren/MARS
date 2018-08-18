#include "VecMath.h"
#include <limits>

const Vector VectorConstants::ZERO(Vector::Make(0.0f, 0.0f, 0.0f, 0.0f));
const Vector VectorConstants::ONE(Vector::Make(1.0f, 1.0f, 1.0f, 1.0f));
const Vector VectorConstants::TWO(Vector::Make(2.0f, 2.0f, 2.0f, 2.0f));
const Vector VectorConstants::HALF(Vector::Make(0.5f, 0.5f, 0.5f, 0.5f));
const Vector VectorConstants::INF(Vector::Make(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity()));
const Vector VectorConstants::MASK_X(Vector::Make((uint32)0, 0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF));
const Vector VectorConstants::MASK_Y(Vector::Make((uint32)0xFFFFFFFF, 0, 0xFFFFFFFF, 0xFFFFFFFF));
const Vector VectorConstants::MASK_Z(Vector::Make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF));
const Vector VectorConstants::MASK_W(Vector::Make((uint32)0xFFFFFFFF, 0xFFFFFFFF, 0xFFFFFFFF, 0));
const Vector VectorConstants::SIGN_MASK(Vector::Make((uint32)0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF, 0x7FFFFFFF));

