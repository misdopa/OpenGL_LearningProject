#pragma once
#include"GL\glew.h"

#include"VertexArray.h"
#include"IndexBuffer.h"
#include"Shader.h"
#include"OpenGLDebug.h"


class  Rendener
{
public:

	void Draw(const VertexArray& va, const IndexBuff& ib, const Shader& shader)const;
	void Clear() const;

};
