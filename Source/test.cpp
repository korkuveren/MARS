#include "tests.hpp"
#include "EngineCore/TimerManager.h"
#include "Math/Transform.h"
#include "Math/Sphere.h"
#include "Math/aabb.h"
#include "Math/Plane.h"
#include "Math/Intersects.h"

static void testMathTypesMemoryLayout()
{
	assert(sizeof(Cartesian3D) == sizeof(Euler3D) && sizeof(Cartesian3D) == sizeof(Spatial3D));
	
	float Vals[4];

	float& FourthVal	= Vals[3];
	float& ThirdVal		= Vals[2];
	float& SecondVal	= Vals[1];
	float& FirstVal		= Vals[0];

	FirstVal = 1.f;
	SecondVal = 2.f;
	ThirdVal = 3.f;
	FourthVal = 0.f;

	size_t sizeTA = sizeof(Cartesian3D);
	void* TestArea = _malloca(sizeTA);
	memcpy(TestArea, &Vals, sizeTA);
	
	assert((*((Cartesian2D*)TestArea)).X() == FirstVal);
	assert((*((Cartesian2D*)TestArea)).Y() == SecondVal);

	assert((*((Cartesian3D*)TestArea))[0] == FirstVal);
	assert((*((Cartesian3D*)TestArea))[1] == SecondVal);
	assert((*((Cartesian3D*)TestArea))[2] == ThirdVal);
	assert((*((Cartesian3D*)TestArea))[3] == FourthVal);

	assert((*((Spatial3D*)TestArea)).X() == FirstVal);
	assert((*((Spatial3D*)TestArea)).Y() == SecondVal);
	assert((*((Spatial3D*)TestArea)).Z() == ThirdVal);
	assert((*((Spatial3D*)TestArea))[3]  == FourthVal);

	assert((*((Euler3D*)TestArea)).Roll() == FirstVal);
	assert((*((Euler3D*)TestArea)).Pitch() == SecondVal);
	assert((*((Euler3D*)TestArea)).Yaw() == ThirdVal);
	assert((*((Euler3D*)TestArea))[3] == FourthVal);

	SecondVal = ThirdVal = FirstVal;
	memcpy(TestArea, &Vals, sizeTA);
	
	assert((*((Cartesian2D*)TestArea)) == Cartesian2D::One);
	assert((*((Spatial3D*)TestArea)) == Spatial3D::One);
	assert((*((Euler3D*)TestArea)) == Euler3D(1.f));

	FourthVal	= 0.f;
	ThirdVal	= 3.f;
	SecondVal	= 2.f;
	FirstVal	= 1.f;

	memcpy(TestArea, &Vals, sizeTA);
	Vector Test2 = Vector::Load4f((float*)TestArea);
	Vector Eq = (*((Vector*)TestArea) == Test2);

	assert(	 Eq[0] != 0
		  && Eq[1] != 0
		  && Eq[2] != 0
		  && Eq[3] != 0);
}

static void testSphere()
{
	Sphere sphere1(Spatial3D(0.0f, 0.0f, 0.0f), 1.0f);
	Sphere sphere2(Spatial3D(0.0f, 3.0f, 0.0f), 1.0f);
	Sphere sphere3(Spatial3D(0.0f, 0.0f, 2.0f), 1.0f);
	Sphere sphere4(Spatial3D(1.0f, 0.0f, 0.0f), 1.0f);
	Sphere sphere5(Spatial3D(1.0f, 0.0f, 0.0f), 2.0f);

	assert(!sphere1.intersects(sphere2));
	assert(!sphere1.intersects(sphere3,0.0f));
	assert(sphere1.intersects(sphere3));
	assert(sphere1.intersects(sphere4));
	assert(sphere1.intersects(sphere5));
	assert(sphere5.contains(sphere1));
	assert(!sphere1.contains(sphere5));
	assert(sphere1.contains(Spatial3D(0.0f,1.0f,0.0f)));
	assert(!sphere1.contains(Spatial3D(-1.1f,0.0f,0.0f)));
	assert(sphere1.moveTo(Spatial3D(-1.0f,0.0f,0.0f)).contains(Spatial3D(-1.1f,0.0f,0.0f)));

	Sphere superSphere = sphere1.addSphere(sphere2).addSphere(sphere3).addSphere(sphere4).addSphere(sphere5);
	assert(superSphere.contains(sphere1));
	assert(superSphere.contains(sphere2));
	assert(superSphere.contains(sphere3));
	assert(superSphere.contains(sphere4));
	assert(superSphere.contains(sphere5));
}

