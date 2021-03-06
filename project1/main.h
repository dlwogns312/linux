#ifndef __MAIN__
#define __MAIN__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>

#include <time.h>
#include <string.h>

//Keys for shared memory
#define KEY_INPUT (key_t) 0x40
#define KEY_OUTPUT (key_t) 0x50

#define BUFF_SIZE 64
#define MAX_BUTTON 9
//Set for input
#define KEY_PRESS 1
#define KEY_RELEASE 0

#define READKEY_BACK 158
#define READKEY_VOLUME_UP 115
#define READKEY_VOLUME_DOWN 114

//Define for semaphore
#define SEMA_MAX 2
#define SEMA_KEY  (key_t)0x47

typedef struct _shm_data_input {
    int readkey;
    int check_terminate;
    unsigned char switchkey[9];
} SHM_INPUT;

typedef struct _shm_data_output {
    int fnd_data;
    unsigned char display_dot[10];
    unsigned char led;
    char text_data[32];
    int check_terminate;
} SHM_OUTPUT;

//semaphore structure
union semun {
    int  val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO (Linux-specific) */                                        
};

int sem_id;
struct sembuf p[SEMA_MAX],v[SEMA_MAX];

#endif