#include"Renderer.h"
#include<iostream>

void Rendener::Draw(const VertexArray& va, const IndexBuff& ib, const Shader& shader)const 
{
    shader.Bind();
    va.Bind();
    ib.Bind();

    GLErrorCallBack(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));

}

void Rendener::Clear() const
{
    GLErrorCallBack(glClear(GL_COLOR_BUFFER_BIT));
}
