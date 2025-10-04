
#pragma once
#ifndef RANDOM_GENERATOR_HPP_
#define RANDOM_GENERATOR_HPP_

// This RandomGenerator class is based on a response by ChatGPT 4.0

#include <random>
#include <limits>

namespace CScore {


    class RandomGenerator {
    public:
        static RandomGenerator& getInstance () {
            static RandomGenerator instance_;
            return instance_;
        }
        ~RandomGenerator()=default;

        RandomGenerator (const RandomGenerator&) = delete;  // Disallow copy construction.
        RandomGenerator& operator= (const RandomGenerator&) = delete;   // Disallow assignment.

        // Either of these constructors would work.
        // I decided to go without an explicit constructor.
        // This is possible because engine_ is initialized with a call to the random_device constructor.

/*
        RandomGenerator() : engine_(std::random_device{}()) {}
*/

/*
        RandomGenerator () {
            std::random_device rd;  // seed for the device.
            engine_ = std::mt19937(rd());
        }
*/
        // Public methods for random number generation
        uint8_t generateUint8(const uint8_t min = 0, const uint8_t max = std::numeric_limits<uint8_t>::max()) {
            std::uniform_int_distribution<uint16_t> dist(min, max);
            return static_cast<uint8_t>(dist(engine_));
        }

        template<typename T>
        T generateInt(T min, T max) {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(engine_);
        }

        template<typename T>
        T generateFloat(T min, T max) {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(engine_);
        }
//--------------------------------------------------------------------------
// Example usage of the methods:

/*
        uint8_t randomUint8 = RandomGenerator::getInstance().generateRandomUint8();
        std::cout << "Random uint8_t: " << std::to_string(randomUint8) << std::endl;

        int randomInt = RandomGenerator::getInstance().generateRandomInt(-50, 50);
        std::cout << "Random int: " << randomInt << std::endl;

        double randomDouble = RandomGenerator::getInstance().generateRandomFloat(1.0, 5.0);
        std::cout << "Random double: " << randomDouble << std::endl;
*/

//--------------------------------------------------------------------------

    private:

        RandomGenerator()=default;

        // Random number engine seeded with std::random_device
        std::random_device rd_;
        std::mt19937 engine_ = std::mt19937(rd_());
    };

}


#endif  // RANDOM_GENERATOR_HPP_
