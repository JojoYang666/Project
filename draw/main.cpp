#include "Semantics.h"

const int size=50;//����ļ�·����
HDC hDC;				// �豸�����ľ��
static char Name[] = "��ͼ���";	// ������
Semantics drawer;			// ������ͼ������Դ�������
char File[size];		// ���ڴ��Դ�����ļ�·��


LRESULT CALLBACK WndProc(HWND hWnd,UINT Message,WPARAM wParam,LPARAM lParam)
{
    //���ڴ�����
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
    //��ʼ�����ں���
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
    //window����������

    std::cout << "�����ļ���: ";
    std::cin >> File;//�����ļ���

    if ( PrepareWindow(hInstance,hPrevInstance,nCmdShow) != true)
    {
        // ��ʼ������
        MessageBox(NULL,"���ڳ�ʼ��ʧ�� !","����",MB_OK);
        return 1;
    }

    FILE * file = NULL;
    if((file = fopen(File, "r")) == NULL)
    {
        MessageBox(NULL,"������ !", "����", MB_OK);
        MessageBox(NULL,File, "�ļ���", MB_OK);
        return false;
    }
    else fclose(file);
    drawer.parser(File);//������ͼ

    /*����window��Ϣѭ��*/
    MSG Msg;
    while(GetMessage(&Msg,NULL,0,0))
    {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
    return Msg.wParam;
}
