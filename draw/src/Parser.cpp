//语法分析器
#include "Parser.h"

void Parser::fetch_token()
{
    //通过词法分析器获得记号
    token=GetToken();
    if (token.type == ERRTOKEN)
        syntax_error(1);//词法错误，即非法记号
}

void Parser::match_token (enum token_type the_token)
{
    //匹配记号
    if (token.type != the_token)
        syntax_error(2);//语法错误，即不属于语法逻辑的记号
    fetch_token();
}

void Parser::syntax_error (int case_of)
{
    //错误处理函数
    switch(case_of)
    {
    case 1:
        error_msg (line_no," 非法记号 ", token.lexeme) ;
        break;
    case 2:
        error_msg (line_no, token.lexeme, " 不是预期记号") ;
        break;
    }
}

void Parser::error_msg(int line, char *descrip, char* temp)
{//错误打印
    std::cout << "Line No " << line << ": " << descrip << temp << std::endl;
    CloseScanner();
    exit(1);
}

bool Parser::parser(char * file_name)
{
    //绘图语言解释器入口（与主程序的外部接口）
    enter("parser");
    if(!InitScanner(file_name))		// 初始化词法分析器
    {
        std::cout << "打开文件错误 !" << std::endl;
        return false;
    }
    fetch_token();					// 获取第一个记号
    program();					// 递归下降分析(主程序)
    CloseScanner();			// 关闭词法分析器
    back("parser");
    return true;
}

void Parser::program ()
{
    //递归主程序
    enter("program");
    while (token.type != NONTOKEN)
    {
        statement();//语法分析（保留字），保证每种单词符合要求的逻辑 如origin后面必须跟is
        match_token(SEMICO) ;//匹配新记号
    }
    back("program");
}

void Parser::statement()
{
    //语句分析
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
        syntax_error(2);//语法错误
    }
    back("statement");
}

void Parser::origin_statement()
{
    //origin_statement 的递归子程序
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
    //scale_statement 的递归子程序
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
    //rot_statement 的递归子程序
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
    //for_statement的递归子程序
    enter("for_statement");
    call_match("FOR");
    match_token (FOR);
    call_match("T");
    match_token(T);
    call_match("FROM");
    match_token (FROM);
    start_ptr = expression();	// 构造参数起始表达式语法树
    call_match("TO");
    match_token (TO);
    end_ptr = expression();		// 构造参数终结表达式语法树
    call_match("STEP");
    match_token (STEP);
    step_ptr = expression();	// 构造参数步长表达式语法树
    call_match("DRAW");
    match_token (DRAW);
    call_match("(");
    match_token (L_BRACKET);
    x_ptr = expression();		// 构造横坐标表达式语法树
    call_match(",");
    match_token (COMMA);
    y_ptr = expression(); 		// 构造纵坐标表达式语法树
    call_match(")");
    match_token (R_BRACKET);
    back("for_statement");
}

tree_node_ptr Parser::expression()
{//+-
    //expression 的递归子程序（表达式需要构造语法树）对于类似于3+2的情况
    tree_node_ptr left, right;			// 左右子树节点的指针
    token_type now;				// 当前记号

    enter("expression");
    left = term();					// 分析左操作数且得到其语法树
    while (token.type==PLUS || token.type==MINUS)
    {
        now = token.type;
        match_token(now);
        right = term();				// 分析右操作数且得到其语法树
        left = make_tree_node(now, left, right);
        // 构造运算的语法树，结果为左子树，数字是结点，操作符是孩子结点
    }
    print_syntax_tree(left, 1);				// 打印表达式的语法树
    back("expression");
    return left;					// 返回最终表达式的语法树
}

tree_node_ptr Parser::term()
{
    //节点的递归子程序  */
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
    //内部节点递归子程序一元加减预算   factor term expression的优先级逐次递减
    tree_node_ptr left, right;

    if(token.type == PLUS) 			// 匹配一元加运算
    {
        match_token(PLUS);
        right = factor();		//  表达式退化为仅有右操作数的表达式
    }
    else if(token.type == MINUS)		// 匹配一元减运算
    {
        match_token(MINUS); 		// 表达式转化为二元减运算的表达式
        right = factor();
        left = new tree_node;
        left->op_code = CONST_ID;
        left->content.tag_const = 0.0;
        right = make_tree_node(MINUS, left, right);
    }
    else right = component();		// 匹配非终结符component
    return right;
}

tree_node_ptr Parser::component()
{
    //乘方运算的递归子程序
    tree_node_ptr left, right;

    left = atom();
    if(token.type == POWER)
    {
        match_token(POWER);
        right = component();	// 递归调用component以实现POWER的右结合性质
        left  = make_tree_node(POWER, left, right);
    }
    return left;
}

tree_node_ptr Parser::atom()
{
    //原子表达式的递归子程序
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
    //构造语法生成树的一个节点
    tree_node_ptr t_ptr = new (struct tree_node);		// 分配节点存储空间
    t_ptr->op_code = opcode;				// 接收记号的类别
    va_list arg_ptr ;
    va_start (arg_ptr, opcode);
    switch(opcode)						// 根据记号的类别构造不同的节点
    {
    case CONST_ID:					// 常数节点
        t_ptr->content.tag_const = (double)va_arg(arg_ptr, double);
        break;
    case T:						// 参数节点
        t_ptr->content.tag_parameter = &parameter;
        break;
    case FUNC:					// 函数调用节点
        t_ptr->content.tag_func.math_func_ptr = (func_ptr)va_arg(arg_ptr, func_ptr);
        t_ptr->content.tag_func.child=(tree_node_ptr)va_arg(arg_ptr, tree_node_ptr);
        break;
    default:					// 二元运算节点
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
    //打印语法树（先序）.仅针对算数表达式 indent为节点层数
    int temp;

    for (temp=1; temp<=indent; temp++)
        std::cout << "	";	// 缩进
    switch(root->op_code)					// 打印根节点
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
        std::cout << "非法的树节点!" << std::endl;
        exit(0);
    }

    if(root->op_code == CONST_ID || root->op_code == T) return;		// 叶子节点返回
    if(root->op_code == FUNC)						// 递归打印一个孩子的节点
        print_syntax_tree(root->content.tag_func.child,indent+1);
    else									// 递归打印两个孩子的节点
    {
        print_syntax_tree(root->content.tag_op.left,  indent+1);
        print_syntax_tree(root->content.tag_op.right, indent+1);
    }
}

/*辅助函数，用于语法分析器中的测试*/
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
