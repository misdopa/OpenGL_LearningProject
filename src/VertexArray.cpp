#include"VertexArray.h"
#include"OpenGLDebug.h"


VertexArray::VertexArray()
{
	GLErrorCallBack(glGenVertexArrays(1, &vao));
}

VertexArray::~VertexArray()
{
	GLErrorCallBack(glDeleteVertexArrays(1,&vao));

}

void VertexArray::AddBuffer(const VertexBuff& Buffer, const VertexBufferLayout& layout)
{
	//��Ҫ���а󶨣���������а󶨾Ͱ���������Ϣ���������ͻᱨ���ҵ��º���
	Bind();
	Buffer.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i=0;i< elements.size();i++)
	{
		const auto& element = elements[i];

		GLErrorCallBack(glEnableVertexAttribArray(i));// ����ͨ�ö�����������,index ָ��ͨ�ö���������Ҫ ���� �� ���õ�������
		/*
		�����������˵��
		index ָ��Ҫ���õĶ������Եı�š�
		size ָ��ÿ���������Եķ�������1��2��3 �� 4������������ά��һ������
		type ָ��ÿ���������������ͣ������� GL_BYTE��GL_UNSIGNED_BYTE��GL_SHORT��GL_UNSIGNED_SHORT��GL_INT��GL_UNSIGNED_INT��GL_FLOAT �� GL_DOUBLE��
		normalized1 ָ���Ƿ����ݹ�һ���� [0,1] �� [-1,1] ��Χ�ڡ�
		stride ��������ָ�����������������Լ���ֽ��������Ϊ 0�����ʾ���������ǽ������еġ�
		pointer ָ�򻺳�����е�һ���������Եĵ�һ�������ĵ�ַ����offset�����ã�
		*/
		GLErrorCallBack(glVertexAttribPointer(i, element.count, element.type, element.nomorlize, layout.GetStride(), (const void*)offset));
		offset += element.count* VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind()const
{
	GLErrorCallBack(glBindVertexArray(vao));
}

void VertexArray::UnBind()const
{
	GLErrorCallBack(glBindVertexArray(0));

}
