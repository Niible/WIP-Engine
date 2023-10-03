#pragma once
#include "../../Camera/Camera.h"
#include "../Buffer/VertexArray.h"
#include "../Shader/Shader.h"
#include "../Texture/Texture.h"

namespace Engine {

	class Renderer
	{
	public:
		//static void init();
		//static void shutdown();

		static void onWindowResize(uint32_t width, uint32_t height);

		static void beginScene(const Camera& camera);

		static void endScene();

		static void submit(const std::shared_ptr<Shader>& shader, const VertexArray& vertex_array, const std::vector<Scope<Texture2D>>& textures, const glm::mat4& transform = glm::mat4(1.0f));

	private:
		struct SceneData
		{
			glm::mat4 view_projection_matrix;
			glm::mat4 view_matrix;
			glm::mat4 projection_matrix;
		};

		static std::unique_ptr<SceneData> s_sceneData;
	};

}