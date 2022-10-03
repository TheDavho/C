## Change the NetBios name of your computer

from ctypes import *
from ctypes.wintypes import HWND, LPCSTR, UINT, INT, LPSTR, LPDWORD, DWORD, HANDLE, BOOL

def errorCheck():
	error = GetLastError()
	if error != 6:
		print(WinError(error))
		exit()

GetUserNameA = windll.advapi32.GetUserNameA
GetUserNameA.argtypes = (LPSTR, LPDWORD)
GetUserNameA.restype = INT

GetComputerNameA = windll.kernel32.GetComputerNameA
GetComputerNameA.argtypes = (LPSTR, LPDWORD)
GetComputerNameA.restype = INT

SetComputerNameA = windll.kernel32.SetComputerNameA
SetComputerNameA.argtypes = (LPCSTR,)
SetComputerNameA.restype = BOOL



username_buffer_size = DWORD(20)
username_buffer = create_string_buffer(username_buffer_size.value)

computer_buffer_size = DWORD(20)
computer_buffer = create_string_buffer(computer_buffer_size.value)

GetUserNameA(username_buffer, byref(username_buffer_size))
GetComputerNameA(computer_buffer, byref(computer_buffer_size))


username = username_buffer.raw.decode('utf-8').rstrip('\x00')
computerName = computer_buffer.raw.decode('utf-8').rstrip('\x00')

print("Getting system info..")
print("\n" * 2)
print("Username: {}".format(username))
print("Computer name: {}".format(computerName))


GetUserNameA(username_buffer, byref(username_buffer_size))
GetComputerNameA(computer_buffer, byref(computer_buffer_size))


username = username_buffer.raw.decode('utf-8').rstrip('\x00')
computerName = computer_buffer.raw.decode('utf-8').rstrip('\x00')

print("Please make sure you are running this program with administrator permissions!")
print("-" * 80)
print()
print("Getting system info..")
print()
print("Username: {}".format(username))
print("Computer name: {}".format(computerName))
print()
newName = LPCSTR(input("Enter new computer name: ").encode('utf-8'))
print("Response: {}".format(SetComputerNameA(newName)))
errorCheck()

print()
print("Now restart the computer.")
print("After that you can check the changes using the nbtstat -n command")
