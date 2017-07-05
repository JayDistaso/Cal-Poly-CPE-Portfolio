#include <minix/const.h>
#include <minix/drivers.h>
#include <sys/ucred.h>
#include <minix/chardriver.h>
#include <minix/ds.h>
#include <minix/syslib.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fcntl.h>


#define O_WRONLY 2
#define O_RDONLY 4
#define O_RDWR 6

#define SECRET_SIZE 8192

static void sef_local_startup();
static int sef_cb_init(int type, sef_init_info_t *UNUSED(info));
static int sef_cb_lu_state_save(int UNUSED(state));
static int lu_state_restore();
static int read_write_secret(endpoint_t endpt, int opcode, u64_t position,
 iovec_t *iov, unsigned nr_req, endpoint_t user_endpt, unsigned 
 int UNUSED(flags));
static int openSecret(message *message);
static int closeSecret(message *message);
static struct device *initSecret(dev_t UNUSED(dev));


static struct chardriver secret_chardriver = 
{
   openSecret,
   closeSecret,
   nop_ioctl,
   initSecret,
   read_write_secret,
   nop_cleanup,
   nop_alarm,
   nop_cancel,
   nop_select,
   NULL
};
