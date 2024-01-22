#include "..\include\VertexBuffer.h"

VertexBuff::VertexBuff(const void* data, int size)
{
    //��һ��������Ҫ���ɵĻ��������������ڶ�����Ҫ���������洢����������Ƶ�����
    GLErrorCallBack(glGenBuffers(1, &m_RenderID)); // �������㻺��������ν�Ļ�����ֵ�þ���һ��û�����õ��ڴ�
    GLErrorCallBack(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID)); //�涨��Ⱦģʽ�����󶨻�����
    GLErrorCallBack(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));//�����ݣ����涨���Ʒ���
}

VertexBuff::~VertexBuff()
{
    GLErrorCallBack(glDeleteBuffers(1,&m_RenderID));
}

void VertexBuff::Bind() const
{
    GLErrorCallBack(glBindBuffer(GL_ARRAY_BUFFER, m_RenderID)); //�涨��Ⱦģʽ�����󶨻�����

}

void VertexBuff::UnBind() const
{
    GLErrorCallBack(glBindBuffer(GL_ARRAY_BUFFER, 0)); //�涨��Ⱦģʽ�����󶨻�����

}
