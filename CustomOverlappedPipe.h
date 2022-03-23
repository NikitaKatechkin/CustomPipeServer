#pragma once 

#include <iostream>
#include <cstdio>
#include <thread>

#include <windows.h>
#include <tchar.h>

const LPCTSTR PIPE_NAME = TEXT("\\\\.\\pipe\\mynamedpipe");
const DWORD PIPE_OPEN_MODE = PIPE_ACCESS_DUPLEX;
const DWORD PIPE_MODE = (PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT);
const DWORD PIPE_CAPACITY = PIPE_UNLIMITED_INSTANCES;
const DWORD PIPE_TIMEOUT = NMPWAIT_USE_DEFAULT_WAIT;
const DWORD BUFSIZE = 512;
const LPSECURITY_ATTRIBUTES PIPE_SECURITY_SETTINGS = nullptr;

class CustomServer final
{
public:
	CustomServer(const LPCTSTR l_pipe_name = nullptr);
	~CustomServer() = default;

	bool Recieve();
	bool Send();

	void RegistrationLoop();
private:
	HANDLE m_pipe = INVALID_HANDLE_VALUE;
	LPCTSTR m_pipe_name = nullptr;

	TCHAR read_buffer[BUFSIZE] = TEXT("");
	TCHAR write_buffer[BUFSIZE] = TEXT("");
};