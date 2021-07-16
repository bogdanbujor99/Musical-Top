#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sqlite3.h>

#define PORT 2024

int callbackcautare(void *data,int argc, char **argv, char **NameCol)
{
   int* x=(int*)data;
   for(int i = 0; i<argc; i++){
      if(argv[i]!=NULL)*x=1;
   }
return 0;
}

int callbackafisare(void *data,int argc, char **argv, char **NameCol)
{
  char *x=(char*)data;
  for(int i = 0; i<argc; i++){
      strcat(x,argv[i]);
      strcat(x," ");
   }
  strcat(x,"\n");
  return 0;
}

int login(char msg[100],char user[100])
{
   sqlite3* baza_date;
   char*errorMsg=0;
   char sql[100],username[100],parola[100];
   int rc,ok=0,i=0,j=0,ok_admin=0;
   strcpy(username,"");
   strcpy(parola,"");
   rc=sqlite3_open("proiectrc.db",&baza_date);
   while(msg[i]!='\n'){username[i]=msg[i];i++;}
   username[i]='\0';
   strcpy(user,username);
   i=i+2;
   while(i<=strlen(msg)-1){parola[j]=msg[i];j++;i++;}
   parola[j-1]='\0';
   strcpy(sql,"SELECT * FROM USERS WHERE NAME_USER='");
   strcat(sql,username);
   strcat(sql,"' AND PAROLA='");
   strcat(sql,parola);
   strcat(sql,"';");
   rc=sqlite3_exec(baza_date,sql,callbackcautare,&ok,&errorMsg);
   if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", errorMsg);
          sqlite3_free(errorMsg);
   }
   if(ok==1){
   strcpy(sql,"SELECT * FROM USERS WHERE NAME_USER='");
   strcat(sql,user);
   strcat(sql,"' AND PAROLA='");
   strcat(sql,parola);
   strcat(sql,"' AND ADMIN='1';");
   rc=sqlite3_exec(baza_date,sql,callbackcautare,&ok_admin,&errorMsg);
   if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", errorMsg);
          sqlite3_free(errorMsg);
   }
   if(ok_admin!=0)ok=2;
   }
   sqlite3_close(baza_date);
   return ok;
}

void inregistrare(char msg[100],char user[100])
{
   sqlite3* baza_date;
   char*errorMsg=0;
   char sql[100],username[100],parola[100];
   int rc,i=0,j=0;
   rc=sqlite3_open("proiectrc.db",&baza_date);
   strcpy(username,"");
   strcpy(parola,"");
   while(msg[i]!='\n'){username[i]=msg[i];i++;}
   username[i]='\0';
   i=i+2;
   while(i<=strlen(msg)-1){parola[j]=msg[i];j++;i++;}
   parola[j-1]='\0';
   strcpy(sql,"INSERT INTO USERS VALUES ('");
   strcat(sql,username);
   strcat(sql,"' , '");
   strcat(sql,parola);
   strcat(sql,"',0,1);");
   rc=sqlite3_exec(baza_date,sql,NULL,0,&errorMsg);
   if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", errorMsg);
          sqlite3_free(errorMsg);
   }
   sqlite3_close(baza_date);
   strcpy(user,username);
} 

