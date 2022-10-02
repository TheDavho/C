# Windows API
# Gets current window's size, then changes the size N times

from ctypes import *
from ctypes.wintypes import HWND, LPCSTR, UINT, INT, LPSTR, LPDWORD, DWORD, HANDLE, BOOL
import time

class RECT(Structure):
	_fields_ = [("left", c_long),
				("top", c_long),
				("right", c_long),
				("bottom", c_long)]
rect = RECT()
print(rect)
print(byref(rect))

GetWindowRect = windll.user32.GetWindowRect
GetWindowRect.argtypes = (HANDLE, POINTER(RECT))

hwnd = windll.user32.GetForegroundWindow()
GetWindowRect(hwnd, byref(rect))

print(rect)
print(rect.left)
print(rect.top)
print(rect.right)
print(rect.bottom)

CX = rect.right
XY = rect.bottom

SetWindowPos = windll.user32.SetWindowPos
SetWindowPos.argtypes = (HANDLE, HANDLE, INT, INT, INT, INT, UINT)
SetWindowPos.restype = BOOL


handle = windll.user32.GetForegroundWindow()
X = INT(100)
Y = INT(100)
uFlags = UINT()

for i in range(20):
	time.sleep(0.4)
	SetWindowPos(handle, None, X, Y, CX - i * 60, XY - i * 30, uFlags)
