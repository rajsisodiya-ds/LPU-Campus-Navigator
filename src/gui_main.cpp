#include <windows.h>
#include "Navigator.h"
#include <string>

#define ID_COMBO_FROM1   101
#define ID_COMBO_TO1     102
#define ID_BTN_ROUTE     103
#define ID_COMBO_FROM2   104
#define ID_COMBO_CAT2    105
#define ID_BTN_NEAREST   106
#define ID_EDIT_OUTPUT   107
#define ID_BANNER        108

Navigator g_navigator;
HWND hComboFrom1, hComboTo1, hComboFrom2, hComboCat2, hEditOutput, hBanner;
HWND hBtnRoute, hBtnNearest;
HBRUSH hBrushBg, hBrushBanner, hBrushOutput;
HFONT hFontTitle, hFontLabel, hFontNormal, hFontButton;

COLORREF COLOR_BANNER   = RGB(30, 60, 114);
COLORREF COLOR_BG       = RGB(240, 242, 245);
COLORREF COLOR_BUTTON   = RGB(41, 128, 185);
COLORREF COLOR_OUTPUT_BG = RGB(255, 255, 255);
COLORREF COLOR_SECTION  = RGB(52, 73, 94);

void PopulateLocationCombo(HWND hCombo) {
    for (const auto& n : g_navigator.getAllLocationNames()) {
        SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)n.c_str());
    }
}

void PopulateCategoryCombo(HWND hCombo) {
    const char* cats[] = { "hostel", "academic", "food", "facility", "admin", "residence", "utility", "gate" };
    for (const char* c : cats) SendMessageA(hCombo, CB_ADDSTRING, 0, (LPARAM)c);
}

std::string GetComboText(HWND hCombo) {
    char buffer[256];
    GetWindowTextA(hCombo, buffer, sizeof(buffer));
    return std::string(buffer);
}

void SetOutputText(const std::string& text) {
    std::string converted;
    for (char c : text) {
        if (c == '\n') converted += "\r\n";
        else converted += c;
    }
    SetWindowTextA(hEditOutput, converted.c_str());
}

