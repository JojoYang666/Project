//语法分析器
#ifndef PARSER_H
#define PARSER_H
#include "Scanner.h"

typedef double (* func_ptr)(double);   //定义源文件中函数
typedef struct	tree_node			// 语法树三类节点类型
{
    //常数绑定右值，变量绑定左值
    enum token_type op_code;           //记号种类
    union//“联合”是一种特殊的类，也是一种构造类型的数据结构
    {
        struct
        {
            tree_node *left, *right;
        } tag_op;//二元运算
        struct
        {
            tree_node *child;
            func_ptr math_func_ptr;
        } tag_func;//函数调用
        double tag_const;//常数叶节点
        double * tag_parameter;//参数T
    } content;
} *tree_node_ptr;

class Parser:public Scanner
{
protected:
    double parameter;	// 参数T
    Token 	token;		// 记号
    tree_node_ptr 	start_ptr,	// 绘图起点表达式的语法树
                    end_ptr, 	// 绘图终点表达式的语法树
                    step_ptr,	// 步长表达式的语法树
                    x_ptr,		// x的语法树
                    y_ptr,		// y的语法树
                    angle_ptr;	// 旋转角度表达式的语法树
    void fetch_token ();						// 获取记号
    void match_token (enum token_type the_token);			// 匹配当前记号
    void syntax_error (int case_of);				// 指出语法错误
    void print_syntax_tree(tree_node *root, int indent);		// 打印语法树
    tree_node_ptr make_tree_node(enum token_type opcode,...);	// 构造语法树
    virtual void error_msg(int line, char *descrip, char *string);	// 在Semantics中重置为窗口打印形式

    /*以下六个函数是语句语法，无需构造语法树*/
    void program();
    void statement();
    virtual void for_statement();
    virtual void origin_statement();
    virtual void rot_statement();
    virtual void scale_statement();

    /*表达式语法构造语法树*/
    tree_node_ptr expression();
    tree_node_ptr term();
    tree_node_ptr factor();
    tree_node_ptr component();
    tree_node_ptr atom();
public:
    Parser()
    {
        parameter = 0;
        start_ptr = NULL;
        end_ptr   = NULL;
        step_ptr  = NULL;
        x_ptr	  = NULL;
        y_ptr	  = NULL;
    }
    ~Parser() {}
    bool parser(char * file_name);		// 语法分析器接口
private:
    // 下述函数用于语法分析器的测试(在Semantics重置为失效)
    virtual void enter(char* x);
    virtual void back(char* x);
    virtual void call_match(char* x);
};
#endif // PARSER_H
