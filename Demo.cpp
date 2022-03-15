# include <Windows.h>
# include <stdio.h>
# include <time.h>
# include <stdlib.h>
#include<Mmsystem.h>		// Winmm.lib

# define MYTIMER 1
#define TIME 1


//Macros For Mahraj
# define TOTALPOINTS 35498
# define CONTOURS 81
# define MAXPOINTS 8060


HWND hwnd;
FILE* fp , * fp2 , *fp3;
HDC hdc;
RECT rc;
PAINTSTRUCT ps;
HPEN hPen;

HBRUSH hbrush[20];
int** matrix = NULL;  

int i, mIndex, iter, num;


int contoursSize[CONTOURS];
int contoursHalfSize[CONTOURS];
int contoursHalfSizeEmpty[CONTOURS];
int masterContourPts[TOTALPOINTS];

//structure for Maharaj

POINT polygon[MAXPOINTS], polygon1[MAXPOINTS];

int polyIndex, polyIndex1;



LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK paint(HWND, UINT, UINT_PTR, DWORD);          //Timer For Maharaj

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow) {

	WNDCLASSEX wndClass;
	HWND hWnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("Aniket");

	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpszClassName = szAppName;
	wndClass.lpszMenuName = NULL;
	wndClass.hInstance = hInstance;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hIcon = LoadIcon(NULL, IDI_ASTERISK);
	wndClass.hIconSm = LoadIcon(NULL, IDI_ASTERISK);
	wndClass.hCursor = LoadCursor(NULL, IDC_HAND);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

	RegisterClassEx(&wndClass);

	hWnd = CreateWindow(szAppName,
		TEXT("SHIVAJI MAHARAJ DEMO BY ANIKET PATIL"),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		1800,
		1000,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}

void InitialiseArrays() {

	static int i = 0;

	//Initialising 2D Array

	matrix = (int**)malloc(CONTOURS * sizeof(int*));

	if (matrix == NULL) {

		MessageBox(hwnd, TEXT("matrix alloation failed"), TEXT("MESSAGE"),MB_OK);
		DestroyWindow(hwnd);
	}

	//Taking cotours size from file

	int ret = fopen_s(&fp, "C:\\Users\\anike\\source\\repos\\shivaji\\size.txt", "r") ;

	if (fp == NULL) {
		MessageBox(hwnd, TEXT("size.txt opening failed"), TEXT("Message"), MB_OK);
		DestroyWindow(hwnd);
	}

	while (fscanf_s(fp, "%d", &num) != EOF) {
		contoursSize[i] = num;
		matrix[i] = (int*)malloc(num * sizeof(int));
		contoursHalfSize[i++] = num / 2;
	}

	//taking all points from the file

	int ret1 = fopen_s(&fp2, "C:\\Users\\anike\\source\\repos\\shivaji\\Master.txt", "r");

	if (fp2 == NULL) {
		MessageBox(hwnd, TEXT("masterfile.txt file opening failed"), TEXT("Message"), MB_OK);
		DestroyWindow(hwnd);
	}

	while (fscanf_s(fp2, "%d", &num) != EOF) {
		masterContourPts[mIndex++] = num + 10;
	}

	//Checking All points filled in Array are correct

	int ret2 = fopen_s(&fp3, "C:\\Users\\anike\\source\\repos\\shivaji\\MasterOk.txt", "w");

	if (fp3 == NULL) {
		MessageBox(hwnd, TEXT("masterfileIsOk.txt opening failed"), TEXT("Message"), MB_OK);
		DestroyWindow(hwnd);
	}

	mIndex = 0;

	for (int i = 0; i < CONTOURS; i++) {
		for (int j = 0; j < contoursSize[i]; j++) {
			matrix[i][j] = masterContourPts[mIndex];
			mIndex++;
		}
	}


	for (int i = 0; i < CONTOURS; i++) {
		for (int j = 0; j < contoursSize[i]; j++) {
			fprintf(fp3, "%d  ", matrix[i][j]);
		}
	}


	if (fp) {
		fclose(fp);
		fp = NULL;
	}

	if (fp2) {
		fclose(fp2);
		fp2 = NULL;
	}

	if (fp3) {
		fclose(fp3);
		fp3 = NULL;
	}
}

void CALLBACK paint(HWND hwnd, UINT iMsg, UINT_PTR timerEvent, DWORD ms) {

	iter += 2;
	KillTimer(hwnd, MYTIMER);
	hdc = GetDC(hwnd);
	SelectObject(hdc, hPen);

	static int x = 2;

   //#######################################################################################################
	if (contoursHalfSizeEmpty[0] <= contoursHalfSize[0]) {					// C1
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;

			for (int i = 0; i < contoursSize[0]; i += 2) {
				polygon[polyIndex].x = matrix[0][i];
				polygon[polyIndex].y = matrix[0][i + 1];
				polyIndex++;
			}

		}
		Ellipse(hdc, matrix[0][i] - x, matrix[0][i + 1] - x, matrix[0][i] + x, matrix[0][i + 1] + x);
		contoursHalfSizeEmpty[0]++;
		i += 2;
	}

	else if (contoursHalfSizeEmpty[1] <= contoursHalfSize[1]) {				// C2
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;
			polyIndex = 0;
			for (int i = 0; i < contoursSize[1]; i += 2) {
				polygon[polyIndex].x = matrix[1][i];
				polygon[polyIndex].y = matrix[1][i + 1];
				polyIndex++;
			}

		}
		Ellipse(hdc, matrix[1][i] - x, matrix[1][i + 1] - x, matrix[1][i] + x, matrix[1][i + 1] + x);
		contoursHalfSizeEmpty[1]++;
		i += 2;
	}

	else if (contoursHalfSizeEmpty[2] <= contoursHalfSize[2]) {				// C4
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;

			polyIndex = 0;

			for (int i = 0; i < contoursSize[2]; i += 2) {
				polygon[polyIndex].x = matrix[2][i];
				polygon[polyIndex].y = matrix[2][i + 1];
				polyIndex++;
			}


		}
		Ellipse(hdc, matrix[2][i] - x, matrix[2][i + 1] - x, matrix[2][i] + x, matrix[2][i + 1] + x);
		contoursHalfSizeEmpty[2]++;
		i += 2;
	}

	else if (contoursHalfSizeEmpty[3] <= contoursHalfSize[3]) {				// C5
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;

			polyIndex = 0;
			for (int i = 0; i < contoursSize[3]; i += 2) {
				polygon[polyIndex].x = matrix[3][i];
				polygon[polyIndex].y = matrix[3][i + 1];
				polyIndex++;
			}


		}
		Ellipse(hdc, matrix[3][i] - x, matrix[3][i + 1] - x, matrix[3][i] + x, matrix[3][i + 1] + x);
		contoursHalfSizeEmpty[3]++;
		i += 2;
	}

	else if (contoursHalfSizeEmpty[4] <= contoursHalfSize[4]) {				// C6
		static int once = 1;
		if (once) {
			once = 0;
			i = 0;

			polyIndex = 0;
			for (int i = 0; i < contoursSize[4]; i += 2) {
				polygon[polyIndex].x = matrix[4][i];
				polygon[polyIndex].y = matrix[4][i + 1];
				polyIndex++;
			}


		}
		Ellipse(hdc, matrix[4][i] - x, matrix[4][i + 1] - x, matrix[4][i] + x, matrix[4][i + 1] + x);
		contoursHalfSizeEmpty[4]++;
		i += 2;
	}

	else if (contoursHalfSizeEmpty[5] <= contoursHalfSize[5]) {				// C7
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;

		polyIndex = 0;


		for (int i = 0; i < contoursSize[5]; i += 2) {
			polygon[polyIndex].x = matrix[5][i];
			polygon[polyIndex].y = matrix[5][i + 1];
			polyIndex++;
		}


	}
	Ellipse(hdc, matrix[5][i] - x, matrix[5][i + 1] - x, matrix[5][i] + x, matrix[5][i + 1] + x);
	contoursHalfSizeEmpty[2]++;
	i += 2;
	}

	else if (contoursHalfSizeEmpty[6] <= contoursHalfSize[6]) {				// C8
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;

		polyIndex = 0;

		for (int i = 0; i < contoursSize[6]; i += 2) {
			polygon[polyIndex].x = matrix[6][i];
			polygon[polyIndex].y = matrix[6][i + 1];
			polyIndex++;
		}


	}
	Ellipse(hdc, matrix[6][i] - x, matrix[6][i + 1] - x, matrix[6][i] + x, matrix[6][i + 1] + x);
	contoursHalfSizeEmpty[6]++;
	i += 2;
	}

	else if (contoursHalfSizeEmpty[7] <= contoursHalfSize[7]) {				// C9
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;

		polyIndex = 0;

		for (int i = 0; i < contoursSize[7]; i += 2) {
			polygon[polyIndex].x = matrix[7][i];
			polygon[polyIndex].y = matrix[7][i + 1];
			polyIndex++;
		}


	}
	Ellipse(hdc, matrix[7][i] - x, matrix[7][i + 1] - x, matrix[7][i] + x, matrix[7][i + 1] + x);
	contoursHalfSizeEmpty[7]++;
	i += 2;
	}

	else if (contoursHalfSizeEmpty[8] <= contoursHalfSize[8]) {				// C10
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;

		polyIndex = 0;

		for (int i = 0; i < contoursSize[8]; i += 2) {
			polygon[polyIndex].x = matrix[8][i];
			polygon[polyIndex].y = matrix[2][i + 1];
			polyIndex++;
		}


	}
	Ellipse(hdc, matrix[8][i] - x, matrix[8][i + 1] - x, matrix[8][i] + x, matrix[8][i + 1] + x);
	contoursHalfSizeEmpty[8]++;
	i += 2;
	}

	else if (contoursHalfSizeEmpty[9] <= contoursHalfSize[9]) {				// C11
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;

		polyIndex = 0;



		for (int i = 0; i < contoursSize[9]; i += 2) {
			polygon[polyIndex].x = matrix[9][i];
			polygon[polyIndex].y = matrix[9][i + 1];
			polyIndex++;
		}


	}
	Ellipse(hdc, matrix[9][i] - x, matrix[9][i + 1] - x, matrix[9][i] + x, matrix[9][i + 1] + x);
	contoursHalfSizeEmpty[9]++;
	i += 2;
	}

	else if (contoursHalfSizeEmpty[10] <= contoursHalfSize[10]) {				// C12
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;

		polyIndex = 0;

		SelectObject(hdc, hbrush[1]);
		Polygon(hdc, polygon, contoursHalfSize[1]);


		for (int i = 0; i < contoursSize[10]; i += 2) {
			polygon[polyIndex].x = matrix[10][i];
			polygon[polyIndex].y = matrix[10][i + 1];
			polyIndex++;
		}


	}
	Ellipse(hdc, matrix[10][i] - x, matrix[10][i + 1] - x, matrix[10][i] + x, matrix[10][i + 1] + x);
	contoursHalfSizeEmpty[10]++;
	i += 2;
	}

	else if (contoursHalfSizeEmpty[11] <= contoursHalfSize[11]) {				// C13
	static int once = 1;
	if (once) {
		once = 0;
		i = 0;

		polyIndex = 0;


		for (int i = 0; i < contoursSize[11]; i += 2) {
			polygon[polyIndex].x = matrix[11][i];
			polygon[polyIndex].y = matrix[11][i + 1];
			polyIndex++;
		}


	}
	Ellipse(hdc, matrix[11][i] - x, matrix[11][i + 1] - x, matrix[11][i] + x, matrix[11][i + 1] + x);
	contoursHalfSizeEmpty[11]++;
	i += 2;
	}
	//####################################################
	if (iter < TOTALPOINTS) {
		SetTimer(hwnd, MYTIMER, TIME, paint);
	}
	else {
		KillTimer(hwnd, MYTIMER);
	}
	
	ReleaseDC(hwnd, hdc);
}

