#pragma once

namespace Util {
namespace Math {

    static int nextPowerOfTwo(int number) {
        --number;

        number |= number >> 1;
        number |= number >> 2;
        number |= number >> 4;
        number |= number >> 16;

        ++number;

        return number;
    }

}
}
