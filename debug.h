#if DEBUG
#define print(a, args...) fprintf(stderr, "%s(%s:%d) " a,  __func__,__FILE__, __LINE__, ##args)
#define println(a, args...) print(a "\n", ##args)
#else
#define println(a, args...)
#endif