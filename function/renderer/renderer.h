#ifndef MX_RENDERER_H
#define MX_RENDERER_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <memory>
#include <functional>

namespace res {
struct Mesh;
struct Model;
struct BVH;
struct Effect;
}

struct TriangleData {
    std::shared_ptr<QOpenGLBuffer> vbo_v, vbo_n, vbo_u, ibo;

    /**
     * @brief Transform from Object to World
     */
    QMatrix4x4 o2w;

    int triangle_nums;

    explicit TriangleData(std::shared_ptr<res::Mesh> mesh);

    void bind(QOpenGLShaderProgram *sprog);

    ~TriangleData();
};

struct RenderData {
    std::vector<std::shared_ptr<TriangleData>> triangleDatas;

    explicit RenderData(std::shared_ptr<res::Model> model);
    explicit RenderData(std::shared_ptr<res::BVH> bvh);

    ~RenderData();

};

struct IShader : public QObject {
    std::shared_ptr<QOpenGLShaderProgram> sprog;
    std::string name;

    explicit IShader(const std::string &name);

    ~IShader();

    virtual void use(std::function<void(QOpenGLShaderProgram *)> func) = 0;
};

struct PhongShader : public IShader {
    explicit PhongShader(const std::string &name = "Phong");

    void use(std::function<void(QOpenGLShaderProgram*)> func) override;
};

class Renderer : public QObject, public QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit Renderer(QObject *parent = nullptr);

    ~Renderer();

    void render(QOpenGLContext *context, float dt);

    void setRenderData(std::shared_ptr<RenderData> renderData);
    void clearRenderData();

private:
    std::shared_ptr<RenderData> m_renderData;
    std::shared_ptr<PhongShader> m_shader;
    float m_rot=.0f;
};

#endif // MX_RENDERER_H
