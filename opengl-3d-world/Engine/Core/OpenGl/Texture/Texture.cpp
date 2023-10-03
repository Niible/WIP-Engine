#include "Texture.h"

#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace Engine;

Texture2D::Texture2D(uint32_t width, uint32_t height) : m_width(width), m_height(height)
{
	m_internalFormat = GL_RGBA8;
	m_dataFormat = GL_RGBA;

	glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
	glTextureStorage2D(m_rendererId, 1, m_internalFormat, m_width, m_height);

	glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture2D::Texture2D(const std::string& name, const std::string& path)
	: m_name(name)
    , m_path(path)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = nullptr;
	{
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
	}

	if (data)
	{
		m_isLoaded = true;

		m_width = width;
		m_height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;

		if (!(internalFormat & dataFormat))
		{
			throw std::runtime_error("Format not supported!");
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_rendererId);
		glTextureStorage2D(m_rendererId, 1, internalFormat, m_width, m_height);

		glTextureParameteri(m_rendererId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_rendererId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_rendererId, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_rendererId);
}

void Texture2D::setData(void* data, uint32_t size)
{
	const uint32_t bpp = m_dataFormat == GL_RGBA ? 4 : 3;
	if (size == m_width * m_height * bpp)
	{
		throw std::runtime_error("Data must be entire texture!");
	}

	glTextureSubImage2D(m_rendererId, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void Texture2D::bind(uint32_t slot) const
{
	glBindTextureUnit(slot, m_rendererId);
}
