#include "svpch.h"
#include "Framebuffer.h"

static const uint32_t s_MaxFramebufferSize = 8192;

// Framebuffer::Framebuffer(FramebufferSpec spec) : specification(spec)
// {
//     invalidate();
// }


Framebuffer::Framebuffer(unsigned int w, unsigned int h, char f)
{
	specification.width = w;
	specification.height = h;

	specification.flags |= f;

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

	if (specification.flags & COLOR_ATTACHMENT)
	{
		if (specification.flags & MULTISAMPLING)
		{
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &color_attachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, color_attachment);

			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			int samples = 4;
    		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGBA8, specification.width,
    		             specification.height, GL_TRUE);

    		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE,
    		                       color_attachment, 0);

		}
		else {
			glCreateTextures(GL_TEXTURE_2D, 1, &color_attachment);
			glBindTexture(GL_TEXTURE_2D, color_attachment);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width,
					specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
    		                       color_attachment, 0);
		}
	}

    // 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


//     glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, specification.width,
//                  specification.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

	if (specification.flags & DEPTH_ATTACHMENT)
	{
		if (specification.flags & MULTISAMPLING)
		{
			glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &depth_attachment);
			glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, depth_attachment);
			glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_DEPTH24_STENCIL8, specification.width,
					specification.height, GL_TRUE);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
					GL_TEXTURE_2D_MULTISAMPLE, depth_attachment, 0);
		}
		else {
			glCreateTextures(GL_TEXTURE_2D, 1, &depth_attachment);
			glBindTexture(GL_TEXTURE_2D, depth_attachment);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, specification.width,
					specification.height);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
					GL_TEXTURE_2D, depth_attachment, 0);
		}
	}

    //   glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
        puts("cos nie tak z framebufferem");
	}

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::bind()
{
//     if (specification.flags & MULTISAMPLING)
//     {
//         glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // Make sure no FBO is set as the draw framebuffer
//         glBindFramebuffer(GL_READ_FRAMEBUFFER, id); // Make sure your multisampled FBO is the read framebuffer
//         glDrawBuffer(GL_BACK); // Set the back buffer as the draw buffer
//         glBlitFramebuffer(0, 0, specification.width, specification.height, 0, 0,
//                           specification.width, specification.height,
//                           GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT, GL_NEAREST);
//     }
//     else
//     {
        glBindFramebuffer(GL_FRAMEBUFFER, id);
//     }
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


void Framebuffer::bind_color_texture(unsigned int slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, color_attachment);
}
