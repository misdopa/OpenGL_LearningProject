# 多看文档

API查看文档：https://docs.gl
GLFW文档：https://www.glfw.org/
GLEW文档：https://glew.sourceforge.net/

# 设置现代OpenGL模式

​     设置现代OpenGL模式来从0来进行绘制，首先我们需要最原始的文件GLFW库，并进行链接（链接的过程在C++学习项目中）,随后按照官方文档中的示例说明来进行环境设置，包括库的初始化，以及GLFW的上下文初始化。随后导入GLEW库，并按照实例进行初始化

# 关于DEBUG

​	OpenGL中DEbug因为制作者在设计的时候，可能处理了一些异常状况，所以会导致我们在使用OpenGL进行开发的时候比较难知道是哪一行以及哪一个文件报错，因此对此功能我们需要进行一个封装来方便我们进行debug。

```c++
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

static bool GLErrorMessage(const char* funcName,const char* fileName,int line)
{
    //在C++中 复制返回左值，在此行中 返回的是unsigned int 
    while (GLenum error = glGetError())
    {
        std::cout << "[OPENGL ERROR]:"<< "FileName:" << fileName << "   " << " FunctionName: " << funcName << "   " << "Line: " << line << std::endl;
        return false;
    }
    return true;
}

//在此调用和OpenGL的方法时 给每个方法使用GLErrorCallBack即可
GLErrorCallBack(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
```



# 顶点缓冲区

​		所谓的顶点缓冲区就是一块内存空间，只不过这个内存空间的大小时未知的，OpenGL的工作流如下：我们献给显卡提供我们要绘制的所有数据，并且将数据存储到GPU的内存中去，随后使用shader（GPU上执行的一种程序），来读取数据并显示到屏幕上。当我们绘制图形的时候，我们使用的就是顶点缓冲区中的数据，而顶点缓冲区实际上就是GPU上的一块内存，当shader读取顶点缓存的时候，他需要知道缓冲区的布局，即缓冲区中存放的是什么。例如，每个顶点坐标的浮点数，纹理，法线等。我们需要具体的告诉OpenGL缓存中有什么以及如何布局的

```c++
    float bufferData[6]{
        -0.5f,-0.5f,
        0,0.5f,
        0.5f,-0.5f,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // 创建顶点缓冲区，所谓的缓冲区值得就是一块没有人用的内存
    glBindBuffer(GL_STATIC_DRAW, buffer); //规定渲染模式，并绑定缓冲区
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), bufferData, GL_STATIC_DRAW);//绑定数据，并规定绘制方法

glDrawArrays(GL_STATIC_DRAW, 0, 3);//进行绘制
```

# 索引缓冲区

​	索引缓冲区，其实就是对于顶点缓冲区的一个优化方法。假设我们需要绘制一个正方形，在电脑中正方形会被分为两个三角形，那么如果使用顶点缓冲区去绘制正方形，我们需要6个顶点来绘制2个三角形。但是对于正方形而言，其中有2个顶点是重复的，这就造成了数据的冗余。若应用到游戏中，则会占用相当大的内存。所以索引缓冲区就出现了，解决了这一问题。
​	索引缓冲区的工作原理很简单，就是声明一个包含各个顶点的索引，以确保GPU可以找到绑定后的顶点信息得到顶点位置，并根据这些数据来进行绘制。因此，大体的声明和绑定相较于顶点缓冲区来说大差不差。

