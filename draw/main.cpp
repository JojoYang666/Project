#include "Semantics.h"

const int size=50;//最大文件路径名
HDC hDC;				// 设备上下文句柄
static char Name[] = "绘图结果";	// 窗口名
Semantics drawer;			// 函数绘图对象，视源程序而定
char File[size];		// 用于存放源程序文件路径


LRESULT CALLBACK WndProc(HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    //窗口处理函数
    switch(Message)
    {
    case WM_DESTROY:
        ReleaseDC(hWnd,hDC);
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        PAINTSTRUCT pt;
        BeginPaint(hWnd,&pt);
        drawer.parser(File);
        EndPaint(hWnd,&pt);
    default:
        return DefWindowProc(hWnd,Message,wParam,lParam);
    }
}

bool PrepareWindow(HINSTANCE hInst,HINSTANCE hPrevInstance,int nCmdShow)
{
    //初始化窗口函数
    HWND hWnd;
    WNDCLASS W;

    memset(&W,0,sizeof(WNDCLASS));
    W.style = CS_HREDRAW | CS_VREDRAW;
    W.lpfnWndProc = WndProc;
    W.hInstance = hInst;
    W.hCursor = LoadCursor(NULL,IDC_ARROW);
    W.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    W.lpszClassName = Name;
    RegisterClass(&W);

    hWnd = CreateWindow(Name, Name, WS_OVERLAPPEDWINDOW,10,10,740,490,NULL,NULL,hInst,NULL);
    if(hWnd == NULL) return false;

    ShowWindow(hWnd,nCmdShow);
    UpdateWindow(hWnd);
    SetCursor(LoadCursor(hInst,IDC_ARROW));

    hDC = GetDC(hWnd);
    return true;
}

int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
    //window程序主函数

    std::cout << "输入文件名: ";
    std::cin >> File;//输入文件名

    if ( PrepareWindow(hInstance,hPrevInstance,nCmdShow) != true)
    {
        // 初始化窗口
        MessageBox(NULL,"窗口初始化失败 !","错误",MB_OK);
        return 1;
    }

    FILE * file = NULL;
    if((file = fopen(File, "r")) == NULL)
    {
        MessageBox(NULL,"出错啦 !", "错误", MB_OK);
        MessageBox(NULL,File, "文件名", MB_OK);
        return false;
    }
    else fclose(file);
    drawer.parser(File);//函数绘图

    /*进入window消息循环*/
    MSG Msg;
    while(GetMessage(&Msg,NULL,0,0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
