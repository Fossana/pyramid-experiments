#include "Pyramid.h"
#include "Constants.h"
#include <iostream>
#include <limits>
#include <cmath>
#include <iomanip>

Pyramid::Pyramid(int baseLength, int height):
	baseLength(PyramidDimension::BASE_LENGTH, baseLength),
	height(PyramidDimension::HEIGHT, height)
{
	basePerimeter = std::make_pair(PyramidDimension::BASE_PERIMETER, CalculateBasePerimeter());
	baseDiagonal = std::make_pair(PyramidDimension::BASE_DIAGONAL, CalculateBaseDiagonal());
	slantLength = std::make_pair(PyramidDimension::SLANT_LENGTH, CalculateSlantLength());
	lateralEdgeLength = std::make_pair(PyramidDimension::LATERAL_EDGE_LENGTH, CalculateLateralEdgeLength());

	westEastCrossSectionCornerAngle = std::make_pair(PyramidDimension::WEST_EAST_CROSS_SECTION_CORNER_ANGLE, CalculateWestEastCrossSectionCornerAngle());
	westEastCrossSectionVertexAngle = std::make_pair(PyramidDimension::WEST_EAST_CROSS_SECTION_VERTEX_ANGLE, CalculateWestEastCrossSectionVertexAngle());
	lateralFaceCornerAngle = std::make_pair(PyramidDimension::LATERAL_FACE_CORNER_ANGLE, CalculateLateralFaceCornerAngle());
	lateralFaceVertexAngle = std::make_pair(PyramidDimension::LATERAL_FACE_VERTEX_ANGLE, CalculateLateralFaceVertexAngle());
	southwestNortheastCrossSectionCornerAngle = std::make_pair(PyramidDimension::SOUTHWEST_NORTHEAST_CROSS_SECTION_CORNER_ANGLE, CalculateSouthwestNortheastCrossSectionCornerAngle());
	southwestNortheastCrossSectionVertexAngle = std::make_pair(PyramidDimension::SOUTHWEST_NORTHEAST_CROSS_SECTION_VERTEX_ANGLE, CalculateSouthwestNortheastCrossSectionVertexAngle());

	baseArea = std::make_pair(PyramidDimension::BASE_AREA, CalculateBaseArea());
	lateralFaceArea = std::make_pair(PyramidDimension::LATERAL_FACE_AREA, CalculateLateralFaceArea());
	surfaceAreaNotIncludingBase = std::make_pair(PyramidDimension::SURFACE_AREA_NOT_INCLUDING_BASE, CalculateSurfaceAreaNotIncludingBase());
	surfaceAreaIncludingBase = std::make_pair(PyramidDimension::SURFACE_AREA_INCLUDING_BASE, CalculateSurfaceAreaIncludingBase());

	volume = std::make_pair(PyramidDimension::VOLUME, CalculateVolume());

	InitializeLengths();
	InitializeAngles();
	InitializeAreas();
}

double Pyramid::GetBasePerimeter() const
{
	return basePerimeter.second;
}

double Pyramid::GetHeight() const
{
	return height.second;
}

void Pyramid::Print()
{
	std::cout << "base length: " << std::setprecision(15) << baseLength.second << '\n';
	std::cout << "height: " << std::setprecision(15) << height.second << '\n';
	std::cout << "base perimeter: " << std::setprecision(15) << basePerimeter.second << '\n';
	std::cout << "base diagonal: " << std::setprecision(15) << baseDiagonal.second << '\n';
	std::cout << "slant length: " << std::setprecision(15) << slantLength.second << '\n';
	std::cout << "lateral edge length: " << std::setprecision(15) << lateralEdgeLength.second << '\n';

	std::cout << PyramidDimensionStrings[(int) westEastCrossSectionCornerAngle.first] << ": " << std::setprecision(15) << westEastCrossSectionCornerAngle.second * 180.0 / const_pi() << '\n';
	std::cout << PyramidDimensionStrings[(int) westEastCrossSectionVertexAngle.first] << ": " << std::setprecision(15) << westEastCrossSectionVertexAngle.second * 180.0 / const_pi() << '\n';
	std::cout << PyramidDimensionStrings[(int) lateralFaceCornerAngle.first] << ": " << std::setprecision(15) << lateralFaceCornerAngle.second * 180.0 / const_pi() << '\n';
	std::cout << PyramidDimensionStrings[(int) lateralFaceVertexAngle.first] << ": " << std::setprecision(15) << lateralFaceVertexAngle.second * 180.0 / const_pi() << '\n';
	std::cout << PyramidDimensionStrings[(int) southwestNortheastCrossSectionCornerAngle.first] << ": " << std::setprecision(15) << southwestNortheastCrossSectionCornerAngle.second * 180.0 / const_pi() << '\n';
	std::cout << PyramidDimensionStrings[(int) southwestNortheastCrossSectionVertexAngle.first] << ": " << std::setprecision(15) << southwestNortheastCrossSectionVertexAngle.second * 180.0 / const_pi() << '\n';

	std::cout << "base area: " << std::setprecision(15) << baseArea.second << '\n';
	std::cout << "lateral face area: " << std::setprecision(15) << lateralFaceArea.second << '\n';
	std::cout << "surface area including base: " << std::setprecision(15) << surfaceAreaIncludingBase.second << '\n';
	std::cout << "surface area not including base: " << std::setprecision(15) << surfaceAreaNotIncludingBase.second << '\n';

	std::cout << PyramidDimensionStrings[(int) volume.first] << ": " << std::setprecision(15) << volume.second << '\n';
}

