/*
 * PONG GAME
 * Developed by: AZE (https://azedev.net/)
 * Part of the Classic Games Project: (https://github.com/azedeveloper/Classic-Games-Project)
 * Copyright (C) 2023 AZE. All rights reserved.
 * This code is for educational and personal use only.
 * Redistribution or commercial use is prohibited without the author's permission.
 */

#include <windows.h>
#include <stdio.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawPong(HWND, int, int, int, int, int, int);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wc = {0};
    wc.lpszClassName = "Pong";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    wc.hIcon = (HICON)LoadImage(NULL, "black_icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE | LR_DEFAULTSIZE | LR_SHARED);
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("Pong", "Pong", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 640, 480, 0, 0, hInstance, 0);
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)wc.hIcon);
    SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)wc.hIcon);
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}


LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static int p1_y = 20, p2_y = 20, ball_x = 295, ball_y = 215;
    static int ball_dx = 2, ball_dy = 2;
    static int p1_score = 0, p2_score = 0;
    static RECT rc;
    switch (msg) {
        case WM_CREATE:
            SetTimer(hwnd, 1, 10, NULL);
            break;
        case WM_TIMER:
            GetClientRect(hwnd, &rc);
            ball_x += ball_dx;
            ball_y += ball_dy;

            if (ball_y <= 0 || ball_y >= rc.bottom - 20) {
                ball_dy = -ball_dy;
            }

            if (ball_x <= 30 && ball_y >= p1_y - 20 && ball_y <= p1_y + 80) {
                ball_dx = -ball_dx;
            }

            if (ball_x >= rc.right - 50 && ball_y >= p2_y - 20 && ball_y <= p2_y + 80) {
                ball_dx = -ball_dx;
            }

            if (ball_x <= 0) {
                p2_score++;
                ball_x = 295;
                ball_y = 215;
            }

            if (ball_x >= rc.right) {
                p1_score++;
                ball_x = 295;
                ball_y = 215;
            }

            InvalidateRect(hwnd, &rc, TRUE);
            break;
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_UP:
                    if (p2_y > 0) {
                        p2_y -= 5;
                    }
                    break;
                case VK_DOWN:
                    if (p2_y < rc.bottom - 80) {
                        p2_y += 5;
                    }
                    break;
                case 'W':
                    if (p1_y > 0) {
                        p1_y -= 5;
                    }
                    break;
                case 'S':
                    if (p1_y < rc.bottom - 80) {
                        p1_y += 5;
                    }
                    break;
            }
            break;
        case WM_PAINT:
            DrawPong(hwnd, p1_y, p2_y, ball_x, ball_y, p1_score, p2_score);
            break;
        case WM_DESTROY:
            KillTimer(hwnd, 1);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void DrawPong(HWND hwnd, int p1_y, int p2_y, int ball_x, int ball_y, int p1_score, int p2_score) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    HBRUSH hbr = CreateSolidBrush(RGB(255, 255, 255));
    SelectObject(hdc, hbr);
    Rectangle(hdc, 20, p1_y, 30, p1_y + 80);   
    Rectangle(hdc, 610, p2_y, 620, p2_y + 80); 
    Rectangle(hdc, ball_x, ball_y, ball_x + 20, ball_y + 20); 
    char score_text[16];
    sprintf_s(score_text, sizeof(score_text), "P1: %d  P2: %d", p1_score, p2_score);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    TextOut(hdc, 280, 10, score_text, strlen(score_text));
    DeleteObject(hbr);
    EndPaint(hwnd, &ps);
}
