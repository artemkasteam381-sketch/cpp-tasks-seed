#include <gtest/gtest.h>
#include "Gauss_solve.h"

TEST(GaussSolve, SimpleSystem2x2)
{
    // Система:
    // 2x + y = 5
    // x - y = 1
    // Ожидаемый ответ: x = 2, y = 1
    GaussMatrix ab(2, 3);
    ab << 2, 1, 5,
          1, -1, 1;

    GaussVector x = Gauss_solve(ab);
    
    ASSERT_EQ(x.size(), 2);
    EXPECT_NEAR(x(0), 2.0, 1e-6);
    EXPECT_NEAR(x(1), 1.0, 1e-6);
}

TEST(GaussSolve, System3x3)
{
    // Система:
    // 3x + 2y - z = 1
    // 2x - 2y + 4z = -2
    // -x + 0.5y - z = 0
    // Ожидаемый ответ: x = 1, y = -2, z = -2
    GaussMatrix ab(3, 4);
    ab << 3, 2, -1, 1,
          2, -2, 4, -2,
          -1, 0.5, -1, 0;

    GaussVector x = Gauss_solve(ab);
    
    ASSERT_EQ(x.size(), 3);
    EXPECT_NEAR(x(0), 1.0, 1e-6);
    EXPECT_NEAR(x(1), -2.0, 1e-6);
    EXPECT_NEAR(x(2), -2.0, 1e-6);
}

TEST(GaussSolve, ThrowsOnSingularMatrix)
{
    // Вырожденная матрица (строки линейно зависимы)
    GaussMatrix ab(2, 3);
    ab << 1, 2, 3,
          2, 4, 8;

    EXPECT_THROW(Gauss_solve(ab), std::runtime_error);
}
