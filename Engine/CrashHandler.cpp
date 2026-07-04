#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "CrashHandler.h"
#include <csignal>
#include <ctime>
#include <cstring>
#include <cstdio>
#include <cstdlib>

#ifdef _WIN32
#include <windows.h>
#include <dbghelp.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#pragma comment(lib, "dbghelp.lib")
#else
#include <unistd.h>
#include <fcntl.h>
#endif

namespace Engine
{
    static void writeCrashDump(const char* signalName, int signalCode)
    {
        const char* filename = "crash_dump.txt";

#ifdef _WIN32
        int fd = _open(filename, _O_WRONLY | _O_CREAT | _O_APPEND, _S_IWRITE);
#else
        int fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
#endif

        if (fd != -1)
        {
            time_t now = time(nullptr);
            char timeBuf[64];
            strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&now));

            char buffer[512];
            int len = snprintf(buffer, sizeof(buffer),
                "========================================\n"
                "CRASH DUMP\n"
                "Time: %s\n"
                "Signal: %s (%d)\n"
                "========================================\n",
                timeBuf, signalName, signalCode);

#ifdef _WIN32
            _write(fd, buffer, len);
            _close(fd);
#else
            write(fd, buffer, len);
            close(fd);
#endif
        }
    }

    static void signalHandler(int signalCode)
    {
        const char* signalName = "UNKNOWN";
        switch (signalCode)
        {
        case SIGSEGV: signalName = "SIGSEGV (Segmentation Fault)"; break;
        case SIGABRT: signalName = "SIGABRT (Abort)"; break;
        case SIGFPE:  signalName = "SIGFPE (Floating Point Exception)"; break;
        case SIGILL:  signalName = "SIGILL (Illegal Instruction)"; break;
        case SIGINT:  signalName = "SIGINT (Interrupt)"; break;
        }

        const char* header = "\n[CRITICAL] Signal received: ";
#ifdef _WIN32
        _write(2, header, (unsigned)strlen(header));
        _write(2, signalName, (unsigned)strlen(signalName));
        const char* newline = "\n";
        _write(2, newline, 1);
#else
        write(2, header, strlen(header));
        write(2, signalName, strlen(signalName));
        const char* newline = "\n";
        write(2, newline, 1);
#endif

        writeCrashDump(signalName, signalCode);

        if (signalCode != SIGINT)
        {
            std::signal(signalCode, SIG_DFL);
            std::raise(signalCode);
        }
    }

#ifdef _WIN32
    static LONG WINAPI unhandledExceptionHandler(EXCEPTION_POINTERS* exceptionInfo)
    {
        const char* msg = "\n[CRITICAL] Unhandled Windows exception occurred!\n";
        _write(2, msg, (unsigned)strlen(msg));
        writeCrashDump("WindowsUnhandledException", exceptionInfo->ExceptionRecord->ExceptionCode);

        HANDLE dumpFile = CreateFileA("crash_memory.dmp", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
        if (dumpFile != INVALID_HANDLE_VALUE)
        {
            MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
            dumpInfo.ThreadId = GetCurrentThreadId();
            dumpInfo.ExceptionPointers = exceptionInfo;
            dumpInfo.ClientPointers = FALSE;

            MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), dumpFile, MiniDumpNormal, &dumpInfo, nullptr, nullptr);
            CloseHandle(dumpFile);

            const char* dumpMsg = "[CRITICAL] Memory dump written to crash_memory.dmp\n";
            _write(2, dumpMsg, (unsigned)strlen(dumpMsg));
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }
#endif

    void installCrashHandler()
    {
        std::signal(SIGSEGV, signalHandler);
        std::signal(SIGABRT, signalHandler);
        std::signal(SIGFPE, signalHandler);
        std::signal(SIGILL, signalHandler);
        std::signal(SIGINT, signalHandler);

#ifdef _WIN32
        SetUnhandledExceptionFilter(unhandledExceptionHandler);
#endif

        const char* msg = "[INFO] Crash handler installed\n";
#ifdef _WIN32
        _write(2, msg, (unsigned)strlen(msg));
#else
        write(2, msg, strlen(msg));
#endif
    }

    void triggerTestCrash(int type)
    {
        switch (type)
        {
        case 1:
        {
            const char* msg = "[INFO] Triggering test crash: division by zero\n";
#ifdef _WIN32
            _write(2, msg, (unsigned)strlen(msg));
#else
            write(2, msg, strlen(msg));
#endif
            volatile int zero = 0;
            volatile int result = 1 / zero;
            (void)result;
            break;
        }
        case 2:
        {
            const char* msg = "[INFO] Triggering test crash: nullptr dereference\n";
#ifdef _WIN32
            _write(2, msg, (unsigned)strlen(msg));
#else
            write(2, msg, strlen(msg));
#endif
            int* ptr = nullptr;
            int val = *ptr;
            (void)val;
            break;
        }
        case 3:
        {
            const char* msg = "[INFO] Triggering test crash: abort\n";
#ifdef _WIN32
            _write(2, msg, (unsigned)strlen(msg));
#else
            write(2, msg, strlen(msg));
#endif
            std::abort();
            break;
        }
        default:
        {
            const char* msg = "[INFO] Unknown crash type\n";
#ifdef _WIN32
            _write(2, msg, (unsigned)strlen(msg));
#else
            write(2, msg, strlen(msg));
#endif
            break;
        }
        }
    }
}