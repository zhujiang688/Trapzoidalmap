#include "Trapezoidalmap.h"

Trapezoidalmap::Trapezoidalmap(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    root = nullptr;
    connect(ui.General_line, &QPushButton::clicked, this, [this]() {
        General_random_Line(ui.Line_num->value());
        });
    connect(ui.General_trap, &QPushButton::clicked, this, [this]() {
        General_Trap_map();
        });
    connect(ui.Fine_loc, &QPushButton::clicked, this, [this]() {
        Point temp;
        temp.x = ui.x_value->value();
        temp.y = ui.y_value->value();
        vector<float>point{temp.x, temp.y,0,0,0,1};
        auto loc = find_loc(&temp,root);
        mesh_data.clear();
        record(Cal_point(loc->trap));
        Display::loc_vertic = (float*)&mesh_data[0];
        Display::loc_size = 8;
        Display::point_vertic = &point[0];
        Display::point_size = 1;
        ui.widget->up_loc();
        });
}

void Trapezoidalmap::General_random_Line(int num)//生成随机的一般性线段集
{
    srand(time(0));
    //要保证生成的线段集不能相交
    for (int i = 0;i < num;i++)
    {
        while (true)
        {
            Point* p1 = new Point;
            Point* p2 = new Point;
            Line* line = new Line;
            p1->x = rand() / (float)RAND_MAX;
            p1->y = rand() / (float)RAND_MAX;
            p2->x = rand() / (float)RAND_MAX;
            p2->y = rand() / (float)RAND_MAX;
            if (p1->x < p2->x)
            {
                line->left = p1;
                line->right = p2;
            }
            else
            {
                line->left = p2;
                line->right = p1;
            }
            //检查相交
            bool flag = true;
            for (auto x : Line_con)
            {
                if (x->intract(line))
                {
                    flag = false;
                    break;
                }
            }
            if (flag)
            {
                Line_con.push_back(line);
                break;
            }
            else
            {
                delete line;
            }
        }
    }
    //绘制相关
    vector<float> loc;
    for (auto x : Line_con)
    {
        loc.push_back(x->left->x);
        loc.push_back(x->left->y);
        loc.push_back(0);
        loc.push_back(0);
        loc.push_back(0);
        loc.push_back(1);
        loc.push_back(x->right->x);
        loc.push_back(x->right->y);
        loc.push_back(0);
        loc.push_back(0);
        loc.push_back(0);
        loc.push_back(1);
    }
    Display::vertice = (float*)&loc[0];
    Display::point_num = loc.size() / 6;
    ui.widget->up_Line();
}


void Trapezoidalmap::General_Trap_map()//生成梯形图
{
    //构造包围框
    init();
    //遍历每一条线段
    for (auto x : Line_con)
    {
        //被线段穿过的梯形
        vector<Trap_Node*> arr;
        //查找左端点所在叶子节点
        auto loc_node = find_loc(x->left, root);
        //依据右邻居信息找到被穿过的所有叶子节点
        Trap_Node* temp = loc_node;
        while (temp)
        {
            arr.push_back(temp);
            if (x->right->x > temp->trap->right_ind->x)//需要继续穿下去
            {
                if (ishigher(temp->trap->right_ind, x)) temp = temp->rightbottom;
                else temp = temp->rightup;
            }
            else
            {
                break;
            }
        }
        //处理这些被穿过三角型的拓扑信息
        deal(x, arr);
    }
    //绘图相关
    get_mesh(root);
    Display::mesh_vertic = &mesh_data[0];
    Display::mesh_size = mesh_data.size() / 6;
    ui.widget->up_mesh();
}

void Trapezoidalmap::init()
{
    //将包围框视为一个大梯形处理
    Point* left_down = new Point;
    Point* left_up = new Point;
    Point* right_down = new Point;
    Point* right_up = new Point;
    Line* upper = new Line;
    Line* down = new Line;
    left_down->x = 0; left_down->y = 0;
    left_up->x = 0; left_up->y = 1;
    right_down->x = 1; right_down->y = 0;
    right_up->x = 1; right_up->y = 1;
    upper->left = left_up;
    upper->right = right_up;
    down->left = left_down;
    down->right = right_down;
    Trap* first = new Trap;
    first->bottom_ind = down;
    first->top_ind = upper;
    first->left_ind = left_down;
    first->right_ind = right_down;
    trap_con.insert(first);
    root = new Trap_Node;
    ((Trap_Node*)root)->trap = first;
}

