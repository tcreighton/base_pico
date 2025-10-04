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
        Component (std::string className,
                   std::string label) :
                        className_(std::move(className)),
                        label_(std::move(label)) {}

        virtual ~Component () = default;

        virtual std::string getClassName() {return className_;}
        [[nodiscard]] virtual std::string getLabel() const { return label_; }

    protected:

        void appendLabel (const std::string &appendix) { setLabel(getLabel() + appendix); }
        virtual void setLabel (const std::string &label) { label_ = label; }

    private:

        const std::string className_;
        std::string label_;


    };

}   // namespace CSdevices

#endif  // COMPONENT_HPP_
