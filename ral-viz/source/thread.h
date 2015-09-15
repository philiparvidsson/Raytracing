#ifndef thread_h_
#define thread_h_

typedef struct threadT threadT;

typedef void (*threadFnT)(void*);

int GetProcessorCount();

threadT* createThread(threadFnT thread_fn, void* arg);
void destroyThread(threadT* thread);

#endif // thread_h_
