#include "FileDescriptor.hpp"
#include <unistd.h>

FileDescriptor::FileDescriptor() noexcept : fd_(-1) {}

FileDescriptor::FileDescriptor(int fd) noexcept : fd_(fd) {}

FileDescriptor::~FileDescriptor() noexcept {
    if (fd_ >= 0)
        ::close(fd_);
}

FileDescriptor::FileDescriptor(FileDescriptor&& other) noexcept : fd_(other.fd_) {
        other.fd_ = -1;
}

FileDescriptor& FileDescriptor::operator=(FileDescriptor&& other) noexcept {
    if (this != &other) {
        if (fd_ >= 0)
            ::close(fd_);
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

int FileDescriptor::get() const noexcept {
    return fd_;
}

int FileDescriptor::release() noexcept {
    int tmp = fd_;
    fd_ = -1;
    return tmp;
}