#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include <string>
#include <vector>
#include "ChatSystem.h"

// Auto-link libraries
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")

// --- Global State ---
ChatSystem* chatSystem;
enum Screen { WELCOME, LOGIN, SIGNUP, FORGET_PASS, DASHBOARD, ADMIN_DASHBOARD };
Screen currentScreen = WELCOME;

// --- UI Resources ---
HFONT hFontTitle, hFontText;
HBRUSH hBrushBg;
#define BG_COLOR RGB(240, 242, 245) // Light gray/blue modern background

// --- Controls ---
// We keep handles global for simplicity in this Win32 wrapper
HWND hTitle, hSubTitle;

// Welcome Screen
HWND hBtnWelcomeLogin, hBtnWelcomeSignup, hBtnWelcomeExit;

// Login Screen
HWND hEditLoginUser, hEditLoginPass;
HWND hLblLoginUser, hLblLoginPass;
HWND hBtnLoginSubmit, hBtnLoginBack, hBtnLoginForget, hBtnLoginExit;

// Signup Screen
HWND hEditSignupUser, hEditSignupPhone, hEditSignupPass;
HWND hLblSignupUser, hLblSignupPhone, hLblSignupPass;
HWND hBtnSignupSubmit, hBtnSignupBack, hBtnSignupExit;

// Forget Password Screen
HWND hEditForgetUser, hEditForgetPhone, hEditForgetNewPass;
HWND hLblForgetUser, hLblForgetPhone, hLblForgetNewPass;
HWND hBtnForgetSubmit, hBtnForgetBack;

// Dashboard
HWND hLblDashUser;
HWND hEditDashRecipient, hEditDashMsg;
HWND hLblDashRecipient, hLblDashMsg;
HWND hBtnDashSend, hBtnDashInbox, hBtnDashLogout, hBtnDashExit;

// Admin Dashboard
HWND hBtnAdminList, hEditAdminDeleteUser, hBtnAdminDelete, hBtnAdminLogout;
HWND hLblAdminDeleteUser;

// IDs
#define ID_BASE 100
#define ID_W_LOGIN 101
#define ID_W_SIGNUP 102
#define ID_W_EXIT 103
#define ID_L_SUBMIT 201
#define ID_L_BACK 202
#define ID_L_FORGET 203
#define ID_L_EXIT 204
#define ID_S_SUBMIT 301
#define ID_S_BACK 302
#define ID_S_EXIT 303
#define ID_F_SUBMIT 401
#define ID_F_BACK 402
#define ID_D_SEND 501
#define ID_D_INBOX 502
#define ID_D_LOGOUT 503
#define ID_D_EXIT 504
#define ID_A_LIST 601
#define ID_A_DELETE 602
#define ID_A_LOGOUT 603

