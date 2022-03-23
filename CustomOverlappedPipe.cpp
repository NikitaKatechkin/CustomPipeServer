#include "CustomOverlappedPipe.h"

CustomServer::CustomServer(const LPCTSTR l_pipe_name):
	m_pipe_name(l_pipe_name)
{
}

bool CustomServer::Recieve()
{
    //TCHAR read_buffer[BUFSIZE];

    DWORD bytes_processed = 0;

    //READ TO BUFFER FROM PIPE
    bool reading_success = ReadFile(
        m_pipe,
        read_buffer,
        BUFSIZE * sizeof(TCHAR),
        &bytes_processed,
        nullptr);

    if (!reading_success || bytes_processed == 0)
    {
        //throw std::exception("[ReadFile()] Error while reading client message.");
        return false;
    }

    //_tprintf(TEXT("[CLIENT]: %s\n"), read_buffer);

    return true;
}

bool CustomServer::Send()
{
    //TCHAR write_buffer[BUFSIZE];

    //WRTIE FROM BUFFER TO PIPE
    DWORD bytes_processed = 0;
    wcscpy_s(write_buffer, TEXT("Message recieved"));

    bool writing_success = WriteFile(
        m_pipe,
        write_buffer,
        BUFSIZE * sizeof(TCHAR),
        &bytes_processed,
        nullptr);

    if (!writing_success ||
        (lstrlen(TEXT("Message recieved") + 1) * sizeof(TCHAR) != bytes_processed))
    {
        //throw std::exception("[WriteFile()] Error while writing server message.");
        return false;
    }

    return true;
}

void CustomServer::RegistrationLoop()
{
    std::cout << "[SERVER]: Main thread awaiting client connection..." << std::endl;
    
    while (true)
    {
        m_pipe = CreateNamedPipe(
            m_pipe_name,
            PIPE_OPEN_MODE,
            PIPE_MODE,
            PIPE_CAPACITY,
            BUFSIZE,
            BUFSIZE,
            PIPE_TIMEOUT,
            PIPE_SECURITY_SETTINGS);

        if (m_pipe == INVALID_HANDLE_VALUE)
        {
            throw std::exception("[CreateNamedPipe()] Failed to create a named pipe");
        }

        bool connection_success = ConnectNamedPipe(m_pipe, nullptr);
        if (connection_success == false)
        {
            connection_success = (GetLastError() == ERROR_PIPE_CONNECTED);
        }

        if (connection_success)
        {
            std::cout << "[ConnectNamedPipe()] Client succesfully connected. Start ping-pong process...";
            std::cout << std::endl;

            while (true)
            {
                if (!this->Recieve())
                {
                    break;
                }
                else
                {
                    _tprintf(TEXT("[CLIENT]: %s\n"), read_buffer);
                }


                if (!this->Send())
                {
                    break;
                }
            }
        }
        else
        {
            std::cout << "[ConnectNamedPipe()] Client connection did not succeed. Closing the pipe...";
            std::cout << std::endl;
            CloseHandle(m_pipe);
        }
    }
}
