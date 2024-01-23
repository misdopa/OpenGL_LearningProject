#pragma once
#include<vector>
#include<string>
#include<functional>
#include<iostream>

namespace test
{
	class test {
	public:
		test() {};
		virtual ~test() {};
		virtual void OnRendener() {};
		virtual void OnUpdate(float delatTime) {};
		virtual void OnImGuiRender() {};
		virtual void OnClear() {};
	};

	class TestMenu :public test {
	public:
		TestMenu(test*& currentTest);

		void OnImGuiRender()override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering Test" << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() {return new T(); }));
		}
	private:
		//通过构造函数中传递过来的指针来绑定这个指针引用，从而可以用外面的变量来控制这个变量
		test*& m_CurrentTest;
		//fuction<返回值(需要的参数)>
		std::vector<std::pair<std::string, std::function<test*()>>> m_Tests;
	};
}
