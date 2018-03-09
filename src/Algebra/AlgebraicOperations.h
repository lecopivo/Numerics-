#pragma once

#include "../Utils/MetaUtils.h"
#include "AlgebraicTraits.h"

namespace Numerics {
namespace Algebra {

auto positive = [](auto &&a) { return +a; };
auto negative = [](auto &&a) { return -a; };

auto asigment_addition       = [](auto &&a, auto &&b) { return a += b; };
auto asigment_subtraction    = [](auto &&a, auto &&b) { return a -= b; };
auto asigment_multiplication = [](auto &&a, auto &&b) { return a *= b; };
auto asigment_division       = [](auto &&a, auto &&b) { return a /= b; };

auto addition       = [](auto &&a, auto &&... b) { return (a + ... + b); };
auto subtraction    = [](auto &&a, auto &&b) { return a - b; };
auto multiplication = [](auto &&a, auto &&... b) { return (a * ... * b); };
auto division       = [](auto &&a, auto &&b) { return a / b; };

auto smaller          = [](auto &&a, auto &&b) { return a < b; };
auto bigger           = [](auto &&a, auto &&b) { return a > b; };
auto smaller_or_equal = [](auto &&a, auto &&b) { return a <= b; };
auto bigger_or_equal  = [](auto &&a, auto &&b) { return a >= b; };

auto equal     = [](auto &&a, auto &&... b) { return (a == ... == b); };
auto not_equal = [](auto &&a, auto &&b) { return (a != b); };

}; // namespace Algebra
} // namespace Numerics
