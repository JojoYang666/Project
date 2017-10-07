//����
#ifndef SEMANTICS_H
#define SEMANTICS_H

#include <windows.h>
#include <wingdi.h>
#include "Parser.h"

extern HDC hDC;//�豸�����ľ��

class Semantics: public Parser
{
protected:
    double 	Origin_x, Origin_y,		// �ᡢ��ƽ�ƾ���
            Scale_x,  Scale_y, 		// �ᡢ�ݱ�������
            rot_angle;			// ��ת�Ƕ�
public:
    Semantics()			// ��������ʱ��������ֵ
    {
        Origin_x  = 0;
        Origin_y = 0;	// ԭ����(0, 0)
        Scale_x   = 1;
        Scale_y  = 1;	// �ᡢ�����������1:1
        rot_angle = 0;			// ��ת�Ƕ���0
    };
    ~Semantics() {};

private:
    /*���û���Parser�еķ����������﷨�����Ļ�����ʵ���﷨�Ƶ�����*/
    void error_msg (int line, char *descrip, char *string);	// ������
    void for_statement ();
    void origin_statement();
    void rot_statement();
    void scale_statement();

    /*����������Semantics�б�����Ϊ�������ã��������﷨����ʱ�ĸ��ٲ���*/
    void enter();
    void back();
    void call_match();

    /*Semantics�еķ������������﷨�Ƶ���������������庯��*/
    double get_tree_value(tree_node_ptr root);			// ��ñ��ʽ��ֵ
    void cal_coord(	tree_node_ptr hor_ptr,				// ����������
                    tree_node_ptr ver_ptr,
                    double &hor_val,
                    double &ver_val);
    void draw_pixel(unsigned long x, unsigned long y);		// ����һ����
    void draw_loop(	double start_val,				// ͼ�λ���
                    double end_val,
                    double step_val,
                    tree_node_ptr x_ptr,
                    tree_node_ptr y_ptr);
    void delete_tree(tree_node_ptr root);				// ɾ��һ����
};
#endif // SEMANTICS_H
