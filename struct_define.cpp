#include "struct_define.h"

bool ishigher(Point* point, Line* line)
{
	return point->y > line->left->y + (line->right->y - line->left->y) * (point->x - line->left->x) / (line->right->x - line->left->x);
}