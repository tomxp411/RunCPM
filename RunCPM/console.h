#ifndef CONSOLE_H
#define CONSOLE_H

/* see main.c for definition */

uint8 mask8bit = 0x7f;		// TO be used for masking 8 bit characters (XMODEM related)
							// If set to 0x7f, RunCPM masks the 8th bit of characters sent
							// to the console. This is the standard CP/M behavior.
							// If set to 0xff, RunCPM passes 8 bit characters. This is
							// required for XMODEM to work.
							// Use the CONSOLE7 and CONSOLE8 programs to change this on the fly.

#ifdef PC_SERIAL
uint8 _serialWaiting(void)
{
	if (!PortOpen)
		return 0;

	COMSTAT status;
	DWORD errVal = 0;
	BOOL dataWaiting = ClearCommError(portHandle, &errVal, &status);
	if (dataWaiting)
		return status.cbInQue;
	
	return 0;
}
#endif

uint8 _chready(void)		// Checks if there's a character ready for input
{
	int8 ret = 0;
	ret = _kbhit() ? 0xff : 0x00;
	if (ret)
		return ret;

#ifdef PC_SERIAL
	return _serialWaiting();
#endif
	return 0;
}

uint8 _getchNB(void)		// Gets a character, non-blocking, no echo
{
	if (_kbhit())
		return _getch();

	if (_serialWaiting())
	{
		DWORD bytesRead;
		uint8 ret;
		ReadFile(portHandle, &ret, sizeof(ret), &bytesRead, NULL);
		if (bytesRead > 0)
			return ret;
	}

	return 0;
}

// get a character from the serial console OR the keyboard
// DO block the thread. 
// do NOT echo the character
uint8 _getchw(void)
{
	while (1)
	{
		if (_kbhit())
			return _getch();

		if (_serialWaiting())
		{
			DWORD bytesRead;
			uint8 ret;
			ReadFile(portHandle, &ret, sizeof(ret), &bytesRead, NULL);
			if (bytesRead > 0)
				return ret;
		}
		// sleep the thread until we get some sort of input
		Sleep(0);
	}
}

void _putcon(uint8 ch)		// Puts a character
{
	_putch(ch & mask8bit);
#ifdef PC_SERIAL
	DWORD dNoOfBytesWritten = 0;
	if(PortOpen)
		WriteFile(portHandle, &ch, 1, dNoOfBytesWritten, NULL);
#endif
}

void _puts(const char* str)	// Puts a \0 terminated string
{
	while (*str)
		_putcon(*(str++));
}

void _puthex8(uint8 c)		// Puts a HH hex string
{
	_putcon(tohex(c >> 4));
	_putcon(tohex(c & 0x0f));
}

void _puthex16(uint16 w)	// puts a HHHH hex string
{
	_puthex8(w >> 8);
	_puthex8(w & 0x00ff);
}

#endif