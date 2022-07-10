#include <Foundation.h>
#include <Foundation/Core/EntryPoint.h>

#include "EditorLayer.h"

namespace Foundation
{
	class FoundationEditor : public Application
	{
	public:
		FoundationEditor() : Application("Foundation Editor")
		{
			PushLayer(new EditorLayer());
		}

		~FoundationEditor()
		{
		}
	};

	Application* CreateApplication()
	{
		return new FoundationEditor();
	}
}