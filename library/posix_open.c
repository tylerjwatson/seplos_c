#include "./internal.h"
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>

seplos_device
seplos_open(const char * serial_device)
{
  struct termios t = {};

  const int fd = open(serial_device, O_RDWR|O_NOCTTY, 0);
  if ( fd < 0 ) {
    _sp_error("%s: %s\n", serial_device, strerror(errno));
    return -1;
  }

  tcgetattr(fd, &t);
  cfsetspeed(&t, 19200);
  cfmakeraw(&t);
  tcflush(fd, TCIOFLUSH); /* Throw away any pending I/O */
  t.c_lflag &= ~ICANON;
  t.c_cc[VTIME] = 1; // 1 second timeout
  t.c_cc[VMIN] = 0;
  tcsetattr(fd, TCSANOW, &t);

  return fd;
}
