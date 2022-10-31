#pragma once
#include <memory>
#include "Core\ChilliEngine.h"
#include "EditorLayer.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif

namespace Chilli {

	class ChilliEditor {
	public:
		ChilliEditor();
		~ChilliEditor();
		ChilliEditor(const ChilliEditor&) = delete;
		ChilliEditor& operator=(const ChilliEditor&) = delete;
		ChilliEditor(ChilliEditor&&) = default;
		void Run();
		static std::shared_ptr<Asset> s_selectedAsset;
		static std::shared_ptr<Scene> s_selectedScene;
		static std::shared_ptr<Entity> s_selectedEntity;
	private:
		std::shared_ptr<EditorLayer> m_editorLayer;
		std::unique_ptr<ChilliEngine> m_engine = nullptr;
	};
}