//词法分析器
#include "Scanner.h"

bool Scanner::InitScanner(const char *f)
{
    //初始化词法分析器：将输入命令的文件打开
    in_flie = fopen(f, "r");
    if (in_flie != NULL)  return true;
    else                  return false;
}

void Scanner::CloseScanner(void)
{
    //关闭分析器
    if (in_flie != NULL) fclose (in_flie) ;
}

char Scanner::GetChar(void)
{
    //从源程序获得一个字符
    char temp = getc(in_flie);
    return toupper(temp);//将字符temp转换成大写字母
}

void Scanner::BackChar(char temp)
{
    //将预读取的字符退回源程序
    if (temp != EOF)   ungetc(temp, in_flie);//把一个（或多个）字符退回到文件中
}


void Scanner::add_in_token_str(char temp)
{
    //加入字符到记号缓冲区
    int token_len = strlen (Buffer);
    if (token_len + 1 >= sizeof (Buffer)) return;//判断缓冲区是否满
    Buffer[token_len]   = temp;//加入
    Buffer[token_len+1] = '\0';
}

void Scanner::EmptyBuffer()
{
    //清空缓冲区
    memset(Buffer, 0, token_len);//将前0个字节返回缓冲区
}

Token Scanner::Check(const char * temp)
{
    //判断所给的字符串是否在符号表中
    int len=sizeof(token_table)/sizeof(token_table[0]);//符号表元素数
    Token wrong;

    for (int i=0; i<len; i++)
    {
        //依次匹配符号表元素
        if (strcmp(token_table[i].lexeme, temp)==0)
            return token_table[i];
    }

    wrong.type = ERRTOKEN;
    return wrong;//返回非法记号
}

Token Scanner::GetToken(void)
{
    //获取记号
    Token token;
    char temp;

    memset(&token, 0, sizeof(Token));//清空新记号
    EmptyBuffer();//清空缓冲区
    token.lexeme = Buffer;//记录缓冲区字符串
//先把之前缓冲区的存值清空才能再加入
    while(1)
    {
        //清空开始符号
        temp = GetChar();
        if (temp == EOF)
        {
            token.type = NONTOKEN;
            return token;
        }
        if (temp == '\n')	line_no ++;
        if (!isspace(temp))	break;
    }

    add_in_token_str(temp);	//加入以上记号

    if(isalpha(temp))		// 字母记号
    {
        while(1)
        {
            temp =GetChar();
            if ( isalnum(temp) )	add_in_token_str (temp);
            else				break;
        }
        BackChar(temp);
        token = Check(Buffer);
        token.lexeme = Buffer;
        return token;
    }

    else if(isdigit(temp))		// 数字记号
    {
        while(1)
        {
            temp = GetChar() ;
            if (isdigit(temp))	add_in_token_str (temp) ;
            else			break ;
        }
        if (temp == '.')
        {
            //可能小数
            add_in_token_str (temp) ;
            while(1)
            {
                temp = GetChar() ;
                if (isdigit(temp))	add_in_token_str (temp) ;
                else			break ;
            }

        }
        BackChar(temp);
        token.type = CONST_ID;
        token.value = atof (Buffer);
        return token;
    }

    else	// 符号记号
    {
        switch (temp)
        {
        case ';' :
            token.type = SEMICO;
            break;
        case '(' :
            token.type = L_BRACKET;
            break;
        case ')' :
            token.type = R_BRACKET;
            break;
        case ',' :
            token.type = COMMA;
            break;
        case '+' :
            token.type =PLUS;
            break;
        case '-' :
            temp = GetChar();
            if (temp =='-')
            {
                //Ada注释（--）
                while (temp != '\n' && temp != EOF) temp = GetChar();
                BackChar(temp);
                return GetToken();//消除注释
            }
            else
            {
                //运算符
                BackChar(temp);
                token.type = MINUS;
                break;
            }
        case '/' :
            temp = GetChar();
            if (temp =='/')
            {
                //c++注释（//）
                while (temp != '\n' && temp != EOF) temp = GetChar();
                BackChar(temp);
                return GetToken();
            }
            else
            {
                BackChar(temp);
                token.type = DIV;
                break;
            }
        case '*' :
            temp = GetChar() ;
            if (temp == '*')
            {
                //乘方运算
                token.type = POWER ;
                add_in_token_str (temp);
            }
            else
            {
                BackChar (temp);
                token.type = MUL;
            }
            break;
        default:
            token.type = ERRTOKEN;
        }

    }

    return token;
}
