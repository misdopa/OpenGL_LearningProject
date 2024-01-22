#include "test\TestClearColor.h"
#include"OpenGLDebug.h"
#include"Renderer.h"
#include"vendor\imgui\imgui.h"

namespace test {
	TestClearColor::TestClearColor() :m_ClearColor{ 0.2f,0.3f,0.8f,1.0f }
	{
	}

	TestClearColor::~TestClearColor()
	{
	}

	void TestClearColor::OnRendener()
	{
		//设置windows背景颜色
		GLErrorCallBack(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		//使用颜色
		GLErrorCallBack(glClear(GL_COLOR_BUFFER_BIT));

	}

	void TestClearColor::OnUpdate(float delatTime)
	{
	}

	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("clear color", m_ClearColor);
	}
}
