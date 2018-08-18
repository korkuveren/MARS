#pragma once

#include "VecMath.h"

class Color
{
public:
	static const Color WHITE;
	static const Color BLACK;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color YELLOW;
	static const Color MAGENTA;
	static const Color TRANSPARENT;

	FORCEINLINE Color(float red, float green, float blue, float alpha=1.0f);
	FORCEINLINE Color(const Vector& colorVector);

	FORCEINLINE float operator[](uint32 index) const;
	FORCEINLINE Color operator-() const;
	FORCEINLINE Color operator+(const Color& other) const;
	FORCEINLINE Color operator+=(const Color& other);
	FORCEINLINE Color operator-(const Color& other) const;
	FORCEINLINE Color operator-=(const Color& other);
	FORCEINLINE Color operator*(const Color& other) const;
	FORCEINLINE Color operator*=(const Color& other);
	FORCEINLINE Color operator/(const Color& other) const;
	FORCEINLINE Color operator/=(const Color& other);
	FORCEINLINE bool operator==(const Color& other) const;
	FORCEINLINE bool operator!=(const Color& other) const;
	FORCEINLINE bool equals(const Color& other, float errorMargin=1.e-4f) const;

	FORCEINLINE float getLuminance() const;
	FORCEINLINE Color quantized() const;
	FORCEINLINE int32 toInt() const;

	FORCEINLINE Color contrastAdjust(float contrast) const;
	FORCEINLINE Color saturationAdjust(float saturation) const;
	FORCEINLINE Color varianceAdjust(float variance) const;
private:
	static const Vector luminanceVector;

	Vector data;
};

FORCEINLINE Color::Color(float red, float green, float blue, float alpha) : data(Vector::Make(red, green, blue, alpha)) {}

FORCEINLINE Color::Color(const Vector& colorVector) :
	data(colorVector) {}

FORCEINLINE float Color::operator[](uint32 index) const
{
	return data[index];
}

FORCEINLINE Color Color::operator-() const
{
	return Color((VectorConstants::ONE - data).Select(VectorConstants::MASK_W, data));
}

FORCEINLINE Color Color::operator+(const Color& other) const
{
	return Color(data + other.data);
}

FORCEINLINE Color Color::operator+=(const Color& other)
{
	data = data + other.data;
	return *this;
}

FORCEINLINE Color Color::operator-(const Color& other) const
{
	return Color(data - other.data);
}

FORCEINLINE Color Color::operator-=(const Color& other)
{
	data = data - other.data;
	return *this;
}

FORCEINLINE Color Color::operator*(const Color& other) const
{
	return Color(data * other.data);
}

FORCEINLINE Color Color::operator*=(const Color& other)
{
	data = data * other.data;
	return *this;
}

FORCEINLINE Color Color::operator/(const Color& other) const
{
	return Color(data / other.data);
}

FORCEINLINE Color Color::operator/=(const Color& other)
{
	data = data / other.data;
	return *this;
}

FORCEINLINE bool Color::operator==(const Color& other) const
{
	return (data != other.data).IsZero4f();
}

FORCEINLINE bool Color::operator!=(const Color& other) const
{
	return (data == other.data).IsZero4f();
}

FORCEINLINE bool Color::equals(const Color& other, float errorMargin) const
{
	return (data.NotEquals(other.data, errorMargin)).IsZero4f();
}


FORCEINLINE float Color::getLuminance() const
{
	return luminanceVector.Dot4(data)[0];
}

FORCEINLINE Color Color::quantized() const
{
	float vals[4];
	Vector conversionAmt(Vector::Load1f(255.0f));
	(data * conversionAmt).Store4f(vals);
	for(uint32 i = 0; i < 4; i++) {
		vals[i] = Math::Clamp(vals[i], 0.0f, 255.0f);
		vals[i] = Math::RoundToFloat(vals[i]);
	}
	return Color(Vector::Load4f(vals) / conversionAmt);
}

FORCEINLINE int32 Color::toInt() const
{
	float vals[4];
	Vector conversionAmt(Vector::Load1f(255.0f));
	(data * conversionAmt).Store4f(vals);
	uint32 result = 0;
	for(uint32 i = 0; i < 4; i++) {
		vals[i] = Math::Clamp(vals[i], 0.0f, 255.0f);
		result = (result << 8) | (uint32)Math::RoundToInt(vals[i]);
	}
	return (int32)result;
}


FORCEINLINE Color Color::contrastAdjust(float contrast) const
{
	return Color(
			Math::Lerp(VectorConstants::HALF, data, contrast)
			.Select(VectorConstants::MASK_W, data));
}

FORCEINLINE Color Color::saturationAdjust(float saturation) const
{
	return Color(
			Math::Lerp(Vector::Load1f(getLuminance()), data, saturation)
			.Select(VectorConstants::MASK_W, data));
}

FORCEINLINE Color Color::varianceAdjust(float variance) const
{
	return Color(
			(data * Vector::Load1f(variance))
			.Select(VectorConstants::MASK_W, data));
}
