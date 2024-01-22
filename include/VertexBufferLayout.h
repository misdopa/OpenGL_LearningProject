#pragma once
#include<vector>
#include"OpenGLDebug.h"


struct VertexBufferElement
{
	unsigned int count; // ���ٸ�Ԫ��Ϊһ��
	unsigned char nomorlize; // �Ƿ���Ҫ��׼��
	unsigned int type; //������ʲô


	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT:   return 4;
		case GL_UNSIGNED_INT:   return 4;
		case GL_UNSIGNED_BYTE:   return 1;

		}

		ASSERT(false);
		return 0;
	}

};

class VertexBufferLayout
{

private:
	//�����Ԫ�ز�ֻ�Ƕ���λ�ã�Ҳ�п����Ƕ������ɫ������ķ��ߵ�
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_Stride;

public:
	VertexBufferLayout() :m_Stride(0) {};
	template<typename T>
	void Push(unsigned int count)
	{
		ASSERT(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ count,GL_FALSE,GL_FLOAT });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT)* count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ count,GL_FALSE,GL_UNSIGNED_INT });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT)* count;

	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ count,GL_TRUE,GL_UNSIGNED_BYTE });
		m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE)* count;

	}

	inline std::vector<VertexBufferElement> GetElements() const { return m_Elements; }
	inline unsigned int GetStride() const { return m_Stride; }
};