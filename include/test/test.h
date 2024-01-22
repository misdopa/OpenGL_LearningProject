#pragma once

namespace test
{
	class test {
	public:
		test() {};
		virtual ~test() {};
		virtual void OnRendener() {};
		virtual void OnUpdate(float delatTime) {};
		virtual void OnImGuiRender() {};
	};
}
