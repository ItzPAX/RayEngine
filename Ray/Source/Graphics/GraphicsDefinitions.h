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

// primitives
class Square;
typedef std::shared_ptr<Square> SquarePtr;

class Triangle;
typedef std::shared_ptr<Triangle> TrianglePtr;

class Pyramid;
typedef std::shared_ptr<Pyramid> PyramidPtr;

class Cube;
typedef std::shared_ptr<Cube> CubePtr;

// all cameras we have
class Camera;
typedef std::shared_ptr<Camera> CameraPtr;

class FPSCamera;
typedef std::shared_ptr<FPSCamera> FPSCameraPtr;

class FloatingCamera;
typedef std::shared_ptr<FloatingCamera> FloatingCameraPtr;