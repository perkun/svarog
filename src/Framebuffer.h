#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include <iostream>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>

using namespace std;

struct FramebufferSpecification {
  unsigned int width;
  unsigned int height;
};

class Framebuffer {
public:
  Framebuffer(FramebufferSpecification spec);
  ~Framebuffer();

  void invalidate();

  void bind();
  void unbind();

  void resize(unsigned int width, unsigned int height);

  unsigned int get_color_attachment_id() const { return color_attachment; }
  unsigned int get_depth_attachment_id() const { return depth_attachment; }

  virtual const FramebufferSpecification &GetSpecification() const {
    return specification;
  }

private:
  unsigned int id = 0;
  unsigned int color_attachment = 0, depth_attachment = 0;
  FramebufferSpecification specification;
};

#endif /* FRAMEBUFFER_H_ */
