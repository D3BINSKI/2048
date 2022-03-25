// 2048Home_Winapi.cpp : Definiuje punkt wejścia dla aplikacji.
//

#include "framework.h"
#include "2048Home_Winapi.h"
#include <time.h>
#include <vector>
#include <sstream>

#define MAX_LOADSTRING 100

// Zmienne globalne:
HINSTANCE hInst;                                // bieżące wystąpienie
WCHAR szTitle[MAX_LOADSTRING];                  // Tekst paska tytułu
WCHAR szWindowClass[MAX_LOADSTRING];            // nazwa klasy okna głównego
WCHAR tileClass[MAX_LOADSTRING];            
WCHAR ScoreBoardClass[MAX_LOADSTRING];

HWND parWnd = nullptr;
HWND chWnd = nullptr;

const int tileSize = 60;
int boardSize = 4;
int gapSize = 5;

int sWidth = GetSystemMetrics(SM_CXFULLSCREEN);
int sHeight = GetSystemMetrics(SM_CYFULLSCREEN);

HWND tiles[16];
HWND tiles2[16];

HWND scores;
HWND scores2;
bool isChanged[16] = {false};
static int arr[16] = { 0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0,
                           0, 0, 0, 0 };

static int Score = 0;
static int goal = 2048;
static bool isGoal = FALSE;
const UINT16 WIDTH = 60;
const UINT16 HEIGHT = 60;

// Przekaż dalej deklaracje funkcji dołączone w tym module kodu:
ATOM                MyRegisterClass(HINSTANCE hInstance);
ATOM                RegisterTileClass(HINSTANCE hInstance);
ATOM RegisterScoreBoardClass(HINSTANCE hInstance);
BOOL                InitInstance(HWND parWnd, HWND* hWnd, HINSTANCE hInstance, int nCmdShow);
BOOL                InitTile(HWND, HWND*, HINSTANCE, int, int, int, int, int);
BOOL                InitScoreBoard(HWND, HWND*, HINSTANCE, int, int, int, int, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    TileProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ScoreBoardProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                SaveGame(HWND hWnd);
void LoadSave(HWND hWnd);
int calculateX(HWND hWnd);
void Four_Line(int* arr, int flag, bool& flag2, int length);
void Line(int& temp1, int& temp2, int& temp3, int& temp4, bool& flag);
void Scores(HWND hWnd);
void Random(HWND hWnd, int* arr, bool& flag);
void Image(HWND hWnd, int* arr);
void Button(HWND hWnd, int length);
void Destroy_Buttons(HWND hWnd, int* arr);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: W tym miejscu umieść kod.

    // Inicjuj ciągi globalne
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY2048HOMEWINAPI, szWindowClass, MAX_LOADSTRING);
    LoadStringW(hInstance, IDS_TILECLASSNAME, tileClass, MAX_LOADSTRING);
    LoadStringW(hInstance, IDS_SCOREBOARDCLASS, ScoreBoardClass, MAX_LOADSTRING);

    MyRegisterClass(hInstance);
    RegisterTileClass(hInstance);
    RegisterScoreBoardClass(hInstance);

    // Wykonaj inicjowanie aplikacji:
    
    if (!InitInstance (nullptr, &parWnd, hInstance, nCmdShow))
    {
        return FALSE;
    }
    
    if (!InitInstance(parWnd, &chWnd, hInstance, nCmdShow))
    {
        return FALSE;
    }
    
    SetWindowLong(chWnd, GWL_EXSTYLE,
        GetWindowLong(chWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    LoadSave(parWnd);
    LoadSave(chWnd);
    int length = 4;
    bool flag = 1;
    Button(parWnd, length);
    Image(parWnd, arr);
    length = 4;
    flag = 1;
    Button(chWnd, length);
    Image(chWnd, arr);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY2048HOMEWINAPI));

    MSG msg;

    // Główna pętla komunikatów:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNKCJA: MyRegisterClass()
//
//  PRZEZNACZENIE: Rejestruje klasę okna.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = CreateSolidBrush(RGB(250, 247, 238));
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY2048HOMEWINAPI);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

    return RegisterClassExW(&wcex);
}


