#include "Gauss_solve.h"
#include <cmath>
#include <stdexcept>

GaussVector Gauss_solve(GaussMatrix &ab)
{
    int n = ab.rows();
    int cols = ab.cols();

    if (cols < n + 1) {
        throw std::invalid_argument("Матрица должна быть расширенной [A|b] (столбцов >= строк + 1).");
    }

    // Прямой ход: исключение переменных с частичным выбором ведущего элемента
    for (int i = 0; i < n; ++i) {
        int max_row = i;
        double max_val = std::abs(ab(i, i));

        // Поиск максимального по модулю элемента в i-м столбце ниже диагонали
        for (int k = i + 1; k < n; ++k) {
            if (std::abs(ab(k, i)) > max_val) {
                max_val = std::abs(ab(k, i));
                max_row = k;
            }
        }

        // Проверка матрицы на вырожденность
        if (max_val < 1e-9) {
            throw std::runtime_error("Система не имеет единственного решения (матрица вырождена).");
        }

        // Перестановка строк, если это необходимо
        if (max_row != i) {
            ab.row(i).swap(ab.row(max_row));
        }

        // Обнуление элементов под ведущим элементом
        for (int k = i + 1; k < n; ++k) {
            double factor = ab(k, i) / ab(i, i);
            ab.row(k) -= factor * ab.row(i);
        }
    }

    // Обратный ход: вычисление вектора неизвестных
    GaussVector x(n);
    for (int i = n - 1; i >= 0; --i) {
        x(i) = ab(i, cols - 1);
        for (int j = i + 1; j < n; ++j) {
            x(i) -= ab(i, j) * x(j);
        }
        x(i) /= ab(i, i);
    }

    return x;
}
