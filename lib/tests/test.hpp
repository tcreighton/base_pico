
#pragma once

#ifndef TESTS_HPP_
#define TESTS_HPP_

#include <string>
#include <vector>
#include <functional>
#include "assertion.hpp"

namespace CStest {

    class Test {

    public:
        virtual ~Test() = default;

        Test() = default;

        using TestFunction_t = std::function<void(Assertion::verbosity)>;

        // setUp and tearDown can be overridden. These will be called by runTests.
        // A typical setup would set the level_ to a value desired for the tests.
        virtual void setUp () {

            displayMessage(getVerbosityLevel(), "In " + getClassName() + "::setUp()\n");
        }

        virtual void tearDown () {
            displayMessage(getVerbosityLevel(), "In " + getClassName() + "::tearDown()\n");
        }

        void addTestFunction (const TestFunction_t &function) {
            functionList_.push_back(function);
//            std::cout << "addTestFunction; count: " << functionList_.size() << std::endl;
        }

        std::string getClassName () {
            return className_;
        }

        [[nodiscard]] Assertion::verbosity getVerbosityLevel () const {
            return level_;
        }

        void executeAll() const;

        void doTests () {
            setUp();
            executeAll();
            tearDown();
        }

        void displayMessage (const std::string &message) const;
        static void displayMessage (Assertion::verbosity level, const std::string &message);

    protected:

        std::vector<TestFunction_t> functionList_{};
        Assertion::verbosity level_ = Assertion::ON_FAILURE;
        void setClassName (const std::string &className) {
            className_ = className;
        }
        void setVerbosityLevel(const Assertion::verbosity level) {
            level_ = level;
        }

    private:
        std::string className_;

    };

    void runTests (Assertion::verbosity level = Assertion::ON_FAILURE);


}

#endif  // TESTS_HPP_
