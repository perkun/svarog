#ifndef FRAMEBUFFER_H_
#define FRAMEBUFFER_H_

#include "Core.h"

using namespace std;

// enum FbFlag {
// 	COLOR_ATTACHMENT = 0b1,
// 	MULTISAMPLING = 0b10,
// 	DEPTH_ATTACHMENT = 0b100,
// };


enum class FramebufferTextureFormat
{
	None = 0,

	// Color
	RGBA8,
	RGBA32F,
	RED_INTEGER,

	// Depth/stencil
	DEPTH24STENCIL8,
	DEPTH32FSTENCIL8,

	// Defaults
	Depth = DEPTH32FSTENCIL8
};

struct FramebufferTextureSpecification
{
	FramebufferTextureSpecification() = default;
	FramebufferTextureSpecification(FramebufferTextureFormat format)
		: texture_format(format) {}

	FramebufferTextureFormat texture_format = FramebufferTextureFormat::None;
	// TODO: filtering/wrap
};

struct FramebufferAttachmentSpecification
{
	FramebufferAttachmentSpecification() = default;
	FramebufferAttachmentSpecification(
			std::initializer_list<FramebufferTextureSpecification> texture_specs)
		: texture_specs(texture_specs) {}

	std::vector<FramebufferTextureSpecification> texture_specs;
};

struct FramebufferSpecification
{
	unsigned int width = 0, height = 0;
	FramebufferAttachmentSpecification attachments;
	unsigned int samples = 1;

	bool SwapChainTarget = false;
	char flags = 0b0;  // TODO delete this
};



// struct FramebufferSpec
// {
// 	unsigned int width;
// 	unsigned int height;
// 	char flags = 0b0;
// };

class Framebuffer
{
	public:
		static void blit(Framebuffer *ms_framebuffer, Framebuffer *framebuffer);
		static GLenum texture_target(bool multisampled);
		static void create_textures(bool multisampled, unsigned int *out_id, unsigned int count);
		static void bind_texture(bool multisampled, unsigned int id);

		static void attach_color_texture(unsigned int id, int samples, GLenum format,
				unsigned int width, unsigned int height, int index);

		static void attach_depth_texture(unsigned int id, int samples, GLenum format,
				GLenum attachment_type,
				unsigned int width, unsigned int height);

// 		Framebuffer(unsigned int w, unsigned int h, char f);
		Framebuffer(const FramebufferSpecification &specs);
		~Framebuffer();

		void invalidate();

		void bind();
		void bind_depth_texture(unsigned int slot);
		void bind_color_texture(unsigned int slot);
		void clear();
		void unbind();

		void resize(unsigned int width, unsigned int height);

		unsigned int get_color_attachment_id(int slot = 0) const
		{
			return color_attachments[slot];
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
		FramebufferSpecification specification;

	private:
		vector<unsigned int> color_attachments;
		unsigned int depth_attachment = 0;

		vector<FramebufferTextureSpecification> color_attachment_specs;
		FramebufferTextureSpecification depth_attachment_specs;
};

#endif /* FRAMEBUFFER_H_ */