ATOM RegisterTileClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = 0;
    wcex.lpfnWndProc = TileProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(204, 192, 174));
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = tileClass;
    wcex.hIconSm = nullptr;

    return RegisterClassExW(&wcex);
}

ATOM RegisterScoreBoardClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = 0;
    wcex.lpfnWndProc = ScoreBoardProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = 0;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = CreateSolidBrush(RGB(204, 192, 174));
    wcex.lpszMenuName = 0;
    wcex.lpszClassName = ScoreBoardClass;
    wcex.hIconSm = nullptr;

    return RegisterClassExW(&wcex);
}

//
//   FUNKCJA: InitInstance(HINSTANCE, int)
//
//   PRZEZNACZENIE: Zapisuje dojście wystąpienia i tworzy okno główne
//
//   KOMENTARZE:
//
//        W tej funkcji dojście wystąpienia jest zapisywane w zmiennej globalnej i
//        jest tworzone i wyświetlane okno główne programu.
//
BOOL InitInstance(HWND parWnd, HWND *hWnd, HINSTANCE hInstance, int nCmdShow)
{
   srand((unsigned int)time(NULL));

   hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

   *hWnd = CreateWindowEx(/*WS_EX_TOOLWINDOW*/ 0, szWindowClass, szTitle, WS_OVERLAPPEDWINDOW | WS_VISIBLE,
      (sWidth-WIDTH)/2, (sHeight-HEIGHT)/2, WIDTH*5-30, HEIGHT*6, parWnd, nullptr, hInstance, nullptr);

   if (!*hWnd)
   {
      return FALSE;
   }

   
   ShowWindow(*hWnd, nCmdShow);
   UpdateWindow(*hWnd);

   return TRUE;
}


BOOL InitTile(HWND hWnd, HWND* hChldWnd, HINSTANCE hInstance, int nCmdShow, int x, int y, int width = tileSize, int heigth = tileSize/*, int i = -1*/)
{
    const int chSize = 25;
    RECT rc;
    GetWindowRect(hWnd, &rc);
    hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

    *hChldWnd = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,
        tileClass,
        (LPCTSTR)NULL,
        WS_CHILD,
        x, y, width, heigth,
        hWnd,
        0,
        hInst,
        NULL);

    if (!*hChldWnd)
    {
        return FALSE;
    }

    ShowWindow(*hChldWnd, nCmdShow);
    UpdateWindow(*hChldWnd);

    return TRUE;
}

BOOL InitScoreBoard(HWND hWnd, HWND* hChldWnd, HINSTANCE hInstance, int nCmdShow, int x, int y, int width = tileSize, int heigth = tileSize/*, int i = -1*/)
{
    const int chSize = 25;
    RECT rc;
    GetWindowRect(hWnd, &rc);
    hInst = hInstance; // Przechowuj dojście wystąpienia w naszej zmiennej globalnej

    *hChldWnd = CreateWindowEx(WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE,
        ScoreBoardClass,
        (LPCTSTR)NULL,
        WS_CHILD,
        x, y, width, heigth,
        hWnd,
        0,
        hInst,
        NULL);

    if (!*hChldWnd)
    {
        return FALSE;
    }

    ShowWindow(*hChldWnd, nCmdShow);
    UpdateWindow(*hChldWnd);
    return TRUE;
}


