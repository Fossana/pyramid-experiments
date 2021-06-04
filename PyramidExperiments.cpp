#include "Pyramid.h"
#include "Constants.h"
#include "MathUtilities.h"

#include <iostream>
#include <cmath>
#include <limits>
#include <set>

constexpr int MIN_BASE_LENGTH = 1;
constexpr int MAX_BASE_LENGTH = 1000;
constexpr int MIN_HEIGHT = 1;
constexpr int MAX_HEIGHT = 1000;
constexpr double MIN_VOLUME = 1.4E7;
constexpr double MAX_VOLUME = 2.2E7;
constexpr double MIN_HEIGHT_TO_BASE_RATIO = 1.0 / 3.0;
constexpr double MAX_HEIGHT_TO_BASE_RATIO = 3.0;
constexpr double EQUATORIAL_CIRCUMFERENCE = 40075.017;
constexpr double POLAR_RADIUS = 6356.752;
constexpr int KHUFU_HEIGHT = 280;
constexpr int KHUFU_BASE_LENGTH = 440;
// constexpr double FINE_STRUCTURE_CONSTANT = 1.37035999206;

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

    std::set<std::pair<int, int>> processedHeightToBaseLengthRatios;

    Pyramid khufu(KHUFU_BASE_LENGTH, KHUFU_HEIGHT);
    processedHeightToBaseLengthRatios.insert(MathUtilities::ReduceFraction(KHUFU_HEIGHT, KHUFU_BASE_LENGTH));
    khufu.Print();
    std::cout << '\n';
    GetClosestResult khufuClosestPi = khufu.GetClosest(pi);
    GetClosestResult khufuClosestPhi = khufu.GetClosest(phi);
    GetClosestResult khufuClosestE = khufu.GetClosest(e);
    // GetClosestResult khufuClosestFSC = khufu.GetClosest(FINE_STRUCTURE_CONSTANT);

    khufuClosestPi.relativeError = CalculateRelativeError(khufuClosestPi.value, pi);
    khufuClosestPhi.relativeError = CalculateRelativeError(khufuClosestPhi.value, phi);
    khufuClosestE.relativeError = CalculateRelativeError(khufuClosestE.value, e);
    // khufuClosestFSC.relativeError = CalculateRelativeError(khufuClosestFSC.value, FINE_STRUCTURE_CONSTANT);

    khufuClosestPi.Print();
    std::cout << '\n';

    khufuClosestPhi.Print();
    std::cout << '\n';

    khufuClosestE.Print();
    std::cout << '\n';

   /* khufuClosestFSC.Print();
    std::cout << '\n';*/

    double khufuRelativeErrorSum = khufuClosestPi.relativeError + khufuClosestPhi.relativeError + khufuClosestE.relativeError;
    
    std::cout << "Great Pyramid relative error sum: " << khufuRelativeErrorSum << '\n';
    std::cout << EQUATORIAL_CIRCUMFERENCE * 1000.0 / (khufu.GetBasePerimeter() * const_pi() / 6.0) << '\n';
    std::cout << POLAR_RADIUS * 1000.0 / (khufu.GetHeight() * const_pi() / 6.0) << '\n';
    std::cout << '\n';

    int moreAccurateThanKhufuCount = 0;
    int lessAccurateThanKhufuCount = 0;
    int pyramidCount = 1;

    int winningBaseLength = 0;
    int winningHeight = 0;
    double minRelativeErrorSum = std::numeric_limits<double>::max();
    double relativeErrorSumSum = 0.0;

    for (int iBaseLength = MIN_BASE_LENGTH; iBaseLength <= MAX_BASE_LENGTH; ++iBaseLength)
    {
        for (int jHeight = MIN_HEIGHT; jHeight <= MAX_HEIGHT; ++jHeight)
        {
            // Check if the ratio of the height to base length has already been processed:
            if (processedHeightToBaseLengthRatios.count(MathUtilities::ReduceFraction(jHeight, iBaseLength)) > 0)
            {
                continue;
            }

            // Check if the height to base ratio is acceptable
            double heightToBaseRatio = (double) jHeight / (double) iBaseLength;
            if (heightToBaseRatio < MIN_HEIGHT_TO_BASE_RATIO || heightToBaseRatio > MAX_HEIGHT_TO_BASE_RATIO)
            {
                continue;
            }

            // Check if the volume is acceptable
            double volume = (double) iBaseLength * (double) iBaseLength * (double) jHeight / 3.0;;
            if (volume < MIN_VOLUME || volume > MAX_VOLUME)
            {
                continue;
            }

            // Check if the ratio of the base perimeter to the height is acceptable
            Pyramid pyramid(iBaseLength, jHeight);
            //double basePerimeterToHeight = pyramid.GetBasePerimeter() / pyramid.GetHeight();
            //if (CalculateRelativeError(basePerimeterToHeight, equatorialCircumferenceToPolarRadius) > 0.01)
            //{
            //    lessAccurateThanKhufuCount += 1.0;
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
                ++moreAccurateThanKhufuCount;
                //std::cout << "height: " << jHeight << '\n';
                //std::cout << "base length: " << iBaseLength << '\n';
                //std::cout << EQUATORIAL_CIRCUMFERENCE * 1000.0 / (pyramid.GetBasePerimeter() * const_pi() / 6.0) << '\n';
                //std::cout << CalculateRelativeError(pyramid.GetBasePerimeter() / pyramid.GetHeight(), equatorialCircumferenceToPolarRadius) << '\n';
                //std::cout << '\n';
            }
            else if (relativeErrorSum > khufuRelativeErrorSum)
            {
                ++lessAccurateThanKhufuCount;
            }
            else // same height to base length ratio as Khufu
            {
                std::cout << "shouldn't happen\n";
            }

            processedHeightToBaseLengthRatios.insert(MathUtilities::ReduceFraction(jHeight, iBaseLength));
        }
    }

    std::cout << "winning base length: " << winningBaseLength << '\n';
    std::cout << "winning height: " << winningHeight << '\n';
    std::cout << "relative error sum: " << minRelativeErrorSum << '\n';
    std::cout << '\n';

    std::cout << "average relative error sum: " << relativeErrorSumSum / (double) pyramidCount << '\n';;
    std::cout << "number of pyramids with a better combined relative error than the Great Pyramid: " << moreAccurateThanKhufuCount << '\n';
    std::cout << "number of pyramids with a worse combined relative error than the Great Pyramid: " << lessAccurateThanKhufuCount << '\n';

    std::cout << "the Great Pyramid is more accurate than " << std::setprecision(15) << (double)lessAccurateThanKhufuCount / (double) (lessAccurateThanKhufuCount + moreAccurateThanKhufuCount) << '\n';
}