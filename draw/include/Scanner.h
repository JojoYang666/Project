//�ʷ�������
#ifndef SCANNER_H
#define SCANNER_H

#include <string.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstdarg>
#include <cmath>

enum token_type						// �Ǻ�����
{
    ORIGIN, SCALE,	ROT, IS,  TO,			// ������
    STEP,   DRAW,	FOR,	FROM,			// ������
    T,						// ����
    SEMICO,L_BRACKET, R_BRACKET, COMMA,		// �ָ�����
    PLUS, MINUS, MUL, DIV, POWER,			// �����
    FUNC,						// ����
    CONST_ID,					// ����
    NONTOKEN,					// �ռǺţ��ļ�������־��
    ERRTOKEN					// ����Ǻ�
};

struct Token					// �Ǻ�����ű�ṹ
{
    token_type  	type;				// �Ǻŵ����
    char*        lexeme;			// ���ɼǺŵ��ַ�����ָ��Դ����
    double		value;				// ��Ϊ���������ǳ�����ֵ
    double		(* func_ptr)(double);		// ��Ϊ���������Ǻ�����ָ��
};

static Token token_table[] =			// �����ֵ�
{
    //�Ǻ����-�ַ���-����ֵ-����ָ��
    {CONST_ID,	"PI",		3.1415926,	NULL},
    {CONST_ID,	"E",		2.71828,	NULL},
    {T,		"T",		0.0,		NULL},
    {FUNC,		"SIN",		0.0,		sin},
    {FUNC,		"COS",		0.0,		cos},
    {FUNC,		"TAN",		0.0,		tan},
    {FUNC,		"LN",		0.0,		log},
    {FUNC,		"EXP",		0.0,		exp},
    {FUNC,		"SQRT",		0.0,		sqrt},
    {ORIGIN,	"ORIGIN",	0.0,		NULL},
    {SCALE,		"SCALE",	0.0,		NULL},
    {ROT,		"ROT",		0.0,		NULL},
    {IS,		"IS",		0.0,		NULL},
    {FOR,		"FOR",		0.0,		NULL},
    {FROM,		"FROM",		0.0,		NULL},
    {TO,		"TO",		0.0,		NULL},
    {STEP,		"STEP",		0.0,		NULL},
    {DRAW,		"DRAW",		0.0,		NULL}
};
const static int token_len=50;  //��������󳤶�

class Scanner
{
protected:
    FILE *in_flie;					// �����ļ���
    char Buffer[token_len];			// �Ǻ��ַ�����

public:
    Scanner()
    {
        line_no=1;
    }     //��¼�к�Ϊ1
    ~Scanner() {}
    unsigned int line_no;				// ���ټǺ�����Դ�ļ��к�
    bool InitScanner(const char *f);		// ��ʼ���ʷ�������
    void CloseScanner(void);			// �رմʷ�������
    Token GetToken(void);			// ��ȡ�Ǻź���
private:
    char GetChar(void);				// ������Դ�����ж���һ���ַ�
    void BackChar(char temp);			// ��Ԥ�����ַ��˻ص�����Դ������
    void add_in_token_str(char temp);		// �����ַ����ǺŻ�����
    void EmptyBuffer();				// ��ռǺŻ�����
    Token Check(const char *temp);	// �ж��������ַ����Ƿ��ڷ��ű���
};

#endif // SCANNER_H