//
//  FUNKCJA: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PRZEZNACZENIE: Przetwarza komunikaty dla okna głównego.
//
//  WM_COMMAND  - przetwarzaj menu aplikacji
//  WM_PAINT    - Maluj okno główne
//  WM_DESTROY  - opublikuj komunikat o wyjściu i wróć
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    static bool flag;
    static int length;

    switch (message)
    {
    case WM_CREATE:
    {
        SetWindowLong(hWnd, GWL_STYLE, GetWindowLong(hWnd, GWL_STYLE) & ~WS_SIZEBOX);

        return 0;
    }
    case WM_COMMAND:
    {
        switch (LOWORD(wParam))
        {
        case ID_OPTIONS_NEWGAME:
        {
            length = 4;
            flag = 1;
            Destroy_Buttons(hWnd, arr);
            Button(hWnd, length);
            Random(hWnd, arr, flag);
            Image(hWnd, arr);
            break;
        }
        case ID_OPTIONS_SAVE:
        {
            SaveGame(hWnd);
            break;
        }
        case ID_OPTIONS_LOADSAVE:
        {
            LoadSave(hWnd);
            break;
        }
        case ID_OPTIONS_EXIT:
        {
            SaveGame(hWnd);
            SendMessage(hWnd, WM_DESTROY, 0, 0);
        }
        case ID_GOAL_8:
        {
            MessageBox(hWnd, L"Nowy cel: 8 [BUŁKA Z MASŁEM]", L"Zmiana celu gry", MB_OK);
            SendMessage(hWnd, WM_COMMAND, (WPARAM)ID_OPTIONS_NEWGAME, 0);
            goal = 8;
            break;
        }
        case ID_GOAL_16:
        {
            MessageBox(hWnd, L"Nowy cel: 16 [ŁATWIZNA]", L"Zmiana celu gry", MB_OK);
            SendMessage(hWnd, WM_COMMAND, (WPARAM)ID_OPTIONS_NEWGAME, 0);
            goal = 16;
            break;
        }
        case ID_GOAL_64:
        {
            MessageBox(hWnd, L"Nowy cel: 64 [DOBRA ROZGRZEWKA]", L"Zmiana celu gry", MB_OK);
            SendMessage(hWnd, WM_COMMAND, (WPARAM)ID_OPTIONS_NEWGAME, 0);
            goal = 64;
            break;
        }
        case ID_GOAL_2048:
        {
            MessageBox(hWnd, L"Nowy cel: 2048 [MOCNY ZAWODNIK 0.0]", L"Zmiana celu gry", MB_OK);
            SendMessage(hWnd, WM_COMMAND, (WPARAM)ID_OPTIONS_NEWGAME, 0);
            goal = 2048;
            break;
        }
        }
    }
    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
        {
            Four_Line(arr, 1, flag, length);
            Random(hWnd, arr, flag);
            Image(hWnd, arr);
            Scores(hWnd);
        }

        break;
        case VK_RIGHT:
        {
            Four_Line(arr, 2, flag, length);
            Random(hWnd, arr, flag);
            Image(hWnd, arr);
            Scores(hWnd);
        }
        break;
        case VK_UP:
        {
            Four_Line(arr, 3, flag, length);
            Random(hWnd, arr, flag);
            Image(hWnd, arr);
            Scores(hWnd);
        }
        break;
        case VK_DOWN:
        {
            Four_Line(arr, 4, flag, length);
            Random(hWnd, arr, flag);
            Image(hWnd, arr);
            Scores(hWnd);
        }
        break;
        }
        break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // TODO: Tutaj dodaj kod rysujący używający elementu hdc...
        RECT rc;
        GetWindowRect(hWnd, &rc);

        int x = (int)wParam;
        int y = (int)lParam;

        MoveWindow(hWnd, sWidth/2 + x - (rc.right - rc.left)/2, sHeight / 2 + y - (rc.bottom-rc.top)/2, rc.right - rc.left, rc.bottom - rc.top, TRUE);
        
        if (abs(x) < abs((rc.right - rc.left) / 2)-8 && abs(y) < abs((rc.bottom - rc.top) / 2)-4)
        {
            SetLayeredWindowAttributes(chWnd, 0, (255 * 10) / 100, LWA_ALPHA);
            ShowWindow(chWnd, SW_SHOW);
            UpdateWindow(chWnd);
        }
        else
        {
            SetLayeredWindowAttributes(chWnd, 0, 255, LWA_ALPHA);
            ShowWindow(chWnd, SW_SHOW);
            UpdateWindow(chWnd);
        }
       
        EndPaint(hWnd, &ps);

        break;
    }
    case WM_MOVING:
    {
        RECT rc;
        
        GetWindowRect(hWnd, &rc);
        
        if (hWnd == chWnd) SendMessage(parWnd, WM_PAINT, (WPARAM)(sWidth/2 - (rc.left+rc.right)/2) , (LPARAM)(sHeight/2 - (rc.top+rc.bottom)/2));
        if (hWnd == parWnd) SendMessage(chWnd, WM_PAINT, (WPARAM)(sWidth / 2 - (rc.left + rc.right) / 2), (LPARAM)(sHeight / 2 - (rc.top + rc.bottom) / 2));

        break;
    }
    case WM_DESTROY:
        SaveGame(hWnd);
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