void adaugare_melodie(int client)
{
   sqlite3* baza_date;
   char*errorMsg=0;
   char msgrasp[100], msg[100],melodie[100],tipuri[100],link[100],descriere[100];
   bzero(msgrasp,100);
   bzero(msg,100);
   strcpy(msgrasp,"melodie:");
   if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
   if (read (client, msg, 100) <= 0)
      {
          close(client);
          exit(1);
      }
   strcpy(melodie,msg);
   strcpy(msgrasp,"tipuri melodie (de forma tip1/tip2/...tipn):");
   if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
   if (read (client, msg, 100) <= 0)
      {
          close(client);
          exit(1);
      }
   strcpy(tipuri,msg);
   strcpy(msgrasp,"link:");
   if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
   if (read (client, msg, 100) <= 0)
      {
          close(client);
          exit(1);
      }
   strcpy(link,msg);
   strcpy(msgrasp,"descriere:");
   if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
   if (read (client, msg, 100) <= 0)
      {
          close(client);
          exit(1);
      }
   strcpy(descriere,msg);
   melodie[strlen(melodie)-1]='\0';
   link[strlen(link)-1]='\0';
   descriere[strlen(descriere)-1]='\0';
   int rc,ok=0;
   char sql[100];
   strcpy(sql,"SELECT * FROM MUZICA WHERE NUME_MELODIE='");
   strcat(sql,melodie);
   strcat(sql,"';");
   sqlite3_open("proiectrc.db",&baza_date);
   rc=sqlite3_exec(baza_date,sql,callbackcautare,&ok,&errorMsg);
   sqlite3_close(baza_date);
   if(ok==0)
   {
   int i=0,j=0;
   char* pch;
   pch=strtok(tipuri,"/\n");
   while(pch!=NULL)
        {
           strcpy(sql,"INSERT INTO MUZICA VALUES('");
           strcat(sql,melodie);
           strcat(sql,"','");
           strcat(sql,pch);
           strcat(sql,"',1,'");
           strcat(sql,link);
           strcat(sql,"','");
           strcat(sql,descriere);
           strcat(sql,"');");
           sqlite3_open("proiectrc.db",&baza_date);
           rc=sqlite3_exec(baza_date,sql,NULL,0,&errorMsg);
           if( rc != SQLITE_OK )
                 {
                    fprintf(stderr, "SQL error: %s\n", errorMsg);
                    sqlite3_free(errorMsg);
                 }
           pch=strtok(NULL,"/\n");
           sqlite3_close(baza_date);
       }    
     strcpy(msgrasp,"Melodie adaugata la top!");
     if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
    }
    else if(ok==1)
    {
      strcpy(msgrasp,"Melodia exista deja!");
      if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
    }
}

void votare(int client, char username[100])
{
  sqlite3* baza_date;
  char*errorMsg=0;
  char msgrasp[100], msg[100],melodie[100];
  bzero(msgrasp,100);
  bzero(msg,100);
  int rc,ok=0,drept_vot=0;
  char sql[100];
  strcpy(sql,"SELECT * FROM USERS WHERE NAME_USER='");
  strcat(sql,username);
  strcat(sql,"' AND VOT=1;");
  sqlite3_open("proiectrc.db",&baza_date);
  rc=sqlite3_exec(baza_date,sql,callbackcautare,&drept_vot,&errorMsg);
  sqlite3_close(baza_date);
    strcpy(msgrasp,"melodie:");
    if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
    if (read (client, msg, 100) <= 0)
      {
            close(client);
            exit(1);
        }
    strcpy(melodie,msg);
    melodie[strlen(melodie)-1]='\0';
    strcpy(sql,"SELECT * FROM MUZICA WHERE NUME_MELODIE='");
    strcat(sql,melodie);
    strcat(sql,"';");
    sqlite3_open("proiectrc.db",&baza_date);
    rc=sqlite3_exec(baza_date,sql,callbackcautare,&ok,&errorMsg);
    if( rc != SQLITE_OK ){
           fprintf(stderr, "SQL error: %s\n", errorMsg);
            sqlite3_free(errorMsg);
      }
    sqlite3_close(baza_date);
    if(drept_vot==0)
      {
         strcpy(msgrasp,"Nu aveti dreptul de a vota!");
         if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
      }
    else if(ok==0)
      { 
        strcpy(msgrasp,"Melodia nu este in top!");
        if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
      }
    else 
    {
      strcpy(sql,"UPDATE MUZICA SET VOTURI=VOTURI+1 WHERE NUME_MELODIE='");
      strcat(sql,melodie);
      strcat(sql,"';");
      sqlite3_open("proiectrc.db",&baza_date);
      rc=sqlite3_exec(baza_date,sql,NULL,0,&errorMsg);
      if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", errorMsg);
          sqlite3_free(errorMsg);
       }
    sqlite3_close(baza_date);
    strcpy(msgrasp,"Votare terminata!");
    if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
    }
}

void vizualizare_top(int client)
{
  sqlite3* baza_date;
  char*errorMsg=0;
  char msgrasp[1024];
  bzero(msgrasp,1024);
  int rc;
  char sql[100];
  char s[1024]="";
  strcpy(sql,"SELECT NUME_MELODIE,VOTURI,TIP_MUZICA FROM MUZICA ORDER BY VOTURI DESC;");
  sqlite3_open("proiectrc.db",&baza_date);
  rc=sqlite3_exec(baza_date,sql,callbackafisare,&s,&errorMsg);
  if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", errorMsg);
          sqlite3_free(errorMsg);
    }
  strcpy(msgrasp,s);
  if( write(client,msgrasp,1024) <= 0)perror ("[server]Eroare la write() catre client.\n");
  sqlite3_close(baza_date);
}

