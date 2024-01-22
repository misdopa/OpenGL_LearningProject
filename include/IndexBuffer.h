#pragma once
#include"GL\glew.h"
#include"OpenGLDebug.h"



class  IndexBuff
{
public:
	IndexBuff(const void* data,int count);
	~IndexBuff();

	void Bind() const;

	void UnBind() const;

	inline unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_RenderID;
	unsigned int m_Count;

};

