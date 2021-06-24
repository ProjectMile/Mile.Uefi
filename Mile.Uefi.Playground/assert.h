#ifndef ASSERT
#define ASSERT

#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) /*implementation defined*/
#endif

#endif // !ASSERT
