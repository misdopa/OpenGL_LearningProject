#include"GL\glew.h"
#include "GLFW\glfw3.h"
#include <iostream>
#include<fstream>
#include<string>  // getline在string里
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
    OpenGL方法文档地址：https://docs.gl
*/

int main(void)
{
    GLFWwindow* window;

    /* 初始化OpenGL链接库 */
    if (!glfwInit())
        return -1;

    //不进行核心设置的话，会出现如果VertexArray没有进行绑定。会黑屏，因为在常规配置中，会有默认的VAO。而我们的数据在我们自己创建的VAO中，所有调用的那个是没有数据的，就会黑屏
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* 使用OpenGL来创建一个窗口 */
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

    {glfwSwapInterval(1); // 设置刷新间隔  将渲染循环同步到监视器的刷新率
    std::cout << glGetString(GL_VERSION) << std::endl;

    /**
         * 混合:
         * 将输出颜色(判断着色器输出的颜色)和目标缓冲区已有的颜色结合
         * glEnable/glDisable(启用&关闭) => glBlendFunc(指定颜色因子) => glBlendEquation(指定混合模式)
         * glBlendEquation(mode) mode: src和dest的混合方式(默认GL_FUNC_ADD, 叠加)
         *
         **/
         /* 启用混合(默认不会启用) */
    GLErrorCallBack(glEnable(GL_BLEND));
    /**
     * glBlendFunc(src, dest) 指定颜色因子
     * src 指定输出颜色(RGBA)因子的计算方式, 默认为GL_ONE
     * dest 指定目标颜色因子的计算方式, 默认为GL_ZERO
     * GL_SRC_ALPHA 因为src的alpha为0, GL_ONE_MINUS_SRC_ALPHA 1-src.alpha
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

    /* 当用户没有关闭窗口的时候执行循环里面的东西 */
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
        //交换缓冲区。GLFW在默认情况下使用两个缓冲区。每个窗口有两个渲染缓冲区——前缓冲区和后缓冲区。
        //前缓冲区是正在显示的缓冲区，后缓冲区是即将显示的缓冲区。
        glfwSwapBuffers(window);

        /* Poll for and process events */
        //glfw处理事件有2个函数：glfwPollEvents()和glfwWaitEvents(),前者会立即处理已经到位的事件，后者等待。
        //当你制作游戏或是动画时，尽量使用轮询。如果相反，你需要在产生事件后才渲染，可是通过等待
        //来处理事件，即glfwWaitEvent，比如制作编辑器的时候，使用等待可以节省大量硬件资源。
        glfwPollEvents();
    }
    }
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();//清除退出
    return 0;

}