#define BMP_WIDTH 60
#define BMP_HEIGHT 60

LRESULT CALLBACK TileProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc, hmemdc;
        HBITMAP hbitmap;
        RECT rt;

        int x = calculateX(hWnd);
        int value = arr[x];
        hdc = BeginPaint(hWnd, &ps);

            GetClientRect(hWnd, &rt);
            hbitmap = LoadBitmap(hInst, MAKEINTRESOURCE(value));
            hmemdc = CreateCompatibleDC(hdc);
            SelectObject(hmemdc, hbitmap);
            BitBlt(hdc, (rt.right - rt.left) / 2 - BMP_WIDTH / 2,
                (rt.bottom - rt.top) / 2 - BMP_HEIGHT / 2,
                BMP_WIDTH, BMP_HEIGHT, hmemdc, 0, 0, SRCCOPY);
            DeleteObject(hbitmap);
            DeleteDC(hmemdc);
        

        EndPaint(hWnd, &ps);
        break;
    }
    /*case WM_DESTROY:
        PostQuitMessage(0);
        break;*/
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

LRESULT ScoreBoardProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;

        HDC hdc = BeginPaint(hWnd, &ps);
        wchar_t buf[30];
        if (isGoal)
        {
            _stprintf_s(buf, 30, _T("Jesteś Zwycięzcą! :)"));
        }
        else 
        {
            _stprintf_s(buf, 30, _T("%d"), Score);
        }
        RECT rc;
        GetClientRect(hWnd, &rc);
        SetBkMode(hdc, TRANSPARENT);
        DrawText(hdc, buf, (int)_tcslen(buf), &rc,
            DT_CENTER | DT_VCENTER | DT_SINGLELINE);
        EndPaint(hWnd, &ps);

        break;
    }
    /*case WM_DESTROY:
        PostQuitMessage(0);
        break;*/
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void Destroy_Buttons(HWND hWnd, int* arr)           //https://github.com/Gektorgit/WinApi-2048
{
    for (int i = 0; i < 16; i++)
    {
        arr[i] = 0;
    }
    Score = 0;
    isGoal = FALSE;
}

void Button(HWND hWnd, int length)
{
    if (tiles[0] != nullptr) return;
    for (int i = 0; i < length; i++)
    {
        for (int j = 0; j < length; j++)
        {
            InitTile(parWnd, &tiles[j + i * length], hInst, SW_SHOW, 9 + (j * WIDTH), HEIGHT + (i * HEIGHT), WIDTH, HEIGHT);
            InitTile(chWnd, &tiles2[j + i * length], hInst, SW_SHOW, 9 + (j * WIDTH), HEIGHT + (i * HEIGHT), WIDTH, HEIGHT);

            SetWindowRgn(tiles[j + i * length], CreateRoundRectRgn(0, 0, 0.95 * WIDTH, 0.95 * HEIGHT, WIDTH / 6, HEIGHT / 6), TRUE);
            SetWindowRgn(tiles2[j + i * length], CreateRoundRectRgn(0, 0, 0.95 * WIDTH, 0.95 * HEIGHT, WIDTH / 6, HEIGHT / 6), TRUE);

        }
    }
    InitScoreBoard(parWnd, &scores, hInst, SW_SHOW, 9, 0, 4 * WIDTH, HEIGHT);
    InitScoreBoard(chWnd, &scores2, hInst, SW_SHOW, 9, 0, 4 * WIDTH, HEIGHT);
    SetWindowRgn(scores, CreateRoundRectRgn(0, 0, 4 * WIDTH, HEIGHT, WIDTH / 4, HEIGHT / 4), TRUE);
    SetWindowRgn(scores2, CreateRoundRectRgn(0, 0, 4 * WIDTH, HEIGHT, WIDTH / 4, HEIGHT / 4), TRUE);
}

void Image(HWND hWnd, int* arr)
{
    for (int i = 0; i < 16; i++)
    {
        /*if (isChanged[i])
        {*/
        InvalidateRect(tiles[i], NULL, TRUE);
        InvalidateRect(tiles2[i], NULL, TRUE);
        /*  isChanged[i] = false;
      }*/
    }
}

