#pragma once

#include"test\test.h"

namespace test {
	class TestClearColor:public test
	{
	public:
		TestClearColor();
		~TestClearColor();

		void OnRendener()override;
		void OnUpdate(float delatTime) override;
		void OnImGuiRender() override;

	private:
		float m_ClearColor[4];

	};
}
