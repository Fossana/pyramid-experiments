#include "MathUtilities.h"

namespace
{

int gcd(int a, int b)
{
    if (b == 0)
    {
        return a;
    }
    return gcd(b, a % b);
}

}

namespace MathUtilities
{

std::pair<int, int> ReduceFraction(int x, int y)
{
    int d = gcd(x, y);

    x = x / d;
    y = y / d;

    return std::make_pair(x, y);
}

}