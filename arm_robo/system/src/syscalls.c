#include <sys/types.h>
#include <errno.h>

extern char _sheap;  // Được định nghĩa trong linker script
extern char _estack; // Đỉnh stack từ linker script
static char *heap_end = 0;

caddr_t _sbrk(int incr)
{
    char *prev_heap_end;
    if (heap_end == 0)
    {
        heap_end = &_sheap;
    }
    prev_heap_end = heap_end;
    if (heap_end + incr > &_estack)
    {
        errno = ENOMEM;
        return (caddr_t)-1;
    }
    heap_end += incr;
    return (caddr_t)prev_heap_end;
}

int *__errno(void)
{
    static int _errno = 0;
    return &_errno;
}
