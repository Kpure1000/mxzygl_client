#ifndef RENDERDATA_H
#define RENDERDATA_H

#include <QObject>
#include <QOpenGLFunctions>
#include <QOpenGLContext>
#include <QVector3D>
#include <QMatrix4x4>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <memory>
#include <functional>

#include "transform.h"
#include "camera.h"

namespace res {
struct Mesh;
struct Model;
struct BVH;
struct Effect;
}

struct TriangleData {
    std::shared_ptr<QOpenGLBuffer> vbo_v = nullptr, vbo_n = nullptr, vbo_u = nullptr, ibo = nullptr;

    int triangle_nums;

    explicit TriangleData(std::shared_ptr<res::Mesh> mesh);

    void bind(QOpenGLShaderProgram *sprog);

    ~TriangleData();
};

struct RenderData {
    std::vector<std::shared_ptr<TriangleData>> triangleDatas;
    Transform trans_ca;
    Transform trans_model;

    std::shared_ptr<PerspectiveCamera> camera;

    explicit RenderData(std::shared_ptr<res::Model> model, int sw, int sh);
    explicit RenderData(std::shared_ptr<res::BVH> bvh, int sw, int sh);

    ~RenderData();

};

struct SkyData {
    std::shared_ptr<TriangleData> triangleData;
    QVector3D sky, ground;
    explicit SkyData(const QVector3D &sky, const QVector3D &ground);
};

struct IShader : public QObject {
    Q_OBJECT
public:
    std::shared_ptr<QOpenGLShaderProgram> sprog;
    std::string name;

    explicit IShader(const std::string &name, QObject *parent = nullptr);

    ~IShader();

//    virtual void use(std::function<void(QOpenGLShaderProgram *)> func) = 0;
};

struct PhongShader : public IShader {
    Q_OBJECT
public:
    explicit PhongShader(const std::string &name, QObject *parent = nullptr);
    ~PhongShader();

//    void use(std::function<void(QOpenGLShaderProgram*)> func) override;
};

struct SkyShader : public IShader {
    Q_OBJECT
public:
    explicit SkyShader(const std::string &name, QObject *parent = nullptr);
    ~SkyShader();

    //    void use(std::function<void(QOpenGLShaderProgram*)> func) override;
};

struct InputData {
    enum class InputType : int { KEY, MOUSE, WHEEL } type;
    // Key Input
    static constexpr int nKeyEvents = 5;
    union {
        struct { bool front = false, back = false, left = false, right = false, reset = false; };
        bool key_event[nKeyEvents];
    };
    // Wheel Input
    float scrollx = 0.0f, scrolly = 0.0f;
    // Mouse Input
    QPoint pos = {0, 0};
    static constexpr int nMouseEvents = 3;
    union {
        struct { bool press = false, release = false, move = false; };
        bool mouse_event[nMouseEvents];
    };

    explicit InputData(InputType type = InputData::InputType::KEY)
        : type(type)
    {}
};

#endif // RENDERDATA_H
