//词法分析器
#ifndef SCANNER_H
#define SCANNER_H

#include <string.h>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cctype>
#include <cstdarg>
#include <cmath>

enum token_type						// 记号种类
{
    ORIGIN, SCALE,	ROT, IS,  TO,			// 保留字
    STEP,   DRAW,	FOR,	FROM,			// 保留字
    T,						// 参数
    SEMICO,L_BRACKET, R_BRACKET, COMMA,		// 分隔符号
    PLUS, MINUS, MUL, DIV, POWER,			// 运算符
    FUNC,						// 函数
    CONST_ID,					// 常数
    NONTOKEN,					// 空记号（文件结束标志）
    ERRTOKEN					// 出错记号
};

struct Token					// 记号与符号表结构
{
    token_type  	type;				// 记号的类别
    char*        lexeme;			// 构成记号的字符串（指向源程序）
    double		value;				// 若为常数，则是常数的值
    double		(* func_ptr)(double);		// 若为函数，则是函数的指针
};

static Token token_table[] =			// 符号字典
{
    //记号类别-字符串-常数值-函数指针
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
const static int token_len=50;  //缓冲区最大长度

class Scanner
{
protected:
    FILE *in_flie;					// 输入文件流
    char Buffer[token_len];			// 记号字符缓冲

public:
    Scanner()
    {
        line_no=1;
    }     //记录行号为1
    ~Scanner() {}
    unsigned int line_no;				// 跟踪记号所在源文件行号
    bool InitScanner(const char *f);		// 初始化词法分析器
    void CloseScanner(void);			// 关闭词法分析器
    Token GetToken(void);			// 获取记号函数
private:
    char GetChar(void);				// 从输入源程序中读入一个字符
    void BackChar(char temp);			// 把预读的字符退回到输入源程序中
    void add_in_token_str(char temp);		// 加入字符到记号缓冲区
    void EmptyBuffer();				// 清空记号缓冲区
    Token Check(const char *temp);	// 判断所给的字符串是否在符号表中
};

#endif // SCANNER_H
