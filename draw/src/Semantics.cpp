//����
#include "Semantics.h"

void Semantics::error_msg(int line, char * descrip, char* temp)
{//������
    const int buf_len=256;
    char msg[buf_len];
    memset(msg, 0, buf_len);
    sprintf(msg, "Line No %3d:  %s %s !",line_no, descrip, temp);
    MessageBox(NULL, msg, "����!", MB_OK);//MessageBox��ʾһ��ģ̬�Ի���
    //���а���һ��ϵͳͼ�ꡢ һ�鰴ť��һ����̵��ض���Ӧ�ó�����Ϣ����״̬��������Ϣ����Ϣ���з���һ������ֵ����ֵָʾ�û��������ĸ���ť��
    CloseScanner();
    exit(-1);
}

void Semantics::cal_coord(tree_node_ptr x_ptr,tree_node_ptr y_ptr,double &x_ret,double &y_ret)
{
    //������Ƶ������
    double x_val, x_temp, y_val;
    /*������ʽ��ֵ���õ����ԭʼ����*/
    x_val=get_tree_value(x_ptr);
    y_val=get_tree_value(y_ptr);

    x_val*=Scale_x ;
    y_val*=Scale_y ;// �����任

    x_temp=x_val*cos(rot_angle) + y_val*sin(rot_angle);
    y_val=y_val*cos(rot_angle) - x_val*sin(rot_angle);
    x_val=x_temp;// ��ת�任

    x_val+=Origin_x;
    y_val+=Origin_y; // ƽ�Ʊ任

    x_ret=x_val;
    y_ret=y_val;// ���ر任��������
}

void Semantics::draw_loop(double start_val,double end_val,double step_val,tree_node_ptr x_ptr,tree_node_ptr y_ptr)
{//ѭ�����Ƶ�����
    double x_val, y_val;
    for(parameter = start_val; parameter <= end_val; parameter += step_val)
    {
        cal_coord(x_ptr, y_ptr, x_val, y_val);//������Ƶ������
		y_val = 460-y_val;			// �޸���ʾ����ֱ������ϵ
        draw_pixel((unsigned long)x_val, (unsigned long)y_val);
    }
}

double Semantics::get_tree_value(tree_node_ptr root)
{//������ʽ��ֵ
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
{//ɾ��һ���﷨��
    if (root == NULL) return;
    switch (root -> op_code)
    {
    case PLUS  :			// �������ӵ��ڲ��ڵ�
    case MINUS :
    case MUL   :
    case DIV   :
    case POWER :
        delete_tree (root->content.tag_op.left );
        delete_tree (root->content.tag_op.right);
        break;
    case FUNC : 			// һ�����ӵ��ڲ��ڵ�
        delete_tree (root->content.tag_func.child) ;
        break;
    default: 			// Ҷ�ӽڵ�
        break ;
    }
    delete(root);			// ɾ���ڵ�
}

void Semantics::draw_pixel(unsigned long x, unsigned long y)
{//����һ����
    SetPixel(hDC, x, y, RGB(199,21,133));//�ú�����ָ�����괦��������Ϊָ������ɫ
    SetPixel(hDC, x+1, y, RGB(199,21,133));
    SetPixel(hDC, x, y+1, RGB(199,21,133));
    SetPixel(hDC, x+1, y+1, RGB(199,21,133));//������ɫ������Ϊ��ɫ

}

/*origin_statement�ĵݹ��ӳ���*/
void Semantics::origin_statement()
{
    Parser::origin_statement();		// �﷨������ȡ�﷨������parser�е����Ա��棩���û����㷨

    Origin_x = get_tree_value(x_ptr);		// �����﷨������������ƽ�ƾ���
    delete_tree(x_ptr);

    Origin_y = get_tree_value(y_ptr);		// �����﷨�������������ƽ�ƾ���
    delete_tree(y_ptr);
}

/*scale_statement�ĵݹ��ӳ���*/
void Semantics::scale_statement ()
{
    Parser::scale_statement();		// �﷨������ȡ�﷨������parser�е����Ա��棩

    Scale_x = get_tree_value(x_ptr);		// �����﷨�����������ı�������
    delete_tree(x_ptr);

    Scale_y = get_tree_value(y_ptr);		// �����﷨������������ı�������
    delete_tree(y_ptr);
}

/*rot_statement�ĵݹ��ӳ���*/
void Semantics::rot_statement ()
{
    Parser::rot_statement();			// �﷨������ȡ�﷨������parser�е����Ա��棩
    rot_angle = get_tree_value(angle_ptr);		// �����﷨��������ת�Ƕ�
    delete_tree(angle_ptr);
}

/*for_statement �ĵݹ��ӳ���*/
void Semantics::for_statement ()
{
    double start_val, end_val, step_val;		// ��ͼ��㡢�յ㡢����

    Parser::for_statement();			// �﷨������ȡ�����ʽ���﷨������parser�е����Ա��棩

    start_val = get_tree_value(start_ptr);		// ���������ʽ��ֵ
    end_val = get_tree_value(end_ptr);		// �����յ���ʽ��ֵ
    step_val = get_tree_value(step_ptr);		// ���㲽�����ʽ��ֵ

    draw_loop (start_val, end_val, step_val, x_ptr, y_ptr); // ��ͼ

    delete_tree(start_ptr);					// �ͷſռ�
    delete_tree(end_ptr);
    delete_tree(step_ptr);
    delete_tree(x_ptr);
    delete_tree(y_ptr);
}

/*�����﷨�������еĸ��ٵ���, �˴�����������*/
void Semantics::enter() {}
void Semantics::back() {}
void Semantics::call_match() {}
