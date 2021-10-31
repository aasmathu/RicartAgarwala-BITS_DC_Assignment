 //RICART AGRAWALA ALGORITHM FOR MUTUAL EXCLUSION  
 #include <stdio.h>  
 #include <stdlib.h>  
 #include <pthread.h>  
 #include <unistd.h>  
 #include <errno.h>  
 #include <string.h>  
 #include <sys/types.h>  
 #include <sys/times.h>  
 #include <sys/socket.h>  
 #include <netinet/in.h>  
 #include <arpa/inet.h>  
 #include <sys/wait.h>  
 #include <semaphore.h>  
 #include <sys/shm.h>  
 #include <sys/ipc.h>  
 #include <signal.h>  
 #include <netdb.h>  
 #include <math.h>  
 #include <time.h>  
 #define BACKLOG 150      //Number of pending connections queue will hold  
 #define MAXDATASIZE 100 //Maximum number of bytes we can get at once  
 #define MAXLINE 750  
 #define TRUE 1  
 #define FALSE 0  
 #define noproc 4     //Total number of sites in the system  
 pthread_t tid1,tid2,tid3;  
 pthread_t proc1[5];  
 int argc1;  
 char argv1[50];  
 char argv[50];  
 int i;  
 int listenPort;     //The process port on which it is recieving the messages  
 int count[25];  
 int serverFlag = 0; //flag to check if all servers/sites are ready  
 int requesttime[5];     //times at which the request message is sent  
 struct host          //Structure to maintain the Id, Server name and Port number  
 {  
      int id;  
      char name[50];  
      int port;  
 };  
 struct host hs[20];  
 typedef struct myinfo1 //Structure to maintain my information  
 {  
      int id;  
      int portno;  
      char mac[50]; //machine or host name eg. net06  
 } myinfo;  
 myinfo my;  
 struct message     //Structure that comtains the message exchanged  
 {  
      int id;               //site ID  
      int procid;          //Process ID  
      char type[10];     //Type of message sent  
      int seq_no;          //sequence number of the process  
      int clock;          //clock at which the message is sent  
 };  
 static int rfront=-1,rrear=-1; //The pointers for REQ_QUEUE  
 static int dfront=-1,drear=-1; //The pointers for the DEFER_QUEUe  
 static int pfront=-1,prear=-1; //The pointers for the PROCESS_QUEUE  
 struct message REQ_QUEUE[200];  //The REQUEST QUEUE  
 struct message DEFER_QUEUE[200]; //The DEFER QUEUE  
 int PROCESS_QUEUE[200];                    //The PROCESS QUEUE  
 sem_t proc[5];  
 sem_t site;  
 //Mutex varialbes used to lock variuos globally shared variables  
 pthread_mutex_t sequence;  
 pthread_mutex_t inCS;  
 pthread_mutex_t reqCS;  
 pthread_mutex_t ccounter;  
 pthread_mutex_t replycnt;  
 pthread_mutex_t signals;  
 pthread_mutex_t     types;  
 pthread_mutex_t     clk;  
 pthread_mutex_t     sending_mutex;  
 pthread_mutex_t sema;  
 pthread_mutex_t pqueue;  
 pthread_mutex_t processthd;  
 pthread_mutex_t counts;  
 pthread_mutex_t requestq;  
 pthread_mutex_t deferq;  
 pthread_mutex_t refront;  
 pthread_mutex_t rerear;  
 pthread_mutex_t defront;  
 pthread_mutex_t derear;  
 //The threads used in this program  
 void * recv_reply_thread ( void *);  
 void * recv_request_thread ( void *);  
 void * process_thread (void *);  
 void * processes (void *);  
 void send_reply(struct message *msg);     //Function to send reply messages  
 void rinsert(struct message);     //Request queue functions  
 void rdisplay(void);  
 struct message rdelete(void);  
 void dinsert(struct message);     //Defer queue functions  
 void ddisplay(void);  
 struct message ddelete(void);  
 void pinsert(int);                    //Process queue functions  
 void pdisplay(void);  
 int pdelete();  
 void sigchld_handler(int s)     // reap all dead processes  
 {  
   while(wait(NULL) > 0);  
 }  
 int me;                         //my id number  
 int our_seq_number=0;     // My sequence number  
 int outstanding_reply_count = noproc-1;     //outstanding reply count..Initially N-1  
 int counter=0;          // counter for clock  
 int clockvalue=1;  
 int highest_sequence_number=0;  
 int counting=0;  
 int req_CS=0; // Request for the Critical section: initially FALSE  
 int in_CS=0; //Inside the Critical Section: initially FALSE  
 int SIGNAL;  
 int in;               //to read if in CS  
 int req;          // to read if req CS  
 int seqno;          // to read seq no  
 int sendcount;  
 int recvcount;  
 int replycount; //to read current outstanding_reply_count