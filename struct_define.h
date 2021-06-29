#pragma once
#include <vector>
struct Point;
struct Line;
struct Trap;
class virtual_Node;
class Point_Node;
class Line_Node;
class Trap_Node;
bool ishigher(Point* point, Line* line);

struct Point
{
	float x;
	float y;
	Point()
	{
		x = 0;
		y = 0;
	}
};

struct Line
{
	Point* left;
	Point* right;
	Line()
	{
		left = nullptr;
		right = nullptr;
	}
	bool intract(Line* line)
	{
		float x1 = line->right->x - left->x;
		float y1 = line->right->y - left->y;
		float x2 = line->left->x - left->x;
		float y2 = line->left->y - left->y;
		float x3 = right->x - left->x;
		float y3 = right->y - left->y;
		return (x1 * y3 - y1 * x3) * (x2 * y3 - y2 * x3) < 0;
	}
	~Line()
	{
		delete left;
		delete right;
	}
};

struct Trap
{
	Point* left_ind;
	Point* right_ind;
	Line* top_ind;
	Line* bottom_ind;
	Trap()
	{
		left_ind = nullptr;
		right_ind = nullptr;
		top_ind = nullptr;
		bottom_ind = nullptr;
	}
};

class virtual_Node
{
public:
	int type = -1;
	bool isvisited = false;
	virtual_Node* leftchild;
	virtual_Node* rightchild;
	std::vector<virtual_Node*>father;
	virtual_Node()
	{
		leftchild = nullptr;
		rightchild = nullptr;
	}
};

class Point_Node :public virtual_Node
{
public:
	Point* point;
	Point_Node()
	{
		type = 0;
		point = nullptr;
	};
	~Point_Node()
	{

	};
};

class Line_Node :public virtual_Node
{
public:
	Line* line;
	Line_Node()
	{
		type = 1;
		line = nullptr;
	}
	~Line_Node()
	{

	};
};

class Trap_Node :public virtual_Node
{
public:
	Trap* trap;
	//相邻拓扑的信息
	Trap_Node* leftup;
	Trap_Node* leftbottom;
	Trap_Node* rightup;
	Trap_Node* rightbottom;
	Trap_Node()
	{
		type = 2;
		trap = nullptr;
		leftup = nullptr;
		leftbottom = nullptr;
		rightup = nullptr;
		rightbottom = nullptr;
	}
	~Trap_Node()
	{
		delete trap;
	}
};