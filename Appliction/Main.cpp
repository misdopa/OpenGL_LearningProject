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
    glm::mat4 ptoj = glm::ortho(0.0f,960.0f,0.0f,540.0f, -1.0f, 1.0f); 
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


    VertexArray va;// ���ȴ����������Ի�����
    VertexBuff vertex(bufferData, 4 * 4 * sizeof(float)); //���������㲿����������
    VertexBufferLayout layout;
    layout.Push<float>(2);//��������Ų�����
    layout.Push<float>(2);
    va.AddBuffer(vertex, layout); //�붥�����Ի������а��չ���ȥ��Ӷ��㲿����������


    IndexBuff index(buffIndex, 6);


    Shader shader("res/Base.shader");
    shader.Bind();

    Texture texture("res/Texture/mimi.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0); // ����shader����������Ϣ


    va.UnBind();
    vertex.UnBind();
    index.UnBind();
    shader.UnBind();

    Rendener rendener;

    glm::vec3 translationA(200,200,0);
    glm::vec3 translationB(400, 200, 0);


    /* ���û�û�йرմ��ڵ�ʱ��ִ��ѭ������Ķ��� */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        rendener.Clear();

        ImGui_ImplGlfwGL3_NewFrame();


        shader.Bind();


        {
            ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
            ImGui::SliderFloat3("TranlstaleAa", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            glm::mat4 modle = glm::translate(glm::mat4(1.0f), translationA);
            glm::mat4 mvp = ptoj * view * modle;

            shader.SetUniformMat4v("u_MVP", mvp);

            rendener.Draw(va, index, shader);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        }


        {
            ImGui::SliderFloat3("TranlstaleB", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
            glm::mat4 modle = glm::translate(glm::mat4(1.0f), translationB);
            glm::mat4 mvp = ptoj * view * modle;

            shader.SetUniformMat4v("u_MVP", mvp);

            rendener.Draw(va, index, shader);
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