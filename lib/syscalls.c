#include <vrlt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int _close(int fd)
{
    errno = ENOENT;
    return -1;
}

int _fstat(int fd, struct stat *st)
{
    errno = ENOENT;
    return 0;
}

int _isatty(int fd)
{
    return 1;
}

off_t _lseek(int fd, off_t offset, int dir)
{
    errno = ENOENT;
    return -1;
}

ssize_t _read(int fd, void *ptr, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        ((char *)ptr)[i] = uart_read(UART_BASE);
    }
    return len;
}

ssize_t _write(int fd, const void *buf, size_t count)
{
    for (size_t i = 0; i < count; i++)
    {
        uart_write(((uint8_t *)buf)[i], UART_BASE);
    }
    return count;
}

char *_sbrk(int increment)
{
    extern char __heap_start[];
    static char *__cur = 0;
    char *__prv;
    if (__cur == 0)
    {
        __cur = __heap_start;
    }
    __prv = __cur;
    __cur += increment;
    return __prv;
}
