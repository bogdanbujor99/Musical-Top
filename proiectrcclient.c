#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno;

/* portul de conectare la server*/
int port;

void encrypt(char parola[100])
{
  FILE *fp; char buff=0;
  char path[1035];
  char comanda_exec[1024]="exec echo -n ";
  strcat(comanda_exec,parola);
  strcat(comanda_exec," | sha256sum");
  
  fp =(FILE*) popen(comanda_exec, "r");

  if (fp == NULL) {
    printf("Failed to run command\n" );
    exit(1);
  }
  strcpy(parola,"\0");
  int i=0;
  while(fread(&buff, sizeof(buff), 1, fp))
  {
    parola[i]=buff;
    i++;
  }
  parola[i]='\0';
  pclose(fp);
}

void adaugare_melodie(int sd)
{ 
  char raspuns[100],date[100];
  bzero(raspuns,100);
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//melodie
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis melodia
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//tipuri melodie
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis tipurile
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//link
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis linkul
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//descriere
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis linkul
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");//raspunsul de comanda
  printf("%s\n",raspuns);
}

void votare(int sd)
{
  char raspuns[100],date[100];
  bzero(raspuns,100);
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//melodie
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis melodia
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");//raspunsul la comanda
  printf("%s\n",raspuns);
}

void vizualizare_top(int sd)
{
  char raspuns[1024];
  bzero(raspuns,100);
  if (read (sd, raspuns, 1024) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
}

void vizualizare_top_gen(int sd)
{
  char raspuns[1024],date[100];
  bzero(raspuns,1024);
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//gen
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis melodia
  if (read (sd, raspuns, 1024) < 0)perror ("[client]Eroare la read() de la server.\n");//raspunsul la comanda
  printf("%s\n",raspuns);
}

void adaugare_comentariu(int sd)
{
  char raspuns[100],date[100];
  bzero(raspuns,100);
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//melodie
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//melodia
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//comentariu
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//comentariul
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");//raspuns la comanda
  printf("%s\n",raspuns);
}

void vizualizare_comentarii(int sd)
{
  char raspuns[1024],date[100];
  bzero(raspuns,1024);
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//melodie
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis melodia
  if (read (sd, raspuns, 1024) < 0)perror ("[client]Eroare la read() de la server.\n");//raspunsul la comanda
  printf("%s\n",raspuns);
}

void stergere_melodie(int sd)
{
  char raspuns[100],date[100];
  bzero(raspuns,100);
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//melodie
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis melodia
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");//raspunsul la comanda
  printf("%s\n",raspuns);
}

void restrictionare_vot(int sd)
{
  char raspuns[100],date[100];
  bzero(raspuns,100);
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");
  printf("%s\n",raspuns);
  bzero(date,100);
  fflush (stdout);
  read(0,date,100);//username
  if (write (sd, date, 100) <= 0)perror ("[client]Eroare la write() spre server.\n");//am trimis username ul
  if (read (sd, raspuns, 100) < 0)perror ("[client]Eroare la read() de la server.\n");//raspunsul la comanda
  printf("%s\n",raspuns);
}


int main (int argc, char *argv[])
{
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 
  char username[100],parola[100],info[100],raspuns[100],comenzi[100];// mesajul trimis
  int ok=0;

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3)
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }
  bzero(info,100);
  bzero(raspuns,100);
  bzero(comenzi,100);
  printf("%s","Doriti sa va logati sau sa va inregistrati?\n");
  fflush (stdout);
  read(0,info,100);
  info[strlen(info)-1]='\0';
  if (write (sd, info, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
  if (read (sd, raspuns, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  if(strcmp(info,"login")==0)
  while(ok==0){
  strcpy(info,"");
  bzero (username, 100);
  printf ("Username:");
  fflush (stdout);
  read (0, username, 100);
  bzero (parola, 100);
  printf ("Parola:");
  fflush (stdout);
  read (0, parola, 100);
  parola[strlen(parola)-1]='\0';
  encrypt(parola);
  strcat(info,username);
  strcat(info,"/");
  strcat(info,parola);
  if (write (sd, info, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
  if (read (sd, raspuns, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  
  if(strcmp(raspuns,"1")==0)ok=1;
  else if(strcmp(raspuns,"2")==0)ok=2;
  else printf("Datele introduse nu sunt corecte!");
  if(ok==1)printf("Buna ziua!\nAveti urmatoarele comenzi:\nAdaugare melodie, \nVotare melodie, \nVizualizare top muzical, \nVizualizare top gen muzical, \nAdaugarea unui comentariu,\nQuit \n");
  if(ok==2)printf("Buna ziua!\nAveti urmatoarele comenzi:\nAdaugare melodie, \nVotare melodie, \nVizualizare top muzical, \nVizualizare top gen muzical, \nAdaugarea unui comentariu, \nStergere melodie, \nRestrictionare vot,\nQuit \n");
  }
 
  if(strcmp(info,"inregistrare")==0)
  { 
  strcpy(info,"");
  bzero (username, 100);
  printf ("[client]username:");
  fflush (stdout);
  read (0, username, 100);
  bzero (parola, 100);
  printf ("[client]parola:");
  fflush (stdout);
  read (0, parola, 100);
  parola[strlen(parola)-1]='\0';
  encrypt(parola);
  strcat(info,username);
  strcat(info,"/");
  strcat(info,parola);
  if (write (sd, info, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
  if (read (sd, raspuns, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  if(strcmp(raspuns,"0")==0){ok=1; printf("Buna ziua, aveti deja cont!\nAveti urmatoarele comenzi:\nAdaugare melodie, \nVotare melodie, \nVizualizare top muzical, \nVizualizare top gen muzical, \nAdaugarea unui comentariu,\nQuit \n");}
  else if(strcmp(raspuns,"1")==0){ ok=2; printf("Buna ziua, aveti deja cont!\nAveti urmatoarele comenzi:\nAdaugare melodie, \nVotare melodie, \nVizualizare top muzical, \nVizualizare top gen muzical, \nAdaugarea unui comentariu, \nStergere melodie, \nRestrictionare vot,\nQuit \n");}
  else {ok=1; printf("Buna ziua, aveti deja cont!\nAveti urmatoarele comenzi:\nAdaugare melodie, \nVotare melodie, \nVizualizare top muzical, \nVizualizare top gen muzical, \nAdaugarea unui comentariu,\nQuit \n");}
  }
  while(ok!=0){
  printf("Comanda dorita:");
  bzero(comenzi,100);
  fflush (stdout);
  read(0,comenzi,100);
  if (write (sd, comenzi, 100) <= 0)
    {
      perror ("[client]Eroare la write() spre server.\n");
      return errno;
    }
  if(strcmp(comenzi,"quit\n")==0)
  {
    printf("O zi buna!\n");
    break;
  }
  else if(strcmp(comenzi,"adaugare melodie\n")==0)adaugare_melodie(sd);
  else if(strcmp(comenzi,"votare\n")==0)votare(sd);
  else if(strcmp(comenzi,"vizualizare top\n")==0)vizualizare_top(sd);
  else if(strcmp(comenzi,"vizualizare top gen\n")==0)vizualizare_top_gen(sd);
  else if(strcmp(comenzi,"adaugare comentariu\n")==0)adaugare_comentariu(sd);
  else if(strcmp(comenzi,"vizualizare comentarii\n")==0)vizualizare_comentarii(sd);
  else if(ok==2 && strcmp(comenzi,"stergere melodie\n")==0)stergere_melodie(sd);
  else if(ok==2 && strcmp(comenzi,"restrictionare vot\n")==0)restrictionare_vot(sd);
  else {
  if (read (sd, raspuns, 100) < 0)
    {
      perror ("[client]Eroare la read() de la server.\n");
      return errno;
    }
  printf("%s",raspuns);
    }//else
  }//while
  close (sd);
}
