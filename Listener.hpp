#pragma once
#include <cstdint>
#include "FileDescriptor.hpp"


class Listener {
public:
    explicit Listener(uint16_t port);
    ~Listener() noexcept = default;

    Listener(const Listener&) = delete;
    Listener& operator=(const Listener&) = delete;

    Listener(Listener&&) noexcept = default;
    Listener& operator=(Listener&&) noexcept = default;

    int accept();
    // int accept_non_blocking();
    // void set_non_blocking(bool nb);
    // int get_fd() const noexcept { return fd_.get(); }
private:
    FileDescriptor fd_;
    uint16_t port_;
    bool non_blocking_ = false;
};

