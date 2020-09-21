//윈도우즈 헤더파일을 불러와서 제작한 다람쥐게임입니다.
//윈도우즈 헤더파일과 리소스 헤더파일을 불러옵니다.
#include <windows.h>
#include "resource.h"
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void TransBlt(HDC hdc, int x, int y, HBITMAP hbitmap, COLORREF clrMask);
HINSTANCE g_hInst;
LPCTSTR lpszClass=TEXT("Class");

//필요한 변수들을 선언하고 초기값을 설정해줬습니다.
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
//게임의 맵의 토대가 되는 3차원 배열입니다.	게임은 2차원이며 나머지 한 차원은 스테이지를 나타냅니다.
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
	

//여기서부터의 코드는 윈도우즈창을 만듭니다. 이부분은 심화공부가 필요하므로 교재의 내용을 그대로 복사했습니다.
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpszCmdParam,int nCmdShow)
{
	HWND hWnd;		//윈도우 핸들
	MSG Message;		//메시지 구조체
	WNDCLASS WndClass;	//윈도우 클래스 구조체
	g_hInst=hInstance;
	
	WndClass.cbClsExtra=0;			//윈도우클레스 데이터 영역
	WndClass.cbWndExtra=0;			//윈도우 데이터 영역
	WndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);	//바탕색 브러쉬 핸들
	WndClass.hCursor=LoadCursor(NULL,IDC_ARROW);			//커서 핸들
	WndClass.hIcon=LoadIcon(NULL,IDI_APPLICATION);			//아이콘 핸들
	WndClass.hInstance=hInstance;		//인스턴스 핸들
	WndClass.lpfnWndProc=(WNDPROC)WndProc;	//윈도우 프로시저 지정
	WndClass.lpszClassName=lpszClass;	//윈도우 클래스 이름
	WndClass.lpszMenuName=NULL;		//메뉴이름
	WndClass.style=CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&WndClass);		//윈도우 클래스 등록

	hWnd=CreateWindow(lpszClass,		//윈도우 클래스 이름
			lpszClass,		//타이틀바에 출력될문자열
			WS_OVERLAPPEDWINDOW,	//윈도우 스타일
		  	0,		//윈도우 좌상x좌표
			0,		//윈도우 좌상y좌표
			1024,		//윈도우 폭
			768,		//윈도우 높이
			NULL,			//부모 윈도우 핸들
			(HMENU)NULL,		//메뉴 혹은 자식 윈도우의 식별자
			hInstance,		//윈도우를 생성한 인스턴스 핸들 
			NULL 			//CREATESTRUCT 구조체를 통해 전달되는 값
			);


	if(!hWnd) return(FALSE);		//윈도우 생성실패시 종료

	ShowWindow(hWnd,nCmdShow);		//윈도우 화면에 나타내기
	UpdateWindow(hWnd);			//윈도우 클라이언트 영역을 칠한다.
	while(GetMessage(&Message,0,0,0)) {
		TranslateMessage(&Message);	//키보드 메시지번역
		DispatchMessage(&Message);	//메시지를 해당프로시저로 보낸다
	}
	return Message.wParam;
}
//여기서부터가 제가 직접 짜낸 코드입니다.
LRESULT CALLBACK WndProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT ps;
	switch(iMessage) {
	case WM_CREATE:
		hdc=GetDC(hWnd);
      
		for(i=5;i<=13;i++)
		{
		hMemDC[i]=CreateCompatibleDC(hdc);
                // hbrick[i]=LoadBitmap(g_hInst, MAKEINTRESOURCE(first_BITMAP+i-1)); 리소스 ID값이 연속적으로 정렬된경우
		}
		// or
		//13개의 비트맵이미지를 불러옵니다. 1번부터 4번까지는 게이머, 5번부터 13번까지는 오브젝트이미지입니다.
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
		//타이머를 설정해 게이머와 각 움직이는 오브젝트들의 속도를 조절해 줍니다.
	case WM_TIMER:
		switch (wParam) {
	case 1:
		y++;
		KillTimer(hWnd,1);
		break;
		//키보드 입력으로 저장된 변수를 비교해서 게이머의 좌표를 변경시키고 #으로 나타낸 벽을 넘지못하게 하는 코드입니다.
	case 2:
		if(direction == 1) {x--; if(ar[stage][y][x]=='#')x++;} 
		if(direction == 2) {x++; if(ar[stage][y][x]=='#')x--;}
		if(direction == 3) {y--; if(ar[stage][y][x]=='#'||ar[stage][y][x]==' ')y++;}
		if(direction == 4) {y++; if(ar[stage][y][x]=='#')y--;}
		//게이머와 0의 좌표가 일치할 경우 0을 ' '로 전환시키고 문턱점수와 디스플레이 점수를 1씩 증가시킵니다.
		if(ar[stage][y][x]=='0') {ar[stage][y][x] = ' ', threshold_score++,display_score++;}
		//실제 게임에서 점수를 나타내는 텍스트를 설정합니다.
		wsprintf(sTime, TEXT("Round %3d  Score : %3d"), stage+1,display_score*10);
		//문턱 점수가 5점이 될경우 (19,9)의 좌표의 #으로된 벽돌을 ' '으로 제거해줍니다.
		if(threshold_score == 5 ) ar[stage][9][19] = ' ';
		//3번째 스테이지의 출구를 탈출할 때 게임을 종료합니다.
		if(stage == 2 && y == 9 && x == 19)PostQuitMessage(0);
		//각 스테이지마다 출구에 도착했을 때 스테이지값을 1증가, 게이머의 좌표를 입구에 위치, 문턱점수를 0으로 초기화합니다.
		if(y == 9 && x == 19){
			stage++, x = 1, y = 9, threshold_score = 0;}
		if(ar[stage][y+1][x]==' '){SetTimer(hWnd,1,10,NULL);y++;}
		//게임 속 천적들과 장애물의 움직임을 설정합니다.
		if(predetor1_X == x && predetor1_Y == y){x = 1, y = 9, life--;}
		if(predetor2_X == x && predetor2_Y == y){x = 1, y = 9, life--;}
		if(obstacle_X == x && obstacle_Y == y){x = 1, y = 9, life--;}
		if(life == 0)PostQuitMessage(0);
        break;
		//포식자들이나 장애물이 벽이나 바닥에 부딪혔을 때 포식자의 방향을 전환하고 장애물의 위치를 초기로 되돌립니다.
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
		//방향 키보드를 눌렀을 때 게이머의 모습과 방향을 전환시킵니다. iBit는 게이머의 모습을 저장하고 direction은 방향을 저장합니다.
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
		//게임속 오브젝트와 변화들을 비트맵이미지와 텍스트를 이용해 시작적으로 나타내는 부분입니다.
	case WM_PAINT:
		hdc=BeginPaint(hWnd,&ps);
		//BitBlt(hdc, x*50, y*50, 50, 50, hMemDC[iBit],0,0,SRCCOPY);
		//게이머의 목숨을 나타내는 하트를 반복문을 이용해 가로로 배열시킵니다.
		if(put_life == 0)
		{for(i=1;i<=life;i++){
		BitBlt(hdc, 50*i,550,50,50,hMemDC[12],0,0,SRCCOPY);
		}}
		put_life++;
		//반복문을 이용해 맵구현 배열을 돌면서 다음의 문자들을 비트맵이미지로 전환합니다.
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
		hFont=CreateFont(50,0,0,0,0,0,0,0,HANGEUL_CHARSET,0,0,0,VARIABLE_PITCH|FF_ROMAN,TEXT("궁서"));
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

//트랜스블릿 함수입니다. 비트맵 이미지에서 필요한 부분이외의 부분을 투명하게 만듭니다.
//제가 직접짠 코드는 아닙니다.
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