// Helper to center windows
void CenterWindow(HWND hwnd) {
    RECT rc;
    GetWindowRect(hwnd, &rc);
    int xPos = (GetSystemMetrics(SM_CXSCREEN) - (rc.right - rc.left)) / 2;
    int yPos = (GetSystemMetrics(SM_CYSCREEN) - (rc.bottom - rc.top)) / 2;
    SetWindowPos(hwnd, 0, xPos, yPos, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
}

// Helper to set font for a list of controls
void SetFontForControls(std::vector<HWND> controls, HFONT font) {
    for (HWND h : controls) {
        SendMessage(h, WM_SETFONT, (WPARAM)font, TRUE);
        InvalidateRect(h, NULL, TRUE); // Force redraw
    }
}

// Function to hide/show controls based on state
void UpdateScreen(HWND hwnd) {
    // Hide ALL first
    std::vector<HWND> all = {
        hBtnWelcomeLogin, hBtnWelcomeSignup, hBtnWelcomeExit,
        hEditLoginUser, hEditLoginPass, hLblLoginUser, hLblLoginPass, hBtnLoginSubmit, hBtnLoginBack, hBtnLoginForget, hBtnLoginExit,
        hEditSignupUser, hEditSignupPhone, hEditSignupPass, hLblSignupUser, hLblSignupPhone, hLblSignupPass, hBtnSignupSubmit, hBtnSignupBack, hBtnSignupExit,
        hEditForgetUser, hEditForgetPhone, hEditForgetNewPass, hLblForgetUser, hLblForgetPhone, hLblForgetNewPass, hBtnForgetSubmit, hBtnForgetBack,
        hLblDashUser, hEditDashRecipient, hEditDashMsg, hLblDashRecipient, hLblDashMsg, hBtnDashSend, hBtnDashInbox, hBtnDashLogout, hBtnDashExit,
        hBtnAdminList, hEditAdminDeleteUser, hBtnAdminDelete, hBtnAdminLogout, hLblAdminDeleteUser
    };
    for (HWND h : all) ShowWindow(h, SW_HIDE);

    // Update Title
    switch (currentScreen) {
    case WELCOME:
        SetWindowText(hTitle, L"Welcome to Chat System");
        SetWindowText(hSubTitle, L"Please select an option to continue");
        ShowWindow(hBtnWelcomeLogin, SW_SHOW);
        ShowWindow(hBtnWelcomeSignup, SW_SHOW);
        ShowWindow(hBtnWelcomeExit, SW_SHOW);
        break;
    case LOGIN:
        SetWindowText(hTitle, L"Login");
        SetWindowText(hSubTitle, L"Enter your credentials");
        ShowWindow(hLblLoginUser, SW_SHOW); ShowWindow(hEditLoginUser, SW_SHOW);
        ShowWindow(hLblLoginPass, SW_SHOW); ShowWindow(hEditLoginPass, SW_SHOW);
        ShowWindow(hBtnLoginSubmit, SW_SHOW);
        ShowWindow(hBtnLoginForget, SW_SHOW);
        ShowWindow(hBtnLoginBack, SW_SHOW);
        ShowWindow(hBtnLoginExit, SW_SHOW);
        SetWindowText(hEditLoginPass, L""); // Clear pass
        break;
    case SIGNUP:
        SetWindowText(hTitle, L"Create Account");
        SetWindowText(hSubTitle, L"Join us today!");
        ShowWindow(hLblSignupUser, SW_SHOW); ShowWindow(hEditSignupUser, SW_SHOW);
        ShowWindow(hLblSignupPhone, SW_SHOW); ShowWindow(hEditSignupPhone, SW_SHOW);
        ShowWindow(hLblSignupPass, SW_SHOW); ShowWindow(hEditSignupPass, SW_SHOW);
        ShowWindow(hBtnSignupSubmit, SW_SHOW);
        ShowWindow(hBtnSignupBack, SW_SHOW);
        ShowWindow(hBtnSignupExit, SW_SHOW);
        break;
    case FORGET_PASS:
        SetWindowText(hTitle, L"Reset Password");
        SetWindowText(hSubTitle, L"Verify your phone number");
        ShowWindow(hLblForgetUser, SW_SHOW); ShowWindow(hEditForgetUser, SW_SHOW);
        ShowWindow(hLblForgetPhone, SW_SHOW); ShowWindow(hEditForgetPhone, SW_SHOW);
        ShowWindow(hLblForgetNewPass, SW_SHOW); ShowWindow(hEditForgetNewPass, SW_SHOW);
        ShowWindow(hBtnForgetSubmit, SW_SHOW);
        ShowWindow(hBtnForgetBack, SW_SHOW);
        break;
    case DASHBOARD:
    { // Scope for string conversion
        std::string user = chatSystem->getCurrentUser();
        std::wstring wuser(user.begin(), user.end());
        std::wstring title = L"Dashboard - " + wuser;
        SetWindowText(hTitle, title.c_str());
        SetWindowText(hSubTitle, L"Send messages or check your inbox");
        
        ShowWindow(hLblDashRecipient, SW_SHOW); ShowWindow(hEditDashRecipient, SW_SHOW);
        ShowWindow(hLblDashMsg, SW_SHOW); ShowWindow(hEditDashMsg, SW_SHOW);
        ShowWindow(hBtnDashSend, SW_SHOW);
        ShowWindow(hBtnDashInbox, SW_SHOW);
        ShowWindow(hBtnDashLogout, SW_SHOW);
        ShowWindow(hBtnDashExit, SW_SHOW);
    }
        break;
    case ADMIN_DASHBOARD:
        SetWindowText(hTitle, L"Admin Dashboard");
        SetWindowText(hSubTitle, L"Manage system users");
        ShowWindow(hBtnAdminList, SW_SHOW);
        ShowWindow(hLblAdminDeleteUser, SW_SHOW);
        ShowWindow(hEditAdminDeleteUser, SW_SHOW);
        ShowWindow(hBtnAdminDelete, SW_SHOW);
        ShowWindow(hBtnAdminLogout, SW_SHOW);
        break;
    }
    InvalidateRect(hwnd, NULL, TRUE);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_CREATE:
    {
        // Fonts
        hFontTitle = CreateFont(32, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");
        hFontText = CreateFont(18, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, L"Segoe UI");

        // Header
        hTitle = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 30, 600, 40, hwnd, NULL, NULL, NULL);
        SendMessage(hTitle, WM_SETFONT, (WPARAM)hFontTitle, TRUE);
        hSubTitle = CreateWindow(L"STATIC", L"", WS_VISIBLE | WS_CHILD | SS_CENTER, 0, 70, 600, 30, hwnd, NULL, NULL, NULL);
        SendMessage(hSubTitle, WM_SETFONT, (WPARAM)hFontText, TRUE);

        int cx = 200, cy = 30, cxL = 80;
        int xCenter = (600 - cx) / 2;
        
        // --- WELCOME ---
        hBtnWelcomeLogin = CreateWindow(L"BUTTON", L"Login", WS_CHILD | BS_FLAT, xCenter, 150, cx, 40, hwnd, (HMENU)ID_W_LOGIN, NULL, NULL);
        hBtnWelcomeSignup = CreateWindow(L"BUTTON", L"Signup", WS_CHILD | BS_FLAT, xCenter, 210, cx, 40, hwnd, (HMENU)ID_W_SIGNUP, NULL, NULL);
        hBtnWelcomeExit = CreateWindow(L"BUTTON", L"Exit", WS_CHILD | BS_FLAT, xCenter, 270, cx, 40, hwnd, (HMENU)ID_W_EXIT, NULL, NULL);

        // --- LOGIN ---
        int y = 140;
        hLblLoginUser = CreateWindow(L"STATIC", L"Username:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditLoginUser = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 40;
        hLblLoginPass = CreateWindow(L"STATIC", L"Password:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditLoginPass = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER | ES_PASSWORD, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 50;
        hBtnLoginSubmit = CreateWindow(L"BUTTON", L"Login", WS_CHILD, xCenter, y, cx, 35, hwnd, (HMENU)ID_L_SUBMIT, NULL, NULL);
        y += 40;
        hBtnLoginForget = CreateWindow(L"BUTTON", L"Forget Password?", WS_CHILD, xCenter, y, cx, 30, hwnd, (HMENU)ID_L_FORGET, NULL, NULL);
        y += 40;
        hBtnLoginBack = CreateWindow(L"BUTTON", L"Back", WS_CHILD, xCenter, y, cx, 30, hwnd, (HMENU)ID_L_BACK, NULL, NULL);
        y += 40;
        hBtnLoginExit = CreateWindow(L"BUTTON", L"Exit App", WS_CHILD, xCenter, y, cx, 30, hwnd, (HMENU)ID_L_EXIT, NULL, NULL);

        // --- SIGNUP ---
        y = 120;
        hLblSignupUser = CreateWindow(L"STATIC", L"Username:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditSignupUser = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 40;
        hLblSignupPhone = CreateWindow(L"STATIC", L"Phone:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditSignupPhone = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 40;
        hLblSignupPass = CreateWindow(L"STATIC", L"Password:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditSignupPass = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 50;
        hBtnSignupSubmit = CreateWindow(L"BUTTON", L"Sign Up", WS_CHILD, xCenter, y, cx, 35, hwnd, (HMENU)ID_S_SUBMIT, NULL, NULL);
        y += 40;
        hBtnSignupBack = CreateWindow(L"BUTTON", L"Back", WS_CHILD, xCenter, y, cx, 30, hwnd, (HMENU)ID_S_BACK, NULL, NULL);
        y += 40;
        hBtnSignupExit = CreateWindow(L"BUTTON", L"Exit App", WS_CHILD, xCenter, y, cx, 30, hwnd, (HMENU)ID_S_EXIT, NULL, NULL);

        // --- FORGET PASS ---
        y = 120;
        hLblForgetUser = CreateWindow(L"STATIC", L"Username:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditForgetUser = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 40;
        hLblForgetPhone = CreateWindow(L"STATIC", L"Phone:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditForgetPhone = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 40;
        hLblForgetNewPass = CreateWindow(L"STATIC", L"New Pass:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditForgetNewPass = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 50;
        hBtnForgetSubmit = CreateWindow(L"BUTTON", L"Reset Password", WS_CHILD, xCenter, y, cx, 35, hwnd, (HMENU)ID_F_SUBMIT, NULL, NULL);
        y += 40;
        hBtnForgetBack = CreateWindow(L"BUTTON", L"Back", WS_CHILD, xCenter, y, cx, 30, hwnd, (HMENU)ID_F_BACK, NULL, NULL);

        // --- DASHBOARD ---
        y = 140;
        hLblDashRecipient = CreateWindow(L"STATIC", L"To (User):", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditDashRecipient = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 40;
        hLblDashMsg = CreateWindow(L"STATIC", L"Message:", WS_CHILD | SS_RIGHT, xCenter - 90, y + 3, 80, 20, hwnd, NULL, NULL, NULL);
        hEditDashMsg = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 50;
        hBtnDashSend = CreateWindow(L"BUTTON", L"Send Message", WS_CHILD, xCenter, y, cx, 35, hwnd, (HMENU)ID_D_SEND, NULL, NULL);
        y += 40;
        hBtnDashInbox = CreateWindow(L"BUTTON", L"Check Inbox", WS_CHILD, xCenter, y, cx, 35, hwnd, (HMENU)ID_D_INBOX, NULL, NULL);
        y += 40;
        hBtnDashLogout = CreateWindow(L"BUTTON", L"Logout", WS_CHILD, xCenter, y, cx, 35, hwnd, (HMENU)ID_D_LOGOUT, NULL, NULL);
        y += 40;
        hBtnDashExit = CreateWindow(L"BUTTON", L"Exit App", WS_CHILD, xCenter, y, cx, 35, hwnd, (HMENU)ID_D_EXIT, NULL, NULL);

        // --- ADMIN DASHBOARD ---
        y = 150;
        hBtnAdminList = CreateWindow(L"BUTTON", L"List All Users", WS_CHILD, xCenter, y, cx, 40, hwnd, (HMENU)ID_A_LIST, NULL, NULL);
        y += 60;
        hLblAdminDeleteUser = CreateWindow(L"STATIC", L"User to Delete:", WS_CHILD | SS_RIGHT, xCenter - 110, y + 3, 100, 20, hwnd, NULL, NULL, NULL);
        hEditAdminDeleteUser = CreateWindow(L"EDIT", L"", WS_CHILD | WS_BORDER, xCenter, y, cx, cy, hwnd, NULL, NULL, NULL);
        y += 40;
        hBtnAdminDelete = CreateWindow(L"BUTTON", L"Delete User", WS_CHILD, xCenter, y, cx, 40, hwnd, (HMENU)ID_A_DELETE, NULL, NULL);
        y += 60;
        hBtnAdminLogout = CreateWindow(L"BUTTON", L"Logout", WS_CHILD, xCenter, y, cx, 40, hwnd, (HMENU)ID_A_LOGOUT, NULL, NULL);

        // Apply Fonts
        std::vector<HWND> allCtrls = {
            hBtnWelcomeLogin, hBtnWelcomeSignup, hBtnWelcomeExit,
            hBtnLoginSubmit, hBtnLoginBack, hBtnLoginForget, hBtnLoginExit, hLblLoginUser, hLblLoginPass, hEditLoginUser, hEditLoginPass,
            hBtnSignupSubmit, hBtnSignupBack, hBtnSignupExit, hLblSignupUser, hLblSignupPhone, hLblSignupPass, hEditSignupUser, hEditSignupPhone, hEditSignupPass,
            hBtnForgetSubmit, hBtnForgetBack, hLblForgetUser, hLblForgetPhone, hLblForgetNewPass, hEditForgetUser, hEditForgetPhone, hEditForgetNewPass,
            hBtnDashSend, hBtnDashInbox, hBtnDashLogout, hBtnDashExit, hLblDashRecipient, hLblDashMsg, hEditDashRecipient, hEditDashMsg,
            hBtnAdminList, hEditAdminDeleteUser, hBtnAdminDelete, hBtnAdminLogout, hLblAdminDeleteUser
        };
        SetFontForControls(allCtrls, hFontText);

        UpdateScreen(hwnd);
        break;
    }
    case WM_CTLCOLORSTATIC:
    {
        HDC hdcStatic = (HDC)wParam;
        SetBkColor(hdcStatic, BG_COLOR);
        return (INT_PTR)hBrushBg;
    }
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            // WELCOME
            case ID_W_LOGIN: currentScreen = LOGIN; UpdateScreen(hwnd); break;
            case ID_W_SIGNUP: currentScreen = SIGNUP; UpdateScreen(hwnd); break;
            case ID_W_EXIT: PostQuitMessage(0); break;

            // LOGIN
            case ID_L_BACK: currentScreen = WELCOME; UpdateScreen(hwnd); break;
            case ID_L_EXIT: PostQuitMessage(0); break;
            case ID_L_FORGET: currentScreen = FORGET_PASS; UpdateScreen(hwnd); break;
            case ID_L_SUBMIT:
            {
                char u[100], p[100];
                GetWindowTextA(hEditLoginUser, u, 100);
                GetWindowTextA(hEditLoginPass, p, 100);
                std::string sUser(u), sPass(p);

                if (sUser == "admin") {
                    if (chatSystem->adminAuth(sUser, sPass)) {
                         MessageBox(hwnd, L"Logged in as Admin!", L"Success", MB_OK);
                         currentScreen = ADMIN_DASHBOARD; 
                         UpdateScreen(hwnd);
                    } else MessageBox(hwnd, L"Admin Login Failed", L"Error", MB_ICONERROR);
                } else {
                    if (chatSystem->authenticateUser(sUser, sPass)) {
                        currentScreen = DASHBOARD;
                        UpdateScreen(hwnd);
                    } else MessageBox(hwnd, L"Login Failed", L"Error", MB_ICONERROR);
                }
                break;
            }

            // SIGNUP
            case ID_S_BACK: currentScreen = WELCOME; UpdateScreen(hwnd); break;
            case ID_S_EXIT: PostQuitMessage(0); break;
            case ID_S_SUBMIT:
            {
                char u[100], ph[100], p[100];
                GetWindowTextA(hEditSignupUser, u, 100);
                GetWindowTextA(hEditSignupPhone, ph, 100);
                GetWindowTextA(hEditSignupPass, p, 100);
                if (chatSystem->registerUser(u, ph, p)) {
                    MessageBox(hwnd, L"Signup Successful! Please Login.", L"Success", MB_OK);
                    currentScreen = LOGIN;
                    UpdateScreen(hwnd);
                } else MessageBox(hwnd, L"Signup Failed. User exists or limit reached.", L"Error", MB_ICONERROR);
                break;
            }

            // FORGET
            case ID_F_BACK: currentScreen = LOGIN; UpdateScreen(hwnd); break;
            case ID_F_SUBMIT:
            {
                char u[100], ph[100], np[100];
                GetWindowTextA(hEditForgetUser, u, 100);
                GetWindowTextA(hEditForgetPhone, ph, 100);
                GetWindowTextA(hEditForgetNewPass, np, 100);
                
                std::string sUser(u);
                if (sUser == "admin") {
                    chatSystem->resetAdminPassword(np);
                    MessageBox(hwnd, L"Admin Password Reset!", L"Success", MB_OK);
                    currentScreen = LOGIN;
                    UpdateScreen(hwnd);
                } else {
                    if (chatSystem->resetPassword(u, ph, np)) {
                        MessageBox(hwnd, L"Password Reset Successful!", L"Success", MB_OK);
                        currentScreen = LOGIN;
                        UpdateScreen(hwnd);
                    } else MessageBox(hwnd, L"Reset Failed. Username/Phone mismatch.", L"Error", MB_ICONERROR);
                }
                break;
            }

            // DASHBOARD
            case ID_D_EXIT: PostQuitMessage(0); break;
            case ID_D_LOGOUT: 
                if (MessageBox(hwnd, L"Are you sure you want to logout?", L"Logout", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                    chatSystem->logout(); 
                    currentScreen = WELCOME; 
                    UpdateScreen(hwnd); 
                }
                break;
            case ID_D_SEND:
            {
                char r[100], m[256];
                GetWindowTextA(hEditDashRecipient, r, 100);
                GetWindowTextA(hEditDashMsg, m, 256);
                if (chatSystem->sendMsg(chatSystem->getCurrentUser(), r, m)) {
                    MessageBox(hwnd, L"Message Sent!", L"Success", MB_OK);
                } else MessageBox(hwnd, L"User not found.", L"Error", MB_ICONERROR);
                break;
            }
            case ID_D_INBOX:
            {
                 std::string msgs = chatSystem->getInboxMessages(chatSystem->getCurrentUser());
                 int size = MultiByteToWideChar(CP_UTF8, 0, msgs.c_str(), -1, NULL, 0);
                 std::wstring wmsgs(size, 0);
                 MultiByteToWideChar(CP_UTF8, 0, msgs.c_str(), -1, &wmsgs[0], size);
                 MessageBox(hwnd, wmsgs.c_str(), L"Inbox", MB_OK);
                 break;
            }

            // ADMIN DASHBOARD
            case ID_A_LOGOUT:
                if (MessageBox(hwnd, L"Are you sure you want to logout?", L"Logout", MB_YESNO | MB_ICONQUESTION) == IDYES) {
                    chatSystem->logout();
                    currentScreen = WELCOME;
                    UpdateScreen(hwnd);
                }
                break;
            case ID_A_LIST:
            {
                std::string users = chatSystem->getAllUsersString();
                int size = MultiByteToWideChar(CP_UTF8, 0, users.c_str(), -1, NULL, 0);
                std::wstring wusers(size, 0);
                MultiByteToWideChar(CP_UTF8, 0, users.c_str(), -1, &wusers[0], size);
                MessageBox(hwnd, wusers.c_str(), L"User List", MB_OK);
                break;
            }
            case ID_A_DELETE:
            {
                char u[100];
                GetWindowTextA(hEditAdminDeleteUser, u, 100);
                std::string sUser(u);
                if (sUser.empty()) {
                    MessageBox(hwnd, L"Please enter a username to delete.", L"Error", MB_ICONWARNING);
                    break;
                }
                if (sUser == "admin") {
                    MessageBox(hwnd, L"Cannot delete the admin account!", L"Error", MB_ICONERROR);
                    break;
                }
                if (MessageBox(hwnd, L"Are you sure you want to delete this user?", L"Confirm Deletion", MB_YESNO | MB_ICONWARNING) == IDYES) {
                    if (chatSystem->deleteUser(sUser)) {
                        MessageBox(hwnd, L"User deleted successfully.", L"Success", MB_OK);
                        SetWindowTextA(hEditAdminDeleteUser, "");
                    } else {
                        MessageBox(hwnd, L"User not found.", L"Error", MB_ICONERROR);
                    }
                }
                break;
            }
        }
        break;

    case WM_DESTROY:
        DeleteObject(hFontTitle);
        DeleteObject(hFontText);
        DeleteObject(hBrushBg);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int main() {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    chatSystem = new ChatSystem(); 
    chatSystem->loadUsersFromFile();

    const wchar_t CLASS_NAME[] = L"ChatAppEnhanced";
    WNDCLASS wc = { };
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    
    // Background Color
    hBrushBg = CreateSolidBrush(BG_COLOR);
    wc.hbrBackground = hBrushBg;
    
    // Icon (Standard)
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    if (!RegisterClass(&wc)) return 0;

    // Center Window (600x600)
    int w = 600, h = 600;
    int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    HWND hwnd = CreateWindowEx(
        0, CLASS_NAME, L"Chat System 2.0", 
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        x, y, w, h,
        NULL, NULL, hInstance, NULL
    );

    if (hwnd == NULL) return 0;

    ShowWindow(hwnd, SW_SHOW);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