void vizualizare_top_gen(int client)
{
  sqlite3* baza_date;
  char*errorMsg=0;
  char msgrasp[1024];
  char msg[100],gen[100],sql[100];
  bzero(msgrasp,1024);
  bzero(msg,100);
  strcpy(msgrasp,"gen:");
  if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  if (read (client, msg, 100) <= 0)
     {
          close(client);
          exit(1);
     }
  strcpy(gen,msg);
  gen[strlen(gen)-1]='\0';
  strcpy(sql,"SELECT NUME_MELODIE,VOTURI FROM MUZICA WHERE TIP_MUZICA='");
  strcat(sql,gen);
  strcat(sql,"' ORDER BY VOTURI DESC;");
  int rc;
  char s[1024]="";
  sqlite3_open("proiectrc.db",&baza_date);
  rc=sqlite3_exec(baza_date,sql,callbackafisare,&s,&errorMsg);
  if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", errorMsg);
          sqlite3_free(errorMsg);
    }
  if(strlen(s)==0)strcpy(s,"Nu exista nicio melodie de genul ales!");
  strcpy(msgrasp,s);
  if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  sqlite3_close(baza_date);
}

void adaugare_comentariu(int client,char username[100])
{
   sqlite3* baza_date;
   char*errorMsg=0;
   char msgrasp[100], msg[100],melodie[100],comentariu[100];
   bzero(msgrasp,100);
   bzero(msg,100);
   strcpy(msgrasp,"melodie:");
   if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
   if (read (client, msg, 100) <= 0)
      {
          close(client);
          exit(1);
      }
   strcpy(melodie,msg);
   strcpy(msgrasp,"comentariu:");
   if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
   if (read (client, msg, 100) <= 0)
      {
          close(client);
          exit(1);
      }
   strcpy(comentariu,msg);
   melodie[strlen(melodie)-1]='\0';
   comentariu[strlen(comentariu)-1]='\0';
   int rc,ok=0;
   char sql[100];
   strcpy(sql,"SELECT * FROM MUZICA WHERE NUME_MELODIE='");
   strcat(sql,melodie);
   strcat(sql,"';");
   sqlite3_open("proiectrc.db",&baza_date);
   rc=sqlite3_exec(baza_date,sql,callbackcautare,&ok,&errorMsg);
   if( rc != SQLITE_OK )
     {
           fprintf(stderr, "SQL error: %s\n", errorMsg);
           sqlite3_free(errorMsg);
     }
   sqlite3_close(baza_date);
   if(ok==1)
   {
   sqlite3_open("proiectrc.db",&baza_date);
   strcpy(sql,"INSERT INTO COMENTARII VALUES('");
   strcat(sql,username);
   strcat(sql,"','");
   strcat(sql,melodie);
   strcat(sql,"','");
   strcat(sql,comentariu);
   strcat(sql,"');");
   rc=sqlite3_exec(baza_date,sql,NULL,0,&errorMsg);
   if( rc != SQLITE_OK )
     {
       fprintf(stderr, "SQL error: %s\n", errorMsg);
       sqlite3_free(errorMsg);
     }
   sqlite3_close(baza_date);
   strcpy(msgrasp,"Comentariu adaugat!");
   if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
   }
   else
   {
     strcpy(msgrasp,"Melodia nu este in top!");
     if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n"); 
   }
   
}

void vizualizare_comentarii(int client)
{
  sqlite3* baza_date;
  char*errorMsg=0;
  char msgrasp[1024];
  char msg[100],melodie[100],sql[100];
  bzero(msgrasp,1024);
  bzero(msg,100);
  strcpy(msgrasp,"melodie");
  if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  if (read (client, msg, 100) <= 0)
     {
          close(client);
          exit(1);
     }
  strcpy(melodie,msg);
  melodie[strlen(melodie)-1]='\0';
  strcpy(sql,"SELECT * FROM COMENTARII WHERE NUME_MELODIE='");
  strcat(sql,melodie);
  strcat(sql,"';");
  char s[1024]="";
  int rc;
  sqlite3_open("proiectrc.db",&baza_date);
  rc=sqlite3_exec(baza_date,sql,callbackafisare,&s,&errorMsg);
  if( rc != SQLITE_OK ){
          fprintf(stderr, "SQL error: %s\n", errorMsg);
          sqlite3_free(errorMsg);
    }
  if(strlen(s)==0)strcpy(s,"Melodia nu are comentarii!");
  strcpy(msgrasp,s);
  if( write(client,msgrasp,1024) <= 0)perror ("[server]Eroare la write() catre client.\n");
  sqlite3_close(baza_date);
}