Trap_Node* Trapezoidalmap::find_loc(Point* point, virtual_Node* node)
{
    //终止条件，叶子节点
    if (node->type == 2) return (Trap_Node*)node;
    else if (node->type == 0)//点
    {
        if (point->x < ((Point_Node*)node)->point->x) return find_loc(point, node->leftchild);
        else return find_loc(point, node->rightchild);
    }
    else //线
    {
        if(ishigher(point, ((Line_Node*)node)->line)) return find_loc(point, node->leftchild);
        else return find_loc(point,node->rightchild);
    }
}

void Trapezoidalmap::flash_neig(Trap_Node* insert, Trap_Node* old, Trap_Node* neig)
{
    if (neig == nullptr) return;
    if (neig->leftup == old)
    {
        neig->leftup = insert;//换成新的
    }
    if (neig->leftbottom == old)
    {
        neig->leftbottom = insert;//换成新的
    }
    if (neig->rightup == old)
    {
        neig->rightup = insert;//换成新的
    }
    if (neig->rightbottom == old)
    {
        neig->rightbottom = insert;//换成新的
    }
}

void Trapezoidalmap::deal(Line* line, vector<Trap_Node*>tri)
{
    //首先建立新的梯形
    Trap* new_left = new Trap;
    Trap* new_right = new Trap;
    vector<Trap*> new_bottom;
    for (int i = 0;i < tri.size();i++)
    {
        new_bottom.push_back(new Trap);
    }
    //更新梯形图的几何信息
    //新建三角形的
    new_left->left_ind = tri[0]->trap->left_ind;//新生成的最左边的三角形的left_ind不变
    new_left->right_ind = line->left;
    new_left->top_ind = tri[0]->trap->top_ind;
    new_left->bottom_ind = tri[0]->trap->bottom_ind;
    new_right->left_ind = line->right;
    new_right->right_ind = tri.back()->trap->right_ind;
    new_right->top_ind = tri.back()->trap->top_ind;
    new_right->bottom_ind = tri.back()->trap->bottom_ind;

    //原有被穿过三角形的几何信息更新
    tri[0]->trap->left_ind = line->left;
    tri.back()->trap->right_ind = line->right;
    //新增bottom三角形的
    for (int i = 0;i < tri.size();i++)
    {
        new_bottom[i]->top_ind = line;
        new_bottom[i]->bottom_ind = tri[i]->trap->bottom_ind;
        new_bottom[i]->left_ind = tri[i]->trap->left_ind;
        new_bottom[i]->right_ind = tri[i]->trap->right_ind;
    }
    for (auto& x : tri) x->trap->bottom_ind = line;
   


    //更新梯形图的拓扑信息
    Trap_Node* A = new Trap_Node;
    Trap_Node* B = new Trap_Node;
    vector<Trap_Node*> D;
    for (int i = 0;i < tri.size();i++)
    {
        D.push_back(new Trap_Node);
    }
    A->trap = new_left;
    B->trap = new_right;
    for (int i = 0;i < tri.size();i++)
    {
        D[i]->trap = new_bottom[i];
    }
    A->leftup = tri[0]->leftup;
    A->leftbottom = tri[0]->leftbottom;
    A->rightup = tri[0];
    A->rightbottom = D[0];
    B->leftup = tri.back();
    B->leftbottom = D.back();
    B->rightup = tri.back()->rightup;
    B->rightbottom = tri.back()->rightbottom;
    //更新新增bottom三角形的拓扑信息
    D[0]->leftup = A;
    D[0]->leftbottom = A;
    D.back()->rightup = B;
    D.back()->rightbottom = B;
    for (int i = 0;i < D.size() - 1;i++) 
    {
        D[i]->rightup = D[i + 1];
        D[i]->rightbottom = tri[i]->rightbottom;
    }
    for (int i = D.size() - 1;i > 1;i--) 
    {
        D[i]->leftup = D[i - 1];
        D[i]->leftbottom = tri[i]->leftbottom;
    }


    //更新周围梯形的拓扑信息
    flash_neig(A, tri[0], tri[0]->leftup);//三个参数的含义：新增的梯形、原来代表这个位置的梯形、原来代表这个位置的梯形的某个邻居。这个函数将会更改第三个参数的邻居
    flash_neig(A, tri[0], tri[0]->leftbottom);
    flash_neig(B, tri.back(), tri.back()->rightup);
    flash_neig(B, tri.back(), tri.back()->rightbottom);
    //更新原有被穿过梯形的拓扑信息
    tri[0]->leftup = A;
    tri[0]->leftbottom = A;
    tri.back()->rightup = B;
    tri.back()->rightbottom = B;
    for (int i = 0;i < tri.size() - 1;i++) tri[i]->rightbottom = tri[i + 1];
    for (int i = tri.size() - 1;i > 1;i--) tri[i]->leftbottom = tri[i - 1];



    //更新查找结构和记录结构
    trap_con.insert(new_left);
    trap_con.insert(new_right);
    for(int i =0;i< new_bottom.size();i++) trap_con.insert(new_bottom[i]);

    //更新查找结构
    if (tri.size() == 1)
    {
        Point_Node* p1 = new Point_Node;
        Point_Node* p2 = new Point_Node;
        Line_Node* L = new Line_Node;
        p1->point = line->left;
        p2->point = line->right;
        L->line = line;
        p1->leftchild = A;
        p1->rightchild = p2;
        p2->leftchild = L;
        p2->rightchild = B;
        L->leftchild = tri[0];
        L->rightchild = D[0];
        //维护father信息
        //更换最上面的节点
        if (tri[0]->father.size() == 0)//第一次插入
        {
            root = p1;
        }
        else
        {
            for (int i = 0;i < tri[0]->father.size();i++)
            {
                if (tri[0]->father[i]->leftchild == tri[0])
                {
                    tri[0]->father[i]->leftchild = p1;
                    p1->father.push_back(tri[0]->father[i]);
                }
                if (tri[0]->father[i]->rightchild == tri[0])
                {
                    tri[0]->father[i]->rightchild = p1;
                    p1->father.push_back(tri[0]->father[i]);
                }
            }
        }
        //更新新建节点的father信息
        A->father.push_back(p1);
        p2->father.push_back(p1);
        L->father.push_back(p2);
        B->father.push_back(p2);
        tri[0]->father.clear();
        tri[0]->father.push_back(L);
        D[0]->father.push_back(L);
    }
    else
    {
        //处理左右端点梯形节点
        Point_Node* p1 = new Point_Node;
        Point_Node* p2 = new Point_Node;
        //处理左边第一个
        {
            Line_Node* L = new Line_Node;
            p1->point = line->left;
            L->line = line;
            p1->leftchild = A;
            p1->rightchild = L;
            L->leftchild = tri[0];
            L->rightchild = D[0];
            //更新father信息
            for (int i = 0;i < tri[0]->father.size();i++)
            {
                if (tri[0]->father[i]->leftchild == tri[0])
                {
                    tri[0]->father[i]->leftchild = p1;
                    p1->father.push_back(tri[0]->father[i]);
                }
                if (tri[0]->father[i]->rightchild == tri[0])
                {
                    tri[0]->father[i]->rightchild = p1;
                    p1->father.push_back(tri[0]->father[i]);
                }
            }
            A->father.push_back(p1);
            L->father.push_back(p1);
            tri[0]->father.clear();
            tri[0]->father.push_back(L);
            D[0]->father.push_back(L);
        }
        //处理右边第一个
        {
            Line_Node* L = new Line_Node;
            p2->point = line->right;
            L->line = line;
            p2->leftchild = L;
            p2->rightchild = B;
            L->leftchild = tri.back();
            L->rightchild = D.back();
            //更新father信息
            for (int i = 0;i < tri.back()->father.size();i++)
            {
                if (tri.back()->father[i]->leftchild == tri.back())
                {
                    tri.back()->father[i]->leftchild = p2;
                    p2->father.push_back(tri.back()->father[i]);
                }
                if (tri.back()->father[i]->rightchild == tri.back())
                {
                    tri.back()->father[i]->rightchild = p2;
                    p2->father.push_back(tri.back()->father[i]);
                }
            }
            B->father.push_back(p2);
            L->father.push_back(p2);
            tri.back()->father.clear();
            tri.back()->father.push_back(L);
            D.back()->father.push_back(L);
        }
        //处理中间被穿过梯形的查找结点
        for (int i = 1;i < tri.size() - 1;i++)
        {
            Line_Node* L = new Line_Node;
            L->line = line;
            L->leftchild = tri[i];
            L->rightchild = D[i];
            //更新father信息
            for (int j = 0;j < tri[i]->father.size();j++)
            {
                if (tri[i]->father[j]->leftchild == tri[i])
                {
                    tri[i]->father[j]->leftchild = L;
                    L->father.push_back(tri[i]->father[j]);
                }
                if (tri[i]->father[j]->rightchild == tri[i])
                {
                    tri[i]->father[j]->rightchild = L;
                    L->father.push_back(tri[i]->father[j]);
                }
            }
            tri[i]->father.clear();
            tri[i]->father.push_back(L);
            D[i]->father.push_back(L);
        }
    }
}

