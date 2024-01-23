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
		//ͨ�����캯���д��ݹ�����ָ���������ָ�����ã��Ӷ�����������ı����������������
		test*& m_CurrentTest;
		//fuction<����ֵ(��Ҫ�Ĳ���)>
		std::vector<std::pair<std::string, std::function<test*()>>> m_Tests;
	};
}