void stergere_melodie(int client)
{
  sqlite3* baza_date;
  char*errorMsg=0;
  char msgrasp[100], msg[100],melodie[100];
  bzero(msgrasp,100);
  bzero(msg,100);
  strcpy(msgrasp,"melodie:");
  if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  if (read (client, msg, 100) <= 0)
     {
          close(client);
          exit(1);
     }
  strcpy(melodie,msg);
  melodie[strlen(melodie)-1]='\0';
  int rc,ok=0;
  char sql[100];
  strcpy(sql,"SELECT * FROM MUZICA WHERE NUME_MELODIE='");
  strcat(sql,melodie);
  strcat(sql,"';");
  sqlite3_open("proiectrc.db",&baza_date);
  rc=sqlite3_exec(baza_date,sql,callbackcautare,&ok,&errorMsg);
  if( rc != SQLITE_OK )
     {
           fprintf(stderr, "SQL error: %s\n", errorMsg);
           sqlite3_free(errorMsg);
     }
  sqlite3_close(baza_date);
  if(ok==1)
  {
  strcpy(msgrasp,"Melodie stearsa!");
  if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  sqlite3_open("proiectrc.db",&baza_date);
  strcpy(sql,"DELETE FROM MUZICA WHERE NUME_MELODIE='");
  strcat(sql,melodie);
  strcat(sql,"';");
  rc=sqlite3_exec(baza_date,sql,NULL,0,&errorMsg);
  if( rc != SQLITE_OK )
     {
       fprintf(stderr, "SQL error: %s\n", errorMsg);
       sqlite3_free(errorMsg);
     }
  strcpy(sql,"DELETE FROM COMENTARII WHERE NUME_MELODIE='");
  strcat(sql,melodie);
  strcat(sql,"';");
  rc=sqlite3_exec(baza_date,sql,NULL,0,&errorMsg);
  if( rc != SQLITE_OK )
     {
       fprintf(stderr, "SQL error: %s\n", errorMsg);
       sqlite3_free(errorMsg);
     }
  sqlite3_close(baza_date);
  }
  else 
  {
     strcpy(msgrasp,"Melodia nu se afla in top!");
     if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  }
}

void restrictionare_vot(int client)
{
  sqlite3* baza_date;
  char*errorMsg=0;
  char msgrasp[100], msg[100],username[100];
  bzero(msgrasp,100);
  bzero(msg,100);
  strcpy(msgrasp,"username:");
  if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  if (read (client, msg, 100) <= 0)
     {
          close(client);
          exit(1);
     }
  strcpy(username,msg);
  username[strlen(username)-1]='\0';
  int rc,ok=0;
  char sql[100];
  strcpy(sql,"SELECT * FROM USERS WHERE NAME_USER='");
  strcat(sql,username);
  strcat(sql,"';");
  sqlite3_open("proiectrc.db",&baza_date);
  rc=sqlite3_exec(baza_date,sql,callbackcautare,&ok,&errorMsg);
  if( rc != SQLITE_OK )
     {
           fprintf(stderr, "SQL error: %s\n", errorMsg);
           sqlite3_free(errorMsg);
     }
  sqlite3_close(baza_date);
  if(ok==1)
  {
  sqlite3_open("proiectrc.db",&baza_date);
  strcpy(sql,"UPDATE USERS SET VOT=0 WHERE NAME_USER='");
  strcat(sql,username);
  strcat(sql,"';");
  rc=sqlite3_exec(baza_date,sql,NULL,0,&errorMsg);
  if( rc != SQLITE_OK )
     {
       fprintf(stderr, "SQL error: %s\n", errorMsg);
       sqlite3_free(errorMsg);
     }
  sqlite3_close(baza_date);
  strcpy(msgrasp,"Utilizatorului i-a fost retras dreptul la vot!");
  if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  }
  else 
  {
    strcpy(msgrasp,"Utilizatorul nu exista!");
    if( write(client,msgrasp,100) <= 0)perror ("[server]Eroare la write() catre client.\n");
  }
}

