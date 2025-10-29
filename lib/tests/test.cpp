
// #include the hpp file for tests.
#include "test.hpp"


namespace CStest {

    void runTests(const Assertion::verbosity level) {


//        TestEndian testEndian(level);
//        testEndian.doTests();
//        TestBitFields testBitFields(level);
//        testBitFields.doTests();
//        TestLogarithms testLogarithms (level);
//        testLogarithms.doTests();
//        TestPowerSupply testPowerSupply(level);
//        testPowerSupply.doTests();
//        TestAdcChatgpt testAdcChatgpt(level);
//        testAdcChatgpt.doTests();
//        TestAdc testAdc(level);
//        testAdc.doTests();
//        TestADS1115 testAds1115(level);
//        while (true) {
//        testAds1115.doTests();
//       }
//        TestPID testPID(level);
//        testPID.doTests();
//        TestMemory testMemory(level);
//        testMemory.doTests();
//        TestCalibrations testCalibrations(level);
//        testCalibrations.doTests();
//        TestPackedDateTime testPackedDateTime(level);
//        testPackedDateTime.doTests();
//        TestConvertCurrent testConvertCurrent(level);
//        testConvertCurrent.doTests();
//        TestConvertVoltage testConvertVoltage(level);
//        testConvertVoltage.doTests();

    }


    void Test::executeAll() const {
        for (const auto& func : functionList_) {
            func(getVerbosityLevel());
        }
    }

    void Test::displayMessage(const Assertion::verbosity level, const std::string &message) {

        if (Assertion::VERBOSE == level) {
            std::cout << message << std::endl;
        }
    }

    void Test::displayMessage(const std::string &message) const {
        displayMessage(getVerbosityLevel(), message);
    }
}
