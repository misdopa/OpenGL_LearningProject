#include "..\include\Shader.h"

Shader::Shader(const std::string& filePath):m_FilePath(filePath),m_RendenID(0)
{
    ShaderSource source= ParseShader(filePath);
    m_RendenID= CreatShade(source.VertexShader, source.FragmentShader);
    GLErrorCallBack(glUseProgram(m_RendenID));
}

Shader::~Shader()
{
    GLErrorCallBack(glDeleteProgram(m_RendenID));
}

void Shader::Bind() const
{
    //通过使用 glAttachShader 成功将着色器对象附加到程序对象，使用 glCompileShader 成功编译着色器对象，并使用 glLinkProgram 成功链接程序对象，可以在程序对象中创建一个或多个可执行文件
    //将程序对象安装为当前呈现状态的一部分
    GLErrorCallBack(glUseProgram(m_RendenID));
}

void Shader::UnBind() const
{
    GLErrorCallBack(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
    int localtion = GetUniformLocation(name);
    if (localtion != -1)
        GLErrorCallBack(glUniform4f(localtion, v0, v1, v2, v3));
}

void Shader::SetUniformMat4v(const std::string& name, const glm::mat4& mat)
{
    int localtion = GetUniformLocation(name);
    if (localtion != -1)
        GLErrorCallBack(glUniformMatrix4fv(localtion,1,GL_FALSE,&mat[0][0]));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
    int localtion = GetUniformLocation(name);
    if (localtion != -1)
        GLErrorCallBack(glUniform1f(localtion, value));
}

void Shader::SetUniform1i(const std::string& name, float value)
{
    int localtion = GetUniformLocation(name);
    if (localtion != -1)
        GLErrorCallBack(glUniform1i(localtion, value));
}

int Shader::GetUniformLocation(const std::string& name)
{
    if (m_UniformID.find(name) != m_UniformID.end())
        return m_UniformID[name];
    GLErrorCallBack(int localtion = glGetUniformLocation(m_RendenID,name.c_str())); //获得uniform表示的变量id,
    if (localtion == -1)
        std::cout << "[ERRO]: The Uniform Cant be find" <<"  name: " << name << std::endl;
    m_UniformID[name] = localtion;
    return localtion;
}

unsigned int Shader::CreatShade(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GLErrorCallBack(glAttachShader(program, vs)); //将着色器对象附加到程序对象
    GLErrorCallBack(glAttachShader(program, fs));
    GLErrorCallBack(glLinkProgram(program)); // 链接程序对象
    //此函数模拟 OpenGL 实现在发出渲染命令时必须执行的验证操作，而可编程着色器是当前状态的一部分。如果出现以下情况，任何触发几何图形渲染的命令都会生成错误：GL_INVALID_OPERATION
    GLErrorCallBack(glValidateProgram(program)); //检查程序中包含的可执行文件是否可以在给定当前 OpenGL 状态的情况下执行。

    GLErrorCallBack(glDeleteShader(vs));
    GLErrorCallBack(glDeleteShader(fs));

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& Source)
{
    unsigned int id = glCreateShader(type);
    const char* src = Source.c_str();
    GLErrorCallBack(glShaderSource(id, 1, &src, nullptr)); // 将代码复制到指定的着色器对象中
    GLErrorCallBack(glCompileShader(id));

    int result;
    GLErrorCallBack(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); //从着色器对象返回一个参数（用来检测着色器编译是否成功）

    //处理异常，将异常情况打印到控制台上
    if (!result)
    {
        int length;
        GLErrorCallBack(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length); //在栈上分配内存，适用于动态分配内存 malloca是在对上分配内存，两者的区别在于alloca除了作用域自动销毁
        GLErrorCallBack(glGetShaderInfoLog(id, length, &length, message)); //返回着色器对象的信息日志
        std::cout << "ShaderErro:" << (type == GL_VERTEX_SHADER ? "VertexShader" : "FragmentShader") << std::endl;
        std::cout << message << std::endl;
        return 0;
    }
    return id;
}

//解析自定义shader文件,通过所给的方法头来提取各个shader信息
ShaderSource Shader::ParseShader(const std::string& shaderPath)
{
    enum class ShaderType
    {
        None = -1,
        VERTEX = 0,
        FRAGMENT = 1,
    };


    std::ifstream stream(shaderPath);

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::None;
    while (getline(stream, line))
    {
        if (line.find("#Shader") != std::string::npos)
        {
            if (line.find("VERTEX") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("FRAGMENT") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else
        {
            if (type != ShaderType::None)
                ss[(int)type] << line << "\n";
        }
    }

    return { ss[0].str(),ss[1].str() };

}
