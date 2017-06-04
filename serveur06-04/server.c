/*
 * server.c
 *
 *  Created on: 28 mars 2017
 *      Author: 3301202
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>
#include "server.h"
#include "serv_client.h"
#include <time.h>


static void init(void)
{
#ifdef WIN32
	WSADATA wsa;
	int err = WSAStartup(MAKEWORD(2, 2), &wsa);
	if(err < 0)
	{
		puts("WSAStartup failed !");
		exit(EXIT_FAILURE);
	}
#endif
}

static void end(void)
{
#ifdef WIN32
	WSACleanup();
#endif
}

int actual = 0;
int max ;
Client clients[MAX_CLIENTS];

char placement[225];
pthread_mutex_t client_mut=PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t timer_mut=PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t timer_cond=PTHREAD_COND_INITIALIZER;
pthread_mutex_t plac_mut=PTHREAD_MUTEX_INITIALIZER;
enum phase_enum phase=DEB;
char reservoir[100];
char tirages[100];
char tirage[7];
int tirages_size=100;
int running_session=0;
int tour=0;
time_t fin=0;
int soumission_valide=0;
int meilleur=0;
int trouve=0;

static void app(void)
{
	init_placement();
	init_tirage();
	SOCKET sock = init_connection();

	char buffer[BUF_SIZE];


	max = sock;
	srand(time(NULL));


	pthread_t main_connect_thread;
	init_reservoir();
	//print_placement();
	//printf("%f\n",89/15);
	//new_tirage();

	if(pthread_create(&main_connect_thread,NULL,&main_connexion,&sock)==0){
		printf("thread créé");
	}
	else {
		printf("erreur");
	}
	if(pthread_join(main_connect_thread,NULL)!=0){
		printf("erreur:join");
	}

	/*
	char tab[]="cedric";
	printf("%s\n",tab);
	inverse(tab,strlen(tab));
	printf("%s\n",tab);*/
/*	char prop[225];
	int i;
	for(i=0;i<225;i++){
		prop[i]='0';
	}




	ajouter_lettre(placement,'R',7,6);
	ajouter_lettre(placement,'U',7,7);
	ajouter_lettre(placement,'E',7,8);
	ajouter_lettre(placement,'P',5,6);
	ajouter_lettre(placement,'0',6,6);
	ajouter_lettre(placement,'T',8,6);
	ajouter_lettre(placement,'E',6,9);
	ajouter_lettre(placement,'S',7,9);
	ajouter_lettre(placement,'T',8,9);
	strcpy(prop,placement);



	//ajouter_lettre(prop,'e',6,9);

	char raison[3];
	char mot[15];
	int sc=placement_valide(prop,raison,mot);
	if(!sc){
		printf("raison:%s\n",raison);
	}
	else{
		printf("score:%d\n",sc);
		printf("mot:%s\n",mot);
	}*/

	//char mot_horiz[15];
	//printf("mot verifie %d\n",ismothorizontal(mot_horiz,prop,changees));


	//printf("%d\n",dico_verif("cedric"));
	//printf("%d\n",dico_verif("arbre"));



}

static void clear_clients(Client *clients, int actual)
{
	int i = 0;
	for(i = 0; i < actual; i++)
	{
		closesocket(clients[i].sock);
	}
}

static void remove_client(Client *clients, char * username)
{
	/* we remove the client in the array */
	int to_remove;
	int trouve=0;

	pthread_mutex_lock(&client_mut);
	for(to_remove=0;to_remove<actual;to_remove++){
		if(strcmp(clients[to_remove].name,username)==0){
			trouve=1;
			break;
		}
	}
	if(trouve==0){
		printf("removeclient:non trouve\n");
		return ;
	}
	memmove(clients + to_remove, clients + to_remove + 1, (actual - to_remove - 1) * sizeof(Client));
	/* number client - 1 */
	(actual)--;
	pthread_mutex_unlock(&client_mut);
}

static void send_message_to_all_clients(char* client, const char *buffer, char from_server)
{
	int i = 0;
	char message[BUF_SIZE];
	message[0] = 0;
	pthread_mutex_lock(&client_mut);
	strncat(message, buffer, sizeof message - strlen(message) - 1);
	strcat(message,"\n");
	for(i = 0; i < actual; i++)
	{


		if(from_server==1 || strcmp(clients[i].name,client)!=0){
			write_client(clients[i].sock, message);
		}


	}
	pthread_mutex_unlock(&client_mut);
}

