#ifdef _WIN32

#include "thread.h"

#include <stdlib.h>

#include <windows.h>

struct threadT {
    DWORD id;

    threadFnT fn;
    void* arg;
};

static DWORD threadFunc(LPVOID arg) {
    threadT* thread = arg;

    thread->fn(thread->arg);

    return (0);
}

threadT* createThread(threadFnT thread_fn, void* arg) {
    threadT* thread = malloc(sizeof(threadT));

    thread->fn  = thread_fn;
    thread->arg = arg;

    HANDLE h = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&threadFunc, thread, 0, &thread->id);
    SetThreadPriority(h, THREAD_PRIORITY_LOWEST);
}

void destroyThread(threadT* thread) {
    TerminateThread(thread->id, 1);

    free(thread);
}

#endif // _WIN32
