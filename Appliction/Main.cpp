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
    glm::mat4 ptoj = glm::ortho(0.0f,960.0f,0.0f,540.0f, -1.0f, 1.0f); 
    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


    VertexArray va;// 首先创建顶点属性缓冲区
    VertexBuff vertex(bufferData, 4 * 4 * sizeof(float)); //在声明顶点部分属性数据
    VertexBufferLayout layout;
    layout.Push<float>(2);//添加数据排布规则
    layout.Push<float>(2);
    va.AddBuffer(vertex, layout); //想顶点属性缓冲区中按照规则去添加顶点部分属性数据


    IndexBuff index(buffIndex, 6);


    Shader shader("res/Base.shader");
    shader.Bind();

    Texture texture("res/Texture/mimi.png");
    texture.Bind();
    shader.SetUniform1i("u_Texture", 0); // 设置shader的纹理插槽信息


    va.UnBind();
    vertex.UnBind();
    index.UnBind();
    shader.UnBind();

    Rendener rendener;

    glm::vec3 translationA(200,200,0);
    glm::vec3 translationB(400, 200, 0);


    /* 当用户没有关闭窗口的时候执行循环里面的东西 */
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