static int init_connection(void)
{
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN sin = { 0 };

	if(sock == INVALID_SOCKET)
	{
		perror("socket()");
		exit(errno);
	}

	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_port = htons(PORT);
	sin.sin_family = AF_INET;

	if(bind(sock,(SOCKADDR *) &sin, sizeof sin) == SOCKET_ERROR)
	{
		perror("bind()");
		exit(errno);
	}

	if(listen(sock, MAX_CLIENTS) == SOCKET_ERROR)
	{
		perror("listen()");
		exit(errno);
	}

	return sock;
}

static void end_connection(int sock)
{
	closesocket(sock);
}

static int read_client(SOCKET sock, char *buffer)
{
	int n = 0;

	if((n = recv(sock, buffer, BUF_SIZE - 1, 0)) < 0)
	{
		perror("recv()");
		/* if recv error we disonnect the client */
		n = 0;
	}

	buffer[n] = 0;

	return n;
}

static void write_client(SOCKET sock, const char *buffer)
{
	if(send(sock, buffer, strlen(buffer), 0) < 0)
	{
		perror("send()");
		//exit(errno);
	}
}

static int starts_with(char *mot,char * debut){
	return (strncmp(mot,debut,strlen(debut))==0);

}

static int ends_with(char* mot, char fin){
	return (mot[strlen(mot)-1]==fin);
}

static void extract_username(char* connexion,char* username){
	strcpy(username,&connexion[strlen("CONNEXION/")]);
	username[strlen(username)-2]='\0';
}
static void extract_placement(char* trouve,char* placement){
	strcpy(placement,&trouve[strlen("TROUVE/")]);
	placement[strlen(placement)-2]='\0';
}


static void* connexion_client(void * arg){
	/* new client */
	//printf("%f\n", (double) clock()/CLOCKS_PER_SEC);

	char buffer[1024];
	SOCKET *csock=(SOCKET*)arg;
	//SOCKADDR_IN csin = { 0 };
	//size_t sinsize = sizeof csin;
	//printf("avant accept");
	//printf("%d",(int)sock);

	//	int csock = //accept(*sock, (SOCKADDR *)&csin, &sinsize);//socket de communication avec le client*/
	printf("a accepté");
	//pthread_cond_signal(&accept_cond);
	if(csock == SOCKET_ERROR)
	{
		perror("accept()");
		return NULL;
	}

	/* after connecting the client sends its name */
	char lecture[BUF_SIZE];
	//printf("avant lecture");
	if(read_client(csock, lecture) == -1)
	{
		/* disconnected */
		printf("erreur :read_client\n");
		pthread_exit(NULL);
	}
	printf("xlient lu\n");
	printf("lecture:%s\n",lecture);

	if(starts_with(lecture,"CONNEXION/")/*&& ends_with((lecture),'/' /*&& strlen(lecture)>11)*/){
		/* what is the new maximum fd ? */
		max = csock > max ? csock : max;
		//on ajoute le socket dans la liste des descr
		//printf("avant fd_set");
		//pthread_mutex_lock(&rdfs_mut);
		//FD_SET(csock, &rdfs);
		//pthread_mutex_unlock(&rdfs_mut);
		//Ajout du client et de son nom dans la liste des sockets
		printf("lecture:%s\n",lecture);
		char username[BUF_SIZE];
		extract_username(lecture,username);
		printf("name: %s\n",username);
		Client c = { csock };
		if(!avail_user(username)){
			char refus[]="REFUS/";
			strcat(refus,"\n");
			//printf("refus");
			write_client(csock,refus);
			//closesocket(csock);
			return NULL;
		}


		//printf("%s\n",username);
		strncpy(c.name, username, BUF_SIZE - 1);
		c.score=0;
		c.curr_score=0;
		//printf("%s\n",c.name);
		pthread_mutex_lock(&client_mut);
		clients[actual] = c;
		actual++;
		pthread_mutex_unlock(&client_mut);
		strcpy(buffer,"CONNECTE/");
		strcat(buffer,c.name);
		strcat(buffer,"/");
		send_message_to_all_clients(username, buffer, 0);
		pthread_t session;
		char bienvenue[BUF_SIZE];
		strcpy(bienvenue,"BIENVENUE/");
		pthread_mutex_lock(&plac_mut);
		strcat(bienvenue,placement);
		pthread_mutex_unlock(&plac_mut);
		strcat(bienvenue,"/");
		if(actual==1 && !running_session){
			if(pthread_create(&session,NULL,&session_thread,NULL)!=0){
				printf("erreur:ecoute client");
			}
			sleep(1);


		}


		strcat(bienvenue,tirage);
		strcat(bienvenue,"/");
		char score[BUF_SIZE];
		scores(score);
		strcat(bienvenue,score);
		strcat(bienvenue,"/");
		char phase_str[3];
		get_phase(phase_str);
		strcat(bienvenue,phase_str);
		strcat(bienvenue,"/");
		char str_time[50];
		int time=get_time();
		if(time<0){time=0;}
		sprintf( str_time, "%d", time );
		strcat(bienvenue,str_time);
		strcat(bienvenue,"/");
		strcat(bienvenue,"\n");
		write_client(csock,bienvenue);
		//printf("write bienvenue");
		pthread_t ecoute;

		//printf("avant ecoute client");
		if(pthread_create(&ecoute,NULL,&ecoute_client,username)!=0){
			printf("erreur:ecoute client\n");
		}
		//print_clients();




		//return NULL;
	}
	else{
		printf("demande invalide\n");

		printf("erreur:connexin client");
		closesocket(csock);
		pthread_exit(NULL);
	}

}


