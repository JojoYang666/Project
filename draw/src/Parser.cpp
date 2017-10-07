//�﷨������
#include "Parser.h"

void Parser::fetch_token()
{
    //ͨ���ʷ���������üǺ�
    token=GetToken();
    if (token.type == ERRTOKEN)
        syntax_error(1);//�ʷ����󣬼��Ƿ��Ǻ�
}

void Parser::match_token (enum token_type the_token)
{
    //ƥ��Ǻ�
    if (token.type != the_token)
        syntax_error(2);//�﷨���󣬼��������﷨�߼��ļǺ�
    fetch_token();
}

void Parser::syntax_error (int case_of)
{
    //��������
    switch(case_of)
    {
    case 1:
        error_msg (line_no," �Ƿ��Ǻ� ", token.lexeme) ;
        break;
    case 2:
        error_msg (line_no, token.lexeme, " ����Ԥ�ڼǺ�") ;
        break;
    }
}

void Parser::error_msg(int line, char *descrip, char* temp)
{//�����ӡ
    std::cout << "Line No " << line << ": " << descrip << temp << std::endl;
    CloseScanner();
    exit(1);
}

bool Parser::parser(char * file_name)
{
    //��ͼ���Խ�������ڣ�����������ⲿ�ӿڣ�
    enter("parser");
    if(!InitScanner(file_name))		// ��ʼ���ʷ�������
    {
        std::cout << "���ļ����� !" << std::endl;
        return false;
    }
    fetch_token();					// ��ȡ��һ���Ǻ�
    program();					// �ݹ��½�����(������)
    CloseScanner();			// �رմʷ�������
    back("parser");
    return true;
}

void Parser::program ()
{
    //�ݹ�������
    enter("program");
    while (token.type != NONTOKEN)
    {
        statement();//�﷨�����������֣�����֤ÿ�ֵ��ʷ���Ҫ����߼� ��origin��������is
        match_token(SEMICO) ;//ƥ���¼Ǻ�
    }
    back("program");
}

void Parser::statement()
{
    //������
    enter("statement");
    switch (token.type)
    {
    case ORIGIN:
        origin_statement();
        break;
    case SCALE:
        scale_statement();
        break;
    case ROT:
        rot_statement();
        break;
    case FOR:
        for_statement();
        break;
        break;
    default:
        syntax_error(2);//�﷨����
    }
    back("statement");
}

void Parser::origin_statement()
{
    //origin_statement �ĵݹ��ӳ���
    enter("origin_statement");
    call_match("ORIGIN");
    match_token (ORIGIN);
    call_match("IS");
    match_token (IS);
    call_match("(");
    match_token (L_BRACKET);
    x_ptr = expression();
    call_match(",");
    match_token (COMMA);
    y_ptr = expression();
    call_match(")");
    match_token (R_BRACKET);
    back("origin_statement");
}

void Parser::scale_statement ()
{
    //scale_statement �ĵݹ��ӳ���
    enter("scale_statement");
    call_match("SCALE");
    match_token (SCALE);
    call_match("IS");
    match_token (IS);
    call_match("(");
    match_token (L_BRACKET);
    x_ptr = expression();
    call_match(",");
    match_token (COMMA);
    y_ptr = expression();
    call_match(")");
    match_token (R_BRACKET);
    back("scale_statement");
}

void Parser::rot_statement ()
{
    //rot_statement �ĵݹ��ӳ���
    enter("rot_statement");
    call_match("ROT");
    match_token (ROT);
    call_match("IS");
    match_token (IS);
    angle_ptr = expression();
    back("rot_statement");
}

void Parser::for_statement ()
{
    //for_statement�ĵݹ��ӳ���
    enter("for_statement");
    call_match("FOR");
    match_token (FOR);
    call_match("T");
    match_token(T);
    call_match("FROM");
    match_token (FROM);
    start_ptr = expression();	// ���������ʼ���ʽ�﷨��
    call_match("TO");
    match_token (TO);
    end_ptr = expression();		// ��������ս���ʽ�﷨��
    call_match("STEP");
    match_token (STEP);
    step_ptr = expression();	// ��������������ʽ�﷨��
    call_match("DRAW");
    match_token (DRAW);
    call_match("(");
    match_token (L_BRACKET);
    x_ptr = expression();		// �����������ʽ�﷨��
    call_match(",");
    match_token (COMMA);
    y_ptr = expression(); 		// ������������ʽ�﷨��
    call_match(")");
    match_token (R_BRACKET);
    back("for_statement");
}

tree_node_ptr Parser::expression()
{//+-
    //expression �ĵݹ��ӳ��򣨱��ʽ��Ҫ�����﷨��������������3+2�����
    tree_node_ptr left, right;			// ���������ڵ��ָ��
    token_type now;				// ��ǰ�Ǻ�

    enter("expression");
    left = term();					// ������������ҵõ����﷨��
    while (token.type==PLUS || token.type==MINUS)
    {
        now = token.type;
        match_token(now);
        right = term();				// �����Ҳ������ҵõ����﷨��
        left = make_tree_node(now, left, right);
        // ����������﷨�������Ϊ�������������ǽ�㣬�������Ǻ��ӽ��
    }
    print_syntax_tree(left, 1);				// ��ӡ���ʽ���﷨��
    back("expression");
    return left;					// �������ձ��ʽ���﷨��
}

tree_node_ptr Parser::term()
{
    //�ڵ�ĵݹ��ӳ���  */
    tree_node_ptr left, right;
    token_type now;

    left = factor();
    while (token.type==MUL || token.type==DIV)
    {
        now = token.type;
        match_token(now);
        right = factor();
        left = make_tree_node(now, left, right);
    }
    return left;
}

