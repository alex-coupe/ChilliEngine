#include "ChilliEditor.h"

namespace Chilli {

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