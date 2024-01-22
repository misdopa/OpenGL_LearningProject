#include "..\include\Texture.h"

// glTexParameteri�ӿڽ���  https://blog.csdn.net/chenxiong668/article/details/8237300


Texture::Texture(const std::string& filePath):m_RenderID(0),m_FilePath(filePath), m_LocalBuffer(nullptr), m_Width(0),m_Height(0),m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);  //��ֱ��ת��Ҫ������ͼƬ����ΪOpenGL������������½�Ϊ0,0�㣬��Windows�����Ͻ�Ϊ0��0��
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4); //���������������˼Ϊ������λ��ʹ�ü�����ɫͨ��

	GLErrorCallBack(glGenTextures(1, &m_RenderID));
	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, m_RenderID));


	//������ͼ��ʹ�õ�һ��С��������״�ֱ���ʱ������
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	//������ͼ��ʹ�õ�һ������������״�ֱ���ʱ������
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//����ˮƽ�����ϵ���ͼģʽ��GL_CLAMP��˼Ϊ�������Ƶ�0.0��1.0֮��
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

	//������ֵ�����ϵ���ͼģʽ
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//��������ĸ�ʽ
	GLErrorCallBack(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer); //�ͷŶ�ȡ����Դ
}

Texture::~Texture()
{
	GLErrorCallBack(glDeleteTextures(1,&m_RenderID));

}

void Texture::Bind(unsigned int slot) const
{
	//�򿪶�Ӧ������
	GLErrorCallBack(glActiveTexture(GL_TEXTURE0 + slot));

	//�󶨴򿪵�������
	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, m_RenderID));
}

void Texture::UnBind() const
{
	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, 0));
}