void InitialiseBrushes() {
	hPen = CreatePen(PS_SOLID, 1, RGB(207, 83, 0));

	hbrush[0] = CreateSolidBrush(RGB(255,215,0)) ;    // C2 && c4 && c6 && c9  (golden)  

	hbrush[1] = CreateSolidBrush(RGB(218, 165, 32)) ; // C3                 (yellow)

	hbrush[2] = CreateSolidBrush(RGB(176, 224, 230)); // (c72 && c76 && c81) && c5 &&  c14 && c15 && c16 && c17             (akkashi)

	hbrush[3] = CreateSolidBrush(RGB(255, 228, 181)); // c7 && c10 && c13 && c63    (skin)

	hbrush[4] = CreateSolidBrush(RGB(255, 182, 193)); //c8 && c12  (light pink)

	hbrush[5] = CreateSolidBrush(RGB(139, 69, 19)); // c11 && c19   && c61       (saddle Brown)

	hbrush[6] = CreateSolidBrush(RGB(240, 230, 140)); //c35 && c73 && c75 && c79 && c80 && c78 && c77 && c74 && c73  (Khaki)

	hbrush[7] = CreateSolidBrush(RGB(255, 0, 0));     // c71 && 71 (Red)

	hbrush[8] = CreateSolidBrush(RGB(0, 0, 0)); // c66 && c65 && c66 && c69 (black)

	hbrush[9] = CreateSolidBrush(RGB(244, 164, 96)); // c64  && c29(dark brown)

	 
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT ps;

	switch (iMsg) {
	case WM_CREATE:
		//SelectObject(hdc, hPen);
		//fopen_s(&fp2, "C:\\Users\\anike\source\\repos\\shivaji\\Master.txt", "w");
		//hPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
		//fillingaray();
		PlaySound(TEXT("sound4.wav"), NULL, SND_ASYNC);
		InitialiseBrushes();

		InitialiseArrays();
		
		SetTimer(hwnd, MYTIMER, TIME, paint);

		//draw(hWnd);
		break;

	case WM_PAINT:
		//hdc = BeginPaint(hWnd, &ps);
		//EndPaint(hWnd, &ps);
		break;

	case WM_RBUTTONDOWN:

		break;

	case WM_DESTROY:
		PostQuitMessage(0);

		break;

	}

	return (DefWindowProc(hwnd, iMsg, wParam, lParam));
}