void Random(HWND hWnd, int* arr, bool& flag)
{
    if (flag)
    {
        std::vector <int> vect;
        for (int i = 0; i < 16; i++)
        {
            if (arr[i] == 0)
                vect.push_back(i);
        }

        int random = rand() % vect.size();
        int count = vect[random];
        isChanged[count] = true;
        for (int i = 0; i < 16; i++)
        {
            if (count == i)
                arr[i] = 2 * (rand() % 2 + 1);
        }
    }
    else
    {
        if (arr[0] != 0 && arr[1] != 0 && arr[2] != 0 && arr[3] != 0 &&
            arr[4] != 0 && arr[5] != 0 && arr[6] != 0 && arr[7] != 0 &&
            arr[8] != 0 && arr[9] != 0 && arr[10] != 0 && arr[11] != 0 &&
            arr[12] != 0 && arr[13] != 0 && arr[14] != 0 && arr[15] != 0)
        {
            MessageBox(hWnd, L"GAME OVER", L"", MB_OK);
        }
    }
    flag = 0;
}

void Scores(HWND hWnd)
{
    InvalidateRect(scores, NULL, TRUE);
    InvalidateRect(scores2, NULL, TRUE);
}


void Line(int& temp1, int& temp2, int& temp3, int& temp4, bool& flag)
{
#pragma region O
    if (temp1 == 0)
    {
        if (temp2 != 0)
        {
            temp1 = temp2;
            temp2 = 0;
            flag = 1;
        }
        else if (temp3 != 0)
        {
            temp1 = temp3;
            temp3 = 0;
            flag = 1;
        }
        else if (temp4 != 0)
        {
            temp1 = temp4;
            temp4 = 0;
            flag = 1;
        }
    }
    if (temp2 == 0)
    {
        if (temp3 != 0)
        {
            temp2 = temp3;
            temp3 = 0;
            flag = 1;
        }
        else if (temp4 != 0)
        {
            temp2 = temp4;
            temp4 = 0;
            flag = 1;
        }
    }
    if (temp3 == 0)
    {
        if (temp4 != 0)
        {
            temp3 = temp4;
            temp4 = 0;
            flag = 1;
        }
    }
#pragma endregion
    if (temp1 > 0)
    {
        if (temp1 == temp2)
        {
            if ((temp1 = temp1 + temp2) == goal) isGoal = TRUE;
            Score += temp1;
            temp2 = temp3;
            temp3 = temp4;
            temp4 = 0;
            flag = 1;
            if (temp2 == temp3 && temp2 > 0)
            {
                if ((temp2 = temp2 + temp3) == goal) isGoal = TRUE;
                Score += temp2;
                temp3 = 0;
                flag = 1;
            }
        }
        else if (temp2 == temp3 && temp2 > 0)
        {
            if ((temp2 = temp2 + temp3) == goal) isGoal = TRUE;
            Score += temp2;
            temp3 = temp4;
            temp4 = 0;
            flag = 1;
        }
        else if (temp3 == temp4 && temp3 > 0)
        {
            if ((temp3 = temp3 + temp4) == goal) isGoal = TRUE;
            Score += temp3;
            temp4 = 0;
            flag = 1;
        }
    }
}

void Four_Line(int* arr, int flag, bool& flag2, int length)   
{
    if (flag == 1)
    {
        Line(arr[0], arr[1], arr[2], arr[3], flag2);
        Line(arr[4], arr[5], arr[6], arr[7], flag2);
        Line(arr[8], arr[9], arr[10], arr[11], flag2);
        Line(arr[12], arr[13], arr[14], arr[15], flag2);
    }
    else if (flag == 2)
    {
        Line(arr[3], arr[2], arr[1], arr[0], flag2);
        Line(arr[7], arr[6], arr[5], arr[4], flag2);
        Line(arr[11], arr[10], arr[9], arr[8], flag2);
        Line(arr[15], arr[14], arr[13], arr[12], flag2);
    }
    else if (flag == 3)
    {
        Line(arr[0], arr[4], arr[8], arr[12], flag2);
        Line(arr[1], arr[5], arr[9], arr[13], flag2);
        Line(arr[2], arr[6], arr[10], arr[14], flag2);
        Line(arr[3], arr[7], arr[11], arr[15], flag2);
    }
    else if (flag == 4)
    {
        Line(arr[12], arr[8], arr[4], arr[0], flag2);
        Line(arr[13], arr[9], arr[5], arr[1], flag2);
        Line(arr[14], arr[10], arr[6], arr[2], flag2);
        Line(arr[15], arr[11], arr[7], arr[3], flag2);
    }
}