static void* main_connexion(void * arg){
	printf("main_connexion");
	SOCKET *sock=(SOCKET*)arg;
	while(1){
		//socket de communication avec le client*/
		pthread_t t;
		SOCKADDR_IN csin = { 0 };
		size_t sinsize = sizeof csin;
		int csock = accept(*sock, (SOCKADDR *)&csin, &sinsize);
		if(pthread_create(&t,NULL,&connexion_client,csock)!=0){
			printf("erreur:connexion client");
		}
		//wait accept
		//pthread_cond_wait(&accept_cond,&accept_mut);
		//connexion réussie
	}
	return NULL;
}

static int avail_user(char * username){
	int i;
	pthread_mutex_lock(&client_mut);

	for(i=0;i<actual;i++){
		printf("client %d : %s\n",i,clients[i].name);
		if(!strcmp(username,clients[i].name)){
			pthread_mutex_unlock(&client_mut);
			return 0;
		}
	}
	pthread_mutex_unlock(&client_mut);
	return 1;
}

static void* ecoute_client(void * arg){
	printf("ecoute client");
	char *username=(char*)arg;

	SOCKET csock=(seek_client(username))->sock;
	while(1){
		char lecture[BUF_SIZE];
		//printf("avant lecture");




		if(read_client(csock, lecture) == 0 ||(starts_with(lecture,"SORT/")&& ends_with(lecture,'/')))
		{
			/* disconnected */
			strcpy(lecture,"DECONNEXION/");

			strcat(lecture,username);

			strcat(lecture,"/");

			send_message_to_all_clients(username,lecture,0);
			remove_client(clients,username);
			closesocket(csock);

			return NULL;
		}
		if(starts_with(lecture,"TROUVE/")&& /*ends_with(lecture,'/'))*/phase==REC){
			//TODO
			char raison[3];
			char prop[225];
			char reponse[BUF_SIZE];
			extract_placement(lecture,prop);
			int res;
			char mot[15];
			if(tour==1){
				res=valide_init(prop,raison,mot);
			}
			else{
				res=placement_valide(prop,raison,mot);
			}
			if(res==0){
				strcpy(reponse,"RINVALIDE/");
				strcat(reponse,raison);
				strcat(reponse,"/");
				strcat(reponse,"\n");
				write_client(csock,reponse);
			}
			else if(stock_score(res,username,prop,mot)==0){
				strcpy(reponse,"RINVALIDE/INF/");
				strcat(reponse,"\n");
				write_client(csock,reponse);
			}
			else{
				pthread_cond_signal(&timer_cond);
				trouve=1;
				strcpy(reponse,"RVALIDE/");
				strcat(reponse,"\n");
				write_client(csock,reponse);
				char message[BUF_SIZE];
				strcpy(message,"RATROUVE/");
				strcat(message,username);
				strcat(message,"/");
				strcat(message,"\n");
				send_message_to_all_clients(username,message,0);
			}
			printf("rectrouve\n");
		}
		if((starts_with(lecture,"TROUVE/")&& ends_with(lecture,'/'))&& phase==SOU){
				//TODO
				char raison[3];
				char prop[225];
				char reponse[BUF_SIZE];
				extract_placement(lecture,prop);
				int res;
				char mot[15];
				if(tour==1){
					res=valide_init(prop,raison,mot);
				}
				else{
					res=placement_valide(prop,raison,mot);
				}
				if(res==0){
					strcpy(reponse,"SINVALIDE/");
					strcat(reponse,raison);
					strcat(reponse,"/");
					strcat(reponse,"\n");
					write_client(csock,reponse);
				}
				else if(stock_score(res,username,prop,mot)==0){
					strcpy(reponse,"SINVALIDE/INF/");
					strcat(reponse,"\n");
					write_client(csock,reponse);
				}
				else{
					pthread_cond_signal(&timer_cond);
					trouve=1;
					strcpy(reponse,"SVALIDE/");
					strcat(reponse,"\n");
					write_client(csock,reponse);

				}
				printf("soutrouve\n");
			}


	}
}

