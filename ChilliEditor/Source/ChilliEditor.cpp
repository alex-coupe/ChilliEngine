#include "ChilliEditor.h"

namespace Chilli {

	ChilliEditor::ChilliEditor()
	{
		m_engine = std::make_unique<ChilliEngine>();
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