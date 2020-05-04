#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <memory>
#include <vector>
#include <chrono>
#include <thread>
#include <random>


// Author: Michal Kotecki
// Those functions are some basic C++ boilerplate to replace some repetitive code and be more intuitive in use.

// To use this functions you need to add
// using namespace Codecki;

/*
List of content:

 LOGs
 - void print(t somethingToPrint)
 - void printline(t somethingToPrint)
 = class measureTime
    - void start()
    - void stop()
    - double getDuration_microseconds()
    - double getDuration_seconds()
- std::string toString(t somethingToConvertToString)

FILEs
 - void collectionToFile(std::string fileName, const t &collection)

 MATHs
 - double randomNumber()
 - int factorial(int num)
 - int binomialCoefficient(int n, int k)



 */
namespace Codecki {

    //  LOGs

    template<class t>
    void print(t const & somethingToPrint) {
        std::cout << somethingToPrint;
    }

    // This printline() with no parameters is necessary in order for the variadic printline() to work.
    void printline() {
        std::cout << '\n';
    }

    // This is Python-like print. This can take any number of arguments of different types. Example: ("text", 1, 5.7, 'e')
    template<class t, class ... tt>
    void printline(t const & somethingToPrint, tt ... moreToPrint) {
        std::cout << somethingToPrint << ' ';
        printline(moreToPrint...);
    }

    template<class t>
    void printVector(std::vector<t> const & vectorToPrint) {

        for (int i = 0; i < vectorToPrint.size(); ++i) {
            std::cout << "index: " << i << "\tvalue: " << vectorToPrint.at(i) << '\n';
        }

    }

    template<class t>
    void printVectorOfVectors(std::vector<std::vector<t>> const & vectorToPrint)
    {
        for (int j = 0; j < vectorToPrint.size(); ++j) {

            std::cout << "Row " << j << '\n';
            for (int i = 0; i < vectorToPrint.at(j).size(); ++i) {
                std::cout << vectorToPrint.at(j).at(i) << ", ";
            }
            std::cout << '\n';
        }
    }

    void Delay(int miliseconds)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(miliseconds));
    }

    void Pause()
    {
        getchar();
    }

    void clearConsole()
    {
        system("cls||clear");
    }

    class measureTime{

    private:
        std::chrono::steady_clock::time_point begin;
        std::chrono::steady_clock::time_point end;

    public:

        void start()
        {
            begin = std::chrono::steady_clock::now();
        }

        void stop()
        {
            end = std::chrono::steady_clock::now();
        }

        double getDuration_microseconds()
        {
            return std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
        }

        double getDuration_seconds()
        {
            return std::chrono::duration_cast<std::chrono::seconds>(end - begin).count();
        }
    };


    template<class t>
    std::string toString(t const & somethingToConvertToString) {
        std::stringstream ssObject;
        ssObject << somethingToConvertToString;
        return ssObject.str();
    }

//  FILEs

    template<class t>
    void collectionToFile(std::string const & fileName, t const & collection) {
        std::fstream file(fileName, std::ios::out);

        for (auto const &x : collection) {
            file << x << '\n';
        }

        file.close();
    }

//  MATHs

    double randomNumber() {
        double rN = (rand() / (double) RAND_MAX);
        return rN;
    }

    double randomNumber(int min, int max) {
        //double rN = (rand() / (double) RAND_MAX);
        // values of min and max can be generated
        double rN = min + ( std::rand() % ( max - min + 1 ) );
        return rN;
    }

    int factorial(int num) {
        if (num > 1)
            return num * factorial(num - 1);
        else
            return 1;
    }

// The polish translation for "Binomial Coeficient" is "Symbol Newtona".
    int binomialCoefficient(int n, int k) {
        if (k == 0 || k == n)
            return 1;
        return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
    }
    
    

//  Distributions

    double getBernulliDistributionNumber(double probabilityOfGetting_1 = 0.5)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 merseneTwisterGenerator{seed};
        std::bernoulli_distribution bernoulliDistribution(probabilityOfGetting_1);
        return bernoulliDistribution(merseneTwisterGenerator);
    }


//  This function takes a vector of probabilities. Let's assume it takes three numbers: {10, 40, 50} (they all sum up to 100 to make it easier)
//  Probability of returning 0 is 10%
//  Probability of returning 1 is 40%
//  Probability of returning 2 is 50%
    double getDiscreteDistributionNumber(std::vector<int> const & probabilitiesOfGettingThisIndexes)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 merseneTwisterGenerator{seed};
        std::discrete_distribution<int> discreteDistribution (probabilitiesOfGettingThisIndexes.begin(), probabilitiesOfGettingThisIndexes.end());
        Delay(1);       //  The use of this delay is necessary. Otherwise some results from a loop call, may appear to be the same numbers.
        return discreteDistribution(merseneTwisterGenerator);
    }


    double getNormalDistributionNumber(int mean, int standardDeviation)
    {
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 merseneTwisterGenerator{seed};
        std::normal_distribution<double> normalDistribution(mean,standardDeviation);
        Delay(1);       //  The use of this delay is necessary. Otherwise, some results from a loop call, may appear to be the same numbers.
        return normalDistribution(merseneTwisterGenerator);
    }



}