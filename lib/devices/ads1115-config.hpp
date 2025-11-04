
#pragma once

#ifndef ADS1115_CONFIG_
#define ADS1115_CONFIG_

#include "ads1115-definitions.hpp"

namespace CSdevices {

    class Ads1115Config {

    public:
        Ads1115Config () {
            data_.shortWord = 0;
        }

        //------------------------------------------------------------------
        // Fluent setters
        //------------------------------------------------------------------

        Ads1115Config& setData (const Ads1115ConfigRegister_t configRegister) {
            data_ = configRegister;
            return *this;
        }

        Ads1115Config& setDataShortWord (const uint16_t data) {
            data_.shortWord = data;
            return *this;
        }

        Ads1115Config& setOperationalStatus (const Ads111xOperationalStatus os) {
            data_.fields.opStatus = ads1115OperationalStatusToNumber(os);
            return *this;
        }

        Ads1115Config& setMux (const Ads1115Channel channel) {
            data_.fields.mux = ads1115ChannelToNumber(channel);
            return *this;
        }

        Ads1115Config& setPGA (const AdsGainValues pga) {
            data_.fields.gain = adsGainValuesToNumber(pga);
            return *this;
        }

        Ads1115Config& setOperatingMode (const Ads111xOperatingMode mode) {
            data_.fields.operatingMode = ads111xOperatingModeToNumber(mode);
            return *this;
        }

        Ads1115Config& setDataRate (const Ads111xDataRates dataRate) {
            data_.fields.dataRate = ads111xDataRatesToNumber(dataRate);
            return *this;
        }

        Ads1115Config& setComparatorMode (const Ads1115ComparatorMode mode) {
            data_.fields.compMode = ads1115ComparatorModeToNumber(mode);
            return *this;
        }

        Ads1115Config& setComparatorPolarity (const Ads1115ComparatorPolarity polarity) {
            data_.fields.compPolarity = ads1115ComparatorPolarityToNumber(polarity);
            return *this;
        }

        Ads1115Config& setLatchingComparator (const Ads1115LatchingComparator compLat) {
            data_.fields.compLatch = ads1115LatchingComparatorToNumber(compLat);
            return *this;
        }

        Ads1115Config& setComparatorQueue (const Ads1115ComparatorQueue comparatorQueue) {
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

        [[nodiscard]] Ads111xOperationalStatus getOperationalStatus () const {
            return numberToAds111xOperationalStatus(data_.fields.opStatus);
        }

        [[nodiscard]] Ads1115Channel getMux () const {
            return numberToAds1115Channel(data_.fields.mux);
        }

        [[nodiscard]] AdsGainValues getPGA () const {
            return numberToAdsGainValues(data_.fields.gain);
        }

        [[nodiscard]] Ads111xOperatingMode getOperatingMode () const {
            return numberToAds111xOperatingMode(data_.fields.operatingMode);
        }

        [[nodiscard]] Ads111xDataRates getDataRate () const {
            return numberToAds111xDataRates(data_.fields.dataRate);
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
