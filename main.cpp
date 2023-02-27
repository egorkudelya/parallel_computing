#include <Eigen/Core>
#include <iostream>
#include <memory>

#include "pool/thread_pool.h"
#include "utils/timer.h"

auto initMatrix(size_t x, size_t y)
{
    assert(x == y);
    Eigen::MatrixXi matrix(x, y);

    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            matrix(i, j) = 1 + rand() % 3;
        }
    }
    return matrix;
}


int main()
{
    const size_t matrixParams = 100;
    const size_t threadCount = 64;

    Timer timer;

    timer.start("pool initialization");
    auto pool = std::make_unique<ThreadPool>(threadCount);
    timer.commit("pool initialization");

    timer.start("matrix initialization");
    Eigen::MatrixXi matrix = initMatrix(matrixParams, matrixParams);
    timer.commit("matrix initialization");

    timer.start("start");
    for (int i = 0; i < matrixParams; i++)
    {
        auto col = matrix.col(i);
        auto task = [&]()
        {
            size_t prod{1};
            for (size_t i = 0; i < col.size(); i++)
            {
                prod *= col(i);
            }
            return prod;
        };
        auto future = pool->pushTask(std::move(task));
        matrix(matrixParams - i - 1, i) = std::any_cast<size_t>(future.get());
    }
    timer.commit("start");

//    std::cout << matrix << std::endl;
    timer.print();
    pool->shutDown();
    return 0;
}
