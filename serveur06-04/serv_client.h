#ifndef CLIENT_H
#define CLIENT_H

#define BUF_SIZE 1024
#include "server.h"

typedef struct
{
   SOCKET sock;
   char name[BUF_SIZE];
   int score;
   int curr_score;
   char curr_plat[225];
   char mot[15];
}Client;

#endif /* guard */
