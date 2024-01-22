#include "..\include\IndexBuffer.h"

IndexBuff::IndexBuff(const void* data, int count):m_Count(count)
{
    GLErrorCallBack(glGenBuffers(1, &m_RenderID)); // 创建索引缓冲区
    GLErrorCallBack(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID)); //绑定缓冲区
    GLErrorCallBack(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));//绑定数据，并规定绘制方法
}

IndexBuff::~IndexBuff()
{
    GLErrorCallBack(glDeleteBuffers(1,&m_RenderID));
}

void IndexBuff::Bind() const
{
    GLErrorCallBack(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID)); //规定渲染模式，并绑定缓冲区

}

void IndexBuff::UnBind() const
{
    GLErrorCallBack(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //规定渲染模式，并绑定缓冲区

}
