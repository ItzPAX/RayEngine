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

class PrimitiveRenderData;
typedef std::shared_ptr<PrimitiveRenderData> PrimitivePtr;

class PointLight;
typedef std::shared_ptr<PointLight> PointLightPtr;

class Model;
typedef std::shared_ptr<Model> ModelPtr;

class LightingManager;
typedef std::shared_ptr<LightingManager> LightingManagerPtr;

class ModelContainer;
typedef std::shared_ptr<ModelContainer> ModelContainerPtr;

class PrimitiveContainer;
typedef std::shared_ptr<PrimitiveContainer> PrimitiveContainerPtr;

class Scene;
typedef std::shared_ptr<Scene> ScenePtr;