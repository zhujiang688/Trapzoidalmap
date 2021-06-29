#pragma once

#include "shader.h"

#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include <qtimer.h>
#include <fstream>
#include <QWidget>
#include <iostream>
#include <QString>
#include <mutex>
#include <QKeyEvent>

using namespace std;

class Display : public QOpenGLWidget
{
    Q_OBJECT

public:
    explicit Display(QWidget* parent = nullptr);
    ~Display();
    void up_Line();
    void up_mesh();
    void up_loc();

    static QVector3D lightColor;
    static QVector3D lightPos;

    //随机线段数据
    static GLfloat* vertice;
    static unsigned int point_num;

    //梯形图数据
    static unsigned int mesh_size;
    static GLfloat* mesh_vertic;
    
    //点的定位数据
    static unsigned int loc_size;
    static GLfloat* loc_vertic;
    static unsigned int point_size;
    static GLfloat* point_vertic;

    static unsigned int POINT_SIZE;
    static QVector3D back_color;
    static QVector3D draw_color;
    static QString vert_shader_path;
    static QString Frag_shader_path;
    void keyPressEvent(QKeyEvent* event);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
    
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void StartAnimating();


private:
    Shader* ourShader;
    QOpenGLFunctions_3_3_Core* core;
    GLuint VBO[4], VAO[4];
    //再添加个VBO绘制路径



    QVector3D cameraPos;
    QVector3D cameraFront;
    QVector3D cameraUp;
    QVector3D cameraleft;
    GLfloat anglex;
    GLfloat angley;
    GLfloat anglez;

    //鼠标移动记录
    QPoint pos;
    QPoint rot;
    bool leftpress = false;
    bool rightpress = false;
    QTimer* m_timer;
    

    bool X_IND = true;
    bool Y_IND = true;
    bool Z_IND = true;

public slots:
    void Reset_loc();
    void Set_X();
    void Set_Y();
    void Set_Z();
};