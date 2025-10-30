#pragma once

#ifndef COMPONENT_HPP_
#define COMPONENT_HPP_

#include <string>

namespace CScore {

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

        void setClassName(const std::string_view className) {
            className_ = className;
            prependToHierarchy(className.data());
        }

        void setLabel (const std::string &label) { label_ = label; }

    private:
        void prependToHierarchy (const std::string_view className) {
            hierarchy_ = hierarchy_.empty() ?
                            className.data()       :
                            className.data() + std::string(":") + hierarchy_;
        }

        std::string className_;
        std::string label_;
        std::string hierarchy_;


    };

}   // namespace CSdevices

#endif  // COMPONENT_HPP_
