#pragma once

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <string>
#include <utility>

namespace CSdevices {

/**
 * @brief Component defines an interface to be implemented by most drivers.
 */

    class Component {

    public:
        Component () {
            setClassName("Component");
            setLabel("Base class for most primary classes.");
        }

        virtual ~Component () = default;

        virtual bool init () {return true;}

        std::string getClassName() {return className_;}
        [[nodiscard]] std::string getLabel() const { return label_; }
        [[nodiscard]] std::string getHierarchy() const { return hierarchy_; }

    protected:

        void prependToHierarchy (const std::string& className) {
            hierarchy_ = hierarchy_.empty() ?
                            className       :
                            className + ":" + hierarchy_;
        }

        void setClassName (const std::string& className) {className_ = className;}
        void setLabel (const std::string &label) { label_ = label; }

    private:

        std::string className_;
        std::string label_;
        std::string hierarchy_;


    };

}   // namespace CSdevices

#endif  // COMPONENT_HPP_
