#pragma once
#include"GL\glew.h"
#include<iostream>

#define ASSERT(x)  if(!(x)) __debugbreak(); // �ó����Լ���ϵ�

#define GLErrorCallBack(x)  GLClearError();\
        x;\
        ASSERT(GLErrorMessage(#x,__FILE__,__LINE__))   //������Щ���е�__XXX__�����ڱ����������õġ�ֱ�ӵ��ü��� #��x��ֱ�ӷ��ط�����

//Ӧ����ÿ������gl�ķ�����ʹ�����涨��� GLErrorCallBack�꣬�Ӷ��õ���Ӧ��debug��Ϣ
static void GLClearError()
{
    //ѭ������    ��Ϊ��������־��¼�˴����򷵻ز������������־ֵ�� ������Ҫѭ�����ã���ÿ����Ҫ���ĵط�ȷ��û�д���
    while (glGetError());
}

static bool GLErrorMessage(const char* funcName, const char* fileName, int line)
{
    //��C++�� ���Ʒ�����ֵ���ڴ����� ���ص���unsigned int 
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR]:" << "FileName:" << fileName << "   " << " FunctionName: " << funcName << "   " << "Line: " << line << "Errorinfo:"<< error << std::endl;
        return false;
    }
    return true;
}