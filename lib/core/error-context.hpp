#pragma once
#ifndef ERROR_CONTEXT_HPP_

#include <sstream>
#include <string>
#include "errors.hpp"

namespace CScore {

    class ErrorContext {

    public:

        ErrorContext () = default;
        ~ErrorContext() = default;

        void set (const ResponseCode code,
                    std::string& component,
                    std::string& function,
                    std::string& file,
                    const int line,
                    std::string& message) {
            component_      = std::move(component);
            function_       = std::move(function);
            file_           = std::move(file);
            line_           = line;
            message_        = std::move(message);
            responseCode_   = code;
        }

        void clear () {
            component_.clear();
            function_.clear();
            file_.clear();
            line_ = 0;
            message_.clear();
            responseCode_ = ResponseCode::NOT_AN_ERROR;
        }

        [[nodiscard]] bool hasError () const {
            return ResponseCode::NOT_AN_ERROR != responseCode_;
        }

        [[nodiscard]] std::string format () const {
            if (!hasError()) return "";

            std::stringstream ss;
            ss << "Error in " << component_ << ":" << function_ << ":" << file_ << "@" << line_ << ": " <<
                        message_ << "; code:" << responseToString(responseCode_);

            return ss.str();
        }

    private:
        std::string component_;
        std::string function_;
        std::string file_;
        int line_{0};
        std::string message_;
        ResponseCode responseCode_{ResponseCode::NOT_AN_ERROR};
    };

}

#endif  // ERROR_CONTEXT_HPP_
