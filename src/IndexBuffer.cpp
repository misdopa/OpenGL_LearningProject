#include "..\include\IndexBuffer.h"

IndexBuff::IndexBuff(const void* data, int count):m_Count(count)
{
    GLErrorCallBack(glGenBuffers(1, &m_RenderID)); // ��������������
    GLErrorCallBack(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID)); //�󶨻�����
    GLErrorCallBack(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));//�����ݣ����涨���Ʒ���
}

IndexBuff::~IndexBuff()
{
    GLErrorCallBack(glDeleteBuffers(1,&m_RenderID));
}

void IndexBuff::Bind() const
{
    GLErrorCallBack(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID)); //�涨��Ⱦģʽ�����󶨻�����

}

void IndexBuff::UnBind() const
{
    GLErrorCallBack(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0)); //�涨��Ⱦģʽ�����󶨻�����

}
