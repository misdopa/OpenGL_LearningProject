#include "..\include\Texture.h"

// glTexParameteri接口讲解  https://blog.csdn.net/chenxiong668/article/details/8237300


Texture::Texture(const std::string& filePath):m_RenderID(0),m_FilePath(filePath), m_LocalBuffer(nullptr), m_Width(0),m_Height(0),m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);  //垂直旋转需要采样的图片，因为OpenGL的左边是以左下角为0,0点，而Windows是左上角为0，0点
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BPP, 4); //最后两个变量的意思为，像素位和使用几个颜色通道

	GLErrorCallBack(glGenTextures(1, &m_RenderID));
	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, m_RenderID));


	//当纹理图象被使用到一个小于它的形状分辨率时的设置
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

	//当纹理图象被使用到一个大于它的形状分辨率时的设置
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	//设置水平方向上的贴图模式，GL_CLAMP意思为将他限制到0.0到1.0之间
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));

	//设置数值方向上的贴图模式
	GLErrorCallBack(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	//绘制纹理的格式
	GLErrorCallBack(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, 0));

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer); //释放读取的资源
}

Texture::~Texture()
{
	GLErrorCallBack(glDeleteTextures(1,&m_RenderID));

}

void Texture::Bind(unsigned int slot) const
{
	//打开对应纹理插槽
	GLErrorCallBack(glActiveTexture(GL_TEXTURE0 + slot));

	//绑定打开的纹理插槽
	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, m_RenderID));
}

void Texture::UnBind() const
{
	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, 0));
}
