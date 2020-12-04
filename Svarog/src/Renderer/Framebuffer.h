#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <GL/glew.h>
#include <GL/gl.h>
#include <iostream>
#include <stdio.h>

using namespace std;

enum FbFlag {
	COLOR_ATTACHMENT = 0b1,
	MULTISAMPLING = 0b10,
	DEPTH_ATTACHMENT = 0b100,
};

struct FramebufferSpec
{
    unsigned int width;
    unsigned int height;
	char flags = 0b0;
};

class Framebuffer
{
public:
    Framebuffer(unsigned int w, unsigned int h, char f);
    ~Framebuffer();

    void invalidate();

    void bind();
	void bind_depth_texture(unsigned int slot);
	void bind_color_texture(unsigned int slot);
    void clear();
    void unbind();

    void resize(unsigned int width, unsigned int height);

    unsigned int get_color_attachment_id() const
    {
        return color_attachment;
    }
    unsigned int get_depth_attachment_id() const
    {
        return depth_attachment;
    }

//     virtual const FramebufferSpec &GetSpecification() const
//     {
//         return specification;
//     }
    unsigned int id = 0;
    FramebufferSpec specification;

private:
    unsigned int color_attachment = 0, depth_attachment = 0;
};

#endif /* FRAMEBUFFER_H_ */
