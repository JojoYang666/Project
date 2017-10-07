//语义
#include "Semantics.h"

void Semantics::error_msg(int line, char * descrip, char* temp)
{//出错处理
    const int buf_len=256;
    char msg[buf_len];
    memset(msg, 0, buf_len);
    sprintf(msg, "Line No %3d:  %s %s !",line_no, descrip, temp);
    MessageBox(NULL, msg, "错误!", MB_OK);//MessageBox显示一个模态对话框，
    //其中包含一个系统图标、 一组按钮和一个简短的特定于应用程序消息，如状态或错误的信息。消息框中返回一个整数值，该值指示用户单击了哪个按钮。
    CloseScanner();
    exit(-1);
}

void Semantics::cal_coord(tree_node_ptr x_ptr,tree_node_ptr y_ptr,double &x_ret,double &y_ret)
{
    //计算绘制点的坐标
    double x_val, x_temp, y_val;
    /*计算表达式的值，得到点的原始坐标*/
    x_val=get_tree_value(x_ptr);
    y_val=get_tree_value(y_ptr);

    x_val*=Scale_x ;
    y_val*=Scale_y ;// 比例变换

    x_temp=x_val*cos(rot_angle) + y_val*sin(rot_angle);
    y_val=y_val*cos(rot_angle) - x_val*sin(rot_angle);
    x_val=x_temp;// 旋转变换

    x_val+=Origin_x;
    y_val+=Origin_y; // 平移变换

    x_ret=x_val;
    y_ret=y_val;// 返回变换后点的坐标
}

void Semantics::draw_loop(double start_val,double end_val,double step_val,tree_node_ptr x_ptr,tree_node_ptr y_ptr)
{//循环绘制点坐标
    double x_val, y_val;
    for(parameter = start_val; parameter <= end_val; parameter += step_val)
    {
        cal_coord(x_ptr, y_ptr, x_val, y_val);//计算绘制点的坐标
		y_val = 460-y_val;			// 修改显示屏的直角坐标系
        draw_pixel((unsigned long)x_val, (unsigned long)y_val);
    }
}

double Semantics::get_tree_value(tree_node_ptr root)
{//计算表达式的值
    if (root == NULL) return 0.0;
    switch (root -> op_code)
    {
    case PLUS  :
        return get_tree_value(root->content.tag_op.left ) + get_tree_value(root->content.tag_op.right);
    case MINUS :
        return get_tree_value(root->content.tag_op.left ) - get_tree_value(root->content.tag_op.right);
    case MUL   :
        return get_tree_value(root->content.tag_op.left ) * get_tree_value(root->content.tag_op.right);
    case DIV   :
        return get_tree_value(root->content.tag_op.left ) / get_tree_value(root->content.tag_op.right);
    case POWER :
        return pow(get_tree_value(root->content.tag_op.left ), get_tree_value(root->content.tag_op.right));
    case FUNC  :
        return (* root->content.tag_func.math_func_ptr)	(get_tree_value(root->content.tag_func.child));
    case CONST_ID :
        return root->content.tag_const;
    case T  :
        return *(root->content.tag_parameter);
    default :
        return 0.0;
    }
}

void Semantics::delete_tree(tree_node_ptr root)
{//删除一棵语法树
    if (root == NULL) return;
    switch (root -> op_code)
    {
    case PLUS  :			// 两个孩子的内部节点
    case MINUS :
    case MUL   :
    case DIV   :
    case POWER :
        delete_tree (root->content.tag_op.left );
        delete_tree (root->content.tag_op.right);
        break;
    case FUNC : 			// 一个孩子的内部节点
        delete_tree (root->content.tag_func.child) ;
        break;
    default: 			// 叶子节点
        break ;
    }
    delete(root);			// 删除节点
}

void Semantics::draw_pixel(unsigned long x, unsigned long y)
{//绘制一个点
    SetPixel(hDC, x, y, RGB(199,21,133));//该函数将指定坐标处的像素设为指定的颜色
    SetPixel(hDC, x+1, y, RGB(199,21,133));
    SetPixel(hDC, x, y+1, RGB(199,21,133));
    SetPixel(hDC, x+1, y+1, RGB(199,21,133));//将线条色彩设置为红色

}

/*origin_statement的递归子程序*/
void Semantics::origin_statement()
{
    Parser::origin_statement();		// 语法分析获取语法树（由parser中的属性保存）重置基类算法

    Origin_x = get_tree_value(x_ptr);		// 根据语法树计算横坐标的平移距离
    delete_tree(x_ptr);

    Origin_y = get_tree_value(y_ptr);		// 根据语法树计算纵坐标的平移距离
    delete_tree(y_ptr);
}

/*scale_statement的递归子程序*/
void Semantics::scale_statement ()
{
    Parser::scale_statement();		// 语法分析获取语法树（由parser中的属性保存）

    Scale_x = get_tree_value(x_ptr);		// 根据语法树计算横坐标的比例因子
    delete_tree(x_ptr);

    Scale_y = get_tree_value(y_ptr);		// 根据语法树计算纵坐标的比例因子
    delete_tree(y_ptr);
}

/*rot_statement的递归子程序*/
void Semantics::rot_statement ()
{
    Parser::rot_statement();			// 语法分析获取语法树（由parser中的属性保存）
    rot_angle = get_tree_value(angle_ptr);		// 根据语法树计算旋转角度
    delete_tree(angle_ptr);
}

/*for_statement 的递归子程序*/
void Semantics::for_statement ()
{
    double start_val, end_val, step_val;		// 绘图起点、终点、步长

    Parser::for_statement();			// 语法分析获取各表达式的语法树（由parser中的属性保存）

    start_val = get_tree_value(start_ptr);		// 计算起点表达式的值
    end_val = get_tree_value(end_ptr);		// 计算终点表达式的值
    step_val = get_tree_value(step_ptr);		// 计算步长表达式的值

    draw_loop (start_val, end_val, step_val, x_ptr, y_ptr); // 绘图

    delete_tree(start_ptr);					// 释放空间
    delete_tree(end_ptr);
    delete_tree(step_ptr);
    delete_tree(x_ptr);
    delete_tree(y_ptr);
}

/*用于语法分析器中的跟踪调试, 此处不再起作用*/
void Semantics::enter() {}
void Semantics::back() {}
void Semantics::call_match() {}
