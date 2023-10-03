#pragma once
#include <string>
#include <gl/glew.h>

namespace Engine
{
	

class Texture
{
public:
	virtual ~Texture() = default;

	virtual uint32_t getWidth() const = 0;
	virtual uint32_t getHeight() const = 0;
	virtual uint32_t getRendererId() const = 0;

	virtual const std::string& getPath() const = 0;

	virtual void setData(void* data, uint32_t size) = 0;

	virtual void bind(uint32_t slot = 0) const = 0;

	virtual bool isLoaded() const = 0;

	virtual bool operator==(const Texture& other) const = 0;
};

class Texture2D : public Texture
{
public:
	Texture2D(uint32_t width, uint32_t height);
	Texture2D(const std::string& name, const std::string& path);
	~Texture2D() override;

	uint32_t getWidth() const override { return m_width; }
	uint32_t getHeight() const override { return m_height; }
	uint32_t getRendererId() const override { return m_rendererId; }

	const std::string& getPath() const override { return m_path; }
    const std::string& getName() const { return m_name; }

	void setData(void* data, uint32_t size) override;

	void bind(uint32_t slot = 0) const override;

	bool isLoaded() const override { return m_isLoaded; }

	bool operator==(const Texture& other) const override
	{
		return m_rendererId == other.getRendererId();
	}

private:
    std::string m_name;
	std::string m_path;
	bool m_isLoaded = false;
	uint32_t m_width = 0, m_height = 0;
	uint32_t m_rendererId = 0;
	GLenum m_internalFormat, m_dataFormat;
};

}
