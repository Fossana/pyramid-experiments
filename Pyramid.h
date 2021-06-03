#ifndef PYRAMID_H_
#define PYRAMID_H_

#include <iostream>
#include <vector>
#include <utility>

enum class PyramidDimension
{
    BASE_LENGTH = 0,
    HEIGHT,
    BASE_PERIMETER,
    BASE_DIAGONAL,
    SLANT_LENGTH,
    LATERAL_EDGE_LENGTH,
    SLANT_ANGLE,
    NINETY_DEGREES_MINUS_SLANT_ANGLE,
    VERTEX_ANGLE,
    CORNER_ANGLE,
    BASE_AREA,
    LATERAL_FACE_AREA,
    SURFACE_AREA_NOT_INCLUDING_BASE,
    SURFACE_AREA_INCLUDING_BASE
};

static const char* PyramidDimensionStrings[] =
{
    "BASE_LENGTH",
    "HEIGHT",
    "BASE_PERIMETER",
    "BASE_DIAGONAL",
    "SLANT_LENGTH",
    "LATERAL_EDGE_LENGTH",
    "SLANT_ANGLE",
    "NINETY_DEGREES_MINUS_SLANT_ANGLE",
    "VERTEX_ANGLE",
    "CORNER_ANGLE",
    "BASE_AREA",
    "LATERAL_FACE_AREA",
    "SURFACE_AREA_NOT_INCLUDING_BASE",
    "SURFACE_AREA_INCLUDING_BASE"

};

struct GetClosestResult
{
public:
    std::pair<PyramidDimension, double> dimension1;
    std::pair<PyramidDimension, double> dimension2;
    double value;
    double relativeError;

    void Print()
    {
        std::cout << "dimension 1 value: " << dimension1.second << " dimension 1 type: " << PyramidDimensionStrings[(int) dimension1.first] << '\n';
        std::cout << "dimension 2 value: " << dimension2.second << " dimension 2 type: " << PyramidDimensionStrings[(int) dimension2.first] << '\n';
        std::cout << "value: " << value << '\n';
        std::cout << "relative error: " << relativeError << '\n';
    }
};

class Pyramid
{
public:
    Pyramid(int baseLength, int height);
    GetClosestResult GetClosest(double target);
    void Print();

private:
    std::pair<PyramidDimension, double> baseLength;
    std::pair<PyramidDimension, double> height;
    std::pair<PyramidDimension, double> basePerimeter;
    std::pair<PyramidDimension, double> baseDiagonal;
    std::pair<PyramidDimension, double> slantLength;
    std::pair<PyramidDimension, double> lateralEdgeLength;

    std::pair<PyramidDimension, double> slantAngle;
    std::pair<PyramidDimension, double> ninetyDegreesMinusSlantAngle;
    std::pair<PyramidDimension, double> vertexAngle;
    std::pair<PyramidDimension, double> cornerAngle;

    std::pair<PyramidDimension, double> baseArea;
    std::pair<PyramidDimension, double> lateralFaceArea;
    std::pair<PyramidDimension, double> surfaceAreaNotIncludingBase;
    std::pair<PyramidDimension, double> surfaceAreaIncludingBase;

    std::vector<std::pair<PyramidDimension, double>*> lengths;
    std::vector<std::pair<PyramidDimension, double>*> angles;
    std::vector<std::pair<PyramidDimension, double>*> areas;

    GetClosestResult GetClosest_Helper(const std::vector<std::pair<PyramidDimension, double>*>& dimensions, double target);

    double CalculateAbsoluteError(double actual, double expected);

    void InitializeLengths();
    void InitializeAngles();
    void InitializeAreas();

    double CalculateBasePerimeter();
    double CalculateBaseDiagonal();
    double CalculateSlantLength();
    double CalculateLateralEdgeLength();
    double CalculateSlantAngle();
    double CalculateNinetyDegreesMinusSlantAngle();
    double CalculateVertexAngle();
    double CalculateCornerAngle();
    double CalculateBaseArea();
    double CalculateLateralFaceArea();
    double CalculateSurfaceAreaNotIncludingBase();
    double CalculateSurfaceAreaIncludingBase();
};

#endif