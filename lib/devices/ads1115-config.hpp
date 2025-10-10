
#pragma once

#ifndef ADS1115_CONFIG_
#define ADS1115_CONFIG_

#include "ads1115-declarations.hpp"

namespace CSdevices {


    class Ads1115Config {

    public:
        Ads1115Config () {
            data_.shortWord = 0;
        }

        //------------------------------------------------------------------
        // Fluent setters
        //------------------------------------------------------------------

        Ads1115Config& setData (Ads1115ConfigRegister_t configRegister) {
            data_ = configRegister;
            return *this;
        }

        Ads1115Config& setDataShortWord (uint16_t data) {
            data_.shortWord = data;
            return *this;
        }

        Ads1115Config& setOperationalStatus (Ads1115OperationalStatus os) {
            data_.fields.opStatus = ads1115OperationalStatusToNumber(os);
            return *this;
        }

        Ads1115Config& setMux (Ads1115Channel channel) {
            data_.fields.mux = ads1115ChannelToNumber(channel);
            return *this;
        }

        Ads1115Config& setPGA (Ads1115GainValues pga) {
            data_.fields.gain = ads1115GainValuesToNumber(pga);
            return *this;
        }

        Ads1115Config& setOperatingMode (Ads1115OperatingMode mode) {
            data_.fields.operatingMode = ads1115OperatingModeToNumber(mode);
            return *this;
        }

        Ads1115Config& setDataRate (Ads1115DataRates dataRate) {
            data_.fields.dataRate = ads1115DataRatesToNumber(dataRate);
            return *this;
        }

        Ads1115Config& setComparatorMode (Ads1115ComparatorMode mode) {
            data_.fields.compMode = ads1115ComparatorModeToNumber(mode);
            return *this;
        }

        Ads1115Config& setComparatorPolarity (Ads1115ComparatorPolarity polarity) {
            data_.fields.compPolarity = ads1115ComparatorPolarityToNumber(polarity);
            return *this;
        }

        Ads1115Config& setLatchingComparator (Ads1115LatchingComparator compLat) {
            data_.fields.compLatch = ads1115LatchingComparatorToNumber(compLat);
            return *this;
        }

        Ads1115Config& setComparatorQueue (Ads1115ComparatorQueue comparatorQueue) {
            data_.fields.compQue = ads1115ComparatorQueueToNumber(comparatorQueue);
            return *this;
        }

        //------------------------------------------------------------------
        // Getters here
        //------------------------------------------------------------------

        [[nodiscard]] Ads1115ConfigRegister_t getConfigRegister () const {
            return data_;
        }

        [[nodiscard]] uint16_t getData () const {
            return data_.shortWord;
        }

        [[nodiscard]] Ads1115OperationalStatus getOperationalStatus () const {
            return numberToAds1115OperationalStatus(data_.fields.opStatus);
        }

        [[nodiscard]] Ads1115Channel getMux () const {
            return numberToAds1115Channel(data_.fields.mux);
        }

        [[nodiscard]] Ads1115GainValues getPGA () const {
            return numberToAds1115GainValues(data_.fields.gain);
        }

        [[nodiscard]] Ads1115OperatingMode getOperatingMode () const {
            return numberToAds1115OperatingMode(data_.fields.operatingMode);
        }

        [[nodiscard]] Ads1115DataRates getDataRate () const {
            return numberToAds1115DataRates(data_.fields.dataRate);
        }

        [[nodiscard]] Ads1115ComparatorMode getComparatorMode () const {
            return numberToAds1115ComparatorMode (data_.fields.compMode);
        }

        [[nodiscard]] Ads1115ComparatorPolarity getComparatorPolarity () const {
            return NumberToAds1115ComparatorPolarity(data_.fields.compPolarity);
        }

        [[nodiscard]] Ads1115LatchingComparator getLatchingComparator () const {
            return numberToAds1115LatchingComparator(data_.fields.compLatch);
        }

        [[nodiscard]] Ads1115ComparatorQueue getComparatorQueue () const {
            return numberToAds1115ComparatorQueue(data_.fields.compQue);
        }

    private:

        static_assert(sizeof(Ads1115ConfigAllFields) == 2, "Bit field struct size is incorrect!");

        Ads1115ConfigRegister_t data_{0, 0};

    };

}

#endif // ADS1115_CONFIG_
