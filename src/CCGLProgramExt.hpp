#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;


#define CHECK_GL_ERROR() \
    [] { \
        GLenum error = glGetError(); \
        if (error) { \
            log::error("OpenGL error: {}", error); \
        } \
    }

class CCGLProgramExt : public CCGLProgram {
public:
    static CCGLProgramExt* create(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);

protected:
    bool compileShader(GLuint* shader, GLenum type, const GLchar* source);
    bool initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray);
};