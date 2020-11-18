#include "Framebuffer.h"

static const uint32_t s_MaxFramebufferSize = 8192;

Framebuffer::Framebuffer(FramebufferSpecification spec) : specification(spec)
{
    invalidate();
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers(1, &id);
    glDeleteTextures(1, &color_attachment);
    glDeleteTextures(1, &depth_attachment);
}

void Framebuffer::invalidate()
{
    if (id)
    {
        glDeleteFramebuffers(1, &id);
        glDeleteTextures(1, &color_attachment);
        glDeleteTextures(1, &depth_attachment);
    }

    glCreateFramebuffers(1, &id);
    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment);
    glBindTexture(GL_TEXTURE_2D, color_attachment);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width,
                 specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           color_attachment, 0);

    glCreateTextures(GL_TEXTURE_2D, 1, &depth_attachment);
    glBindTexture(GL_TEXTURE_2D, depth_attachment);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, specification.width,
                   specification.height);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                           GL_TEXTURE_2D, depth_attachment, 0);

    //   glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        puts("cos nie tak z framebufferem");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    glViewport(0, 0, specification.width, specification.height);
}

void Framebuffer::clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::resize(unsigned int width, unsigned int height)
{
    if (width == 0 || height == 0 || width > s_MaxFramebufferSize ||
        height > s_MaxFramebufferSize)
    {
        cout << "Attempted to rezize framebuffer to " << width << ", " << height
             << endl;
        return;
    }
    specification.width = width;
    specification.height = height;

    invalidate();
}


void Framebuffer::bind_depth_texture(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, depth_attachment);
}