void Trapezoidalmap::get_mesh(virtual_Node* node)
{
    if (node->type == 2)
    {
        Trap_Node* temp = (Trap_Node*)node;
        record(Cal_point(temp->trap));
    }
    else
    {
        get_mesh(node->leftchild);
        get_mesh(node->rightchild);
    }
}

vector<Point> Trapezoidalmap::Cal_point(Trap* trap)
{
    Point p1, p2, p3, p4;
    p1.x = trap->left_ind->x;
    p1.y = trap->top_ind->left->y + (trap->top_ind->right->y - trap->top_ind->left->y) * (trap->left_ind->x - trap->top_ind->left->x) / (trap->top_ind->right->x - trap->top_ind->left->x);
    p2.x = trap->right_ind->x;
    p2.y = trap->top_ind->left->y + (trap->top_ind->right->y - trap->top_ind->left->y) * (trap->right_ind->x - trap->top_ind->left->x) / (trap->top_ind->right->x - trap->top_ind->left->x);
    p3.x = trap->right_ind->x;
    p3.y = trap->bottom_ind->left->y + (trap->bottom_ind->right->y - trap->bottom_ind->left->y) * (trap->right_ind->x - trap->bottom_ind->left->x) / (trap->bottom_ind->right->x - trap->bottom_ind->left->x);
    p4.x = trap->left_ind->x;
    p4.y = trap->bottom_ind->left->y + (trap->bottom_ind->right->y - trap->bottom_ind->left->y) * (trap->left_ind->x - trap->bottom_ind->left->x) / (trap->bottom_ind->right->x - trap->bottom_ind->left->x);
    return vector<Point>{p1, p2, p3, p4};
}

void Trapezoidalmap::record(vector<Point> trap)
{
    mesh_data.push_back(trap[0].x);
    mesh_data.push_back(trap[0].y);
    mesh_data.push_back(0);
    mesh_data.push_back(0);
    mesh_data.push_back(0);
    mesh_data.push_back(1);
    for (int i = 1;i < trap.size();i++)
    {
        mesh_data.push_back(trap[i].x);
        mesh_data.push_back(trap[i].y);
        mesh_data.push_back(0);
        mesh_data.push_back(0);
        mesh_data.push_back(0);
        mesh_data.push_back(1);
        mesh_data.push_back(trap[i].x);
        mesh_data.push_back(trap[i].y);
        mesh_data.push_back(0);
        mesh_data.push_back(0);
        mesh_data.push_back(0);
        mesh_data.push_back(1);
    }
    mesh_data.push_back(trap[0].x);
    mesh_data.push_back(trap[0].y);
    mesh_data.push_back(0);
    mesh_data.push_back(0);
    mesh_data.push_back(0);
    mesh_data.push_back(1);
}