#pragma once

enum class PRIMITIVE_TYPE
{
	PRIMITIVE_CUBE = 0,
	PRIMITIVE_PYRAMID,
	PRIMITIVE_SQUARE,
	PRIMITIVE_TRIANGLE
};

class VertexArrayObject;
typedef std::shared_ptr<VertexArrayObject> VertexArrayObjectPtr;

class ShaderProgram;
typedef std::shared_ptr<ShaderProgram> ShaderProgramPtr;

class UniformBuffer;
typedef std::shared_ptr<UniformBuffer> UniformBufferPtr;

class FrameBuffer;
typedef std::shared_ptr<FrameBuffer> FrameBufferPtr;

class Texture;
typedef std::shared_ptr<Texture> TexturePtr;