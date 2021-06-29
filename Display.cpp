#include "Display.h"
#include<QMouseEvent>
#include<sstream>

Display::Display(QWidget* parent) :QOpenGLWidget(parent)
{
    m_timer = new QTimer(this);
    QObject::connect(m_timer, &QTimer::timeout, [this]() {
        this->update();
        });
    StartAnimating();
    setFocusPolicy(Qt::StrongFocus);
}

void Display::StartAnimating()
{
    m_timer->start(100);
}

Display::~Display()
{
    m_timer->deleteLater();
    delete ourShader;
}

void Display::initializeGL()
{
    //��ɫ������
    core = QOpenGLContext::currentContext()->versionFunctions<QOpenGLFunctions_3_3_Core>();
    ourShader = new Shader(vert_shader_path, Frag_shader_path);

    //����VAO�����к�
    core->glGenVertexArrays(4, VAO);//������������һ��Ϊ��Ҫ�����Ļ����������ڶ���Ϊ���ڴ洢��һID����ID��GLuint����������ĵ�ַ
    core->glGenBuffers(4, VBO);
    up_Line();
    up_mesh();
    up_loc();
    //��ʼ��һЩ����
    cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    cameraFront = QVector3D(0.0f, 0.0f, -1.0f);
    cameraUp = QVector3D(0.0f, 1.0f, 0.0f);
    cameraleft = QVector3D(1.0f, 0.0f, 0.0f);
    anglex = 0;
    angley = 0;
    anglez = 0;
}

void Display::up_Line()
{
    cout << "up_data" << endl;
    //��
    core->glBindVertexArray(VAO[0]);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);//���´�����VBO�󶨵�GL_ARRAY_BUFFER��
    //�ϴ�����
    core->glBufferData(GL_ARRAY_BUFFER, 6 * point_num * sizeof(GLfloat), vertice, GL_STATIC_DRAW);//����ǰ���ݸ��Ƶ�GL_ARRAY_BUFFER��
    //���ý������ݵķ�ʽ
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);//���Ӷ�������
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    core->glEnableVertexAttribArray(1);
    //�����
    core->glBindBuffer(GL_ARRAY_BUFFER, 0);
    core->glBindVertexArray(0);
}

void Display::up_mesh()
{
    core->glBindVertexArray(VAO[1]);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);//���´�����VBO�󶨵�GL_ARRAY_BUFFER��
    //�ϴ�����
    core->glBufferData(GL_ARRAY_BUFFER, 6 * mesh_size * sizeof(GLfloat), mesh_vertic, GL_STATIC_DRAW);//����ǰ���ݸ��Ƶ�GL_ARRAY_BUFFER��
    //���ý������ݵķ�ʽ
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);//���Ӷ�������
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    core->glEnableVertexAttribArray(1);
    //�����
    core->glBindBuffer(GL_ARRAY_BUFFER, 0);
    core->glBindVertexArray(0);
}

void Display::up_loc()
{
    core->glBindVertexArray(VAO[2]);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);//���´�����VBO�󶨵�GL_ARRAY_BUFFER��
    //�ϴ�����
    core->glBufferData(GL_ARRAY_BUFFER, 6 * loc_size * sizeof(GLfloat), loc_vertic, GL_STATIC_DRAW);//����ǰ���ݸ��Ƶ�GL_ARRAY_BUFFER��
    //���ý������ݵķ�ʽ
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);//���Ӷ�������
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    core->glEnableVertexAttribArray(1);
    //�����
    core->glBindBuffer(GL_ARRAY_BUFFER, 0);
    core->glBindVertexArray(0);


    core->glBindVertexArray(VAO[3]);
    core->glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);//���´�����VBO�󶨵�GL_ARRAY_BUFFER��
    //�ϴ�����
    core->glBufferData(GL_ARRAY_BUFFER, 6 * point_size * sizeof(GLfloat), point_vertic, GL_STATIC_DRAW);//����ǰ���ݸ��Ƶ�GL_ARRAY_BUFFER��
    //���ý������ݵķ�ʽ
    core->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)0);//���Ӷ�������
    core->glEnableVertexAttribArray(0);
    core->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    core->glEnableVertexAttribArray(1);
    //�����
    core->glBindBuffer(GL_ARRAY_BUFFER, 0);
    core->glBindVertexArray(0);
}

void Display::resizeGL(int w, int h) {
    core->glViewport(0, 0, w, h);
}

