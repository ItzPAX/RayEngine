#pragma once

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

// all cameras we have
class Camera;
typedef std::shared_ptr<Camera> CameraPtr;

class FPSCamera;
typedef std::shared_ptr<FPSCamera> FPSCameraPtr;

class FloatingCamera;
typedef std::shared_ptr<FloatingCamera> FloatingCameraPtr;