#include <iostream>
#include <string>
#include <stdexcept>

#include "util.h"
#include "Gauss_solve.h"

int main(int argc, const char *argv[])
{
    if (argc < 2) {
        std::cerr << "Использование: " << argv[0] << " <путь_к_файлу_csv>\n";
        return 1;
    }

    try {
        // Загрузка матрицы из файла переданного в аргументах
        GaussMatrix ab = load_csv_to_matrix(argv[1]);
        if (ab.rows() == 0 || ab.cols() == 0) {
            std::cerr << "Ошибка: Файл пуст или имеет неверный формат.\n";
            return 1;
        }

        // Решение системы уравнений
        GaussVector x = Gauss_solve(ab);

        // Переводим вектор ответов в GaussMatrix (столбец Nx1), 
        // чтобы корректно использовать штатную функцию print_matrix_as_csv
        GaussMatrix x_matrix(x.rows(), 1);
        x_matrix.col(0) = x;

        // Печать финального результата в stdout
        print_matrix_as_csv(std::cout, x_matrix);
    } 
    catch (const std::exception &e) {
        std::cerr << "Критическая ошибка: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
