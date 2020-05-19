/*
**  CoordBox.c
**
**      Extracted from MathTools.c
**
/* MathTools.c was written by:

      Mark C. Peterson
      The Yankee Programmer
      405-C Queen Street, Suite #181
      Southington, CT 06489
      (203) 276-9721

   If you make any changes to this file, please comment out the older
   code with your name, date, and a short description of the change.

   Thanks!

                                 -Mark
*/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define STRICT
#include <windows.h>
#include <windowsx.h>

#include "resource.h"

#include "port.h"
#include "prototyp.h"

#include "winfract.h"
#include "mathtool.h"
#include "profile.h"

extern HINSTANCE hThisInst;
extern HWND hFractalWnd;
extern int win_xoffset, win_yoffset;   /* BDT 11/6/91 */

static double const Pi = 3.1415926535897932384626433832795;

static WORD AngleFormat = IDM_DEGREES;
static WORD CoordFormat = IDM_RECT;

int CoordBoxOpen = 0;
int KillCoordBox = 0;
HWND hCoordBox;


static void onClose(HWND hwnd)
{
    KillCoordBox = 1;
    ProgStr = Winfract;
}

static void onCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
    HMENU hDlgMenu = GetMenu(hwnd);

    CheckMenuItem(hDlgMenu, AngleFormat, MF_UNCHECKED);
    CheckMenuItem(hDlgMenu, CoordFormat, MF_UNCHECKED);

    switch (id)
    {
        case IDM_RADIANS:
        case IDM_GRAD:
        case IDM_DEGREES:
            AngleFormat = id;
            break;

        case IDM_POLAR:
        case IDM_RECT:
        case IDM_PIXEL:
            CoordFormat = id;
            break;

        default:
            break;
    }

    CheckMenuItem(hDlgMenu, AngleFormat, MF_CHECKED);
    CheckMenuItem(hDlgMenu, CoordFormat, MF_CHECKED);

    if (CoordFormat == IDM_POLAR)
    {
        SetDlgItemText(hwnd, ID_X_NAME, "|z|");
        SetDlgItemText(hwnd, ID_Y_NAME, "\xD8");
        EnableMenuItem(hDlgMenu, IDM_DEGREES, MF_ENABLED);
        EnableMenuItem(hDlgMenu, IDM_RADIANS, MF_ENABLED);
        EnableMenuItem(hDlgMenu, IDM_GRAD, MF_ENABLED);
    }
    else
    {
        SetDlgItemText(hwnd, ID_X_NAME, "x");
        SetDlgItemText(hwnd, ID_Y_NAME, "y");
        EnableMenuItem(hDlgMenu, IDM_DEGREES, MF_DISABLED | MF_GRAYED);
        EnableMenuItem(hDlgMenu, IDM_RADIANS, MF_DISABLED | MF_GRAYED);
        EnableMenuItem(hDlgMenu, IDM_GRAD, MF_DISABLED | MF_GRAYED);
    }
}

static void onDestroy(HWND hwnd)
{
    CoordBoxOpen = 0;
}

static BOOL onInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
    CoordBoxOpen = 1;
    ProgStr = Winfract;
    SaveParamSwitch(CoordBoxStr, TRUE);
    CheckMenuItem(GetMenu(hFractalWnd), IDM_COORD, MF_CHECKED);
    hCoordBox = hwnd;
    PositionWindow(hwnd, CoordBoxPosStr);

    return TRUE;
}

static void onMove(HWND hwnd, int x, int y)
{
    SaveWindowPosition(hwnd, CoordBoxPosStr);
}

BOOL CALLBACK __export CoordBoxDlg(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
        case WM_INITDIALOG:
            return HANDLE_WM_INITDIALOG(hwnd, wParam, lParam, onInitDialog);

        case WM_MOVE:
            return HANDLE_WM_MOVE(hwnd, wParam, lParam, onMove);

        case WM_CLOSE:
            return HANDLE_WM_CLOSE(hwnd, wParam, lParam, onClose);

        case WM_DESTROY:
            return HANDLE_WM_DESTROY(hwnd, wParam, lParam, onDestroy);

        case WM_COMMAND:
            return HANDLE_WM_COMMAND(hwnd, wParam, lParam, onCommand);

        default:
            return FALSE;
   }
}

void UpdateCoordBox(DWORD dw)
{
   unsigned xPixel, yPixel;
   double xd, yd, Angle, Modulus;
   char xStr[40], yStr[40];

   xPixel = (unsigned)(dw & 0xFFFF) + win_xoffset;  /* BDT 11/6/91 */
   yPixel = (unsigned)(dw >> 16) + win_yoffset;  /* BDT 11/6/91 */
   xd = xxmin + ((double)delxx * xPixel);
   yd = yymax - ((double)delyy * yPixel);
   switch(CoordFormat) {
      case IDM_PIXEL:
         sprintf(xStr, "%d", xPixel);
         sprintf(yStr, "%d", yPixel);
         break;
      case IDM_RECT:
         sprintf(xStr, "%+.8g", xd);
         sprintf(yStr, "%+.8g", yd);
         break;
      case IDM_POLAR:
         Modulus = (xd*xd) + (yd*yd);
         if(Modulus > 1E-20) {
/*            Modulus = sqrt(Modulus);  This breaks!! JCO 07 JAN 2007*/
            Modulus = pow(Modulus , 0.5);
            Angle = atan2(yd, xd);
            switch(AngleFormat) {
               case IDM_DEGREES:
                  Angle = (Angle / Pi) * 180;
                  break;
               case IDM_GRAD:
                  Angle = (Angle / Pi) * 200;
               case IDM_RADIANS:
                  break;
            }
         }
         else {
            Modulus = 0.0;
            Angle = 0.0;
         }
         sprintf(xStr, "%+.8g", Modulus);
         sprintf(yStr, "%+.8g", Angle);
         break;
   }
   SetDlgItemText(hCoordBox, ID_X_COORD, xStr);
   SetDlgItemText(hCoordBox, ID_Y_COORD, yStr);
}

void CoordinateBox(HWND hWnd)
{
    hFractalWnd = hWnd;
    if (CoordBoxOpen)
        KillCoordBox = TRUE;
    else
    {
        if (CreateDialog(hThisInst, MAKEINTRESOURCE(IDD_COORDBOX), hWnd, CoordBoxDlg))
            return;
        MessageBox(hWnd, "Error Opening Coordinate Box", NULL, MB_ICONEXCLAMATION | MB_OK);
    }
    ProgStr = Winfract;
}

