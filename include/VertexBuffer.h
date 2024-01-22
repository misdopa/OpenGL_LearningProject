#pragma once
#include"GL\glew.h"
#include"OpenGLDebug.h"


class  VertexBuff
{
public:
	 VertexBuff(const void* data,int size);
	~ VertexBuff();

	void Bind() const;

	void UnBind() const;

private:
	unsigned int m_RenderID;

};

