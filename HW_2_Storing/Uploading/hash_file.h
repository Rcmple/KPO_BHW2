//
// Created by imaginelipa on 5/25/25.
//

#ifndef HASH_FILE_H
#define HASH_FILE_H
#include <openssl/evp.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

std::string sha256_file(const std::string& filename) {
    const int bufSize = 4096;
    unsigned char buffer[bufSize];

    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file");
    }

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        throw std::runtime_error("Failed to create EVP_MD_CTX");
    }

    if (EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestInit_ex failed");
    }

    while (file.good()) {
        file.read(reinterpret_cast<char*>(buffer), bufSize);
        std::streamsize bytesRead = file.gcount();
        if (bytesRead > 0) {
            if (EVP_DigestUpdate(ctx, buffer, bytesRead) != 1) {
                EVP_MD_CTX_free(ctx);
                throw std::runtime_error("EVP_DigestUpdate failed");
            }
        }
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;
    if (EVP_DigestFinal_ex(ctx, hash, &lengthOfHash) != 1) {
        EVP_MD_CTX_free(ctx);
        throw std::runtime_error("EVP_DigestFinal_ex failed");
    }

    EVP_MD_CTX_free(ctx);

    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        oss << std::setw(2) << static_cast<int>(hash[i]);
    }

    return oss.str();
}
#endif //HASH_FILE_H
