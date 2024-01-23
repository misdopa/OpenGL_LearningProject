#include"GL\glew.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include<fstream>
#include<string>  // getline��string��
#include<sstream>

#include"Renderer.h"
#include"OpenGLDebug.h"
#include"IndexBuffer.h"
#include"VertexBuffer.h"
#include"VertexArray.h"
#include"Shader.h"
#include"Texture.h"

#include"vendor\glm\glm.hpp"
#include"vendor\glm\gtc\matrix_transform.hpp"
#include "vendor\imgui\imgui.h"
#include "vendor\imgui\imgui_impl_glfw_gl3.h"
#include"test\TestClearColor.h"
#include"test\TestTexture2D.h"

/*
    OpenGL�����ĵ���ַ��https://docs.gl
*/

int main(void)
{
    GLFWwindow* window;

    /* ��ʼ��OpenGL���ӿ� */
    if (!glfwInit())
        return -1;

    //�����к������õĻ�����������VertexArrayû�н��а󶨡����������Ϊ�ڳ��������У�����Ĭ�ϵ�VAO�������ǵ������������Լ�������VAO�У����е��õ��Ǹ���û�����ݵģ��ͻ����
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* ʹ��OpenGL������һ������ */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    ImGui::CreateContext();
    ImGui_ImplGlfwGL3_Init(window, true);

    ImGui::StyleColorsDark();

    {glfwSwapInterval(1); // ����ˢ�¼��  ����Ⱦѭ��ͬ������������ˢ����
    std::cout << glGetString(GL_VERSION) << std::endl;

    /**
         * ���:
         * �������ɫ(�ж���ɫ���������ɫ)��Ŀ�껺�������е���ɫ���
         * glEnable/glDisable(����&�ر�) => glBlendFunc(ָ����ɫ����) => glBlendEquation(ָ�����ģʽ)
         * glBlendEquation(mode) mode: src��dest�Ļ�Ϸ�ʽ(Ĭ��GL_FUNC_ADD, ����)
         *
         **/
         /* ���û��(Ĭ�ϲ�������) */
    GLErrorCallBack(glEnable(GL_BLEND));
    /**
     * glBlendFunc(src, dest) ָ����ɫ����
     * src ָ�������ɫ(RGBA)���ӵļ��㷽ʽ, Ĭ��ΪGL_ONE
     * dest ָ��Ŀ����ɫ���ӵļ��㷽ʽ, Ĭ��ΪGL_ZERO
     * GL_SRC_ALPHA ��Ϊsrc��alphaΪ0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
     * RGBA = Srgba * GL_SRC_ALPHA + Drgba * GL_ONE_MINUS_SRC_ALPHA
     **/
    GLErrorCallBack(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


    if (glewInit() != GLEW_OK)
        std::cout << "Erro" << std::endl;

    test::test* current=nullptr;
    test::TestMenu* TestMune = new test::TestMenu(current);

    current = TestMune;

    TestMune->RegisterTest<test::TestClearColor>("Clear Color");
    TestMune->RegisterTest<test::TestTextrue2D>("Texture");

    Rendener rendener;

    /* ���û�û�йرմ��ڵ�ʱ��ִ��ѭ������Ķ��� */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        rendener.Clear();

        //TestColor.OnRendener();

        ImGui_ImplGlfwGL3_NewFrame();

        //TestColor.OnImGuiRender();
        if (current)
        {
            current->OnUpdate(0.0f);
            current->OnRendener();
            ImGui::Begin("Test");
            if (current != TestMune && ImGui::Button("<-"))
            {
                current->OnClear();
                delete current;
                current = TestMune;

            }
            current->OnImGuiRender();
            ImGui::End();
        }



        ImGui::Render();
        ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

        /* Swap front and back buffers */
        //������������GLFW��Ĭ�������ʹ��������������ÿ��������������Ⱦ����������ǰ�������ͺ󻺳�����
        //ǰ��������������ʾ�Ļ��������󻺳����Ǽ�����ʾ�Ļ�������
        glfwSwapBuffers(window);

        /* Poll for and process events */
        //glfw�����¼���2��������glfwPollEvents()��glfwWaitEvents(),ǰ�߻����������Ѿ���λ���¼������ߵȴ���
        //����������Ϸ���Ƕ���ʱ������ʹ����ѯ������෴������Ҫ�ڲ����¼������Ⱦ������ͨ���ȴ�
        //�������¼�����glfwWaitEvent�����������༭����ʱ��ʹ�õȴ����Խ�ʡ����Ӳ����Դ��
        glfwPollEvents();
    }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();//����˳�
    return 0;

}