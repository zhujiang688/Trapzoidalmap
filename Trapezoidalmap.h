#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Trapezoidalmap.h"
#include "struct_define.h"
#include <set>
using namespace std;
class Trapezoidalmap : public QMainWindow
{
    Q_OBJECT

public:
    Trapezoidalmap(QWidget *parent = Q_NULLPTR);

    //����ͼ���
    set<Trap*> trap_con;
    virtual_Node* root;//��λ�ṹ
    vector<Line*> Line_con;
    void General_random_Line(int num);//���������һ�����߶μ�
    void General_Trap_map();//��������ͼ
    void init();
    Trap_Node* find_loc(Point* point, virtual_Node* node);
    void flash_neig(Trap_Node* insert, Trap_Node* old, Trap_Node* neig);
    void deal(Line* line, vector<Trap_Node*>tri);
    vector<float> mesh_data;
    void get_mesh(virtual_Node* node);
    vector<Point> Cal_point(Trap* trap);
    void record(vector<Point> trap);
private:
    Ui::TrapezoidalmapClass ui;
};
