#include "ChilliEditor.h"

namespace Chilli {

	std::shared_ptr<Asset> ChilliEditor::s_selectedAsset = nullptr;
	std::shared_ptr<Scene> ChilliEditor::s_selectedScene = nullptr;
	std::shared_ptr<Entity> ChilliEditor::s_selectedEntity = nullptr;

	ChilliEditor::ChilliEditor()
	{
		m_engine = std::make_unique<ChilliEngine>();
		m_engine->SetAppLayer(std::make_shared<EditorLayer>());
		ImGui::SetCurrentContext(GuiManager::GetContext());
	}

	ChilliEditor::~ChilliEditor()
	{
		m_engine.reset();
	}

	void ChilliEditor::Run()
	{
		return m_engine->Run();
	}

}