static void testAABB()
{
	AABB aabb1(Spatial3D(0.0f, 0.0f, 0.0f), Spatial3D(1.0f, 1.0f, 1.0f));
	AABB aabb2(Spatial3D(1.0f, 1.0f, 1.0f), Spatial3D(2.0f, 2.0f, 2.0f));
	AABB aabb3(Spatial3D(1.0f, 0.0f, 0.0f), Spatial3D(2.0f, 1.0f, 1.0f));
	AABB aabb4(Spatial3D(0.0f, 0.0f, -2.0f), Spatial3D(1.0f, 1.0f, -1.0f));
	AABB aabb5(Spatial3D(0.0f, 0.5f, 0.0f), Spatial3D(1.0f, 1.5f, 1.0f));
	AABB aabb6(Spatial3D(0.3f, 0.5f, 0.7f), Spatial3D(1.3f, 1.5f, 1.7f));
	AABB aabb7(Spatial3D(0.3f, 0.5f, 0.7f), Spatial3D(0.5f, 0.7f, 0.9f));

	assert(aabb1.Intersects(aabb2) == false);
	assert(aabb1.Intersects(aabb2.Translate(Spatial3D(-0.5f))) == true);
	assert(aabb1.Intersects(aabb2.Translate(Spatial3D(0.5f))) == false);
	assert(aabb1.Intersects(aabb3) == false);
	assert(aabb1.Intersects(aabb4) == false);
	assert(aabb1.Intersects(aabb5) == true);
	assert(aabb1.Intersects(aabb6) == true);

	assert(aabb1.Intersects(aabb6.Translate(Spatial3D(1.0f))) == false);
	assert(aabb1.Intersects(aabb6.Translate(Spatial3D(0.2f))) == true);
	assert(aabb1.Contains(aabb1) == false);
	assert(aabb1.Contains(aabb2) == false);
	assert(aabb1.Contains(aabb3) == false);
	assert(aabb1.Contains(aabb4) == false);
	assert(aabb1.Contains(aabb5) == false);
	assert(aabb1.Contains(aabb6) == false);
	assert(aabb1.Contains(aabb7) == true);
	AABB aabb8(aabb1.Overlap(aabb6));
	assert(aabb1.Intersects(aabb8) == true);
	assert(aabb2.Intersects(aabb8) == false);
	assert(aabb3.Intersects(aabb8) == false);
	assert(aabb4.Intersects(aabb8) == false);
	assert(aabb5.Intersects(aabb8) == true);
	assert(aabb6.Intersects(aabb8) == true);
	assert(aabb7.Intersects(aabb8) == true);

	Transform transform(Spatial3D(2.0f,1.0f,-1.0f), Quaternion(0.0f,0.0f,0.0f,1.0f), Spatial3D(0.5f,2.0f,3.0f));
	AABB aabb1Transformed = aabb1.Transform(transform.ToMatrix());
	assert(Math::Abs(aabb1Transformed.GetCenter()[0]-2.25f) < 1.e-4f);
	assert(Math::Abs(aabb1Transformed.GetCenter()[1]-2.0f) < 1.e-4f);
	assert(Math::Abs(aabb1Transformed.GetCenter()[2]-0.5f) < 1.e-4f);
	assert(Math::Abs(aabb1Transformed.GetExtents()[0]-0.25f) < 1.e-4f);
	assert(Math::Abs(aabb1Transformed.GetExtents()[1]-1.0f) < 1.e-4f);
	assert(Math::Abs(aabb1Transformed.GetExtents()[2]-1.5f) < 1.e-4f);
}

