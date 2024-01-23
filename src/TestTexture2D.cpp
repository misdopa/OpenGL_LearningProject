#include "test\TestClearColor.h"
#include"OpenGLDebug.h"
#include"Renderer.h"
#include"vendor\imgui\imgui.h"
#include "test\TestTexture2D.h"

namespace test {
	TestTextrue2D::TestTextrue2D():m_TranslationA(200.0f,200.0f,0.0f), m_TranslationB(400.0f, 200.0f, 0.0f)
    {
        float bufferData[] = {
    -50.0f,-50.0f,0.0f,0.0f,
    50.0f,-50.0f,1.0f,0.0f,
    50.0f,50.0f,1.0f,1.0f,
    -50.0f,50.0f,0.0f,1.0f,
        };

        unsigned int buffIndex[] = {
            0,1,2,
            2,3,0,
        };

        //生成一个正交矩阵，而这里面的参数就代表了，左边的最小边界值，右边的最大边界值，上面对应的最大边界值，下面对应的最小边界值。
        //他和顶点位置有着关联，举个例子：-0.5f,-0.5f 这个顶点在-1.0f，1.0f构成的正交矩阵上的比例为-0.5f/-1.0f   -0.5f/-1.0f  也就是第三象限的最中间位置，所以相对于-2.0f 2.0f构成的正交矩阵而言，呈现的图像就会大一些。因为-0.5f-0.5f相对于-2.0f 2.0f 构成的位置在第三象限的右上角1/4处，整体的面积用肉眼看也就小了
        //随后顶点根据这个正交投影矩阵去计算，将顶点全部映射到-1到1之间的话就会存在区别，如果边界为-1.0f 1.0f就会投影到1/2处（0.5f/1.0f=1/2），而边界为-2.0f,2.0f就会投影到1/4出（0.5f/2.0f=1/4）
        //对于这个正交投影矩阵而言，0.0点为他的中心点。若参数改为0,640,0,480 他的坐下脚就为0.0点
        //可以简单的将这些参数理解为，设置上下左右的边界值（相当于构建一个坐标系，而顶点位置为这个坐标系上的点，如果顶点坐标的点没在这个坐标系中，则不会显示）
        ptoj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        m_Shader = std::make_unique<Shader>("res/Base.shader");
        m_VAO =std::make_unique<VertexArray>();
        m_IndexBuffer= std::make_unique<IndexBuff>(buffIndex, 6);

        m_VertexBuffer= std::make_unique<VertexBuff>(bufferData, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);//添加数据排布规则
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout); //想顶点属性缓冲区中按照规则去添加顶点部分属性数据


        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/Texture/mimi.png");
        m_Texture->Bind();
        m_Shader->SetUniform1i("u_Texture", 0); // 设置shader的纹理插槽信息


        m_VAO->UnBind();
        m_VertexBuffer->UnBind();
        m_IndexBuffer->UnBind();
        m_Shader->UnBind();

        glm::vec3 translationA(200, 200, 0);
        glm::vec3 translationB(400, 200, 0);
	}

	TestTextrue2D::~TestTextrue2D()
	{
	}

	void TestTextrue2D::OnRendener()
	{

        Rendener rendener;

        m_Shader->Bind();

        {
            glm::mat4 modle = glm::translate(glm::mat4(1.0f), m_TranslationA);
            glm::mat4 mvp = ptoj * view * modle;

            m_Shader->SetUniformMat4v("u_MVP", mvp);

            rendener.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }

        {
            glm::mat4 modle = glm::translate(glm::mat4(1.0f), m_TranslationB);
            glm::mat4 mvp = ptoj * view * modle;

            m_Shader->SetUniformMat4v("u_MVP", mvp);

            rendener.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
        }
	}

	void TestTextrue2D::OnUpdate(float delatTime)
	{
	}

	void TestTextrue2D::OnImGuiRender()
	{
        {
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat3("TranlstaleAa", &m_TranslationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }


        {
            ImGui::SliderFloat3("TranlstaleB", &m_TranslationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        }
	}

	void TestTextrue2D::OnClear()
	{
        rendener.Clear();
        //设置windows背景颜色
        GLErrorCallBack(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        //使用颜色
        GLErrorCallBack(glClear(GL_COLOR_BUFFER_BIT));
	}
}
