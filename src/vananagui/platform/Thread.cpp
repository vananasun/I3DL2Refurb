#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "vananagui/platform/Thread.h"

int Thread::GetThreadId() { return GetCurrentThreadId(); }
