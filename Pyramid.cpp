#include "Pyramid.h"
#include "Constants.h"
#include <iostream>
#include <limits>
#include <cmath>

Pyramid::Pyramid(int baseLength, int height):
	baseLength(PyramidDimension::BASE_LENGTH, baseLength),
	height(PyramidDimension::HEIGHT, height)
{
	basePerimeter = std::make_pair(PyramidDimension::BASE_PERIMETER, CalculateBasePerimeter());
	baseDiagonal = std::make_pair(PyramidDimension::BASE_DIAGONAL, CalculateBaseDiagonal());
	slantLength = std::make_pair(PyramidDimension::SLANT_LENGTH, CalculateSlantLength());
	lateralEdgeLength = std::make_pair(PyramidDimension::LATERAL_EDGE_LENGTH, CalculateLateralEdgeLength());

	slantAngle = std::make_pair(PyramidDimension::SLANT_ANGLE, CalculateSlantAngle());
	ninetyDegreesMinusSlantAngle = std::make_pair(PyramidDimension::NINETY_DEGREES_MINUS_SLANT_ANGLE, CalculateNinetyDegreesMinusSlantAngle());
	vertexAngle = std::make_pair(PyramidDimension::VERTEX_ANGLE, CalculateVertexAngle());
	cornerAngle = std::make_pair(PyramidDimension::CORNER_ANGLE, CalculateCornerAngle());

	baseArea = std::make_pair(PyramidDimension::BASE_AREA, CalculateBaseArea());
	lateralFaceArea = std::make_pair(PyramidDimension::LATERAL_FACE_AREA, CalculateLateralFaceArea());
	surfaceAreaNotIncludingBase = std::make_pair(PyramidDimension::SURFACE_AREA_NOT_INCLUDING_BASE, CalculateSurfaceAreaNotIncludingBase());
	surfaceAreaIncludingBase = std::make_pair(PyramidDimension::SURFACE_AREA_INCLUDING_BASE, CalculateSurfaceAreaIncludingBase());

	InitializeLengths();
	InitializeAngles();
	InitializeAreas();
}

void Pyramid::Print()
{
	std::cout << "base length: " << baseLength.second << '\n';
	std::cout << "height: " << height.second << '\n';
	std::cout << "base perimeter: " << basePerimeter.second << '\n';
	std::cout << "base diagonal: " << baseDiagonal.second << '\n';
	std::cout << "slant length: " << slantLength.second << '\n';
	std::cout << "lateral edge length: " << lateralEdgeLength.second << '\n';

	std::cout << "slant angle: " << slantAngle.second * 180.0 / const_pi() << '\n';
	std::cout << "90 - slant angle: " << ninetyDegreesMinusSlantAngle.second * 180.0 / const_pi() << '\n';
	std::cout << "vertex angle: " << vertexAngle.second * 180.0 / const_pi() << '\n';
	std::cout << "corner angle: " << cornerAngle.second * 180.0 / const_pi() << '\n';

	std::cout << "base area: " << baseArea.second << '\n';
	std::cout << "lateral face area: " << lateralFaceArea.second << '\n';
	std::cout << "surface area including base: " << surfaceAreaIncludingBase.second << '\n';
	std::cout << "surface area not including base: " << surfaceAreaNotIncludingBase.second << '\n';
}

double Pyramid::CalculateAbsoluteError(double actual, double expected)
{
	return abs(actual - expected);
}

GetClosestResult Pyramid::GetClosest(double target)
{
	GetClosestResult closest1 = GetClosest_Helper(lengths, target);
	GetClosestResult closest2 = GetClosest_Helper(angles, target);
	GetClosestResult closest3 = GetClosest_Helper(areas, target);

	double absoluteError1 = CalculateAbsoluteError(closest1.value, target);
	double absoluteError2 = CalculateAbsoluteError(closest2.value, target);
	double absoluteError3 = CalculateAbsoluteError(closest3.value, target);

	GetClosestResult retval = closest1;
	double minAbsoluteError = absoluteError1;

	if (absoluteError2 < minAbsoluteError)
	{
		retval = closest2;
		minAbsoluteError = absoluteError2;
	}

	if (absoluteError3 < minAbsoluteError)
	{
		retval = closest3;
	}

	return retval;
}

GetClosestResult Pyramid::GetClosest_Helper(const std::vector<std::pair<PyramidDimension, double>*>& dimensions, double target)
{
	GetClosestResult closest;
	double minAbsoluteError = std::numeric_limits<double>::max();

	std::vector<double> allowedFactors = { 0.1, 0.125, 0.25, 0.5, 1.0, 2.0, 4.0, 8.0, 10.0 };

	for (int i = 0; i < dimensions.size(); ++i)
	{
		for (int j = 0; j < dimensions.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			double temp = (*dimensions[i]).second / (*dimensions[j]).second;

			for (int k = 0; k < allowedFactors.size(); ++k)
			{
				double scaledTemp = temp / allowedFactors[k];
				double absoluteError = CalculateAbsoluteError(scaledTemp, target);
				if (absoluteError < minAbsoluteError)
				{
					closest.dimension1 = *dimensions[i];
					closest.dimension2 = *dimensions[j];
					closest.value = scaledTemp;
					minAbsoluteError = absoluteError;
				}
			}
		}
	}

	return closest;
}

void Pyramid::InitializeLengths()
{
	lengths.push_back(&baseLength);
	lengths.push_back(&height);
	lengths.push_back(&basePerimeter);
	lengths.push_back(&baseDiagonal);
	lengths.push_back(&slantLength);
	lengths.push_back(&lateralEdgeLength);
}

void Pyramid::InitializeAngles()
{
	angles.push_back(&slantAngle);
	angles.push_back(&ninetyDegreesMinusSlantAngle);
	angles.push_back(&vertexAngle);
	angles.push_back(&cornerAngle);
}

void Pyramid::InitializeAreas()
{
	areas.push_back(&baseArea);
	areas.push_back(&lateralFaceArea);
	areas.push_back(&surfaceAreaNotIncludingBase);
	areas.push_back(&surfaceAreaIncludingBase);
}

double Pyramid::CalculateBasePerimeter()
{
	return 4.0 * baseLength.second;
}

double Pyramid::CalculateBaseDiagonal()
{
	return sqrt(2.0) * baseLength.second;
}

double Pyramid::CalculateSlantLength()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return sqrt(halfBaseLength * halfBaseLength + height.second * height.second);
}

double Pyramid::CalculateLateralEdgeLength()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return sqrt(halfBaseLength * halfBaseLength + slantLength.second * slantLength.second);
}

double Pyramid::CalculateSlantAngle()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return atan(height.second / halfBaseLength);
}

double Pyramid::CalculateNinetyDegreesMinusSlantAngle()
{
	return 0.5 * const_pi() - slantAngle.second;
}

double Pyramid::CalculateVertexAngle()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return 2.0 * atan(halfBaseLength / slantLength.second);
}

double Pyramid::CalculateCornerAngle()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return atan(slantLength.second / halfBaseLength);
}

double Pyramid::CalculateBaseArea()
{
	return baseLength.second * baseLength.second;
}

double Pyramid::CalculateLateralFaceArea()
{
	return baseLength.second * slantLength.second * 0.5;
}

double Pyramid::CalculateSurfaceAreaNotIncludingBase()
{
	return 4.0 * lateralFaceArea.second;
}

double Pyramid::CalculateSurfaceAreaIncludingBase()
{
	return 4.0 * lateralFaceArea.second + baseArea.second;
}