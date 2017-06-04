/*
 * server.h
 *
 *  Created on: 28 mars 2017
 *      Author: 3301202
 */

#ifndef SERVER_H
#define SERVER_H

#ifdef WIN32

#include <winsock2.h>

#elif defined (linux)

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> /* close */
#include <netdb.h> /* gethostbyname */
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket(s) close(s)
typedef int SOCKET;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr SOCKADDR;
typedef struct in_addr IN_ADDR;

#else

#error not defined for this platform

#endif

#define CRLF		"\r\n"
#define PORT	 	2017
#define MAX_CLIENTS 	100

#define BUF_SIZE	1024

#include "serv_client.h"

static void init(void);
static void end(void);
static void app(void);
static int init_connection(void);
static void end_connection(int sock);
static int read_client(SOCKET sock, char *buffer);
static void write_client(SOCKET sock, const char *buffer);
static void send_message_to_all_clients(char  *client, const char *buffer, char from_server);
static void remove_client(Client *clients, char* username);
static void clear_clients(Client *clients, int actual);
static int starts_with(char* mot,char* debut);
static int ends_with(char* mot, char fin);
static void extract_username(char* connexion,char* username);
static void* connexion_client(void * arg);
static int avail_user(char * username);
static void* main_connexion(void * arg);
static void* ecoute_client(void *arg);
Client* seek_client(char *username);
void print_clients();
double get_time();
void timer(int secs);
void* session_thread(void * arg);
enum phase_enum{DEB,REC,SOU,RES};
void init_placement();
void init_reservoir();
void init_tirage();
void new_tirage();
int rand_a_b(int a, int b);
void scores (char * buf);
void get_phase(char *str);
int dico_verif(char * mot);
int placement_valide(char * prop,char *res,char * mot);
int verif_presents(char *plac,char* prop);
void print_placement();
static void extract_placement(char* trouve,char* placement);
int cases_changees(char * plateau, char * prop, int * res);
int verif_lignes(int * changees);
int verif_colonnes(int * changees );
int concat_ligne(char *plac,char * prop,int * changees,char * res,char *mot);
int concat_colonnes(char * plac,char * prop,int * changees,char * res,char *mot);
int ismotvertical(char * res,char * prop,int* changees);
int ismothorizontal(char * res,char * prop,int* changees);
int verif_intersection(char *plateau,int * changees,char * res);
void inverse(char arr[], int len);
void ajouter_lettre(char *plateau,char lettre, int ligne ,int colonne);
void lower_string(char*);
void timer_wait();
void* timer_signal(void * arg);
int valide_init(char * prop,char *res,char *mot);
int milieu(int *changees);
int suite_ligne(int * changees);
int suite_colonne(int * changees);
int verif_presents(char *plac,char* prop);
void from_changees(int* changees,char *res,char *plac);
int calcul_score(char* mot);
int stock_score(int score,char *username,char * plat,char *mot);
void bilan(char* mot,char * vainqueur);

#endif /* guard */