extern int errno;

int main ()
{
  pid_t pid;
  struct sockaddr_in server;
  struct sockaddr_in from;	
  char msg[100];
  char msgrasp[100]=" ";
  int sd; // descriptor 
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
  bzero (&server, sizeof (server));
  bzero (&from, sizeof (from));
  server.sin_family = AF_INET;	
  server.sin_addr.s_addr = htonl (INADDR_ANY); //ataseaza socketul la adresa locala (adresa IP)
  server.sin_port = htons (PORT); 
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }
  if (listen (sd, 5) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }
  while (1)
    {
      int client;
      int length = sizeof (from);
      printf ("[server]Asteptam la portul %d...\n",PORT);
      fflush (stdout);
      client = accept (sd, (struct sockaddr *) &from, &length);
      if (client < 0)
        {
	  perror ("[server]Eroare la accept().\n");
	  continue;
	      }
      pid=fork();
      if(pid==0)//sunt in copil
        {
              printf ("[server]Asteptam mesajul...\n");
              fflush (stdout);
              bzero(msg,100);
              bzero(msgrasp,100);
              int ok=0;
              char username[100];
              if (read (client, msg, 100) <= 0)
	             {
                  close(client);
                  break;
	              }
              if(strlen(msg)==0)close(client);
              strcpy(msgrasp,"Am primit comanda");
              if( write(client,msgrasp,100) <= 0)
                     {
                        perror ("[server]Eroare la write() catre client.\n");
		                    continue;
                     }
              if(strcmp(msg,"login")==0)//daca clientul a ales sa se logeze
              while(ok==0){
                  if (read (client, msg, 100) <= 0)
	             {
	                close(client);
                  break;
	              }
                   if(strlen(msg)==0)break;
                   ok=login(msg,username); //verific daca clientul are cont
                   if(ok==0)strcpy(msgrasp,"Usernameul sau parola sunt gresite! Introduceti-le din nou!");
                   else if(ok==1)strcpy(msgrasp,"1");
                   else strcpy(msgrasp,"2");
                   if( write(client,msgrasp,100) <= 0)
                     {
                        perror ("[server]Eroare la write() catre client.\n");
		                    continue;
                     }
               }
               if(strcmp(msg,"inregistrare")==0) //daca clientul a ales sa se inregistreze
               {
                  ok=0;
                  if (read (client, msg, 100) <= 0)
	             {
	                close(client);
                  break;
	              }  
                   if(strlen(msg)==0)break;
                   ok=login(msg,username); //verific daca clientul are deja cont
                   if(ok==1)strcpy(msgrasp,"1");
                   else if(ok==2)strcpy(msgrasp,"2");
                   else
                   { 
                   inregistrare(msg,username); //inregistrez clientul
                   strcpy(msgrasp,"0");
                   }
                   if( write(client,msgrasp,100) <= 0)
                     {
                        perror ("[server]Eroare la write() catre client.\n");
		                    continue;
                     }
                }
               while(1) //primire comenzi
                {
                    if (read (client, msg, 100) <= 0)
	                     {
	                        close(client);
                          break;
	                     }
                   if(strlen(msg)==0)break;
                   else if(strcmp(msg,"adaugare melodie\n")==0)adaugare_melodie(client);
                   else if(strcmp(msg,"votare\n")==0)votare(client,username);
                   else if(strcmp(msg,"vizualizare top\n")==0)vizualizare_top(client);
                   else if(strcmp(msg,"vizualizare top gen\n")==0)vizualizare_top_gen(client);
                   else if(strcmp(msg,"adaugare comentariu\n")==0)adaugare_comentariu(client,username);
                   else if(strcmp(msg,"vizualizare comentarii\n")==0)vizualizare_comentarii(client);
                   else if(ok==2 && strcmp(msg,"stergere melodie\n")==0)stergere_melodie(client);
                   else if(ok==2 && strcmp(msg,"restrictionare vot\n")==0)restrictionare_vot(client); 
                   else {
                   strcpy(msgrasp,"Comanda invalida!\n");
                   if( write(client,msgrasp,100) <= 0)
                     {
                        perror ("[server]Eroare la write() catre client.\n");
		                    continue;
                     }
                   }//else
                }//while(1)
            close(sd);
            exit(0);
        } //inchid copilul
      else {close(client);}
    }				/* while */
}				/* main */
