#pragma once

class FileDescriptor {
public:
	FileDescriptor() noexcept;
	FileDescriptor(int fd) noexcept;
	~FileDescriptor() noexcept;

	FileDescriptor(const FileDescriptor&) = delete;
	FileDescriptor&	operator=(const FileDescriptor&) = delete;

    FileDescriptor(FileDescriptor&&) noexcept;
	FileDescriptor&	operator=(FileDescriptor&&) noexcept;
    
    int get() const noexcept;
    int release() noexcept;
private:
    int fd_;
};

