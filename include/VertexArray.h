#pragma once
#include"VertexBuffer.h"
#include"VertexBufferLayout.h"

class  VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void AddBuffer(const VertexBuff& Buffer, const VertexBufferLayout& layout);
	void Bind()const;
	void UnBind()const;

private:
	unsigned int vao;
};