BOOL CALLBACK SetLabelFont(HWND child, LPARAM lParam) {
    char cls[64];
    GetClassNameA(child, cls, sizeof(cls));
    if (lstrcmpiA(cls, "STATIC") == 0 && child != hBanner) {
        SendMessageA(child, WM_SETFONT, (WPARAM)lParam, TRUE);
    }
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        hBrushBg = CreateSolidBrush(COLOR_BG);
        hBrushBanner = CreateSolidBrush(COLOR_BANNER);
        hBrushOutput = CreateSolidBrush(COLOR_OUTPUT_BG);

        hFontTitle = CreateFontA(26, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
        hFontLabel = CreateFontA(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
        hFontNormal = CreateFontA(15, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");
        hFontButton = CreateFontA(16, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY, DEFAULT_PITCH, "Segoe UI");

        hBanner = CreateWindowA("STATIC", "LPU Campus Navigator",
            WS_VISIBLE | WS_CHILD | SS_CENTER,
            0, 0, 560, 70, hwnd, (HMENU)ID_BANNER, NULL, NULL);
        SendMessageA(hBanner, WM_SETFONT, (WPARAM)hFontTitle, TRUE);

        int leftMargin = 30;
        int y = 95;

        CreateWindowA("STATIC", "FIND SHORTEST ROUTE", WS_VISIBLE | WS_CHILD,
            leftMargin, y, 300, 22, hwnd, NULL, NULL, NULL);
        y += 35;

        CreateWindowA("STATIC", "From", WS_VISIBLE | WS_CHILD, leftMargin, y + 4, 70, 20, hwnd, NULL, NULL, NULL);
        hComboFrom1 = CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_VSCROLL,
            leftMargin + 75, y, 400, 200, hwnd, (HMENU)ID_COMBO_FROM1, NULL, NULL);
        y += 40;

        CreateWindowA("STATIC", "To", WS_VISIBLE | WS_CHILD, leftMargin, y + 4, 70, 20, hwnd, NULL, NULL, NULL);
        hComboTo1 = CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_VSCROLL,
            leftMargin + 75, y, 400, 200, hwnd, (HMENU)ID_COMBO_TO1, NULL, NULL);
        y += 48;

        hBtnRoute = CreateWindowA("BUTTON", "Find Route", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            leftMargin + 75, y, 160, 38, hwnd, (HMENU)ID_BTN_ROUTE, NULL, NULL);
        y += 60;

        CreateWindowA("STATIC", "FIND NEAREST FACILITY", WS_VISIBLE | WS_CHILD,
            leftMargin, y, 300, 22, hwnd, NULL, NULL, NULL);
        y += 35;

        CreateWindowA("STATIC", "From", WS_VISIBLE | WS_CHILD, leftMargin, y + 4, 70, 20, hwnd, NULL, NULL, NULL);
        hComboFrom2 = CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_VSCROLL,
            leftMargin + 75, y, 400, 200, hwnd, (HMENU)ID_COMBO_FROM2, NULL, NULL);
        y += 40;

        CreateWindowA("STATIC", "Category", WS_VISIBLE | WS_CHILD, leftMargin, y + 4, 70, 20, hwnd, NULL, NULL, NULL);
        hComboCat2 = CreateWindowA("COMBOBOX", "", WS_VISIBLE | WS_CHILD | CBS_DROPDOWN | WS_VSCROLL,
            leftMargin + 75, y, 400, 200, hwnd, (HMENU)ID_COMBO_CAT2, NULL, NULL);
        y += 48;

        hBtnNearest = CreateWindowA("BUTTON", "Find Nearest", WS_VISIBLE | WS_CHILD | BS_OWNERDRAW,
            leftMargin + 75, y, 160, 38, hwnd, (HMENU)ID_BTN_NEAREST, NULL, NULL);
        y += 60;

        hEditOutput = CreateWindowA("EDIT", "", WS_VISIBLE | WS_CHILD | WS_BORDER |
            ES_MULTILINE | ES_READONLY | WS_VSCROLL,
            leftMargin, y, 475, 190, hwnd, (HMENU)ID_EDIT_OUTPUT, NULL, NULL);

        HWND fontCtrls[] = { hComboFrom1, hComboTo1, hComboFrom2, hComboCat2, hEditOutput };
        for (HWND h : fontCtrls) SendMessageA(h, WM_SETFONT, (WPARAM)hFontNormal, TRUE);

        EnumChildWindows(hwnd, SetLabelFont, (LPARAM)hFontLabel);

        PopulateLocationCombo(hComboFrom1);
        PopulateLocationCombo(hComboTo1);
        PopulateLocationCombo(hComboFrom2);
        PopulateCategoryCombo(hComboCat2);

        SetOutputText("Welcome! Choose a From and To location, then click Find Route.\r\nOr choose a From location and Category, then click Find Nearest.");
        break;
    }
    case WM_CTLCOLORSTATIC: {
        HDC hdc = (HDC)wParam;
        HWND hCtrl = (HWND)lParam;
        if (hCtrl == hBanner) {
            SetTextColor(hdc, RGB(255,255,255));
            SetBkColor(hdc, COLOR_BANNER);
            return (LRESULT)hBrushBanner;
        }
        SetBkColor(hdc, COLOR_BG);
        SetTextColor(hdc, COLOR_SECTION);
        return (LRESULT)hBrushBg;
    }
    case WM_CTLCOLOREDIT: {
        HDC hdc = (HDC)wParam;
        SetBkColor(hdc, COLOR_OUTPUT_BG);
        return (LRESULT)hBrushOutput;
    }
    case WM_CTLCOLORLISTBOX: {
        HDC hdc = (HDC)wParam;
        SetBkColor(hdc, RGB(255,255,255));
        return (LRESULT)hBrushOutput;
    }
    case WM_ERASEBKGND: {
        HDC hdc = (HDC)wParam;
        RECT rc; GetClientRect(hwnd, &rc);
        FillRect(hdc, &rc, hBrushBg);
        return 1;
    }
    case WM_DRAWITEM: {
        LPDRAWITEMSTRUCT dis = (LPDRAWITEMSTRUCT)lParam;
        if (dis->CtlID == ID_BTN_ROUTE || dis->CtlID == ID_BTN_NEAREST) {
            bool pressed = (dis->itemState & ODS_SELECTED);
            HBRUSH hBtnBrush = CreateSolidBrush(pressed ? RGB(31,97,141) : COLOR_BUTTON);
            FillRect(dis->hDC, &dis->rcItem, hBtnBrush);
            DeleteObject(hBtnBrush);

            char text[64];
            GetWindowTextA(dis->hwndItem, text, sizeof(text));
            SetBkMode(dis->hDC, TRANSPARENT);
            SetTextColor(dis->hDC, RGB(255,255,255));
            SelectObject(dis->hDC, hFontButton);
            RECT rc = dis->rcItem;
            DrawTextA(dis->hDC, text, -1, &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
            return TRUE;
        }
        break;
    }
    case WM_COMMAND: {
        int id = LOWORD(wParam);
        if (id == ID_BTN_ROUTE) {
            std::string from = GetComboText(hComboFrom1);
            std::string to = GetComboText(hComboTo1);
            if (from.empty() || to.empty())
                SetOutputText("Please select both a From and To location.");
            else
                SetOutputText(g_navigator.getRouteText(from, to));
        } else if (id == ID_BTN_NEAREST) {
            std::string from = GetComboText(hComboFrom2);
            std::string cat = GetComboText(hComboCat2);
            if (from.empty() || cat.empty())
                SetOutputText("Please select both a From location and a Category.");
            else
                SetOutputText(g_navigator.getNearestFacilityText(from, cat));
        }
        break;
    }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProcA(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    if (!g_navigator.loadData("data/locations.csv", "data/edges.csv")) {
        MessageBoxA(NULL, "Failed to load campus data.", "Error", MB_OK | MB_ICONERROR);
        return 1;
    }

    const char CLASS_NAME[] = "LPUCampusNavigatorWindow";
    WNDCLASSA wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    RegisterClassA(&wc);

    HWND hwnd = CreateWindowExA(0, CLASS_NAME, "LPU Campus Navigator",
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME,
        CW_USEDEFAULT, CW_USEDEFAULT, 580, 700,
        NULL, NULL, hInstance, NULL);

    if (!hwnd) return 0;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