Client* seek_client(char *username){
	int i;
	pthread_mutex_lock(&client_mut);
	for(i=0;i<actual;i++){
		if(strcmp(clients[i].name,username)==0){
			pthread_mutex_unlock(&client_mut);
			return &clients[i];
		}
	}
	pthread_mutex_unlock(&client_mut);
	return NULL;
}

void timer(int secs){
	fin=time(NULL)+secs;
	sleep(secs);
}

double get_time(){
	return difftime(fin,time(NULL));
}

void print_clients(){
	pthread_mutex_lock(&client_mut);
	int i;
	for(i=0;i<actual;i++){
		printf("Client %d: %s\n",i,clients[i].name);
	}
	pthread_mutex_unlock(&client_mut);

}

void* session_thread(void * arg){
	new_tirage();
	running_session=1;
	timer(20);
	pthread_mutex_lock(&client_mut);
	int act_tmp=actual;
	pthread_mutex_unlock(&client_mut);

	send_message_to_all_clients(NULL,"SESSION/",1);
	while(tirages_size>0 && act_tmp>0){
		tour++;


		phase=REC;
		char buf[BUF_SIZE];
		strcpy(buf,"TOUR/");
		pthread_mutex_lock(&plac_mut);
		strcat(buf,placement);
		pthread_mutex_unlock(&plac_mut);
		strcat(buf,"/");
		strcat(buf,tirage);
		strcat(buf,"/");
		send_message_to_all_clients(NULL,buf,1);
		pthread_t timer_thread;
		int att_rec=300;
		pthread_create(&timer_thread,NULL,&timer_signal,&att_rec);
		timer_wait();
		if(!trouve){
			char rfin[10];
			strcpy(rfin,"RFIN/");
			send_message_to_all_clients(NULL,rfin,1);
			new_tirage();
			pthread_mutex_lock(&client_mut);
			act_tmp=actual;
			pthread_mutex_unlock(&client_mut);
			continue;
		}
		phase=SOU;
		timer(120);
		char sfin[10];
		strcpy(sfin,"SFIN/");
		send_message_to_all_clients(NULL,sfin,1);
		phase=RES;
		char mot[15];
		char vainqueur[BUF_SIZE];
		bilan(mot,vainqueur);
		char bilan_str[BUF_SIZE];
		strcpy(bilan_str,"BILAN/");
		strcat(bilan_str,mot);
		strcat(bilan_str,"/");
		strcat(bilan_str,vainqueur);
		char score_str[BUF_SIZE];
		scores(score_str);
		strcat(bilan_str,"/");
		strcat(bilan_str,score_str);
		strcat(bilan_str,"/");
		send_message_to_all_clients(NULL,bilan_str,1);
		timer(10);


	}
	timer(120);
	pthread_t session;
	init_reservoir();
	if(pthread_create(&session,NULL,&session_thread,NULL)!=0){
					printf("erreur:creation session");
	}
	printf("session terminée");
	pthread_exit(NULL);
}

