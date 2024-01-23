#include"test\test.h"
#include"vendor\imgui\imgui.h"

namespace test {
	TestMenu::TestMenu(test*& currentTest):m_CurrentTest(currentTest)
	{

	}

	void TestMenu::OnImGuiRender()
	{
		for (auto& it:m_Tests)
		{
			if (ImGui::Button(it.first.c_str()))
			{
				m_CurrentTest=it.second();
			}
		}
	}
}
