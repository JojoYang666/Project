//�ʷ�������
#include "Scanner.h"

bool Scanner::InitScanner(const char *f)
{
    //��ʼ���ʷ���������������������ļ���
    in_flie = fopen(f, "r");
    if (in_flie != NULL)  return true;
    else                  return false;
}

void Scanner::CloseScanner(void)
{
    //�رշ�����
    if (in_flie != NULL) fclose (in_flie) ;
}

char Scanner::GetChar(void)
{
    //��Դ������һ���ַ�
    char temp = getc(in_flie);
    return toupper(temp);//���ַ�tempת���ɴ�д��ĸ
}

void Scanner::BackChar(char temp)
{
    //��Ԥ��ȡ���ַ��˻�Դ����
    if (temp != EOF)   ungetc(temp, in_flie);//��һ�����������ַ��˻ص��ļ���
}


void Scanner::add_in_token_str(char temp)
{
    //�����ַ����ǺŻ�����
    int token_len = strlen (Buffer);
    if (token_len + 1 >= sizeof (Buffer)) return;//�жϻ������Ƿ���
    Buffer[token_len]   = temp;//����
    Buffer[token_len+1] = '\0';
}

void Scanner::EmptyBuffer()
{
    //��ջ�����
    memset(Buffer, 0, token_len);//��ǰ0���ֽڷ��ػ�����
}

Token Scanner::Check(const char * temp)
{
    //�ж��������ַ����Ƿ��ڷ��ű���
    int len=sizeof(token_table)/sizeof(token_table[0]);//���ű�Ԫ����
    Token wrong;

    for (int i=0; i<len; i++)
    {
        //����ƥ����ű�Ԫ��
        if (strcmp(token_table[i].lexeme, temp)==0)
            return token_table[i];
    }

    wrong.type = ERRTOKEN;
    return wrong;//���طǷ��Ǻ�
}

Token Scanner::GetToken(void)
{
    //��ȡ�Ǻ�
    Token token;
    char temp;

    memset(&token, 0, sizeof(Token));//����¼Ǻ�
    EmptyBuffer();//��ջ�����
    token.lexeme = Buffer;//��¼�������ַ���
//�Ȱ�֮ǰ�������Ĵ�ֵ��ղ����ټ���
    while(1)
    {
        //��տ�ʼ����
        temp = GetChar();
        if (temp == EOF)
        {
            token.type = NONTOKEN;
            return token;
        }
        if (temp == '\n')	line_no ++;
        if (!isspace(temp))	break;
    }

    add_in_token_str(temp);	//�������ϼǺ�

    if(isalpha(temp))		// ��ĸ�Ǻ�
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

    else if(isdigit(temp))		// ���ּǺ�
    {
        while(1)
        {
            temp = GetChar() ;
            if (isdigit(temp))	add_in_token_str (temp) ;
            else			break ;
        }
        if (temp == '.')
        {
            //����С��
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

    else	// ���żǺ�
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
                //Adaע�ͣ�--��
                while (temp != '\n' && temp != EOF) temp = GetChar();
                BackChar(temp);
                return GetToken();//����ע��
            }
            else
            {
                //�����
                BackChar(temp);
                token.type = MINUS;
                break;
            }
        case '/' :
            temp = GetChar();
            if (temp =='/')
            {
                //c++ע�ͣ�//��
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
                //�˷�����
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
