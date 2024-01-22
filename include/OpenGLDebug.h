#pragma once
#include"GL\glew.h"
#include<iostream>

#define ASSERT(x)  if(!(x)) __debugbreak(); // 让程序自己打断点

#define GLErrorCallBack(x)  GLClearError();\
        x;\
        ASSERT(GLErrorMessage(#x,__FILE__,__LINE__))   //上面这些所有的__XXX__都是在编译器中内置的。直接调用即可 #加x会直接返回方法名

//应该在每个调用gl的方法上使用上面定义的 GLErrorCallBack宏，从而得到对应的debug信息
static void GLClearError()
{
    //循环调用    因为如果多个标志记录了错误，则返回并清除任意错误标志值。 所以需要循环调用，在每个需要检测的地方确保没有错误
    while (glGetError());
}

static bool GLErrorMessage(const char* funcName, const char* fileName, int line)
{
    //在C++中 复制返回左值，在此行中 返回的是unsigned int 
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR]:" << "FileName:" << fileName << "   " << " FunctionName: " << funcName << "   " << "Line: " << line << "Errorinfo:"<< error << std::endl;
        return false;
    }
    return true;
}