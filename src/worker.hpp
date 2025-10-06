#pragma once
#ifndef WORKER_HPP_
#define WORKER_HPP_
#include "csi2c.hpp"

namespace CSworkers {

    class Worker final {
    public:
        Worker () = default;
        ~Worker() = default;

        bool doWork();

    };
};



#endif  // WORKER_HPP_
