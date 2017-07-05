#include "secret.h"

/*Keep track of who owns the secret*/
static uid_t owner;

/*Keep track of file descriptors*/
static int numberOfOpenFileDescriptors;

/*0 no, 1 yes*/
static int hasSecretBeenOpenedForReading;

/*The actual secret itself!*/
static char secret[SECRET_SIZE];

/*The /dev/secret device*/
static struct device secretDevice;

/** State variable to count the number of times the device has been opened.
 * Note that this is not the regular type of open counter: it never decreases.
 */
static int open_counter;

/*Our drivers main function*/
int main(void) {
   
   int index;
   
   /*init variables*/
   owner = -1;
   numberOfOpenFileDescriptors = 0;
   hasSecretBeenOpenedForReading = 0;
   
   /*Clear out the secret*/
   for (index = 0; index < SECRET_SIZE; index++) {
      secret[index] = '\0';
   }
   sef_local_startup();
   
   chardriver_task(&secret_chardriver, CHARDRIVER_SYNC);
}

/*Source: 
 http://wiki.minix3.org/doku.php?id=developersguide:driverprogramming */
static void sef_local_startup()
{
    /*
     * Register init callbacks. Use the same function for all event types
     */
    sef_setcb_init_fresh(sef_cb_init);
    sef_setcb_init_lu(sef_cb_init);
    sef_setcb_init_restart(sef_cb_init);
 
    /*
     * Register live update callbacks.
     */
    sef_setcb_lu_state_save(sef_cb_lu_state_save);
 
    /* Let SEF perform startup. */
    sef_startup();
}

/*Source: 
 http://wiki.minix3.org/doku.php?id=developersguide:driverprogramming */
static int sef_cb_init(int type, sef_init_info_t *UNUSED(info))
{
/* Initialize the hello driver. */
    int do_announce_driver = TRUE;
 
    open_counter = 0;
    switch(type) {
        case SEF_INIT_FRESH:
        break;
 
        case SEF_INIT_LU:
            /* Restore the state. */
            lu_state_restore();
            do_announce_driver = FALSE;
 
        break;
 
        case SEF_INIT_RESTART:
        break;
    }
 
    /* Announce we are up when necessary. */
    if (do_announce_driver) {
        chardriver_announce();
    }
 
    /* Initialization completed successfully. */
    return OK;
}

/*Source: 
 http://wiki.minix3.org/doku.php?id=developersguide:driverprogramming */
static int sef_cb_lu_state_save(int UNUSED(state)) {
/* Save the state. */
    ds_publish_u32("open_counter", open_counter, DSF_OVERWRITE);
 
    return OK;
}

/*Source: 
 http://wiki.minix3.org/doku.php?id=developersguide:driverprogramming */ 
static int lu_state_restore() {
/* Restore the state. */
    u32_t value;
 
    ds_retrieve_u32("open_counter", &value);
    ds_delete_u32("open_counter");
    open_counter = (int) value;
 
    return OK;
}

/*This is function used when reading or writing to device*/
static int read_write_secret(endpoint_t endpt, int opcode, u64_t position,
 iovec_t *iov, unsigned nr_req, endpoint_t user_endpt, unsigned 
 int UNUSED(flags)) {
 
   int bytes, retVal;
 
   if (SECRET_SIZE - ex64lo(position) > iov->iov_size) {
      bytes = iov->iov_size;
   }
   else {
      bytes = SECRET_SIZE - ex64lo(position);
   }
   
   if (bytes <= 0) {
      return OK;
   }
   /*If opcode == write*/
   if (opcode == DEV_SCATTER_S) {
      retVal = sys_safecopyfrom(user_endpt, (cp_grant_id_t) iov->iov_addr, 0,
       (vir_bytes) (secret + ex64lo(position)), bytes);
      iov->iov_size += bytes;
   }
   /*If opcode == read*/
   else if (opcode == DEV_GATHER_S) {
      retVal = sys_safecopyto(endpt, (cp_grant_id_t) iov->iov_addr, 0,
       (vir_bytes) (secret + ex64lo(position)), bytes);
      iov->iov_size -= bytes;
   }
   else {
      fprintf(stderr, "Unknown opcode during read/write operation!\n");
      return EINVAL;
   }
   /*Return number of bytes read/written*/
   return retVal;
   
}

/*Used when for open call, abstraced by the SEF*/
static int openSecret(message *message) {
   /*message fields defined in driver.h*/
   /*defined in ucred.h, userland view of creds*/
   struct ucred newAccess;
   
   /*Check if trying to open in read/write mode*/
   if (message->COUNT == O_RDWR) {
      fprintf(stderr, "open() was opened in RDWR mode.\nPERMISSION DENIED\n");
      return EACCES;
   }
   
   if (getnucred(message->USER_ENDPT, &newAccess)) {
         fprintf(stderr, "getnucred call failed\n");
         return errno;
   }
   /*See if the secret is owned or not*/
   if (owner == -1) {
      /*The owner of the process becomes owner of secret*/
      
      if (message->COUNT == O_WRONLY) {
         /*Remember who we gave ownership to*/
         owner = newAccess.uid;
         numberOfOpenFileDescriptors++;
      }
      /*Opening empty secret in read only mode*/
      else if (message->COUNT == O_RDONLY) {
         numberOfOpenFileDescriptors++;
         return OK;
      }
      else {
         fprintf(stderr, "Invalid open() COUNT property, must open secret in"
          " O_WRONLY or O_RDONLY\n");
         return -1;
      }
   }
   /*This branch if secret is owned*/
   else {
      /*If the incorrect user no need to check*/
      if (owner != newAccess.uid) {
            fprintf(stderr, "Permission is denied.\n");
            return EACCES;
      }
      /*Reading owned secret is allowed, not writing*/
      if (message->COUNT == O_RDONLY) {
         hasSecretBeenOpenedForReading = 1;
         numberOfOpenFileDescriptors++;
      } 
      else if (message->COUNT == O_WRONLY) {
         fprintf(stderr, "Cannot write, device full\n");
         return ENOSPC;
      }
      else {
         fprintf(stderr, "Invalid open() COUNT property, must open secret in"
          " O_WRONLY or O_RDONLY\n");
         return -1;
      } 
   }
   return OK;
   
}

/*Used by close call, abstracted by SEF*/
static int closeSecret(message *message) {
   int index;
   
   numberOfOpenFileDescriptors--;
   
   /*Check if we have closed last file descriptor, clear secret for safety*/
   if (numberOfOpenFileDescriptors == 0 && hasSecretBeenOpenedForReading) {
      for (index = 0; index < SECRET_SIZE; index++) {
         secret[index] = '\0';
      }
      hasSecretBeenOpenedForReading = 0;
      /*The secret is now empty and nobody owns it*/
      owner = -1; 
   }
   return OK;
}

static struct device *initSecret(dev_t UNUSED(dev)) {
   /* Base and size of a partition in bytes. In driver.h */
   secretDevice.dv_base = 0;
   secretDevice.dv_size = SECRET_SIZE;
   return &secretDevice;
}
