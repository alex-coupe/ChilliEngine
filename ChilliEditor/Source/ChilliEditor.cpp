#include "ChilliEditor.h"

namespace Chilli {

	ChilliEditor::ChilliEditor()
	{
		m_engine = std::make_unique<ChilliEngine>();
		m_engine->AddLayer(std::make_shared<EditorLayer>());
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