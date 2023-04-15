#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define ID_RESET 1

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawBoard(HDC);
void CheckForWinner(HWND);
int CheckForVictory();
void MakeAIMove(HWND);

int board[3][3] = {0};
int currentPlayer = 1;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wc = {0};
    wc.lpszClassName = "TicTacToe";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("TicTacToe", "Tic Tac Toe", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 640, 480, 0, 0, hInstance, 0);

    srand(time(NULL));

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND resetBtn;

    switch (msg) {
        case WM_CREATE:
            resetBtn = CreateWindow("button", "Reset", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 500, 10, 100, 30, hwnd, (HMENU)ID_RESET, NULL, NULL);
            break;
        case WM_COMMAND:
            if (LOWORD(wParam) == ID_RESET) {
                memset(board, 0, sizeof(board));
                currentPlayer = 1;
                InvalidateRect(hwnd, NULL, TRUE);
            }
            break;
        case WM_PAINT:
            {
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                DrawBoard(hdc);
                EndPaint(hwnd, &ps);
            }
            break;
        case WM_LBUTTONDOWN:
            {
                int x = LOWORD(lParam) / 100;
                int y = HIWORD(lParam) / 100;
                if (x < 3 && y < 3 && board[y][x] == 0 && currentPlayer == 1) {
                    board[y][x] = currentPlayer;
                    currentPlayer = 3 - currentPlayer;
                    InvalidateRect(hwnd, NULL, TRUE);
                    CheckForWinner(hwnd);
                    if (currentPlayer == 2) {
                        Sleep(1000);
                        MakeAIMove(hwnd);
                    }
                }
            }
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

void DrawBoard(HDC hdc) {
    HPEN hPen = CreatePen(PS_SOLID, 3, RGB(255, 255, 255));
    HGDIOBJ hOldPen = SelectObject(hdc, hPen);

    for (int i = 0; i < 4; i++) {
        MoveToEx(hdc, 100 * i, 0, NULL);
        LineTo(hdc, 100 * i, 300);
        MoveToEx(hdc, 0, 100 * i, NULL);
        LineTo(hdc, 300, 100 * i);
    }

    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == 1) {
                MoveToEx(hdc, x * 100 + 10, y * 100 + 10, NULL);
                LineTo(hdc, x * 100 + 90, y * 100 + 90);
                MoveToEx(hdc, x * 100 + 90, y * 100 + 10, NULL);
                LineTo(hdc, x * 100 + 10, y * 100 + 90);
            } else if (board[y][x] == 2) {
                Ellipse(hdc, x * 100 + 10, y * 100 + 10, x * 100 + 90, y * 100 + 90);
            }
        }
    }

    SelectObject(hdc, hOldPen);
    DeleteObject(hPen);
}

void CheckForWinner(HWND hwnd) {
    int winner = CheckForVictory();
    if (winner) {
        if (winner == 1) {
            MessageBox(hwnd, "X wins!", "Game Over", MB_OK);
        } else if (winner == 2) {
            MessageBox(hwnd, "O wins!", "Game Over", MB_OK);
        } else {
            MessageBox(hwnd, "It's a draw!", "Game Over", MB_OK);
        }
        memset(board, 0, sizeof(board));
        currentPlayer = 1;
        InvalidateRect(hwnd, NULL, TRUE);
    }
}

int CheckForVictory() {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            return board[i][0];
        }
        if (board[0][i] && board[0][i] == board[1][i] && board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }
    if (board[0][0] && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        return board[0][0];
    }
    if (board[0][2] && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        return board[0][2];
}

   bool isFull = true;
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y][x] == 0) {
                isFull = false;
                break;
            }
        }
    }
    return isFull ? 3 : 0;
}

void MakeAIMove(HWND hwnd) {
    int x, y;
    do {
        x = rand() % 3;
        y = rand() % 3;
    } while (board[y][x] != 0);

    board[y][x] = currentPlayer;
    currentPlayer = 3 - currentPlayer;
    InvalidateRect(hwnd, NULL, TRUE);
    CheckForWinner(hwnd);
}