tree_node_ptr Parser::factor ()
{
    //�ڲ��ڵ�ݹ��ӳ���һԪ�Ӽ�Ԥ��   factor term expression�����ȼ���εݼ�
    tree_node_ptr left, right;

    if(token.type == PLUS) 			// ƥ��һԪ������
    {
        match_token(PLUS);
        right = factor();		//  ���ʽ�˻�Ϊ�����Ҳ������ı��ʽ
    }
    else if(token.type == MINUS)		// ƥ��һԪ������
    {
        match_token(MINUS); 		// ���ʽת��Ϊ��Ԫ������ı��ʽ
        right = factor();
        left = new tree_node;
        left->op_code = CONST_ID;
        left->content.tag_const = 0.0;
        right = make_tree_node(MINUS, left, right);
    }
    else right = component();		// ƥ����ս��component
    return right;
}

tree_node_ptr Parser::component()
{
    //�˷�����ĵݹ��ӳ���
    tree_node_ptr left, right;

    left = atom();
    if(token.type == POWER)
    {
        match_token(POWER);
        right = component();	// �ݹ����component��ʵ��POWER���ҽ������
        left  = make_tree_node(POWER, left, right);
    }
    return left;
}

tree_node_ptr Parser::atom()
{
    //ԭ�ӱ��ʽ�ĵݹ��ӳ���
    struct Token t=token;
    tree_node_ptr t_ptr, tmp;

    switch (token.type)
    {
    case CONST_ID:
        match_token (CONST_ID) ;
        t_ptr = make_tree_node(CONST_ID,t.value);
        break;
    case T:
        match_token(T);
        t_ptr = make_tree_node(T);
        break;
    case FUNC:
        match_token (FUNC);
        match_token (L_BRACKET);
        tmp = expression ();
        t_ptr = make_tree_node(FUNC,t.func_ptr,tmp);
        match_token (R_BRACKET);
        break ;
    case L_BRACKET:
        match_token (L_BRACKET);
        t_ptr = expression ();
        match_token (R_BRACKET);
        break ;
    default :
        syntax_error (2);
    }
    return t_ptr;
}

tree_node_ptr Parser::make_tree_node(enum token_type opcode, ...)
{
    //�����﷨��������һ���ڵ�
    tree_node_ptr t_ptr = new (struct tree_node);		// ����ڵ�洢�ռ�
    t_ptr->op_code = opcode;				// ���ռǺŵ����
    va_list arg_ptr ;
    va_start (arg_ptr, opcode);
    switch(opcode)						// ���ݼǺŵ�����첻ͬ�Ľڵ�
    {
    case CONST_ID:					// �����ڵ�
        t_ptr->content.tag_const = (double)va_arg(arg_ptr, double);
        break;
    case T:						// �����ڵ�
        t_ptr->content.tag_parameter = &parameter;
        break;
    case FUNC:					// �������ýڵ�
        t_ptr->content.tag_func.math_func_ptr = (func_ptr)va_arg(arg_ptr, func_ptr);
        t_ptr->content.tag_func.child=(tree_node_ptr)va_arg(arg_ptr, tree_node_ptr);
        break;
    default:					// ��Ԫ����ڵ�
        t_ptr->content.tag_op.left
            = (tree_node_ptr)va_arg(arg_ptr, tree_node_ptr);
        t_ptr->content.tag_op.right
            = (tree_node_ptr)va_arg(arg_ptr, tree_node_ptr);
        break;
    }
    va_end(arg_ptr);
    return t_ptr;
}

void Parser::print_syntax_tree(tree_node_ptr root, int indent)
{
    //��ӡ�﷨��������.������������ʽ indentΪ�ڵ����
    int temp;

    for (temp=1; temp<=indent; temp++)
        std::cout << "	";	// ����
    switch(root->op_code)					// ��ӡ���ڵ�
    {
    case PLUS:
        std::cout << "+" << std::endl;
        break;
    case MINUS:
        std::cout << "-" << std::endl;
        break;
    case MUL:
        std::cout << "*" << std::endl;
        break;
    case DIV:
        std::cout << "/" << std::endl;
        break;
    case POWER:
        std::cout << "**" << std::endl;
        break;
    case FUNC:
        std::cout << root->content.tag_func.math_func_ptr << std::endl;
        break;
    case CONST_ID:
        std::cout <<root->content.tag_const << std::endl;
        break;
    case T:
        std::cout << "T" << std::endl;
        break;
    default:
        std::cout << "�Ƿ������ڵ�!" << std::endl;
        exit(0);
    }

    if(root->op_code == CONST_ID || root->op_code == T) return;		// Ҷ�ӽڵ㷵��
    if(root->op_code == FUNC)						// �ݹ��ӡһ�����ӵĽڵ�
        print_syntax_tree(root->content.tag_func.child,indent+1);
    else									// �ݹ��ӡ�������ӵĽڵ�
    {
        print_syntax_tree(root->content.tag_op.left,  indent+1);
        print_syntax_tree(root->content.tag_op.right, indent+1);
    }
}

/*���������������﷨�������еĲ���*/
void Parser::enter(char * x)
{
    std::cout << "enter in  "    << x << std::endl;
}
void Parser::back(char * x)
{
    std::cout << "exit from  "   << x << std::endl;
}
void Parser::call_match(char * x)
{
    std::cout << "matchtoken " << x << std::endl;
}