void Display::paintGL() {
    //����ģʽ����
    core->glPointSize(POINT_SIZE);
    core->glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//���û���ģʽ���㡢�ߡ���
    core->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//�������
    core->glEnable(GL_DEPTH_TEST);//��Ȼ���
    core->glClearColor(back_color[0], back_color[1], back_color[2], 1);//���õ�ɫ

    ourShader->use();

    //����uniform
    QMatrix4x4 view, projection;
    projection.perspective(45.0f, (GLfloat)width() / (GLfloat)height(), 0.1f, 100.0f);
    view.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
    QMatrix4x4 model;
    model.rotate(anglex, QVector3D(1.0f, 0.0f, 0.0f));
    model.rotate(angley, QVector3D(0.0f, 1.0f, 0.0f));
    model.rotate(anglez, QVector3D(0.0f, 0.0f, 1.0f));
    ourShader->setMat4("model", model);
    ourShader->setMat4("view", view);
    ourShader->setMat4("projection", projection);
    ourShader->setVector3f("viewPos", cameraPos);
    ourShader->setVector3f("lightColor", lightColor);
    ourShader->setVector3f("lightPos", lightPos);
    ourShader->setVector3f("Color", draw_color);

    //����
    core->glLineWidth(1);
    core->glBindVertexArray(VAO[0]);
    core->glDrawArrays(GL_LINES, 0, point_num);
    //�����
    core->glBindVertexArray(0);

    //����·��
    if (mesh_size != 0)
    {
        core->glBindVertexArray(VAO[1]);
        ourShader->setVector3f("Color", QVector3D(1, 0, 0));//��������Ļ�����ɫ
        core->glLineWidth(1);
        core->glDrawArrays(GL_LINES, 0, mesh_size);
        core->glBindVertexArray(0);
    }

    if (loc_size != 0)
    {
        core->glBindVertexArray(VAO[2]);
        ourShader->setVector3f("Color", QVector3D(0, 1, 0));//��������Ļ�����ɫ
        core->glLineWidth(3);
        core->glDrawArrays(GL_LINES, 0, loc_size);
        core->glBindVertexArray(0);

        core->glBindVertexArray(VAO[3]);
        ourShader->setVector3f("Color", QVector3D(0, 0, 0));//��������Ļ�����ɫ
        core->glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);//���û���ģʽ���㡢�ߡ���
        core->glDrawArrays(GL_POINTS, 0, point_size);
        core->glBindVertexArray(0);
    }
}


void Display::mouseMoveEvent(QMouseEvent* event)
{
    if (leftpress)
    {
        QPoint move_pos = event->globalPos() - pos;
        cameraPos += 0.0001 * cameraUp * move_pos.y();
        cameraPos -= 0.0001 * cameraleft * move_pos.x();
    }
    if (rightpress)
    {
        QPoint rot_pos = event->globalPos() - rot;
        anglex += 0.001 * rot_pos.y();
        angley += 0.001 * rot_pos.x();
    }
}

void Display::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        leftpress = true;
        pos = event->globalPos();
    }
    else if (event->button() == Qt::RightButton)
    {
        rightpress = true;
        rot = event->globalPos();
    }
}

void Display::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        leftpress = false;
    }
    else if (event->button() == Qt::RightButton)
    {
        rightpress = false;
    }
}

void Display::wheelEvent(QWheelEvent* event)
{
    cameraPos += 0.003 * cameraFront * event->delta();
}



void Display::keyPressEvent(QKeyEvent* event)
{
    cout << "Key" << endl;
    if (event->key() == Qt::Key_1)
    {
        anglez += 1;
    }
    else if (event->key() == Qt::Key_3)
    {
        anglez -= 1;
    }
    else if (event->key() == Qt::Key_4)
    {
        anglez += 10;
    }
    else if (event->key() == Qt::Key_6)
    {
        anglez -= 10;
    }
}


void Display::Reset_loc()
{
    cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    anglex = 0;
    angley = 0;
    anglez = 0; 
}

void Display::Set_X()
{
    cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    if (X_IND)
    {
        anglex = 0;
        angley = 90;
        anglez = 0;
    }
    else
    {
        anglex = 0;
        angley = -90;
        anglez = 0;
    }
    X_IND = !X_IND;
}

void Display::Set_Y()
{
    cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    if (Y_IND)
    {
        anglex = 90;
        angley = 0;
        anglez = 0;
    }
    else
    {
        anglex = -90;
        angley = 0;
        anglez = 0;
    }
    Y_IND = !Y_IND;
}

void Display::Set_Z()
{
    cameraPos = QVector3D(0.0f, 0.0f, 3.0f);
    if (Z_IND)
    {
        anglex = 0;
        angley = 0;
        anglez = 0;
    }
    else
    {
        anglex = 180;
        angley = 0;
        anglez = 0;
    }
    Z_IND = !Z_IND;
}