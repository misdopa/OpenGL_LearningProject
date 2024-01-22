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
    //ͨ��ʹ�� glAttachShader �ɹ�����ɫ�����󸽼ӵ��������ʹ�� glCompileShader �ɹ�������ɫ�����󣬲�ʹ�� glLinkProgram �ɹ����ӳ�����󣬿����ڳ�������д���һ��������ִ���ļ�
    //���������װΪ��ǰ����״̬��һ����
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
    GLErrorCallBack(int localtion = glGetUniformLocation(m_RendenID,name.c_str())); //���uniform��ʾ�ı���id,
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

    GLErrorCallBack(glAttachShader(program, vs)); //����ɫ�����󸽼ӵ��������
    GLErrorCallBack(glAttachShader(program, fs));
    GLErrorCallBack(glLinkProgram(program)); // ���ӳ������
    //�˺���ģ�� OpenGL ʵ���ڷ�����Ⱦ����ʱ����ִ�е���֤���������ɱ����ɫ���ǵ�ǰ״̬��һ���֡������������������κδ�������ͼ����Ⱦ����������ɴ���GL_INVALID_OPERATION
    GLErrorCallBack(glValidateProgram(program)); //�������а����Ŀ�ִ���ļ��Ƿ�����ڸ�����ǰ OpenGL ״̬�������ִ�С�

    GLErrorCallBack(glDeleteShader(vs));
    GLErrorCallBack(glDeleteShader(fs));

    return program;
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& Source)
{
    unsigned int id = glCreateShader(type);
    const char* src = Source.c_str();
    GLErrorCallBack(glShaderSource(id, 1, &src, nullptr)); // �����븴�Ƶ�ָ������ɫ��������
    GLErrorCallBack(glCompileShader(id));

    int result;
    GLErrorCallBack(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); //����ɫ�����󷵻�һ�����������������ɫ�������Ƿ�ɹ���

    //�����쳣�����쳣�����ӡ������̨��
    if (!result)
    {
        int length;
        GLErrorCallBack(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length); //��ջ�Ϸ����ڴ棬�����ڶ�̬�����ڴ� malloca���ڶ��Ϸ����ڴ棬���ߵ���������alloca�����������Զ�����
        GLErrorCallBack(glGetShaderInfoLog(id, length, &length, message)); //������ɫ���������Ϣ��־
        std::cout << "ShaderErro:" << (type == GL_VERTEX_SHADER ? "VertexShader" : "FragmentShader") << std::endl;
        std::cout << message << std::endl;
        return 0;
    }
    return id;
}

//�����Զ���shader�ļ�,ͨ�������ķ���ͷ����ȡ����shader��Ϣ
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