static void testMath()
{
	testMathTypesMemoryLayout();

	Spatial3D _Translation(5.64635f, 1.325345f, 2.02523f);
	Quaternion _Rotation(Spatial3D(1.242f, 2.2432f, 3.75354f).Normalized().Inner(), 2.54343f);
	Spatial3D _Scale(1.4215f,0.123141f,3.7423f);
	Transform _Transform(_Translation, _Rotation, _Scale);
	Matrix _TransformMat(_Transform.ToMatrix());

	Quaternion _Rot2(_TransformMat.GetRotation());
	assert(_Scale.Inner().Equals(_TransformMat.GetScale()));
	assert(_Translation.Inner().Equals(_TransformMat.GetTranslation()));
	assert(_Rot2.Equals(_Rotation));
	
	assert(Math::Abs(_TransformMat.Determinant4x4() - 0.655071f) < 1.e-4f);
	Matrix _InverseMat = _TransformMat.Inverse();
	Matrix _ShouldBeIdentity = _InverseMat * _TransformMat;
	assert(_ShouldBeIdentity.Equals(Matrix::Identity()));

	Spatial3D _Point(1.337f,3.778f,-2.419f);
	Spatial3D _Point2(1.337f,3.778f,-2.419f);
	_Point = _TransformMat.Transform(_Point.AsIntrinsic(1.0f));
	_Point2 = _Transform.Transforms(_Point2.AsIntrinsic(1.0f));
	assert(_Point.Inner().Equals(_Point2.Inner()));

	Spatial3D _Point3(1.337f,3.778f,-2.419f);
	Spatial3D _Point4(1.337f,3.778f,-2.419f);
	_Point3 = _TransformMat.Inverse().Transform(_Point3.AsIntrinsic(1.0f));
	_Point4 = _Transform.Inverse().Transform(_Point4.AsIntrinsic(1.0f));
	assert(_Point3.Inner().Equals(_Point4.Inner()));
	assert(Spatial3D(_Transform.Transforms(Spatial3D(_Transform.InverseTransform(_Point3, 1.0f)), 1.0f)).Inner().Equals(_Point3.Inner()));
	
	Matrix _InverseMat2(_Transform.Inverse());
	Matrix _TestResult(_InverseMat2*_TransformMat);
	assert(_InverseMat.Equals(_InverseMat2));

	Quaternion _Rotation2(Spatial3D(13.242f, 22.2432f, 3.745354f).Normalized().Inner(), -22.54343f);
	Quaternion _Mul = _Rotation*_Rotation2;
	_Mul = _Mul*_Rotation2.Inverse();
	assert(_Mul.Equals(_Rotation));
}

