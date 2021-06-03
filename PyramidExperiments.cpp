#include "Pyramid.h"
#include "Constants.h"

#include <iostream>
#include <cmath>
#include <limits>

constexpr int MIN_BASE_LENGTH = 1;
constexpr int MAX_BASE_LENGTH = 1000;
constexpr int MIN_HEIGHT = 1;
constexpr int MAX_HEIGHT = 1000;
constexpr double MIN_HEIGHT_TO_BASE_RATIO = 0.2;
constexpr double MAX_HEIGHT_TO_BASE_RATIO = 5.0;

double CalculateRelativeError(double actual, double expected)
{
    return abs(actual - expected) / expected;
}

int main()
{
    double pi = const_pi();
    double phi = const_phi();
    double e = const_e();

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
    std::cout << '\n';

    int moreAccurateThanKhufuCount = 0;
    int lessAccurateThanKhufuCount = 0;
    int pyramidCount = 0;

    int winningBaseLength = 0;
    int winningHeight = 0;
    double minRelativeErrorSum = std::numeric_limits<double>::max();
    double relativeErrorSumSum = 0.0;

    for (int iBaseLength = MIN_BASE_LENGTH; iBaseLength <= MAX_BASE_LENGTH; ++iBaseLength)
    {
        for (int jHeight = MIN_HEIGHT; jHeight <= MAX_HEIGHT; ++jHeight)
        {
            double heightToBaseRatio = (double) jHeight / (double) iBaseLength;
            if (heightToBaseRatio < MIN_HEIGHT_TO_BASE_RATIO || heightToBaseRatio > MAX_HEIGHT_TO_BASE_RATIO)
            {
                continue;
            }

            ++pyramidCount;

            Pyramid pyramid(iBaseLength, jHeight);
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
            }
            else if (relativeErrorSum > khufuRelativeErrorSum)
            {
                ++lessAccurateThanKhufuCount;
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