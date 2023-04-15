/*
 * SNAKE GAME
 * Developed by: AZE (https://azedev.net/)
 * Part of the Classic Games Project: (https://github.com/azedeveloper/Classic-Games-Project)
 * Copyright (C) 2023 AZE. All rights reserved.
 * This code is for educational and personal use only.
 * Redistribution or commercial use is prohibited without the author's permission.
*/

#include <windows.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define SEGMENT_SIZE 20

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void DrawSnake(HWND, const POINT*, int);
bool CheckCollision(const POINT*, int, POINT);
void DrawGameOver(HWND, int);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    MSG msg;
    WNDCLASS wc = {0};
    wc.lpszClassName = "Snake";
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
    RegisterClass(&wc);
    HWND hwnd = CreateWindow("Snake", "Snake", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 640, 480, 0, 0, hInstance, 0);
    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static POINT snake[100] = {{20, 20}, {20, 21}, {20, 22}};
    static int snake_length = 3;
    static POINT direction = {0, -1};
    static RECT rc;
    static POINT food = {2, 2};
    static int score = 0;
    static bool gameOver = false;

    switch (msg) {
        case WM_CREATE:
            SetTimer(hwnd, 1, 100, NULL);
            srand(GetTickCount()); // Add this line to initialize the random seed
            break;
        case WM_TIMER:
            if (!gameOver) {
                GetClientRect(hwnd, &rc);

                POINT new_head = snake[0];
                new_head.x += direction.x;
                new_head.y += direction.y;

                if (CheckCollision(snake, snake_length, new_head) || new_head.x < 0 || new_head.y < 0 || new_head.x >= rc.right / SEGMENT_SIZE || new_head.y >= rc.bottom / SEGMENT_SIZE) {
                    gameOver = true;
                    KillTimer(hwnd, 1);
                } else {
                    memmove(snake + 1, snake, sizeof(POINT) * snake_length);
                    snake[0] = new_head;

                    if (new_head.x == food.x && new_head.y == food.y) {
                        score++;
                        food.x = rand() % (rc.right / SEGMENT_SIZE);
                        food.y = rand() % (rc.bottom / SEGMENT_SIZE);
                        snake_length++;
                    }

                    InvalidateRect(hwnd, &rc, TRUE);
                }
            }
            break;
        case WM_KEYDOWN:
            switch (wParam) {
                case VK_UP:
                    direction = (POINT){0, -1};
                    break;
                case VK_DOWN:
                    direction = (POINT){0, 1};
                    break;
                case VK_LEFT:
                    direction = (POINT){-1, 0};
                    break;
                case VK_RIGHT:
                    direction = (POINT){1, 0};
                    break;
            }
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            if (!gameOver) {
                DrawSnake(hwnd, snake, snake_length);
                RECT food_rect = {food.x * SEGMENT_SIZE, food.y * SEGMENT_SIZE, (food.x + 1) * SEGMENT_SIZE, (food.y + 1) * SEGMENT_SIZE};
                HBRUSH food_brush = CreateSolidBrush(RGB(255, 0, 0));
                FillRect(hdc, &food_rect, food_brush);
                DeleteObject(food_brush);
            } else {
                DrawGameOver(hwnd, score);
            }
            EndPaint(hwnd, &ps);
        }
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

void DrawSnake(HWND hwnd, const POINT* snake, int length) {
    HDC hdc = GetDC(hwnd);
    HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));
    for (int i = 0; i < length; i++) {
        RECT rect = {snake[i].x * SEGMENT_SIZE, snake[i].y * SEGMENT_SIZE, (snake[i].x + 1) * SEGMENT_SIZE, (snake[i].y + 1) * SEGMENT_SIZE};
        FillRect(hdc, &rect, brush);
    }
    DeleteObject(brush);
    ReleaseDC(hwnd, hdc);
}

bool CheckCollision(const POINT* snake, int length, POINT point) {
    for (int i = 0; i < length; i++) {
        if (snake[i].x == point.x && snake[i].y == point.y) {
            return true;
        }
    }
    return false;
}

void DrawGameOver(HWND hwnd, int score) {
    HDC hdc = GetDC(hwnd);
    RECT rc;
    GetClientRect(hwnd, &rc);
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);

    char text[50];
    sprintf(text, "Game Over! Score: %d", score);
    DrawText(hdc, text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    ReleaseDC(hwnd, hdc);
}
