#include "ChilliEditor.h"

int main()
{
	{
		std::unique_ptr<Chilli::ChilliEditor> editor = std::make_unique<Chilli::ChilliEditor>();
		editor->Run();
	}
	return 0;
}