double Pyramid::CalculateAbsoluteError(double actual, double expected)
{
	return abs(actual - expected);
}

double Pyramid::GetVolume() const
{
	return volume.second;
}

GetClosestResult Pyramid::GetClosest(double target)
{
	GetClosestResult closest1 = GetClosest_Helper(lengths, target);
	GetClosestResult closest2 = GetClosest_Helper(angles1, target);
	GetClosestResult closest3 = GetClosest_Helper(angles2, target);
	GetClosestResult closest4 = GetClosest_Helper(angles3, target);
	GetClosestResult closest5 = GetClosest_Helper(areas, target);

	double absoluteError1 = CalculateAbsoluteError(closest1.value, target);
	double absoluteError2 = CalculateAbsoluteError(closest2.value, target);
	double absoluteError3 = CalculateAbsoluteError(closest3.value, target);
	double absoluteError4 = CalculateAbsoluteError(closest4.value, target);
	double absoluteError5 = CalculateAbsoluteError(closest5.value, target);

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
		minAbsoluteError = absoluteError3;
	}

	if (absoluteError4 < minAbsoluteError)
	{
		retval = closest4;
		minAbsoluteError = absoluteError4;
	}

	if (absoluteError5 < minAbsoluteError)
	{
		retval = closest5;
	}

	return retval;
}

GetClosestResult Pyramid::GetClosest_Helper(const std::vector<std::pair<PyramidDimension, double>*>& dimensions, double target)
{
	GetClosestResult closest;
	double minAbsoluteError = std::numeric_limits<double>::max();

	for (int i = 0; i < dimensions.size(); ++i)
	{
		for (int j = 0; j < dimensions.size(); ++j)
		{
			if (i == j)
			{
				continue;
			}

			double temp = dimensions[i]->second / dimensions[j]->second;

			for (int k = 0; k < allowedFactors.size(); ++k)
			{
				double scaledTemp = temp * allowedFactors[k];
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
	angles1.push_back(&westEastCrossSectionCornerAngle);
	angles1.push_back(&westEastCrossSectionVertexAngle);

	angles2.push_back(&lateralFaceCornerAngle);
	angles2.push_back(&lateralFaceVertexAngle);

	angles3.push_back(&southwestNortheastCrossSectionCornerAngle);
	angles3.push_back(&southwestNortheastCrossSectionVertexAngle);
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

double Pyramid::CalculateWestEastCrossSectionCornerAngle()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return atan(height.second / halfBaseLength);
}

double Pyramid::CalculateWestEastCrossSectionVertexAngle()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return 2.0 * atan(halfBaseLength / height.second);
}

double Pyramid::CalculateLateralFaceCornerAngle()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return atan(slantLength.second / halfBaseLength);
}

double Pyramid::CalculateLateralFaceVertexAngle()
{
	double halfBaseLength = 0.5 * baseLength.second;
	return 2.0 * atan(halfBaseLength / slantLength.second);
}

double Pyramid::CalculateSouthwestNortheastCrossSectionCornerAngle()
{
	double halfBaseDiagonal = 0.5 * CalculateBaseDiagonal();
	return atan(height.second / halfBaseDiagonal);
}

double Pyramid::CalculateSouthwestNortheastCrossSectionVertexAngle()
{
	double halfBaseDiagonal = 0.5 * CalculateBaseDiagonal();
	return 2.0 * atan(halfBaseDiagonal / height.second);
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

double Pyramid::CalculateVolume()
{
	return baseLength.second * baseLength.second * height.second / 3.0;
}