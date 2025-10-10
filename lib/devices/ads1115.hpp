#pragma once

#ifndef ADS1115_HPP_
#define ADS1115_HPP_

#include "component.hpp"
#include "csi2c.hpp"
#include "ads1115-declarations.hpp"

namespace CSdevices {

//    class CsI2C;    // forward reference
    CsI2C& getController(ControllerId controllerId); // A little forward reference.

    class Ads1115 final : public Component {

    public:
        Ads1115( const std::string&         label,
                 const ControllerId         controllerId,
                 const uint8_t              i2cAddress,
                 const Ads1115GainValues    gain = Ads1115GainValues::GAIN_2p048V,
                 const Ads1115DataRates     dataRate = Ads1115DataRates::DR_860SPS) :
                                            controllerId_(controllerId),
                                            i2cAddress_(i2cAddress),
                                            gain_(gain),
                                            dataRate_(dataRate),
                                            conversionTimeout_(::get_absolute_time()) {
            setClassName("Ads1115");
            setLabel(label);
            prependToHierarchy("Ads1115");
        }

        Ads1115 () = delete;
        Ads1115 (const Ads1115& other) = delete;
        Ads1115& operator=(const Ads1115& other) = delete;
        ~Ads1115 () override = default;


        bool startConversion (Ads1115Channel_t channel);
        int16_t completeConversion (Ads1115Channel_t channel);  // Returns the counts
        static Ads1115ConfigRegister_t buildConfigRegister (Ads1115Channel_t channel,
                                                            Ads1115OperationalStatus opStatus);
        static Ads1115ConfigRegister_t buildConfigRegister ();
        static std::string registerAddressToName (Ads1115RegisterAddresses address);

        // PGA: 2; FSR: 2.048V
        // 6.25e-5 == (2.048/32767)
        [[nodiscard]] float getVoltsPerCount () const {
            return ads1115GetFSRRatio(getGain());
        }

        [[nodiscard]] CsI2C& getController () const {
            return CSdevices::getController(getControllerId());
        }
        [[nodiscard]] ControllerId getControllerId () const {
            return controllerId_;
        }

        [[nodiscard]] uint8_t getDeviceAddress () const {return i2cAddress_;}
        [[nodiscard]] float getFSRRatio () const {return ads1115GetFSRRatio(getGain());}

    protected:

        bool startConversion (Ads1115ConfigRegister_t configRegister);

        [[nodiscard]] Ads1115OperationalStatus isConversionPending () const {
            return conversionPending_;
        }
        void setConversionPendingState (const Ads1115OperationalStatus state) {
            conversionPending_ = state;
        }
        /**
         * This checks the config register to see if the conversion is ready.
         * To ensure that there are no mistakes, this will first write the address register to prepare for reads
         * of the config register.
         *
         * @return true if the op bit set on config. Else returns false.
         */
        Ads1115OperationalStatus checkConversionReady (Ads1115Channel_t channel);


        /**
         * @brief Returns the voltage value based on current gain setting GAIN_2p048V: 2.048
         * This is used to compute the voltage from count.
         * @return voltage gain: the range value given the gain setting.
         */
        [[nodiscard]] Ads1115Gain_t getGain () const {return gain_;}
        // Just the ordinal number for the enum.
        [[nodiscard]] uint8_t getGainValue () const {return ads1115GainValuesToNumber(getGain());}
        // Just the ordinal number for the enum.
        [[nodiscard]] Ads1115DataRates getDataRate () const {return dataRate_;}
        // Just the ordinal number for the enum.
        [[nodiscard]] uint8_t getDataRateValue () const {return static_cast<uint8_t> (getDataRate());}
        void setGain (const Ads1115Gain_t gain) { gain_ = gain;}
        void setDataRate (const Ads1115DataRates dataRate) {dataRate_ = dataRate;}

        [[maybe_unused]] [[nodiscard]] uint32_t getExpectedConversionTime_us () const;


        /**
         * @brief This can be used in busy_wait_until, for example.
         * @return
         */
        [[nodiscard]] absolute_time_t getConversionTimeout () const {
            return conversionTimeout_;
        }
        /**
         * @brief Calculates the timeout destination and passes it on to the other overload of setConversionTimeout
         */
        void setConversionTimeout () {
            conversionTimeout_ = make_timeout_time_us(getExpectedConversionTime_us());
        }
        void waitOnConversionTimeout() const {
            sleep_until(getConversionTimeout());
        }

        /**
         * @brief Writes the register address. Then reads the register data.
         * Don't call this if you are unsure that the current address pointer is the one you want to read.
         * @param registerAddresses
         * @return
         */
        uint16_t setAndReadRegister (Ads1115RegisterAddresses registerAddresses);

        /**
         * @brief Reads the config register.
         * This will check if the current register is the config register. If not, it writes the address register.
         * After reading the config register, we check that the AIN (MUX) value matches the current AIN.
         * If it doesn't match, set the status to error!
         * @return The read config register. Sets status to NO_ERROR if all is well.
         */
        Ads1115ConfigRegister_t readConfigRegister () {
            Ads1115ConfigRegister_t reg;

            reg.shortWord =  readRegister(Ads1115RegisterAddresses::ADS1115_CONFIG_REG_ADDR);

            return reg;
        }

        /**
         * @brief Reads the register indicated. Note that this does not check the current register.
         * This is called from setAndReadRegister. It assumes the current register check has been done.
         * This not check the results. That's up to who calls it. This simply provides an interface to the i2c function.
         * @param reg
         * @return
         */
        uint16_t readRegister (const Ads1115RegisterAddresses reg) {
            return setAndReadRegister(reg);
        }

        /**
         * @brief Writes the configuration register to the device.
         * @param configRegister
         * @return
         */
        bool writeConfigRegister (Ads1115ConfigRegister_t configRegister);


        /**
         * @brief Writes the address pointer register.
         * The value written indicates what kind of register is next to be read or written.
         * See Ads1115RegisterAddresses for the bit values.
         * @param reg
         * @return true on success. false on any error.
         */
        bool writeAddressRegister(Ads1115RegisterAddresses reg);



    private:

        // Note that I've not implemented the pre-check of current register or the post-check of current channel!!

        ControllerId controllerId_;
        uint8_t                     i2cAddress_;    // This is
        Ads1115Gain_t               gain_;
        Ads1115DataRates            dataRate_;
        uint8_t                     dataBuffer_ [3] = {0,0,0};
//        Ads1115RegisterAddresses    currentRegisterAddress_{0}; // Last register written
        // This is the AIN used in the last config. Again, a shortcut.
//        Ads1115Channel_t            currentChannel_;
        absolute_time_t             conversionTimeout_;    // This gets set when we start a conversion.
        Ads1115OperationalStatus    conversionPending_ =
                                        Ads1115OperationalStatus::START_NEW_CONVERSION_OR_CONVERSION_COMPLETE;

    };

}   // namespace CSconverters

#endif  // ADS1115_HPP_
