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
	//需要进行绑定，如果不进行绑定就绑定其他的信息缓冲区，就会报错。且导致黑屏
	Bind();
	Buffer.Bind();

	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (int i=0;i< elements.size();i++)
	{
		const auto& element = elements[i];

		GLErrorCallBack(glEnableVertexAttribArray(i));// 启用通用顶点属性数组,index 指定通用顶点属性需要 启用 或 禁用的索引。
		/*
		顶点绘制数据说明
		index 指定要配置的顶点属性的编号。
		size 指定每个顶点属性的分量数（1、2、3 或 4，就像向量的维度一样）。
		type 指定每个分量的数据类型，可以是 GL_BYTE、GL_UNSIGNED_BYTE、GL_SHORT、GL_UNSIGNED_SHORT、GL_INT、GL_UNSIGNED_INT、GL_FLOAT 或 GL_DOUBLE。
		normalized1 指定是否将数据归一化到 [0,1] 或 [-1,1] 范围内。
		stride （步长）指定连续两个顶点属性间的字节数。如果为 0，则表示顶点属性是紧密排列的。
		pointer 指向缓冲对象中第一个顶点属性的第一个分量的地址。（offset的作用）
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
