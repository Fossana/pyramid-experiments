#include "Pyramid.h"
#include "Constants.h"

#include <iostream>
#include <cmath>
#include <limits>

constexpr int MIN_BASE_LENGTH = 10;
constexpr int MAX_BASE_LENGTH = 800;
constexpr int MIN_HEIGHT = 10;
constexpr int MAX_HEIGHT = 600;
constexpr double MIN_HEIGHT_TO_BASE_RATIO = 0.2;
constexpr double MAX_HEIGHT_TO_BASE_RATIO = 5.0;
constexpr double EQUATORIAL_CIRCUMFERENCE = 40075.017;
constexpr double POLAR_RADIUS = 6356.752;

double CalculateRelativeError(double actual, double expected)
{
    return abs(actual - expected) / expected;
}

int main()
{
    double pi = const_pi();
    double phi = const_phi();
    double e = const_e();

    
    double equatorialCircumferenceToPolarRadius = EQUATORIAL_CIRCUMFERENCE / POLAR_RADIUS;

    Pyramid khufu(440, 280);
    khufu.Print();
    std::cout << '\n';
    GetClosestResult khufuClosestPi = khufu.GetClosest(pi);
    GetClosestResult khufuClosestPhi = khufu.GetClosest(phi);
    GetClosestResult khufuClosestE = khufu.GetClosest(e);

    khufuClosestPi.relativeError = CalculateRelativeError(khufuClosestPi.value, pi);
    khufuClosestPhi.relativeError = CalculateRelativeError(khufuClosestPhi.value, phi);
    khufuClosestE.relativeError = CalculateRelativeError(khufuClosestE.value, e);

    khufuClosestPi.Print();
    std::cout << '\n';

    khufuClosestPhi.Print();
    std::cout << '\n';

    khufuClosestE.Print();
    std::cout << '\n';

    double khufuRelativeErrorSum = khufuClosestPi.relativeError + khufuClosestPhi.relativeError + khufuClosestE.relativeError;
    
    std::cout << "Great Pyramid relative error sum: " << khufuRelativeErrorSum << '\n';
    std::cout << EQUATORIAL_CIRCUMFERENCE * 1000.0 / (khufu.GetBasePerimeter() * const_pi() / 6.0) << '\n';
    std::cout << POLAR_RADIUS * 1000.0 / (khufu.GetHeight() * const_pi() / 6.0) << '\n';
    std::cout << '\n';

    double moreAccurateThanKhufuCount = 0.0;
    double lessAccurateThanKhufuCount = 0.0;
    int pyramidCount = 0;

    int winningBaseLength = 0;
    int winningHeight = 0;
    double minRelativeErrorSum = std::numeric_limits<double>::max();
    double relativeErrorSumSum = 0.0;

    for (int iBaseLength = MIN_BASE_LENGTH; iBaseLength <= MAX_BASE_LENGTH; ++iBaseLength)
    {
        for (int jHeight = MIN_HEIGHT; jHeight <= MAX_HEIGHT; ++jHeight)
        {
            // Check if the height to base ratio is acceptable
            double heightToBaseRatio = (double) jHeight / (double) iBaseLength;
            if (heightToBaseRatio < MIN_HEIGHT_TO_BASE_RATIO || heightToBaseRatio > MAX_HEIGHT_TO_BASE_RATIO)
            {
                continue;
            }

            // Check if the ratio of the base perimeter to the height is acceptable
            Pyramid pyramid(iBaseLength, jHeight);
            //double basePerimeterToHeight = pyramid.GetBasePerimeter() / pyramid.GetHeight();
            //if (CalculateRelativeError(basePerimeterToHeight, equatorialCircumferenceToPolarRadius) > 0.01)
            //{
            //    continue;
            //}

            ++pyramidCount;

            GetClosestResult closestPi = pyramid.GetClosest(pi);
            GetClosestResult closestPhi = pyramid.GetClosest(phi);
            GetClosestResult closestE = pyramid.GetClosest(e);

            closestPi.relativeError = CalculateRelativeError(closestPi.value, pi);
            closestPhi.relativeError = CalculateRelativeError(closestPhi.value, phi);
            closestE.relativeError = CalculateRelativeError(closestE.value, e);

            double relativeErrorSum = closestPi.relativeError + closestPhi.relativeError + closestE.relativeError;
            relativeErrorSumSum += relativeErrorSum;
            if (relativeErrorSum < minRelativeErrorSum)
            {
                winningBaseLength = iBaseLength;
                winningHeight = jHeight;
                minRelativeErrorSum = relativeErrorSum;
            }

            if (relativeErrorSum < khufuRelativeErrorSum)
            {
                moreAccurateThanKhufuCount += 1.0;
                //std::cout << "height: " << jHeight << '\n';
                //std::cout << "base length: " << iBaseLength << '\n';
                //std::cout << EQUATORIAL_CIRCUMFERENCE * 1000.0 / (pyramid.GetBasePerimeter() * const_pi() / 6.0) << '\n';
                //std::cout << '\n';
            }
            else if (relativeErrorSum > khufuRelativeErrorSum)
            {
                lessAccurateThanKhufuCount += 1.0;
            }
            else // same height to base length ratio as Khufu
            {
                moreAccurateThanKhufuCount += 0.5;
                lessAccurateThanKhufuCount += 0.5;
            }
        }
    }

    std::cout << "winning base length: " << winningBaseLength << '\n';
    std::cout << "winning height: " << winningHeight << '\n';
    std::cout << "relative error sum: " << minRelativeErrorSum << '\n';
    std::cout << '\n';

    std::cout << "average relative error sum: " << relativeErrorSumSum / (double) pyramidCount << '\n';;
    std::cout << "number of pyramids with a better combined relative error than the Great Pyramid: " << moreAccurateThanKhufuCount << '\n';
    std::cout << "number of pyramids with a worse combined relative error than the Great Pyramid: " << lessAccurateThanKhufuCount << '\n';
}