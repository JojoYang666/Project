//语义
#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <windows.h>
#include <wingdi.h>
#include "Parser.h"

extern HDC hDC;//设备上下文句柄

class Semantics: public Parser
{
protected:
    double 	Origin_x, Origin_y,		// 横、纵平移距离
            Scale_x,  Scale_y, 		// 横、纵比例因子
            rot_angle;			// 旋转角度
public:
    Semantics()			// 声明对象时置下述初值
    {
        Origin_x  = 0;
        Origin_y = 0;	// 原点是(0, 0)
        Scale_x   = 1;
        Scale_y  = 1;	// 横、纵坐标比例是1:1
        rot_angle = 0;			// 旋转角度是0
    };
    ~Semantics() {};

private:
    /*重置基类Parser中的方法，以在语法分析的基础上实现语法制导翻译*/
    void error_msg (int line, char *descrip, char *string);	// 出错处理
    void for_statement ();
    void origin_statement();
    void rot_statement();
    void scale_statement();

    /*下述函数在Semantics中被重置为不起作用，以屏蔽语法分析时的跟踪测试*/
    void enter();
    void back();
    void call_match();

    /*Semantics中的方法，它们是语法制导翻译中所需的语义函数*/
    double get_tree_value(tree_node_ptr root);			// 获得表达式的值
    void cal_coord(	tree_node_ptr hor_ptr,				// 计算点的坐标
                    tree_node_ptr ver_ptr,
                    double &hor_val,
                    double &ver_val);
    void draw_pixel(unsigned long x, unsigned long y);		// 绘制一个点
    void draw_loop(	double start_val,				// 图形绘制
                    double end_val,
                    double step_val,
                    tree_node_ptr x_ptr,
                    tree_node_ptr y_ptr);
    void delete_tree(tree_node_ptr root);				// 删除一棵树
};
#endif // SEMANTICS_H