int calculateX(HWND hWnd)
{
    for (int i = 0; i < 16; i++)
    {
        if (tiles[i] == hWnd || tiles2[i] == hWnd) return i;
    }
    return -1;
}

void LoadSave(HWND hWnd)
{
    HKEY hkSoftware;
    LONG result;
    HKEY hkTest;
    DWORD dwDisp;

    Destroy_Buttons(hWnd, arr);

    result = RegOpenKeyEx(HKEY_CURRENT_USER, L"software", 0, KEY_ALL_ACCESS, &hkSoftware);
    if (result != ERROR_SUCCESS)
    {
        MessageBox(hWnd, L"Błąd odczytu zapisu.", L"Test", MB_ICONINFORMATION);
        return;
    }


    result = RegCreateKeyEx(hkSoftware, L"test", 0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, &hkTest, &dwDisp);
    if (result != ERROR_SUCCESS)
    {
        MessageBox(hWnd, L"Błąd odczytu zapisu.", L"Test", MB_ICONINFORMATION);
        return;
    }

    wchar_t buf[101];
    DWORD dwBufSize = 100;
    DWORD dwRegsz = REG_SZ;
    result = RegQueryValueEx(hkTest, L"ValueArray", NULL, &dwRegsz, (LPBYTE)buf, &dwBufSize);
    if (result != ERROR_SUCCESS) {
        MessageBox(hWnd, L"Błąd odczytu zapisu.", L"Test", MB_ICONINFORMATION);
        return;
    }

    int i = 0;
    int j = 0;
    while (buf[i] != '\0' && (int)buf[i] < 10000)
    {
        if (j < 16) {
            if (buf[i] == ' ')
            {
                j++; i++;
                continue;
            }

            arr[j] = arr[j] * 10 + ((int)buf[i] - 48);
            i++;
        }
        else
        {
            if (buf[i] == ' ')
            {
                i++;
                continue;
            }
            Score = 10 * Score + ((int)buf[i] - 48);
            i++;
        }
    }

    int length = 4;
    bool flag = 1;
    Button(hWnd, length);
    Image(hWnd, arr);
}

void SaveGame(HWND hWnd)
{
    HKEY hkSoftware;
    LONG result;
    HKEY hkTest;
    DWORD dwDisp;

    std::string str;
    for (int i : arr) {
        str += std::to_string(i);
        str += " ";
    }
    str += std::to_string(Score);
    str += " ";
    str += "\0";

    result = RegOpenKeyEx(HKEY_CURRENT_USER, L"software", 0, KEY_ALL_ACCESS, &hkSoftware);
    if (result != ERROR_SUCCESS)
    {
        MessageBox(hWnd, L"Błąd zapisu.", L"Test", MB_ICONINFORMATION);
        return;
    }


    result = RegCreateKeyEx(hkSoftware, L"test", 0, NULL, REG_OPTION_NON_VOLATILE,
        KEY_ALL_ACCESS, NULL, &hkTest, &dwDisp);
    if (result != ERROR_SUCCESS)
    {
        MessageBox(hWnd, L"Błąd zapisu.", L"Test", MB_ICONINFORMATION);
        return;
    }

    wchar_t buf[100];
    int i = 0;
    for (i = 0; i < str.size() && str[i] != '\0'; i++)
    {
        buf[i] = str[i];
    }

    result = RegSetValueEx(hkTest, L"ValueArray", 0, REG_SZ, (LPBYTE)buf, 100);
    if (result == ERROR_SUCCESS)
        MessageBox(hWnd, L"Zapisano stan gry.", L"Test", MB_ICONINFORMATION);
}
