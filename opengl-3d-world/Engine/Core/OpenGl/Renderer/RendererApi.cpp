#include "RendererAPI.h"

#include <iostream>
#include <ostream>

using namespace Engine;

void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam)
{
	std::cout << "OpenGL Error: " << message << std::endl;
}

RendererAPI* RendererAPI::s_instance = nullptr;

void RendererAPI::init()
{
#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);

	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
}

void RendererAPI::setViewport(const uint32_t x, const uint32_t y, const uint32_t width, const uint32_t height)
{
	glViewport(x, y, width, height);
}

void RendererAPI::setClearColor(const glm::vec4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RendererAPI::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::drawIndexed(const VertexArray& vertex_array, const uint32_t index_count)
{
	vertex_array.bind();
	const uint32_t count = index_count ? index_count : vertex_array.getIndexBuffer()->getCount();
	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
}
