//�﷨������
#ifndef PARSER_H
#define PARSER_H
#include "Scanner.h"

typedef double (* func_ptr)(double);   //����Դ�ļ��к���
typedef struct	tree_node			// �﷨������ڵ�����
{
    //��������ֵ����������ֵ
    enum token_type op_code;           //�Ǻ�����
    union//�����ϡ���һ��������࣬Ҳ��һ�ֹ������͵����ݽṹ
    {
        struct
        {
            tree_node *left, *right;
        } tag_op;//��Ԫ����
        struct
        {
            tree_node *child;
            func_ptr math_func_ptr;
        } tag_func;//��������
        double tag_const;//����Ҷ�ڵ�
        double * tag_parameter;//����T
    } content;
} *tree_node_ptr;

class Parser:public Scanner
{
protected:
    double parameter;	// ����T
    Token 	token;		// �Ǻ�
    tree_node_ptr 	start_ptr,	// ��ͼ�����ʽ���﷨��
                    end_ptr, 	// ��ͼ�յ���ʽ���﷨��
                    step_ptr,	// �������ʽ���﷨��
                    x_ptr,		// x���﷨��
                    y_ptr,		// y���﷨��
                    angle_ptr;	// ��ת�Ƕȱ��ʽ���﷨��
    void fetch_token ();						// ��ȡ�Ǻ�
    void match_token (enum token_type the_token);			// ƥ�䵱ǰ�Ǻ�
    void syntax_error (int case_of);				// ָ���﷨����
    void print_syntax_tree(tree_node *root, int indent);		// ��ӡ�﷨��
    tree_node_ptr make_tree_node(enum token_type opcode,...);	// �����﷨��
    virtual void error_msg(int line, char *descrip, char *string);	// ��Semantics������Ϊ���ڴ�ӡ��ʽ

    /*������������������﷨�����蹹���﷨��*/
    void program();
    void statement();
    virtual void for_statement();
    virtual void origin_statement();
    virtual void rot_statement();
    virtual void scale_statement();

    /*���ʽ�﷨�����﷨��*/
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
    bool parser(char * file_name);		// �﷨�������ӿ�
private:
    // �������������﷨�������Ĳ���(��Semantics����ΪʧЧ)
    virtual void enter(char* x);
    virtual void back(char* x);
    virtual void call_match(char* x);
};
#endif // PARSER_H