void init_placement(){
	int i;
	for(i=0;i<225;i++){
		placement[i]='0';
	}
}
void init_reservoir(){
	int i;
	tirages_size=100;
	for(i=0;i<15;i++){
		reservoir[i]='E';
		tirages[i]='E';
	}
	for(;i<24;i++){
		reservoir[i]='A';
		tirages[i]='A';
	}
	for(;i<32;i++){
		reservoir[i]='I';
		tirages[i]='I';
	}
	for(;i<38;i++){
		reservoir[i]='N';
		tirages[i]='N';
	}
	for(;i<44;i++){
		reservoir[i]='O';
		tirages[i]='O';
	}
	for(;i<50;i++){
		reservoir[i]='R';
		tirages[i]='R';
	}
	for(;i<56;i++){
		reservoir[i]='S';
		tirages[i]='S';
	}
	for(;i<62;i++){
		reservoir[i]='T';
		tirages[i]='T';
	}
	for(;i<68;i++){
		reservoir[i]='U';
		tirages[i]='U';
	}
	for(;i<73;i++){
		reservoir[i]='L';
		tirages[i]='L';
	}
	for(;i<76;i++){
		reservoir[i]='D';
		tirages[i]='D';
	}
	for(;i<79;i++){
		reservoir[i]='M';
		tirages[i]='M';
	}
	for(;i<81;i++){
		reservoir[i]='G';
		tirages[i]='G';
	}
	for(;i<83;i++){
		tirages[i]='B';
		reservoir[i]='B';
	}
	for(;i<85;i++){
		reservoir[i]='B';
		tirages[i]='C';
	}
	for(;i<87;i++){
		reservoir[i]='P';
		tirages[i]='P';
	}
	for(;i<89;i++){
		reservoir[i]='F';
		tirages[i]='F';
	}
	for(;i<91;i++){
		reservoir[i]='H';
		tirages[i]='H';
	}
	for(;i<93;i++){
		reservoir[i]='V';
		tirages[i]='V';
	}
	for(;i<94;i++){
		reservoir[i]='J';
		tirages[i]='J';
	}
	for(;i<95;i++){
		reservoir[i]='Q';
		tirages[i]='Q';
	}
	for(;i<96;i++){
		reservoir[i]='K';
		tirages[i]='K';
	}
	for(;i<97;i++){
		reservoir[i]='W';
		tirages[i]='W';
	}
	for(;i<98;i++){
		reservoir[i]='X';
		tirages[i]='X';
	}
	for(;i<99;i++){
		reservoir[i]='Y';
		tirages[i]='Y';
	}
	for(;i<100;i++){
		reservoir[i]='Z';
		tirages[i]='Z';
	}
	//printf("reservoir %s\n",reservoir);
}
void new_tirage(){

	int i;
	for(i=0;i<7;i++){
		int a=rand_a_b(0,tirages_size);
		tirage[i]=tirages[a];
		memmove(tirages + a, tirages + a + 1, (tirages_size - a - 1) * sizeof(char));
		tirages_size--;

		//printf("tirages %s\n",tirages);
		//printf("tirages_size %d\n",tirages_size);

	}
	//printf("tirage %s\n",tirage);
}
void init_tirage(){
	int i;
	for(i=0;i<7;i++){
		tirage[i]=0;
	}
}
int rand_a_b(int a, int b){
	return rand()%(b-a) +a;
}
void scores (char * buf){
	sprintf(buf,"%d",tour);
	int i;
	pthread_mutex_lock(&client_mut);
	for(i=0;i<actual;i++){
		strcat(buf,"*");
		strcat(buf,clients[i].name);
		strcat(buf,"*");
		char tmp[10];
		sprintf(tmp,"%d",clients[i].score);
		strcat(buf,tmp);
	}
	printf("%s\n",buf);
	pthread_mutex_unlock(&client_mut);


}