static void testPlane()
{
	Plane plane1(Spatial3D(1.0f,0.0f,0.0f),-1.0f);
	Plane plane2(Spatial3D(0.0f,1.0f,0.0f),-1.0f);
	Plane plane3(Spatial3D(0.0f,0.0f,1.0f),-1.0f);
	assert(Math::Abs(plane1.dot(Spatial3D(2.0f,0.0f,0.0f))-1.0f) < 1.e-4f);
	assert(Math::Abs(plane2.dot(Spatial3D(2.0f,0.0f,0.0f))+1.0f) < 1.e-4f);
	assert(Math::Abs(plane3.dot(Spatial3D(2.0f,0.0f,0.0f))+1.0f) < 1.e-4f);
	Spatial3D intersectionPoint;
	plane1.intersectPlanes(intersectionPoint,plane2,plane3);
	assert(Math::Abs(plane1.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::Abs(plane2.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::Abs(plane3.dot(intersectionPoint)) < 1.e-4f);

	Plane plane4(Spatial3D(0.13f,0.46f,0.89f).Normalized(),-0.1f);
	Plane plane5(Spatial3D(-0.74f,2.3f,-0.1f).Normalized(),2.3f);
	Plane plane6(Spatial3D(1.0f,-2.0f,10.0f).Normalized(),-23.7f);
	plane4.intersectPlanes(intersectionPoint,plane5,plane6);
	assert(Math::Abs(plane4.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::Abs(plane5.dot(intersectionPoint)) < 1.e-4f);
	assert(Math::Abs(plane6.dot(intersectionPoint)) < 1.e-4f);

	Spatial3D lineStart(0.0f,0.0f,0.0f);
	Spatial3D lineEnd(2.0f,0.0f,0.0f);
	assert(Math::Abs(plane1.intersectLine(lineStart,lineEnd)-0.5f) < 1.e-4);
	float amt = plane5.intersectLine(lineStart,lineEnd);
	intersectionPoint = lineStart+(lineEnd-lineStart)*amt;
	assert(Math::Abs(plane5.dot(intersectionPoint)) < 1.e-4f);
	amt = plane6.intersectLine(lineStart,lineEnd);
	intersectionPoint = lineStart+(lineEnd-lineStart)*amt;
	assert(Math::Abs(plane6.dot(intersectionPoint)) < 1.e-4f);

	Spatial3D scale = Spatial3D(0.4f,2.3f,1.7f);
	Transform transform(Spatial3D(0.0f,0.0f,2.0f).Inner(),
			Quaternion(Spatial3D(0.0f,1.0f,0.0f).Inner(),Math::ToRad(90.0f)),
		scale);
	Matrix transformMat = transform.ToMatrix();

	Plane plane1Transformed = plane1.transform(transformMat);
	assert(Math::Abs(plane1Transformed.dot(Spatial3D(2.0f,0.0f,0.0f))-1.6f) < 1.e-4f);
}

static void testIntersects()
{
	Plane plane1(Spatial3D(1.0f,0.0f,0.0f),0.0f);
	AABB aabb1(Spatial3D(1.0f), Spatial3D(2.0f));
	AABB aabb2(Spatial3D(-2.0f), Spatial3D(-1.0f));
	AABB aabb3(Spatial3D(-1.0f), Spatial3D(1.0f));
	bool isFullyInside;
	bool isPartiallyInside;
	
	Intersects::intersectPlaneAABB(aabb1, plane1, isFullyInside, isPartiallyInside);
	assert(isFullyInside && isPartiallyInside);
	Intersects::intersectPlaneAABB(aabb2, plane1, isFullyInside, isPartiallyInside);
	assert(!isFullyInside && !isPartiallyInside);
	Intersects::intersectPlaneAABB(aabb3, plane1, isFullyInside, isPartiallyInside);
	assert(!isFullyInside && isPartiallyInside);

	Sphere sphere1(Spatial3D(1.0f), 0.5f);
	Sphere sphere2(Spatial3D(-1.0f), 0.5f);
	Sphere sphere3(Spatial3D(0.0f), 0.5f);

	Intersects::intersectPlaneSphere(sphere1,plane1,isFullyInside,isPartiallyInside);
	assert(isFullyInside && isPartiallyInside);
	Intersects::intersectPlaneSphere(sphere2,plane1,isFullyInside,isPartiallyInside);
	assert(!isFullyInside && !isPartiallyInside);
	Intersects::intersectPlaneSphere(sphere3,plane1,isFullyInside,isPartiallyInside);
	assert(!isFullyInside && isPartiallyInside);

	assert(Intersects::intersectSphereAABB(sphere1,aabb1));
	assert(!Intersects::intersectSphereAABB(sphere1,aabb2));
	assert(Intersects::intersectSphereAABB(sphere1,aabb3));
	assert(!Intersects::intersectSphereAABB(sphere2,aabb1));
	assert(Intersects::intersectSphereAABB(sphere2,aabb2));
	assert(Intersects::intersectSphereAABB(sphere2,aabb3));
	assert(!Intersects::intersectSphereAABB(sphere3,aabb1));
	assert(!Intersects::intersectSphereAABB(sphere3,aabb2));
	assert(Intersects::intersectSphereAABB(sphere3,aabb3));

	Sphere sphere4(Spatial3D(0.0f,1.2f,0.0f), 0.5f);
	AABB aabb4(Spatial3D(-100.0f,0.3f,0.2f),Spatial3D(0.5f,0.6f,0.5f));
	assert(!Intersects::intersectSphereAABB(sphere4,aabb4));

	float p1,p2;
	assert(sphere3.intersectRay(Spatial3D(0.0f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::Abs(p1-2.5f) < 1.e-4f);
	assert(Math::Abs(p2-3.5f) < 1.e-4f);

	assert(sphere3.intersectRay(Spatial3D(-0.5f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::Abs(p1-3.0f) < 1.e-4f);
	assert(Math::Abs(p2-3.0f) < 1.e-4f);
	assert(!sphere3.intersectRay(Spatial3D(0.6f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));

	assert(sphere3.intersectLine(Spatial3D(0.0f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,3.0f)));
	assert(sphere3.intersectLine(Spatial3D(-0.4f,0.0f,-3.0f),Spatial3D(-0.4f,0.0f,3.0f)));
	assert(sphere3.intersectLine(Spatial3D(0.4f,0.0f,-3.0f),Spatial3D(0.4f,0.0f,3.0f)));
	assert(!sphere3.intersectLine(Spatial3D(0.6f,0.0f,-3.0f),Spatial3D(0.6f,0.0f,3.0f)));
	assert(sphere3.intersectLine(Spatial3D(0.5f,0.0f,-3.0f),Spatial3D(0.5f,0.0f,3.0f)));

	assert(aabb3.IntersectRay(Spatial3D(0.0f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::Abs(p1-2.0f) < 1.e-4f);
	assert(Math::Abs(p2-4.0f) < 1.e-4f);
	assert(aabb3.IntersectRay(Spatial3D(0.99f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::Abs(p1-2.0f) < 1.e-4f);
	assert(Math::Abs(p2-4.0f) < 1.e-4f);
	assert(aabb3.IntersectRay(Spatial3D(1.0f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(!aabb3.IntersectRay(Spatial3D(1.01f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(!aabb3.IntersectRay(Spatial3D(2.0f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(aabb3.IntersectRay(Spatial3D(-0.5f,0.0f,-3.0f),Spatial3D(0.0f,0.0f,1.0f),p1,p2));
	assert(Math::Abs(p1-2.0f) < 1.e-4f);
	assert(Math::Abs(p2-4.0f) < 1.e-4f);

	Spatial3D points[] = {
		Spatial3D(1.0f,1.0f,1.0f),
		Spatial3D(1.0f,0.0f,1.0f),
		Spatial3D(0.0f,1.0f,1.0f),
		Spatial3D(0.0f,-1.0f,-1.0f),
		Spatial3D(0.0f,0.0f,0.2f),
		Spatial3D(0.7f,0.4f,0.3f),
		Spatial3D(0.3f,-0.8f,0.4f),
		Spatial3D(0.3f,-0.8f,-0.4f),
	};
	AABB boundingAABB(points, ARRAY_SIZE_IN_ELEMENTS(points));
	assert(boundingAABB.GetMinExtents().Inner().Equals(Spatial3D(0.0f, -1.0f, -1.0f).Inner()));
	assert(boundingAABB.GetMaxExtents().Inner().Equals(Spatial3D(1.0f, 1.0f, 1.0f).Inner()));

	Sphere boundingSphere(points, ARRAY_SIZE_IN_ELEMENTS(points));
	assert(boundingSphere.getCenter().Inner().Equals(Spatial3D(0.5f,0.0f,0.0f).Inner()));
	assert(Math::Equals(boundingSphere.getRadius(), 1.5f, 1.e-4f));

	float points2[] = {
		1.0f,1.0f,1.0f,
		1.0f,0.0f,1.0f,
		0.0f,1.0f,1.0f,
		0.0f,-1.0f,-1.0f,
		0.0f,0.0f,0.2f,
		0.7f,0.4f,0.3f,
		0.3f,-0.8f,0.4f,
		0.3f,-0.8f,-0.4f,
	};

	boundingAABB = AABB(points2, ARRAY_SIZE_IN_ELEMENTS(points2)/3);
	assert(boundingAABB.GetMinExtents().Inner().Equals(Spatial3D(0.0f,-1.0f,-1.0f).Inner()));
	assert(boundingAABB.GetMaxExtents().Inner().Equals(Spatial3D(1.0f,1.0f,1.0f).Inner()));

	boundingSphere = Sphere(points2, ARRAY_SIZE_IN_ELEMENTS(points2)/3);
	assert(boundingSphere.getCenter().Inner().Equals(Spatial3D(0.5f,0.0f,0.0f).Inner()));
	assert(Math::Equals(boundingSphere.getRadius(), 1.5f, 1.e-4f));
}

void testMemory()
{
//	int32 v1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//	int32 v2[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};
//
//	for(uint32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(v1); i++) {
//		DEBUG_LOG_TEMP("%i, %i", v1[i], v2[i]);
//	}
//
//	Memory::memswap(v1, v2, sizeof(v1));
//	DEBUG_LOG_TEMP("%i", sizeof(v1));
//
//	for(uint32 i = 0; i < ARRAY_SIZE_IN_ELEMENTS(v1); i++) {
//		DEBUG_LOG_TEMP("%i, %i", v1[i], v2[i]);
//	}

}


void Tests::RunTests()
{
	testSphere();
	testAABB();
	testMath();
	testPlane();
	testIntersects();
	testMemory();
}

inline void naiveMatrixMultiply(float* output, float* input, float* other)
{
	float* m = (float*)input;
	float* r = (float*)other;
	float* ret = (float*)output;
	for (unsigned int i = 0 ; i < 4; i++) 
	{
		for (unsigned int j = 0 ; j < 4; j++) 
		{
			ret[i*4+j] = 0.0f;
			for(unsigned int k = 0; k < 4; k++) {
				ret[i*4+j] += m[k*4+j] * r[i*4+k];
			}
		}
	}
}

inline void naiveCrossProduct(float* output, float* v1, float* v2)
{
	float out0 = v1[1] * v2[2] - v1[2] * v2[1];
	float out1 = v1[2] * v2[0] - v1[0] * v2[2];
	float out2 = v1[0] * v2[1] - v1[1] * v2[0];
	output[0] = out0;
	output[1] = out1;
	output[2] = out2;
}

inline void naiveQuatMul(float* output, float* a, float* b)
{
	const float w = (a[3] * b[3]) - (a[0] * b[0]) - (a[1] * b[1]) - (a[2] * b[2]);
	const float x = (a[0]* b[3]) + (a[3] * b[0]) + (a[1] * b[2]) - (a[2] * b[1]);
	const float y = (a[1] * b[3]) + (a[3] * b[1]) + (a[2] * b[0]) - (a[0] * b[2]);
	const float z = (a[2] * b[3]) + (a[3] * b[2]) + (a[0] * b[1]) - (a[1] * b[0]);

	output[0] = x;
	output[1] = y;
	output[2] = z;
	output[3] = w;
}

inline void naiveQuatRotate(float* output, float* a, float* b)
{
	float conjugate[4];
	float temp[4];
	conjugate[0] = -a[0];
	conjugate[1] = -a[1];
	conjugate[2] = -a[2];
	conjugate[3] = a[3];
	naiveQuatMul(temp, a, b);
	naiveQuatMul(output, temp, conjugate);
}

inline void naiveTransformCreate(float* output, float* translation, float* rotation, float* scale)
{
	Spatial3D translationVec(translation[0],translation[1],translation[2]);
	Spatial3D scaleVec(scale[0],scale[1],scale[2]);
	Quaternion rotationVec(rotation[0],rotation[1],rotation[2],rotation[3]);
	Spatial3D nullTranslation(0.0f);
	Spatial3D nullScale(1.0f);
	Matrix translationMatrix = Matrix::Translate(translationVec.Inner());
	Matrix rotationMatrix = Matrix::TransformMatrix(nullTranslation.Inner(), rotationVec, nullScale.Inner());
	Matrix scaleMatrix = Matrix::Scale(scaleVec.Inner());
	float temp[16];
	naiveMatrixMultiply(temp, (float*)&scaleMatrix, (float*)&rotationMatrix);
	naiveMatrixMultiply(output, temp, (float*)&translationMatrix);
}

inline void naiveMatrixInverse(void* dest, void* src)
{
	int i, j, k;
	float* s = (float*)dest;
	float* t = (float*)src;
	const int D = 4;

	// Forward elimination
	for (i = 0; i < D - 1 ; i++) {
		int pivot = i;

		float pivotsize = t[i*4+i];

		if (pivotsize < 0)
			pivotsize = -pivotsize;

		for (j = i + 1; j < D; j++) {
			float tmp = t[j*4+i];

			if (tmp < 0)
				tmp = -tmp;

			if (tmp > pivotsize) {
				pivot = j;
				pivotsize = tmp;
			}
		}

		if (pivotsize == 0) {
			return;
		}

		if (pivot != i) {
			for (j = 0; j < D; j++) {
				float tmp;

				tmp = t[i*4+j];
				t[i*4+j] = t[pivot*4+j];
				t[pivot*4+j] = tmp;

				tmp = s[i*4+j];
				s[i*4+j] = s[pivot*4+j];
				s[pivot*4+j] = tmp;
			}
		}

		for (j = i + 1; j < D; j++) {
			float f = t[j*4+i] / t[i*4+i];

			for (k = 0; k < D; k++) {
				t[j*4+k] -= f * t[i*4+k];
				s[j*4+k] -= f * s[i*4+k];
			}
		}
	}

	// Backward substitution
	for (i = D - 1; i >= 0; --i) {
		float f;

		if ((f = t[i*4+i]) == 0) {
			return;
		}

		for (j = 0; j < D; j++) {
			t[i*4+j] /= f;
			s[i*4+j] /= f;
		}

		for (j = 0; j < i; j++) {
			f = t[j*4+i];

			for (k = 0; k < D; k++) {
				t[j*4+k] -= f * t[i*4+k];
				s[j*4+k] -= f * s[i*4+k];
			}
		}
	}
}

void Tests::runPerformanceTests()
{
	double startTime = Time::getTime();
	Transform transform;
	Matrix transformMat = transform.ToMatrix();
	// Performance results for release build on AMD Athlon X4 860K
	// 100M sets of 8 dot3 products = 0.602984 seconds (or 1.122339)
	// 100M sets of 8 naive dot3 products = ~0.66 seconds (or 1.177717)
	// Vectorized ~ 1.1x faster

	// 100M sets of 8 dot4 products = 0.749585 seconds (or 1.19)
	// 100M sets of 8 naive dot4 products = ~1.15 seconds (or 1.478542)
	// Vectorized ~ 1.5x faster

	// 100M sets of 8 cross products = 0.421812 seconds (or 0.816521)
	// 100M sets of 8 generic cross products = 1.787127 seconds
	// 100M sets of 8 naive cross products = 1.908947 seconds
	// Vectorized ~ 2 to 4.5x faster

	// 100M sets of 8 quaternion multiplies = 1.981629 seconds
	// 100M sets of 8 generic quaternion multiplies = 5.189267 seconds (now replaced with naive method)
	// 100M sets of 8 naive quaternion multiplies = 4.489309 seconds
	// Vectorized ~ 2.27x faster

// NOTE: From this point on, some of the algorithmic improvements
// could also be applied to the non-vectorized techniques

	// 100M sets of 8 quaternion rotates = 2.076706 seconds
	// 100M sets of 8 generic quaternion rotates = 5.436006 seconds
	// 100M sets of 8 naive quaternion rotates = 7.881377 seconds
	// Vectorized ~ 3.79x faster

	// 100M sets of 2 matrix multiplies = 1.244078 seconds
	// 100M sets of 2 generic matrix multiplies = 3.678728 seconds
	// 100M sets of 2 naive matrix multiplies = 32.592669 seconds
	// Vectorized ~ 26.2x faster

	// 100M sets of 2 transform matrix generations = 0.142155 seconds (???) (Probably more like 1.8 seconds in reality)
	// 100M sets of 2 naive transform matrix generations = 34.912099 seconds
	// Vectorized ~ 245x faster?????  (Probably more like 20x in reality)

	// 100M sets of 2 matrix inverses = 3.439625 seconds
	// 100M sets of 2 generic matrix inverses = 7.160924 seconds
	// 100M sets of 2 naive matrix inverses = 26.904145 seconds
	// 100M sets of 2 UE4 matrix inverses = 6.570406 seconds
	// 100M sets of 2 algorithmically faster matrix inverses = 6.578339 seconds
	// Vectorized ~ 7.82x faster

	Vector vec1s[8] = {
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
	};
	Vector vec2s[8] = {
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
		Vector::Make(1.0f,1.0f,1.0f,1.0f),
	};

	float point1Floats[8][4];
	float point2Floats[8][4];
	float temp = 4.0f;
	for(uint32 i = 0; i < 8; i++) {
//		vec1s[i] = Vector::make(Math::randf()*2.0f-1.0f, Math::randf()*2.0f-1.0f, Math::randf()*2.0f-1.0f, Math::randf()*2.0f-1.0f);
//		vec2s[i] = Vector::make(Math::randf()*2.0f-1.0f, Math::randf()*2.0f-1.0f, Math::randf()*2.0f-1.0f, Math::randf()*2.0f-1.0f);
		vec1s[i] = vec1s[i] / Vector::Make(temp,temp,temp,temp);
		vec2s[i] = vec2s[i] / Vector::Make(temp,temp,temp,temp);
		vec1s[i].Store4f(point1Floats[i]);
		vec2s[i].Store4f(point2Floats[i]);
	}
	for(uint32 i = 0; i < 100000000; i++) {
//		naiveQuatRotate(point1Floats[0],point1Floats[0],point2Floats[0]);
//		naiveQuatRotate(point1Floats[1],point1Floats[1],point2Floats[1]);
//		naiveQuatRotate(point1Floats[2],point1Floats[2],point2Floats[2]);
//		naiveQuatRotate(point1Floats[3],point1Floats[3],point2Floats[3]);
//		naiveQuatRotate(point1Floats[4],point1Floats[4],point2Floats[4]);
//		naiveQuatRotate(point1Floats[5],point1Floats[5],point2Floats[5]);
//		naiveQuatRotate(point1Floats[6],point1Floats[6],point2Floats[6]);
//		naiveQuatRotate(point1Floats[7],point1Floats[7],point2Floats[7]);
//		naiveMatrixMultiply(point1Floats[0], point1Floats[0], point1Floats[0]);
//		naiveMatrixMultiply(point2Floats[4], point2Floats[4], point2Floats[4]);
//		Vector::matrixInverse(&vec1s[0], &vec1s[0]);
//		Vector::matrixInverse(&vec1s[4], &vec1s[4]);
//		Vector::matrixMul(&vec1s[0], &vec1s[0], &vec1s[0]);
//		Vector::matrixMul(&vec1s[4], &vec1s[4], &vec1s[4]);
//		Vector::createTransformMatrix(&vec1s[0], vec1s[0], vec2s[0], vec2s[0]);
//		Vector::createTransformMatrix(&vec1s[4], vec1s[4], vec2s[4], vec2s[4]);
//		naiveTransformCreate(point1Floats[0], point1Floats[0], point2Floats[0], point2Floats[0]);
//		naiveTransformCreate(point1Floats[4], point1Floats[4], point2Floats[4], point2Floats[4]);
//		vec1s[0] = vec1s[0].quatRotateVec(vec2s[0]);
//		vec1s[0] = vec1s[0].quatRotateVec(vec2s[0]);
//		vec1s[1] = vec1s[1].quatRotateVec(vec2s[1]);
//		vec1s[2] = vec1s[2].quatRotateVec(vec2s[2]);
//		vec1s[3] = vec1s[3].quatRotateVec(vec2s[3]);
//		vec1s[4] = vec1s[4].quatRotateVec(vec2s[4]);
//		vec1s[5] = vec1s[5].quatRotateVec(vec2s[5]);
//		vec1s[6] = vec1s[6].quatRotateVec(vec2s[6]);
//		vec1s[7] = vec1s[7].quatRotateVec(vec2s[7]);
		for(uint32 j = 0; j < 8; j++) {
//			vec1s[j] = vec1s[j].dot4(vec2s[j]);
//			float dotProduct0 = point1Floats[j][0]*point2Floats[j][0]+point1Floats[j][1]*point2Floats[j][1]+point1Floats[j][2]*point2Floats[j][2]+point1Floats[j][3]*point2Floats[j][3];
//			point1Floats[j][0] = dotProduct0;
//			point1Floats[j][1] = dotProduct0;
//			point1Floats[j][2] = dotProduct0;
//			point1Floats[j][3] = dotProduct0;
		}
	}
	for(uint32 i = 0; i < 8; i++) {
//		vec1s[i]=vec1s[i].load4f(point1Floats[i]);
//		vec2s[i]=vec2s[i].load4f(point2Floats[i]);
	}
	Vector pointVector=vec1s[0]+vec1s[1]+vec1s[2]+vec1s[3]+vec1s[4]+vec1s[5]+vec1s[6]+vec1s[7];
	Vector point=transformMat.Transform(pointVector);
	double passedTime = Time::getTime() - startTime;
	DEBUG_LOG_TEMP("%f", passedTime);
	DEBUG_LOG_TEMP("%f %f %f", point[0], point[1], point[2]);
	DEBUG_LOG_TEMP("%f %f %f", pointVector[0], pointVector[1], pointVector[2]);
}


