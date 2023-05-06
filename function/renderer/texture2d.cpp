#include "texture2d.h"

#include <QOpenGLFunctions>

Texture2D::Texture2D(int w, int h)
{
    auto glf = QOpenGLContext::currentContext()->functions();
    glf->glGenTextures(1, &m_ID);
    glf->glActiveTexture(GL_TEXTURE0);
    glf->glBindTexture(GL_TEXTURE_2D, m_ID);
    glf->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glf->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glf->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glf->glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glf->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glf->glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
    auto glf = QOpenGLContext::currentContext()->functions();
    glf->glDeleteTextures(1, &m_ID);
}