void get_phase (char* str){
	switch(phase){
	case DEB:
		strcpy(str,"DEB");
		break;
	case REC:
		strcpy(str,"REC");
		break;
	case SOU:
		strcpy(str,"SOU");
		break;
	case RES:
		strcpy(str,"SOU");
		break;
	default:
		printf("erreur:get phase");
	}
}
int dico_verif(char * mot){
	FILE * f=fopen("dico.txt","r");
	char chaine[BUF_SIZE];
	char tmp[BUF_SIZE];
	strcpy(tmp,mot);
	strcat(tmp,"\n");

	if (f != NULL)
	{
		while (fgets(chaine, BUF_SIZE, f) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
		{
			if(strcmp(chaine,tmp)==0){
				fclose(f);
				return 1;
			}
			// On affiche la chaîne qu'on vient de lire
		}

		fclose(f);
		return 0;
	}
	printf("erreur ouverture fichier");
	return 0;
}
void lower_string(char * str){
	int i;
	for(i = 0; str[i]; i++){
		str[i] = tolower(str[i]);
	}
}
/*void timer_wait
 * fin=time(NULL)+secs
 * wait sur condition*/
/*void timer signal
 * sleep(secs);
 * signal condition
 * */
/*int placement valide(char * prop)
 * si pas d'enlevement
 * si nbnewlignes==1 &&  la ligne est liée à un mot déja présent && toutes les colonnes sont liées à un mot déja présent
 * && toutes les lignes et les colonnes sont dans le dico
 * alors placement valide calcul des points
 * similaire pour les colonnes
 *
 * */


int placement_valide(char * prop,char *res,char * mot){
	char plac_tmp[225];
	pthread_mutex_lock(&plac_mut);
	strcpy(plac_tmp,placement);
	pthread_mutex_unlock(&plac_mut);
	if(!verif_presents(plac_tmp,prop)){
		strcpy(res,"POS");
		return 0;
	}
	int changees[225];
	if(!cases_changees(plac_tmp,prop,changees)){
		strcpy(res,"POS");
		return 0;
	}

	if(verif_lignes(changees)){

		return concat_ligne(plac_tmp,prop,changees,res,mot);
	}
	else if (verif_colonnes(changees)){

		return concat_colonnes(plac_tmp,prop,changees,res,mot);
	}
	strcpy(res,"POS");
	return 0;
	//pthread_cond_signal(&timer_cond);

}

int valide_init(char * prop,char *res,char * mot){
	printf("valide init\n");
	char plac_tmp[225];
	pthread_mutex_lock(&plac_mut);
	strcpy(plac_tmp,placement);
	pthread_mutex_unlock(&plac_mut);
	int changees[225];
	if(!cases_changees(plac_tmp,prop,changees)){
		strcpy(res,"POS");
		return 0;
	}
	if(!milieu(changees)){
		printf("milieu\n");
		strcpy(res,"POS");
		return 0;
	}
	if(verif_lignes(changees)){
		if(!suite_ligne(changees)){
			printf("suitel\n");
			strcpy(res,"POS");
			return 0;
		}

		from_changees(changees,mot,prop);
		if(!dico_verif(mot)){
			printf("dico\n");
			strcpy(res,"DIC");
			return 0;
		}
		return calcul_score(mot);
	}
	else if (verif_colonnes(changees)){
		if(!suite_colonne(changees)){
			printf("suitec\n");
			strcpy(res,"POS");
			return 0;
		}


		from_changees(changees,mot,prop);
		if(!dico_verif(mot)){
			printf("dico\n");
			strcpy(res,"DIC");
			return 0;
		}
		return calcul_score(mot);
	}
	strcpy(res,"POS");
	return 0;


}
int calcul_score(char * mot){
	int score=0;
	int i;
	for(i=0;mot[i];i++){
		switch(mot[i]){
			case 'A':
				score+=1;
				break;
			case 'E':
				score+=1;
				break;
			case 'I':
				score+=1;
				break;
			case 'N':
				score+=1;
				break;
			case 'O':
				score+=1;
				break;
			case 'R':
				score+=1;
				break;
			case 'S':
				score+=1;
				break;
			case 'T':
				score+=1;
				break;
			case 'L':
				score+=1;
				break;
			case 'U':
				score+=1;
				break;
			case 'D':
				score+=2;
				break;
			case 'M':
				score+=2;
				break;

			case 'G':
				score+=2;
				break;
			case 'B':
				score+=3;
				break;
			case 'C':
				score+=3;
				break;
			case 'P':
				score+=3;
				break;
			case 'F':
				score+=4;
				break;
			case 'H':
				score+=4;
				break;
			case 'V':
				score+=4;
				break;
			case 'J':
				score+=8;
				break;
			case 'Q':
				score+=8;
				break;
			case 'K':
				score+=10;
				break;
			case 'W':
				score+=10;
				break;
			case 'X':
				score+=10;
				break;
			case 'Y':
				score+=10;
				break;
			case 'Z':
				score+=10;
				break;

		}

	}
	return score;
}

int milieu(int *changees){
	int i;
	for(i=0;changees[i]!=225;i++){
		if(changees[i]==112){
			return 1;
		}
	}
	return 0;
}

int suite_ligne(int * changees){
	int i;
	for(i=1;changees[i]!=225;i++){
		if(changees[i]!=changees[i-1]+1 && changees[i]/15!=changees[i-1]/15){
			return 0;
		}
	}
	return 1;
}
int suite_colonne(int * changees){
	int i;
	for(i=1;changees[i]!=225;i++){
		if(changees[i]!=changees[i-1]+15){
			return 0;
		}
	}
	return 1;
}

int verif_presents(char *plac,char* prop){
	int i;
	for(i=0;i<225;i++){
		if(plac[i]!='0' && prop[i]!=plac[i]){
			return 0;
		}
	}
	return 1;
}
void from_changees(int* changees,char *res,char *plac){
	int res_it=0;
	int ch_it;
	for(ch_it=0;changees[ch_it]<225;ch_it++){
		res[res_it]=plac[changees[ch_it]];
		res_it++;
	}
	res[res_it]='\0';

}

void print_placement(){
	char plac_tmp[225];
	pthread_mutex_lock(&plac_mut);
	strcpy(plac_tmp,placement);
	pthread_mutex_unlock(&plac_mut);
	int i;
	int j;
	printf("plateau:\n");
	for(i=0;i<15;i++){
		for(j=0;j<15;j++){
			printf("%c",plac_tmp[(15*i)+j]);
		}
		printf("\n");
	}
}

int cases_changees(char * plateau, char * prop, int * res){

	int res_i=0;
	int i;
	for(i=0;i<225;i++){
		if(plateau[i]!=prop[i]){
			res[res_i]=i;
			res_i++;
		}
	}
	res[res_i]=225;
	if(res_i==0){
		return 0;
	}
	return 1;

}

int verif_lignes(int * changees){//verifie que toutes les lettres modifiées sont sur la meme ligne

	int ligne=changees[0]/15;
	int i=1;
	while(changees[i]!=225){
		if(ligne!=changees[i]/15){
			return 0;
		}
		i++;
	}
	return 1;

}

int verif_colonnes(int * changees ){//verifie que toutes les lettres modifiées sont sur la meme colonne
	int colonne=changees[0]%15;
	int i=1;
	while(changees[i]!=225){
		if(colonne!=changees[i]%15){
			return 0;
		}
		i++;
	}
	return 1;
}

int concat_ligne(char * plac,char * prop,int * changees,char * raison,char * mot){//créé le mot qui est formé avec les lettres changées
	int deb=changees[0];

	int ligne=changees[0]/15;
	char res1[16];
	char res2[16];
	char res[16];
	int j=0;

	//on parcourt la ligne en arriere tant qu'on n'a pas fini le mot et qu'on reste sur la meme ligne
	while(prop[deb]!='0'&& ligne==deb/15){
		//printf("char %c\n",prop[deb]);
		res1[j]=prop[deb];
		j++;
		deb--;
	}

	res1[j]='\0';
	//printf("res1: %s\n",res1);
	deb=changees[0]+1;
	j=0;

	//on parcourt la ligne vers l'avant tant qu'on n'a pas fini le mot et qu'on reste sur la meme ligne
	while(prop[deb]!='0'&& ligne==deb/15){
		printf("char %c\n",prop[deb]);
		res2[j]=prop[deb];
		j++;
		deb++;
	}
	res2[j]='\0';
	//printf("res2: %s\n",res1);

	inverse(res1,strlen(res1));
	strcat(res1,res2);
	strcpy(res,res1);
	strcpy(mot,res);



	if(!verif_intersection(plac,changees,res)){
		strcpy(raison,"POS");
		return 0;
	}

	if(!dico_verif(res)){
		strcpy(raison,"DIC");
		return 0;
	}
	int score=calcul_score(res);
	int ver= ismotvertical(raison,prop,changees);
	if(ver>=0){
		return score+ver;
	}
	return 0;

}

int concat_colonnes(char*plac,char * prop,int * changees,char * raison,char * mot){
	int deb=changees[0];


	char res1[16];
	char res2[16];
	char res[16];
	int j=0;
	//on parcourt la colonne en arriere tant qu'on n'a pas fini le mot et qu'on reste sur le plateau
	while(prop[deb]!='0'&& deb>0){
		//printf("char %c\n",prop[deb]);
		res1[j]=prop[deb];
		j++;
		deb=deb-15;
	}
	res1[j]='\0';
	deb=changees[0]+15;
	j=0;
	//on parcourt la colonne en avant tant qu'on n'a pas fini le mot et qu'on reste sur le plateau
	while(prop[deb]!='0'&& deb<225){
		//printf("char %c\n",prop[deb]);
		res2[j]=prop[deb];
		j++;
		deb=deb+15;
	}


	res2[j]='\0';
	inverse(res1,strlen(res1));
	strcat(res1,res2);
	strcpy(res,res1);
	strcpy(mot,res);
	printf("concat:%s\n",mot);
	if(!verif_intersection(plac,changees,res)){
		printf("verifinters:\n");
		strcpy(raison,"POS");
		return 0;
	}
	if(!dico_verif(res)){
		strcpy(raison,"DIC");
		return 0;
	}
	int score=calcul_score(res);
	int hor= ismothorizontal(raison,prop,changees);
	if(hor>=0){
		return score+hor;
	}
	strcpy(raison,"");
	return 0;
}


int ismotvertical(char * raison,char * prop,int* changees){
	int k;
	int ver=1;
	int size_c=0;
	int h;
	int score=0;

	for(h=0;changees[h]!=225;h++){
		size_c++;
	}
	for(k=0;k<size_c;k++)
	{
		int deb=changees[k];


		char res1[16];
		char res2[16];
		char res[15];
		int j=0;
		//on parcourt la colonne en arriere tant qu'on n'a pas fini le mot et qu'on reste sur le plateau
		while(prop[deb]!='0'&& deb>0){
			//printf("char %c\n",prop[deb]);
			res1[j]=prop[deb];
			j++;
			deb=deb-15;
		}
		res1[j]='\0';
		deb=changees[k]+15;
		j=0;
		//on parcourt la colonne en avant tant qu'on n'a pas fini le mot et qu'on reste sur le plateau
		while(prop[deb]!='0'&& deb<225){
			//printf("char %c\n",prop[deb]);
			res1[j]=prop[deb];
			j++;
			deb=deb+15;
		}


		res2[j]='\0';
		inverse(res1,strlen(res1));
		strcat(res1,res2);
		strcpy(res,res1);
		printf("motvertical:%s\n",res);
		if(strlen(res)>1){

			ver=dico_verif(res);
			if(!ver){
				strcpy(raison,"DIC");
				return -1;
			}
			score+=calcul_score(res);
		}

	}

	return score;
}
int ismothorizontal(char * raison,char * prop,int* changees){
	int k;
	int ver=1;
	int size_c=0;
	int h;
	int score=0;
	for(h=0;changees[h]!=225;h++){
		size_c++;
	}
	for(k=0;k<size_c;k++)
	{
		int deb=changees[k];

		int ligne=changees[k]/15;
		char res1[16];
		char res2[16];
		char res[15];
		int j=0;

		//on parcourt la ligne en arriere tant qu'on n'a pas fini le mot et qu'on reste sur la meme ligne
		while(prop[deb]!='0'&& ligne==deb/15){
			//printf("char %c\n",prop[deb]);
			res1[j]=prop[deb];
			j++;
			deb--;
		}

		res1[j]='\0';
		//printf("res1: %s\n",res1);
		deb=changees[k]+1;
		j=0;

		//on parcourt la ligne vers l'avant tant qu'on n'a pas fini le mot et qu'on reste sur la meme ligne
		while(prop[deb]!='0'&& ligne==deb/15){
			printf("char %c\n",prop[deb]);
			res2[j]=prop[deb];
			j++;
			deb++;
		}
		res2[j]='\0';
		//printf("res2: %s\n",res1);

		inverse(res1,strlen(res1));
		strcat(res1,res2);
		strcpy(res,res1);
		if(strlen(res)>1){
			printf("dicoverif:%s\n",res);
			ver=dico_verif(res);
			if(!ver){

				strcpy(raison,"DIC");
				return -1;
			}
			score+=calcul_score(res);
		}




	}

	return score;
}

int verif_intersection(char * plateau,int * changees,char * res){
	int size_c=0;
	int h;
	for(h=0;changees[h]!=225;h++){
		size_c++;
	}
	if(size_c==strlen(res)){
		for(h=0;changees[h]!=225;h++){
			if( (changees[h]-1>=0 && (changees[h]-1)/15==changees[h]/15 && plateau[changees[h]-1]!='0')
					|| (changees[h]+1<225 && (changees[h]+1)/15==changees[h]/15 && plateau[changees[h]+1]!='0')
					||(changees[h]+15<225  && plateau[changees[h]+15]!='0')
					|| (changees[h]-15>=0  && plateau[changees[h]-15]!='0')){
				return 1;
			}

		}
		return 0;
	}
	else{
		return 1;
	}

}




void inverse(char arr[], int len) {
	char i = 0, tmp;
	for(i=0 ; i < len/2 ; i++) {
		tmp = arr[i];
		arr[i] = arr[len-i-1];
		arr[len-i-1] = tmp;
	}
}
void ajouter_lettre(char *plateau,char lettre, int ligne ,int colonne){
	plateau[(15*ligne)+colonne]=lettre;
	int i,j;
	printf("plateau:\n");
	for(i=0;i<15;i++){
		for(j=0;j<15;j++){
			printf("%c",plateau[15*i+j]);
		}
		printf("\n");
	}
}
void timer_wait(){
	pthread_cond_wait(&timer_cond,&timer_mut);
}
void* timer_signal(void * arg){
	int * temps=(int*)arg;
	sleep(*temps);
	pthread_cond_signal(&timer_cond);
	pthread_exit(NULL);
}
int stock_score(int score,char *username,char * plat,char *mot){
	int i;
	pthread_mutex_lock(&client_mut);
	for(i=0;i<actual;i++){
		if(strcmp(clients[i].name,username)==0){
			if(score>clients[i].curr_score){
				clients[i].curr_score=score;
				strcpy(clients[i].curr_plat,plat);
				strcpy(clients[i].mot,mot);
				pthread_mutex_unlock(&client_mut);
				return score;
			}
			else{
				pthread_mutex_unlock(&client_mut);
				return 0;
			}

		}
	}
	pthread_mutex_unlock(&client_mut);
	return 0;
}
void bilan(char * mot, char *vainqueur){
	int i;
	int max=0;
	char pl_tmp[225];
	pthread_mutex_lock(&client_mut);
	for(i=0;i<actual;i++){
		if(clients[i].curr_score>max){
			max=clients[i].curr_score;
			strcpy(vainqueur,clients[i].name);
			strcpy(mot,clients[i].mot);
			strcpy(pl_tmp,clients[i].curr_plat);
		}
		clients[i].score+=clients[i].curr_score;
		clients[i].curr_score=0;
	}
	pthread_mutex_unlock(&client_mut);
	pthread_mutex_lock(&plac_mut);
	strcpy(placement,pl_tmp);
	pthread_mutex_unlock(&plac_mut);
}
int main(int argc, char **argv)
{
	init();

	app();

	end();

	return EXIT_SUCCESS;
}
