//�������� ��������� �ҷ��ͼ� ������ �ٶ�������Դϴ�.
//�������� ������ϰ� ���ҽ� ��������� �ҷ��ɴϴ�.
#include <windows.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);
HINSTANCE g_hInst;
LPCTSTR lpszClass=TEXT("Class");

//�ʿ��� �������� �����ϰ� �ʱⰪ�� ����������ϴ�.
HDC hdc;
HFONT hFont, OldFont;
HDC hMemDC[14];
HBITMAP hbrick[14];
HBITMAP brick[14];
int i;
int x = 1, y = 9;
int environment_X, environment_Y;
int iBit = 3;
int direction = 2;
int threshold_score = 0,display_score=0;
int stage = 0;

int direction_factor1 = 1,direction_factor2 = 1, direction_factor3 = 1, put_life=0;
int predetor1_X=1, predetor1_Y=7;
int obstacle_X=10, obstacle_Y=5;
int predetor2_X=18, predetor2_Y=3;
int life=3;
TCHAR sTime[100];
//������ ���� ��밡 �Ǵ� 3���� �迭�Դϴ�.	������ 2�����̸� ������ �� ������ ���������� ��Ÿ���ϴ�.
char ar[3][11][21] = {
{
	"####################",
	"#    .0           0#",
	"#####$##############",
	"#    ^     .       #",
	"### #######$########",
	"#0.        ^ .     #",
	"##$####### ##$### ##",
	"#0^ .        ^    0#",
	"####$########## ####",
	"#   ^              #",
	"####################"
},{
	"####################",
	"#0   .            0#",
	"#####$##############",
	"#    ^     .      0#",
	"### #######$########",
	"#  .       ^ .     #",
	"###$###### ##$### ##",
	"#0 ^       . ^    0#",
	"###########$### ####",
	"#          ^       #",
	"####################"
},{
	"####################",
	"#    .0           0#",
	"#####$##############",
	"#    ^     .       #",
	"### #######$########",
	"#0         ^ .     #",
	"########## ##$### ##",
	"#0  .        ^    0#",
	"####$########## ####",
	"#   ^              #",
	"####################"
	}};
	

