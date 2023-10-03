#include "Renderer.h"
#include "RendererAPI.h"

using namespace Engine;

std::unique_ptr<Renderer::SceneData> Renderer::s_sceneData = std::make_unique<Renderer::SceneData>();

void Renderer::onWindowResize(const uint32_t width, const uint32_t height)
{
	RendererAPI::get()->setViewport(0, 0, width, height);
}

void Renderer::beginScene(const Camera& camera)
{
	s_sceneData->view_projection_matrix = camera.getViewProjection();
	s_sceneData->view_matrix = camera.getView();
	s_sceneData->projection_matrix = camera.getProjection();
}

void Renderer::endScene()
{

}

void Renderer::submit(const std::shared_ptr<Shader>& shader, const VertexArray& vertex_array
                        , const std::vector<Scope<Texture2D>>& textures
                        , const glm::mat4& transform)
{
	shader->bind();
	shader->setMat4("u_ViewProjection", s_sceneData->view_projection_matrix);
	shader->setMat4("u_View", s_sceneData->view_matrix);
	shader->setMat4("u_Projection", s_sceneData->projection_matrix);
	shader->setMat4("u_Transform", transform);

	vertex_array.bind();

    if (!textures.empty())
    {
        for (auto i = 0; i < textures.size(); i++)
        {
            textures[i]->bind(i);
            shader->setInt(textures[i]->getName(), i);
        }
    }

	RendererAPI::get()->drawIndexed(vertex_array);
}