```C++

    float bufferData[]={
        -0.5f,-0.5f,
        0.5f,-0.5f,
        0.5f,0.5f,
        -0.5f,0.5f,
    };

    unsigned int buffIndex[] = {
        0,1,2,
        2,3,0,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // 创建顶点缓冲区，所谓的缓冲区值得就是一块没有人用的内存
    glBindBuffer(GL_ARRAY_BUFFER, buffer); //规定渲染模式，并绑定缓冲区
    glBufferData(GL_ARRAY_BUFFER, 6 *2* sizeof(float), bufferData, GL_STATIC_DRAW);//绑定数据，并规定绘制方法
    glEnableVertexAttribArray(0);// 启用或禁用通用顶点属性数组,后面的参数应该是从哪里开始，按这个例子来说如果从第二个元素开始index就是1
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);//上面两行代码就是告诉opengl我们的缓存布局是怎么样的


    unsigned int ibo;
    glGenBuffers(1, &ibo); // 创建索引缓冲区
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo); //绑定缓冲区
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), buffIndex, GL_STATIC_DRAW);//绑定数据，并规定绘制方法

glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); // 进行绘制，90%的情况下使用他
```



# 顶点

   所谓的顶点，也就是一系列数据的结合体，其实更像是一个结构体。他里面包含了绘制在屏幕上的位置，材质，法线等数据。通过这些数据来存储在顶点缓冲区，随后读取数据进行绘制图像。而如何布局，内存里面有什么是我们通过C++去告诉GPU的，而这也是OpenGL的作用

```c++
    float bufferData[6]{
        -0.5f,-0.5f,
        0,0.5f,
        0.5f,-0.5f,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer); // 创建顶点缓冲区，所谓的缓冲区值得就是一块没有人用的内存
    glBindBuffer(GL_STATIC_DRAW, buffer); //规定渲染模式，并绑定缓冲区
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), bufferData, GL_STATIC_DRAW);//绑定数据，并规定绘制方法

/*  设置顶点位置并将顶点显示出来的操作  */
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,sizeof(float)*2,0);//上面两行代码就是告诉opengl我们的缓存布局是怎么样的

glDrawArrays(GL_STATIC_DRAW, 0, 3);//进行绘制
```

# 顶点数据缓冲区

​		顶点数据缓冲区，可以看作时一组顶点信息的集合，里面包括了我们指定的所有的关于顶点的信息（如：顶点位置，顶点颜色，法线等）。当我们创建了一个顶数据缓冲区的时候，可以向里面添加相关的信息。当相关信息全部添加完毕的时候，将起关闭。这样就封装成了一个固定的渲染信息，如果我们需要就可以重新拿到顶点数据缓冲区的ID启动使用或者更改。大体代码如下：

设置：

```c++
//创建VAO
GLuint VAO;
glGenVertexArrays(1, &VAO);
//设置当前VAO，之后所有操作(注意：这些操作必须是上文VAO中包含的内容所注明的调用，其他非VAO中存储的内容即使调用了也不会影响VAO）存储在该VAO中
glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO); //设置了VBO
   glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);//设置VBO中的数据
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0); //设置顶点属性（索引为0的属性，与shader中的内容有交互）
    glEnableVertexAttribArray(0); //设置开启顶点属性（索引为0的属性,与shader中的内容有交互）
glBindVertexArray(0); //解绑VAO（解绑主要是为了不影响后续VAO的设置，有点类似于C++中指针delete后置空，是个好习惯
```

再次使用：

```c++
glUseProgram(shaderProgram);
glBindVertexArray(VAO); //绑定我们需要的VAO，会导致上面所有VAO保存的设置自动设置完成
someOpenGLFunctionThatDrawsOurTriangle();   
glBindVertexArray(0);   //解绑VAO
```

