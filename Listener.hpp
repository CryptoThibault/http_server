#pragma once
#include "FileDescriptor.hpp"
#include <cstdint>


class Listener {
public:
    explicit Listener(uint16_t port);
    ~Listener() noexcept = default;

    Listener(const Listener&) = delete;
    Listener& operator=(const Listener&) = delete;

    Listener(Listener&&) noexcept = default;
    Listener& operator=(Listener&&) noexcept = default;

    int accept();

private:
    FileDescriptor fd_;
    uint16_t port_;
};

