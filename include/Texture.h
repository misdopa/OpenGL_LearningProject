#pragma once
#include"vendor\stb_image\stb_image.h"
#include"OpenGLDebug.h"
#include<string>

class Texture
{
private:
	unsigned int m_RenderID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer; // 储存从图片张提取的数据
	int m_Width, m_Height, m_BPP; // BPP为像素信息

public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot=0) const;
	void UnBind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }
};