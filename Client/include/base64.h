#pragma once
#ifndef BASE64_H
#define BASE64_H

#include <string>
#include <vector>

std::string base64_encode(const unsigned char* data, size_t len);
std::vector<unsigned char> base64_decode(const std::string& s);
std::string encodeBase64(const std::vector<char>& data);
std::vector<char> decodeBase64(const std::string& encodedData);
#endif // BASE64_H