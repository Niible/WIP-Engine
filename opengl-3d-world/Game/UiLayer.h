#pragma once
#include <imgui.h>
#include <fstream>
#include "HeightMap.h"
#include "TerrainGenerator.h"
#include "../Engine/ImGui/ImGuiLayer.h"
#include "../Engine/Core/Application/Application.h"


class UiLayer : public ImGuiLayer
{
public:
	UiLayer() = default;

	void onUpdate(float dt) override
	{
		begin();
		{
			//ImGui::ShowDemoWindow();
			auto& io = ImGui::GetIO();
			ImGui::Begin("Debug");

			if (ImGui::BeginTabBar("Settings")) {
				if (ImGui::BeginTabItem("Thresholds")) {
					{
						bool changed = false;
						changed |= ImGui::SliderFloat("Sand", &m_terrain_thresholds.sand_threshold, 0.f, 100.f);
						changed |= ImGui::SliderFloat("Grass", &m_terrain_thresholds.grass_threshold, 0.f, 100.f);
						changed |= ImGui::SliderFloat("Rock", &m_terrain_thresholds.rock_threshold, 0.f, 100.f);

						if (changed)
						{
							DataEvent event = { m_terrain_thresholds };
							notify(event);
						}
					}
					ImGui::EndTabItem();
				}


				if (ImGui::BeginTabItem("Noise Settings")) {
					bool changed = false;
					changed |= ImGui::SliderInt("Octave", &m_height_map_noise_settings.octave, 1, 10);
					changed |= ImGui::SliderFloat("Lacunarity", &m_height_map_noise_settings.lacunarity, 1.f, 3.f);
					changed |= ImGui::SliderFloat("Gain", &m_height_map_noise_settings.gain, 0.f, 1.f);
					changed |= ImGui::SliderFloat("Frequency", &m_height_map_noise_settings.frequency, 0.01f, 0.1f);
					changed |= ImGui::InputInt("Seed", &m_height_map_noise_settings.seed);

					if (changed) {
						DataEvent event = { m_height_map_noise_settings };
						notify(event);
					}
					ImGui::EndTabItem();
				}


				if (ImGui::BeginTabItem("Map Settings")) {
					bool changed = false;
					changed |= ImGui::InputInt2("Position", &m_height_map_settings.position[0]);
					changed |= ImGui::InputInt2("Size", &m_height_map_settings.size[0]);
					changed |= ImGui::InputInt("LOD", &m_height_map_settings.lod);

					{
						int map_point_size = m_height_map_settings.map_points.size();
						if (ImGui::InputInt("step", &map_point_size))
						{
							m_height_map_settings.map_points.resize(map_point_size);
							m_height_map_settings.map_values.resize(map_point_size);
						}

						float windowWidth = ImGui::GetWindowContentRegionWidth();
						float sliderWidth = windowWidth / (m_height_map_settings.map_points.size() * 2);

						for (int i = 0; i < m_height_map_settings.map_points.size(); i++) {
							ImGui::PushItemWidth(sliderWidth);
							changed |= ImGui::SliderFloat(("p" + std::to_string(i)).c_str(), &m_height_map_settings.map_points[i], -1.0f, 1.0f);
							ImGui::PopItemWidth();
							if (i < m_height_map_settings.map_points.size() - 1) ImGui::SameLine();
						}

						for (int i = 0; i < m_height_map_settings.map_values.size(); i++) {
							ImGui::PushItemWidth(sliderWidth);
							changed |= ImGui::SliderFloat(("v" + std::to_string(i)).c_str(), &m_height_map_settings.map_values[i], 0.0f, 20.f);
							ImGui::PopItemWidth();
							if (i < m_height_map_settings.map_values.size() - 1) ImGui::SameLine();
						}
					}

					if (changed) {
						DataEvent event = { m_height_map_settings };
						notify(event);
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}

			ImGui::End();


		}
		end();
	}

	void onDetach() override
	{
		ImGuiLayer::onDetach();
		saveData();
	}

	void onAttach() override
	{
		ImGuiLayer::onAttach();
		loadData();
	}

private:
	static void notify(Event& event)
	{
		Application::get().onEvent(event);
	}

	void saveData()
	{
		std::ofstream outFile("appData.txt");
		if (outFile.is_open()) {
			outFile << m_terrain_thresholds.sand_threshold << std::endl;
			outFile << m_terrain_thresholds.grass_threshold << std::endl;
			outFile << m_terrain_thresholds.rock_threshold << std::endl;

			outFile << m_height_map_noise_settings.octave << std::endl;
			outFile << m_height_map_noise_settings.lacunarity << std::endl;
			outFile << m_height_map_noise_settings.gain << std::endl;
			outFile << m_height_map_noise_settings.frequency << std::endl;
			outFile << m_height_map_noise_settings.seed << std::endl;

			outFile << m_height_map_settings.position.x << std::endl;
			outFile << m_height_map_settings.position.y << std::endl;
			outFile << m_height_map_settings.size.x << std::endl;
			outFile << m_height_map_settings.size.y << std::endl;
			outFile << m_height_map_settings.lod << std::endl;
			outFile << m_height_map_settings.map_points.size() << std::endl;
			for (auto f : m_height_map_settings.map_points)
			{
				outFile << f << std::endl;
			}
			outFile << m_height_map_settings.map_values.size() << std::endl;
			for (auto f : m_height_map_settings.map_values)
			{
				outFile << f << std::endl;
			}

			// Write other data members as needed
			outFile.close();
		}
	}

	void loadData()
	{
		std::ifstream inFile("appData.txt");
		if (inFile.is_open()) {
			inFile >> m_terrain_thresholds.sand_threshold;
			inFile >> m_terrain_thresholds.grass_threshold;
			inFile >> m_terrain_thresholds.rock_threshold;

			inFile >> m_height_map_noise_settings.octave;
			inFile >> m_height_map_noise_settings.lacunarity;
			inFile >> m_height_map_noise_settings.gain;
			inFile >> m_height_map_noise_settings.frequency;
			inFile >> m_height_map_noise_settings.seed;

			inFile >> m_height_map_settings.position.x;
			inFile >> m_height_map_settings.position.y;
			inFile >> m_height_map_settings.size.x;
			inFile >> m_height_map_settings.size.y;
			inFile >> m_height_map_settings.lod;
			size_t size;
			inFile >> size;
			m_height_map_settings.map_points.resize(size);
			for (int i = 0; i < size; ++i)
			{
				float d;
				inFile >> d;
				m_height_map_settings.map_points[i] = d;
			}

			inFile >> size;
			m_height_map_settings.map_values.resize(size);
			for (int i = 0; i < size; ++i)
			{
				float d;
				inFile >> d;
				m_height_map_settings.map_values[i] = d;
			}

			DataEvent event_terrain_thresholds = { m_terrain_thresholds };
			notify(event_terrain_thresholds);

			DataEvent event_height_map_noise_settings = { m_height_map_noise_settings };
			notify(event_height_map_noise_settings);


			DataEvent event_height_map_settings = { m_height_map_settings };
			notify(event_height_map_settings);

			inFile.close();
		}
	}

private:
	TerrainThresholds m_terrain_thresholds;
	HeightMapNoiseSettings m_height_map_noise_settings;
	HeightMapSettings m_height_map_settings;
};