//���⼭������ �ڵ�� ��������â�� ����ϴ�. �̺κ��� ��ȭ���ΰ� �ʿ��ϹǷ� ������ ������ �״�� �����߽��ϴ�.
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;		//������ �ڵ�
	MSG Message;		//�޽��� ����ü
	WNDCLASS WndClass;	//������ Ŭ���� ����ü
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;			//������Ŭ���� ������ ����
	WndClass.cbWndExtra=0;			//������ ������ ����
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	//������ �귯�� �ڵ�
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);			//Ŀ�� �ڵ�
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);			//������ �ڵ�
	WndClass.hInstance=hInstance;		//�ν��Ͻ� �ڵ�
	WndClass.lpfnWndProc=(WNDPROC)WndProc;	//������ ���ν��� ����
	WndClass.lpszClassName=lpszClass;	//������ Ŭ���� �̸�
	WndClass.lpszMenuName=NULL;		//�޴��̸�
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);		//������ Ŭ���� ���

	hWnd=CreateWindow(lpszClass,		//������ Ŭ���� �̸�
			lpszClass,		//Ÿ��Ʋ�ٿ� ��µɹ��ڿ�
			WS_OVERLAPPEDWINDOW,	//������ ��Ÿ��
		  	0,		//������ �»�x��ǥ
			0,		//������ �»�y��ǥ
			1024,		//������ ��
			768,		//������ ����
			NULL,			//�θ� ������ �ڵ�
			(HMENU)NULL,		//�޴� Ȥ�� �ڽ� �������� �ĺ���
			hInstance,		//�����츦 ������ �ν��Ͻ� �ڵ� 
			NULL 			//CREATESTRUCT ����ü�� ���� ���޵Ǵ� ��
			);


	if(!hWnd) return(FALSE);		//������ �������н� ����

	ShowWindow(hWnd,nCmdShow);		//������ ȭ�鿡 ��Ÿ����
	UpdateWindow(hWnd);			//������ Ŭ���̾�Ʈ ������ ĥ�Ѵ�.
	while(GetMessage(&Message,0,0,0)) {
		TranslateMessage(&Message);	//Ű���� �޽�������
		DispatchMessage(&Message);	//�޽����� �ش����ν����� ������
	}
	return Message.wParam;
}
//���⼭���Ͱ� ���� ���� ¥�� �ڵ��Դϴ�.
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch(iMessage) {
	case WM_CREATE:
		hdc=GetDC(hWnd);
      
		for(i=5;i<=13;i++)
		{
		hMemDC[i]=CreateCompatibleDC(hdc);
                // hbrick[i]=LoadBitmap(g_hInst, MAKEINTRESOURCE(first_BITMAP+i-1)); ���ҽ� ID���� ���������� ���ĵȰ��
		}
		// or
		//13���� ��Ʈ���̹����� �ҷ��ɴϴ�. 1������ 4�������� ���̸�, 5������ 13�������� ������Ʈ�̹����Դϴ�.
		hbrick[1]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
		hbrick[2]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
		hbrick[3]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
		hbrick[4]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
		hbrick[5]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
		hbrick[6]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
		hbrick[7]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
		hbrick[8]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
		hbrick[9]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP9));
		hbrick[10]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP10));
        hbrick[11]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP11));
		hbrick[12]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP12));
		hbrick[13]=LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP13));



		for(i=5;i<=13;i++)
		brick[i]=(HBITMAP)SelectObject(hMemDC[i], hbrick[i]);
		wsprintf(sTime, TEXT("Score : %d"), threshold_score);
		SetTimer(hWnd, 2, 100, NULL);
		SetTimer(hWnd, 3, 100, NULL);
		SetTimer(hWnd, 4, 1000, NULL);
		SetTimer(hWnd, 5, 100, NULL);
		ReleaseDC(hWnd,hdc);
		return 0;
		//Ÿ�̸Ӹ� ������ ���̸ӿ� �� �����̴� ������Ʈ���� �ӵ��� ������ �ݴϴ�.
	case WM_TIMER:
		switch (wParam) {
	case 1:
		y++;
		KillTimer(hWnd,1);
		break;
		//Ű���� �Է����� ����� ������ ���ؼ� ���̸��� ��ǥ�� �����Ű�� #���� ��Ÿ�� ���� �������ϰ� �ϴ� �ڵ��Դϴ�.
	case 2:
		if(direction == 1) {x--; if(ar[stage][y][x]=='#')x++;} 
		if(direction == 2) {x++; if(ar[stage][y][x]=='#')x--;}
		if(direction == 3) {y--; if(ar[stage][y][x]=='#'||ar[stage][y][x]==' ')y++;}
		if(direction == 4) {y++; if(ar[stage][y][x]=='#')y--;}
		//���̸ӿ� 0�� ��ǥ�� ��ġ�� ��� 0�� ' '�� ��ȯ��Ű�� ���������� ���÷��� ������ 1�� ������ŵ�ϴ�.
		if(ar[stage][y][x]=='0') {ar[stage][y][x] = ' ', threshold_score++,display_score++;}
		//���� ���ӿ��� ������ ��Ÿ���� �ؽ�Ʈ�� �����մϴ�.
		wsprintf(sTime, TEXT("Round %3d  Score : %3d"), stage+1,display_score*10);
		//���� ������ 5���� �ɰ�� (19,9)�� ��ǥ�� #���ε� ������ ' '���� �������ݴϴ�.
		if(threshold_score == 5 ) ar[stage][9][19] = ' ';
		//3��° ���������� �ⱸ�� Ż���� �� ������ �����մϴ�.
		if(stage == 2 && y == 9 && x == 19)PostQuitMessage(0);
		//�� ������������ �ⱸ�� �������� �� ������������ 1����, ���̸��� ��ǥ�� �Ա��� ��ġ, ���������� 0���� �ʱ�ȭ�մϴ�.
		if(y == 9 && x == 19){
			stage++, x = 1, y = 9, threshold_score = 0;}
		if(ar[stage][y+1][x]==' '){SetTimer(hWnd,1,10,NULL);y++;}
		//���� �� õ����� ��ֹ��� �������� �����մϴ�.
		if(predetor1_X == x && predetor1_Y == y){x = 1, y = 9, life--;}
		if(predetor2_X == x && predetor2_Y == y){x = 1, y = 9, life--;}
		if(obstacle_X == x && obstacle_Y == y){x = 1, y = 9, life--;}
		if(life == 0)PostQuitMessage(0);
        break;
		//�����ڵ��̳� ��ֹ��� ���̳� �ٴڿ� �ε����� �� �������� ������ ��ȯ�ϰ� ��ֹ��� ��ġ�� �ʱ�� �ǵ����ϴ�.
	case 3:
		predetor1_X+=direction_factor1;
		if(predetor1_X == 1 || predetor1_X == 18) direction_factor1*=-1;
		break;
	case 4:
        obstacle_Y+=direction_factor2;
		if(obstacle_Y == 8) obstacle_Y = 5;
		break;
	case 5:
		predetor2_X-=direction_factor3;
		if(predetor2_X == 1 || predetor2_X == 18) direction_factor3*=-1;
		break;
		}

		InvalidateRect(hWnd, NULL, 0);
		return 0;
		//���� Ű���带 ������ �� ���̸��� ����� ������ ��ȯ��ŵ�ϴ�. iBit�� ���̸��� ����� �����ϰ� direction�� ������ �����մϴ�.
    case WM_KEYDOWN:
		switch(wParam){
		case VK_LEFT:
			iBit=2;
            direction = 1;
			break;
		case VK_RIGHT:
			iBit=3;
			direction = 2;
			break;
		case VK_UP:
			iBit=4;
		    direction = 3;
			break;
		case VK_DOWN:
			iBit=4;
		    direction = 4;
			break;
				
		}
		
		return 0;
		//���Ӽ� ������Ʈ�� ��ȭ���� ��Ʈ���̹����� �ؽ�Ʈ�� �̿��� ���������� ��Ÿ���� �κ��Դϴ�.
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		//BitBlt(hdc, x*50, y*50, 50, 50, hMemDC[iBit],0,0,SRCCOPY);
		//���̸��� ����� ��Ÿ���� ��Ʈ�� �ݺ����� �̿��� ���η� �迭��ŵ�ϴ�.
		if(put_life == 0)
		{for(i=1;i<=life;i++){
		BitBlt(hdc, 50*i,550,50,50,hMemDC[12],0,0,SRCCOPY);
		}}
		put_life++;
		//�ݺ����� �̿��� �ʱ��� �迭�� ���鼭 ������ ���ڵ��� ��Ʈ���̹����� ��ȯ�մϴ�.
		for(environment_Y=0;environment_Y<11; environment_Y++){
		for(environment_X=0; environment_X <21; environment_X++){
		if(ar[stage][environment_Y][environment_X] == '#')BitBlt(hdc, environment_X*50, environment_Y*50, 50, 50, hMemDC[5], 0, 0, SRCCOPY);
		if(ar[stage][environment_Y][environment_X] == '$')BitBlt(hdc, environment_X*50, environment_Y*50, 50, 50, hMemDC[6], 0, 0, SRCCOPY);
		if(ar[stage][environment_Y][environment_X] == '^')BitBlt(hdc, environment_X*50, environment_Y*50, 50, 50, hMemDC[7], 0, 0, SRCCOPY);
		if(ar[stage][environment_Y][environment_X] == ' '||ar[stage][environment_Y][environment_X] == '.')BitBlt(hdc, environment_X*50, environment_Y *50, 50, 50, hMemDC[8], 0, 0, SRCCOPY);
		if(ar[stage][environment_Y][environment_X] == '0')BitBlt(hdc, environment_X*50, environment_Y*50, 50, 50, hMemDC[9], 0, 0, SRCCOPY);
		
		}}
		if(stage == 0 || stage == 1 || stage == 2)BitBlt(hdc, predetor1_X*50, predetor1_Y*50, 50, 50, hMemDC[10],0,0,SRCCOPY);
        if(stage == 0 || stage == 1 || stage == 2)BitBlt(hdc, obstacle_X*50, obstacle_Y*50, 50, 50, hMemDC[13],0,0,SRCCOPY);
		if(stage == 0 || stage == 1 || stage == 2)BitBlt(hdc, predetor2_X*50, predetor2_Y*50, 50, 50, hMemDC[10],0,0,SRCCOPY);
		TransBlt(hdc,x*50,y*50,hbrick[iBit],RGB(255,0,0));
		hFont=CreateFont(50,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,VARIABLE_PITCH|FF_ROMAN,TEXT("�ü�"));
		OldFont=(HFONT)SelectObject(hdc,hFont);
        TextOut(hdc, 400, 550, sTime, lstrlen(sTime));
		
		BitBlt(hdc, 50*(life+1),550,50,50,hMemDC[8],0,0,SRCCOPY);
		
		EndPaint(hWnd,&ps);
		return 0;

	case WM_DESTROY:
		for(i=5;i<=13;i++)
			SelectObject(hMemDC[i],brick[i]);
		PostQuitMessage(0);
		return 0;
	}
	return(DefWindowProc(hWnd,iMessage,wParam,lParam));
}

