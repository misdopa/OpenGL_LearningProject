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

        //����һ���������󣬶�������Ĳ����ʹ����ˣ���ߵ���С�߽�ֵ���ұߵ����߽�ֵ�������Ӧ�����߽�ֵ�������Ӧ����С�߽�ֵ��
        //���Ͷ���λ�����Ź������ٸ����ӣ�-0.5f,-0.5f ���������-1.0f��1.0f���ɵ����������ϵı���Ϊ-0.5f/-1.0f   -0.5f/-1.0f  Ҳ���ǵ������޵����м�λ�ã����������-2.0f 2.0f���ɵ�����������ԣ����ֵ�ͼ��ͻ��һЩ����Ϊ-0.5f-0.5f�����-2.0f 2.0f ���ɵ�λ���ڵ������޵����Ͻ�1/4�����������������ۿ�Ҳ��С��
        //��󶥵�����������ͶӰ����ȥ���㣬������ȫ��ӳ�䵽-1��1֮��Ļ��ͻ������������߽�Ϊ-1.0f 1.0f�ͻ�ͶӰ��1/2����0.5f/1.0f=1/2�������߽�Ϊ-2.0f,2.0f�ͻ�ͶӰ��1/4����0.5f/2.0f=1/4��
        //�����������ͶӰ������ԣ�0.0��Ϊ�������ĵ㡣��������Ϊ0,640,0,480 �������½ž�Ϊ0.0��
        //���Լ򵥵Ľ���Щ�������Ϊ�������������ҵı߽�ֵ���൱�ڹ���һ������ϵ��������λ��Ϊ�������ϵ�ϵĵ㣬�����������ĵ�û���������ϵ�У��򲻻���ʾ��
        ptoj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

        m_Shader = std::make_unique<Shader>("res/Base.shader");
        m_VAO =std::make_unique<VertexArray>();
        m_IndexBuffer= std::make_unique<IndexBuff>(buffIndex, 6);

        m_VertexBuffer= std::make_unique<VertexBuff>(bufferData, 4 * 4 * sizeof(float));
        VertexBufferLayout layout;
        layout.Push<float>(2);//��������Ų�����
        layout.Push<float>(2);
        m_VAO->AddBuffer(*m_VertexBuffer, layout); //�붥�����Ի������а��չ���ȥ��Ӷ��㲿����������


        m_Shader->Bind();

        m_Texture = std::make_unique<Texture>("res/Texture/mimi.png");
        m_Texture->Bind();
        m_Shader->SetUniform1i("u_Texture", 0); // ����shader����������Ϣ


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
        //����windows������ɫ
        GLErrorCallBack(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
        //ʹ����ɫ
        GLErrorCallBack(glClear(GL_COLOR_BUFFER_BIT));
	}
}
