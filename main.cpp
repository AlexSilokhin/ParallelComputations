// MultiThreaded_01.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <array>
#include <limits>
#include <thread>

int* generate_values(size_t num);
void find_maximum(const size_t num, int* values, int& result);

int main()
{
    const size_t NumberValues = 100000000;
    auto data = generate_values(NumberValues);

    const size_t NumberThreads = std::max<size_t>(std::thread::hardware_concurrency(), 4UL);

    std::vector<std::thread> routines;
    routines.reserve(NumberThreads);

    std::vector<int> results = std::vector<int>(NumberThreads, 0);

    const size_t Portion = NumberValues / NumberThreads;
    
    for (size_t i = 0; i < NumberThreads; ++i)
    {
        routines.push_back(std::thread(find_maximum, Portion, data + i * Portion, std::ref(results[i])));
        routines[i].join();
    }

    delete[] data;

    std::cout << "Intermediate results: \n";
    for (size_t i = 0; i < NumberThreads; ++i)
        std::cout << results[i] << std::endl;

    int finalResult = 0;
    for (size_t i = 0; i < NumberThreads; ++i)
    {
        if (finalResult < results[i])
            finalResult = results[i];
    }

    std::cout << "Final result: " << finalResult << std::endl;

    return EXIT_SUCCESS;
}

int* generate_values(size_t num)
{
    auto data = new int[num];

    srand(static_cast<unsigned int>(time(nullptr)));

    for (size_t i = 0; i < num; ++i)
    {
        data[i] = rand();
    }

    return data;
}

void find_maximum(const size_t num, int* values, int& result)
{
    result = std::numeric_limits<int>::min();
    for (size_t i = 0; i < num; ++i)
    {
        const auto curVal = values[i];
        if (curVal > result)
            result = curVal;
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