附详细理解资料：[OpenGL缓冲区对象之VAO_glgenvertexarrays-CSDN博客](https://blog.csdn.net/zxzzt119/article/details/53501883)

# 着色器（Shader）

   着色器实际上就是只是一个运行在gpu上的程序而已，CPU只负责给GPU发送数据，但是他不会告诉GPU如何去处理这些数据。这个时候就需要着色器告诉GPU应该怎么去做，以什么样的规则去处理这些数据，随后CPU发起DrawCall，在发起DrawCall的时候，顶点着色器会得到调用，片面着色器会获取颜色，随后我们会在屏幕上看到结果。常用的着色器分为：顶点着色器和片面着色器

顶点着色器：每个顶点都会使得顶点着色器运行一次，根据现在学习的三角形示例而言，他有三个顶点他就会运行三次。

片面着色器：在片面着色器的阶段会给各个像素去光栅化，也就是说在这个阶段片面着色器会把图像绘制到屏幕上。为我们像素决定颜色和输出颜色。在这个阶段每个像素都会调用一次片面着色器

PS：顶点着色器和片面着色器的关系其实有一点像，顶点着色器来绘制框架，告诉电脑我这些顶点在哪里，最终会是什么材质的。而片面着色器则会在这些框架内根据编写的规则去在框架里面去涂涂画画。
       Shader的本质其实是字符串。。。。。。将这些字符串通过OpenGL来进行编译解析随后给到GPU 



# Uniform关键字

​		Uniform关键字的作用在于，如果再shader中声明了这个关键字，则可以通过代码去控制shader中的这个变量。（unity中shader声明变量也应该是这个原理？通过内部数据反射到编辑器中？）。如果在shader中使用Uniform声明变量，但是如果没在shader代码中使用这个变量，则在编译阶段会直接剔除。也就是说在C++代码中会找不到这个变量

Shader代码：

```
#Shader FRAGMENT
#version 330 core

layout(location=0) out vec4 color;
uniform vec4 u_Color;

void main()
{
   color = u_Color;
}
```

C++代码控制：

```c++
int localtion = glGetUniformLocation(shader, "u_Color"); //获得uniform表示的变量id,
glUniform4f(localtion,r,0.3f,0.8f,1.0f);
//当localtion为-1 的时候说明没找到这个变量
```

# 纹理

​	所谓的纹理可以理解为在一些图片或者美术素材上面的像素采样信息，只要可以进行采样的美术素材都可以称之为纹理。最简单的纹理就是.png图片。如果想使用OpenGL在屏幕上绘画出对应的图片，则需要先把这个素材绑定到对应的顶点信息的纹理缓冲区中,随后通过接入的第三方库，对每个像素信息进行缓存并绑定在对应的纹理缓冲区中。这时顶点信息中就存放了对应的纹理数据以供OpenGL使用。随后对他扩大 缩小，水平，竖直啥办法的采样过滤进行限制即可。

```c++
//需要导入stb_image,导入地址如下
//https://github.com/nothings/stb/blob/master/stb_image.h

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


//绑定对应的纹理插槽如下
//打开对应纹理插槽
	GLErrorCallBack(glActiveTexture(GL_TEXTURE0 + slot));

	//绑定打开的纹理插槽
	GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, m_RenderID));

//解绑插槽
GLErrorCallBack(glBindTexture(GL_TEXTURE_2D, 0));
```

# 混合

​	混合可以从字面意思上理解，就是简单的将两个颜色融合到一起形成一个新的颜色。对于在计算机来说，他就是将从四片面着色器（fragmente shader）中输出的颜色中的数据和目标缓冲区中的数据进行一定规律的替换或者计算，从而得出正确的可以在屏幕上显示的颜色。而这一部分是OpenGL允许我们自己启动禁用，设置输出颜色的计算方式以及自定义混合模式的。所以，如果我们需要使用混合这一功能，则我们需要自行去设置他，OpenGL默认是关闭的。只是单纯的做颜色的替换

```c++
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
```

# 投影矩阵

   投影矩阵简单来说，就是将3D世界中的顶点变成屏幕上显示的顶点位置的一个操作矩阵。而经过投影矩阵的顶点最终他的x,y值坐标都会被压缩在-1到1之间。

## 正交投影矩阵

​	正交投影矩阵是不会考虑z轴上的大小的，也就是说，他跟人眼所识别的近大远小不一样。他只会将3D世界上的顶点位置显示到屏幕上，不会进行特殊处理，远处的物体不会变小。通常使用在UI和一些不考虑景深的2D游戏中

## 透视投影矩阵

​	透视投影矩阵，相对于正交投影矩阵而言会比较符合人眼识别的规则。即近大远小，会对z轴比较大的物体的顶点进行特殊的处理。使得其看上去和人眼所得的信息相同。常规的3D游戏会使用此投影矩阵

对投影矩阵而言，无论是正交投影矩阵还是透视投影矩阵，他们最终都会把顶点显示到屏幕上，且将其x,y压缩到-1到1之间

```c++
//使用这个接口需要导入OpenGL数据库 URL:https://github.com/g-truc/glm/releases



//生成一个正交矩阵，而这里面的参数就代表了，左边的最小边界值，右边的最大边界值，上面对应的最大边界值，下面对应的最小边界值。
    //他和顶点位置有着关联，举个例子：-0.5f,-0.5f 这个顶点在-1.0f，1.0f构成的正交矩阵上的比例为-0.5f/-1.0f   -0.5f/-1.0f  也就是第三象限的最中间位置，所以相对于-2.0f 2.0f构成的正交矩阵而言，呈现的图像就会大一些。因为-0.5f-0.5f相对于-2.0f 2.0f 构成的位置在第三象限的右上角1/4处，整体的面积用肉眼看也就小了
    //随后顶点根据这个正交投影矩阵去计算，将顶点全部映射到-1到1之间的话就会存在区别，如果边界为-1.0f 1.0f就会投影到1/2处（0.5f/1.0f=1/2），而边界为-2.0f,2.0f就会投影到1/4出（0.5f/2.0f=1/4）
    //对于这个正交投影矩阵而言，0.0点为他的中心点。若参数改为0,640,0,480 他的坐下脚就为0.0点
    //可以简单的将这些参数理解为，设置上下左右的边界值（相当于构建一个坐标系，而顶点位置为这个坐标系上的点，如果顶点坐标的点没在这个坐标系中，则不会显示）
    glm::mat4 ptoj = glm::ortho(-1.0f, 1.0f, -0.75f, 0.75f, -1.0f, 1.0f); 
```

# MVP（Modle,View,project）模型矩阵，视图矩阵，投影矩阵

## 	模型矩阵

​		其实所谓的模型矩阵就是使得同一空间下的物体顶点遵照统一的坐标系。也就是将他们的坐标转化在同一坐标系上，方便建模人员确定位置，旋转，大小。而这个转化的矩阵就成为模型矩阵

## 	视图矩阵

​		将这些模型顶点统一在一个坐标系之后，就要规定他们在眼睛（相机）里面看到的位置，因为观察的位置，角度不同，呈现的就不同（生活中观察从各个方向观察物体看到的东西不一样）。而视图矩阵就是干这个事情的。通过视图矩阵，将世界中的模型根据所观察的位置，方向来重新进行屏幕位置上的调整，最后得出一个观察所得位置。。。。。。。PS：在计算机中没有相机这个概念，所谓的相机运动，其实是操作模型的顶点按照相机的逆运动所得出来的一个新的顶点坐标位置。举个例子：如果将相机向左移动，那么对应的模型顶点需要按照之前算出来的位置向右移动就可以得出相同的结果。也就是说，我们的做法就是，将之前的物体在视图矩阵上的值按照向右偏移和相机向左移动的相同值，就可以达到相同的结果。

## 投影矩阵

​		最后经过投影矩阵的操作，将视图坐标系中的顶点转化到屏幕上

# 批量渲染

​		批量渲染很简单，可以从字面理解就是渲染多个对象。但是如果渲染相同的对象，他们的各项数据（顶点颜色，顶点位置等）都是相同的。那如果重新建立缓冲区把这些数据填充进去，就会产生数据冗余问题。所以，还有个简单的想法就是公用同一个顶点数据，随后通过shader使用MVP来对各个顶点进行改变。然后进行渲染即可。