//Ʈ������ �Լ��Դϴ�. ��Ʈ�� �̹������� �ʿ��� �κ��̿��� �κ��� �����ϰ� ����ϴ�.
//���� ����§ �ڵ�� �ƴմϴ�.
void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask)
{
	BITMAP bm;
	COLORREF cColor;
	HBITMAP bmAndBack, bmAndObject, bmAndMem, bmSave;
	HBITMAP bmBackOld, bmObjectOld, bmMemOld, bmSaveOld;
	HDC		hdcMem, hdcBack, hdcObject, hdcTemp, hdcSave;
	POINT	ptSize;

	hdcTemp = CreateCompatibleDC(hdc);
	SelectObject(hdcTemp, hbitmap);
	GetObject(hbitmap, sizeof(BITMAP), (LPSTR)&bm);
	ptSize.x = bm.bmWidth;
	ptSize.y = bm.bmHeight;
	DPtoLP(hdcTemp, &ptSize,1);

	hdcBack   = CreateCompatibleDC(hdc);
	hdcObject = CreateCompatibleDC(hdc);
	hdcMem    = CreateCompatibleDC(hdc);
	hdcSave   = CreateCompatibleDC(hdc);

	bmAndBack   = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndObject = CreateBitmap(ptSize.x, ptSize.y, 1, 1, NULL);
	bmAndMem    = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);
	bmSave      = CreateCompatibleBitmap(hdc, ptSize.x, ptSize.y);

	bmBackOld   = (HBITMAP) SelectObject(hdcBack, bmAndBack);
	bmObjectOld = (HBITMAP) SelectObject(hdcObject, bmAndObject);
	bmMemOld    = (HBITMAP) SelectObject(hdcMem, bmAndMem);
	bmSaveOld   = (HBITMAP) SelectObject(hdcSave, bmSave);

	SetMapMode(hdcTemp, GetMapMode(hdc));

	BitBlt(hdcSave, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	cColor = SetBkColor(hdcTemp, clrMask);

	BitBlt(hdcObject, 0, 0, ptSize.x, ptSize.y, hdcTemp, 0, 0, SRCCOPY);

	SetBkColor(hdcTemp, cColor);

	BitBlt(hdcBack, 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, NOTSRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdc      , x, y, SRCCOPY);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcObject, 0, 0, SRCAND);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcBack  , 0, 0, SRCAND);
	BitBlt(hdcMem , 0, 0, ptSize.x, ptSize.y, hdcTemp  , 0, 0, SRCPAINT);
	BitBlt(hdc    , x, y, ptSize.x, ptSize.y, hdcMem   , 0, 0, SRCCOPY);
	BitBlt(hdcTemp, 0, 0, ptSize.x, ptSize.y, hdcSave  , 0, 0, SRCCOPY);

	DeleteObject(SelectObject(hdcBack, bmBackOld));
	DeleteObject(SelectObject(hdcObject, bmObjectOld));
	DeleteObject(SelectObject(hdcMem, bmMemOld));
	DeleteObject(SelectObject(hdcSave, bmSaveOld));
	
	DeleteDC(hdcMem);
	DeleteDC(hdcBack);
	DeleteDC(hdcObject);
	DeleteDC(hdcSave);
	DeleteDC(hdcTemp);
}