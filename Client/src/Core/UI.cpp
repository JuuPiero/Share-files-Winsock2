#include "Core/UI.h"
#include "pch.h"
#include <stb_image.h>
GLuint LoadTextureFromFile(const char* filename) {
    // Load ảnh từ file
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 0);
    if (data == nullptr) {
        std::cerr << "Failed to load image: " << filename << std::endl;
        return 0;
    }

    // Tạo texture OpenGL
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Giải phóng dữ liệu ảnh sau khi tạo texture
    stbi_image_free(data);

    return texture;
}

GLuint BasicFileTexture() {
    return LoadTextureFromFile("./assets/file.png");
}
