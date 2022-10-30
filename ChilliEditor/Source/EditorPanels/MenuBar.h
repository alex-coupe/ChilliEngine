#pragma once
#include "../EditorPanel.h"

namespace Chilli {
	class MenuBar : public EditorPanel {
	public:
		MenuBar();
		virtual void Draw()override;
	private:
		std::string m_savePath = "";
		void NewProject();
		void SaveProject();
		void OpenProject();
		void SaveAs();
	};
}