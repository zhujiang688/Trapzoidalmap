#include "Trapezoidalmap.h"
#include <QtWidgets/QApplication>
GLfloat loc[] = {
     0.0f,  0.707f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,
     0.0f,  0.707f, 0.0f, 0.0f, 0.0f, -1.0f,
    -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, -1.0f,
     0.5f, -0.5f, 0.0f,   0.0f, 0.0f, -1.0f
};

GLfloat* Display::vertice = loc;
unsigned int Display::point_num = 6;
unsigned int Display::POINT_SIZE = 10;
QVector3D Display::back_color = QVector3D(0.4f, 0.3f, 0.5f);
QVector3D Display::draw_color = QVector3D(0, 0, 1);
QString Display::vert_shader_path = "D:/VSproject/QtWidgetsApplication2/vertexshadersource.vert";
QString Display::Frag_shader_path = "D:/VSproject/QtWidgetsApplication2/fragmentshadersource.frag";
QVector3D Display::lightColor = QVector3D(1.0, 1.0, 1.0);
QVector3D Display::lightPos = QVector3D(0, 0, 3.0);

unsigned int Display::mesh_size = 0;
GLfloat* Display::mesh_vertic = nullptr;
unsigned int Display::loc_size = 0;
GLfloat* Display::loc_vertic = nullptr;
unsigned int Display::point_size = 0;
GLfloat* Display::point_vertic = nullptr;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Trapezoidalmap w;
    w.show();
    return a.exec();
}
