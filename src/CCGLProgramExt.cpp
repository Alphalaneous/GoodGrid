#include "CCGLProgramExt.hpp"

CCGLProgramExt* CCGLProgramExt::create(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray) {
    auto ret = new CCGLProgramExt();
    if (ret->initWithVertexShaderByteArray(vShaderByteArray, fShaderByteArray)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

bool CCGLProgramExt::initWithVertexShaderByteArray(const GLchar* vShaderByteArray, const GLchar* fShaderByteArray) {
    m_uProgram = glCreateProgram();
    CHECK_GL_ERROR();

    m_uVertShader = m_uFragShader = 0;

    if (vShaderByteArray) {
        if (!compileShader(&m_uVertShader, GL_VERTEX_SHADER, vShaderByteArray)) {
            log::error("ERROR: Failed to compile vertex shader");
 			return false;
        }
    }

    if (fShaderByteArray) {
        if (!compileShader(&m_uFragShader, GL_FRAGMENT_SHADER, fShaderByteArray)) {
            log::error("ERROR: Failed to compile fragment shader");
			return false;
        }
    }

    if (m_uVertShader) {
        glAttachShader(m_uProgram, m_uVertShader);
    }
    CHECK_GL_ERROR();

    if (m_uFragShader) {
        glAttachShader(m_uProgram, m_uFragShader);
    }
    m_pHashForUniforms = NULL;
    
    CHECK_GL_ERROR();

    return true;
}

// cuz cocos adds stuff at the start, I cant use extensions without this :(
bool CCGLProgramExt::compileShader(GLuint* shader, GLenum type, const GLchar* source) {
    GLint status;
 
    if (!source) {
        return false;
    }
    
    const GLchar *sources[] = {
        "#extension GL_OES_standard_derivatives : enable\n", 
#if GEODE_IS_MOBILE
        (type == GL_VERTEX_SHADER ? "precision highp float;\n" : "precision mediump float;\n"),
#endif
        "uniform mat4 CC_PMatrix;\n"
        "uniform mat4 CC_MVMatrix;\n"
        "uniform mat4 CC_MVPMatrix;\n"
        "uniform vec4 CC_Time;\n"
        "uniform vec4 CC_SinTime;\n"
        "uniform vec4 CC_CosTime;\n"
        "uniform vec4 CC_Random01;\n"
        "//CC INCLUDES END\n\n",
        source,
    };

    *shader = glCreateShader(type);
    glShaderSource(*shader, sizeof(sources)/sizeof(*sources), sources, NULL);
    glCompileShader(*shader);

    glGetShaderiv(*shader, GL_COMPILE_STATUS, &status);

    if (!status) {
        GLsizei length;
		glGetShaderiv(*shader, GL_SHADER_SOURCE_LENGTH, &length);
		GLchar* src = (GLchar *)malloc(sizeof(GLchar) * length);
		
		glGetShaderSource(*shader, length, NULL, src);
        
        free(src);
    }
    return (status == GL_TRUE);
}