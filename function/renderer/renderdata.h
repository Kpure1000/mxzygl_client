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

struct PerspectiveCamera;

struct RenderData {
    std::vector<std::shared_ptr<TriangleData>> triangleDatas;

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
    enum KeyCode { W = 0x1, A = 0x1 << 1, S = 0x1 << 2, D = 0x1 << 3, R = 0x1 << 4 } keyCode;
    static constexpr int nKeyEvents = 2;
    union Key {
        struct { bool press, release; };
        bool key_event[nKeyEvents];
    } key;
    // Wheel Input
    float scrollx = 0.0f, scrolly = 0.0f;
    // Mouse Input
    enum MouseButton { LEFT = 0x1, RIGHT = 0x1 << 1, MIDDLE = 0x1 << 2 } mouseButton;
    unsigned int mouseButtons = 0;
    QPoint pos = {0, 0};
    static constexpr int nMouseEvents = 3;
    union Mouse {
        struct { bool press, release, move; };
        bool mouse_event[nMouseEvents];
    } mouse;

    explicit InputData(InputType type = InputData::InputType::KEY)
        : type(type), key{{false, false}}, mouse{{false, false, false}}
    {
    }
};

#endif // RENDERDATA_H
