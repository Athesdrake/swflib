#pragma once
#include <exception>

namespace swf {
class MessageException : public std::exception {
public:
    MessageException() : message("MessageException") { }
    MessageException(std::string message) : message(message) { }

    const char* what() const noexcept override { return message.c_str(); }

protected:
    std::string message;
};

class InvalidSignature : public MessageException {
public:
    InvalidSignature(const char* signature) : signature(signature) {
        message = std::string("'") + signature + "' is not a valid signature.";
    }

private:
    const char* signature;
};

class UnsupportedCompression : public MessageException {
public:
    UnsupportedCompression(const char compression) : compression(compression) {
        message = std::string("The compression '") + compression + "' is not supported.";
    }

private:
    const char compression;
};

class CompressionError : public MessageException {
public:
    CompressionError(const char* compression) : compression(compression) {
        message = compression + std::string(" failed to (de)compress stream.");
    }

private:
    const char* compression;
};
}