#include "..\include\VertexBuffer.h"

VertexBuff::VertexBuff(const void* data, int size)
{
    //第一个参数是要生成的缓冲对象的数量，第二个是要输入用来存储缓冲对象名称的数组
    GLErrorCallBack(glGenBuffers(1, &m_RenderID)); // 创建顶点缓冲区，所谓的缓冲区值得就是一块没有人用的内存
    GLErrorCallBack(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID)); //规定渲染模式，并绑定缓冲区
    GLErrorCallBack(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//绑定数据，并规定绘制方法
}

VertexBuff::~VertexBuff()
{
    GLErrorCallBack(glDeleteBuffers(1,&m_RenderID));
}

void VertexBuff::Bind() const
{
    GLErrorCallBack(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID)); //规定渲染模式，并绑定缓冲区

}

void VertexBuff::UnBind() const
{
    GLErrorCallBack(glBindBuffer(GL_ARRAY_BUFFER, 0)); //规定渲染模式，并绑定缓冲区

}
