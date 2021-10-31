#include "header.h"
#include "defer_queue.c"
#include "receive_message.c"
#include "reply_message.c"
#include "request_message.c" 
 
 // SAVE CONNECTION - RECV  
 void saveconn(int sockfdr, int id, int counter)  
 {  
      int n;  
      FILE *file;  
   char line[MAXLINE];  
   struct message * msg, m;  
      pthread_mutex_lock(&sending_mutex);  
   msg = (struct message *)malloc(sizeof(struct message));  
      n = recv(sockfdr,(void *)msg,sizeof(struct message),0);  
      pthread_mutex_unlock(&sending_mutex);  
      m = *((struct message *)msg);  
   if(n == 0)  
           return;  
   else if(n < 0)  
        printf("saveconn(): read error\n");  
   else  
             printf("Site %d receiving %s from site %d with clock %d. \n",me,m.type,m.id,m.clock);  
           if(highest_sequence_number < m.seq_no)  
           {  
                highest_sequence_number = m.seq_no;  
           }  
           else  
           highest_sequence_number = highest_sequence_number;  
        pthread_mutex_lock(&clk);  
             clockvalue++;  
                if(clockvalue < (m.clock+1))  
                {  
                     clockvalue = (m.clock+1);  
                }  
           pthread_mutex_unlock(&clk);  
        if(strcmp(m.type,"REQUEST") == 0)  
      {  
           printf("Recieving REQUEST message from site %d\n",m.id);  
           pthread_mutex_lock(&requestq);  
                     rinsert(m);  
                    rdisplay();  
        pthread_mutex_unlock(&requestq);  
           SIGNAL=1; //Process wakeup  
      }  
      else if(strcmp(m.type,"REPLY") == 0)  
      {  
           printf("Recieving REPLY message from site %d\n",m.id);  
           pthread_mutex_lock(&replycnt);  
                replycount++;  
           printf("CURRENT REPLYCOUNT : %d\n",replycount);  
           pthread_mutex_unlock(&replycnt);  
      }  
      else  
      {  
           printf("Improper message : message not received properly\n");  
           rdisplay();  
      }  
 }  
 // CLIENT CONNECTION - SEND  
 void cliconn(FILE *file,int sockfds, char *mac, int portno, int id,struct message *messg,int counter)  
 {  
      int n,i;  
      char sendline[400],recvline[MAXLINE + 1];  
      portno = my.portno;  
      pthread_mutex_lock(&clk);  
           messg->clock = clockvalue;  
      pthread_mutex_unlock(&clk);  
      if(send(sockfds,messg,sizeof(struct message),0) != sizeof(struct message))  
           printf("cliconn(): write error on socket\n");  
      printf("Site %d has sent the request message..\n",me);  
 }  
 // MAIN FUNCTION  
 int main(int argc, char **argv)  
 {  
      struct message *msg;  
      int s;  
      pthread_mutex_init(&sequence,NULL);  
      pthread_mutex_init(&inCS,NULL);  
      pthread_mutex_init(&reqCS,NULL);  
      pthread_mutex_init(&ccounter,NULL);  
      pthread_mutex_init(&replycnt,NULL);  
      pthread_mutex_init(&signals,NULL);  
      pthread_mutex_init(&types,NULL);  
      pthread_mutex_init(&clk,NULL);  
      pthread_mutex_init(&sending_mutex,NULL);  
      pthread_mutex_init(&sema,NULL);  
      pthread_mutex_init(&processthd,NULL);  
      pthread_mutex_init(&pqueue,NULL);  
      pthread_mutex_init(&counts,NULL);  
      pthread_mutex_init(&requestq,NULL);  
      pthread_mutex_init(&deferq,NULL);  
      pthread_mutex_init(&refront,NULL);  
      pthread_mutex_init(&rerear,NULL);  
      pthread_mutex_init(&defront,NULL);  
      pthread_mutex_init(&derear,NULL);  
      FILE *file;  
      file = fopen("config.txt", "r"); //Open the configuration file  
        if(file==NULL)  
      {  
           printf("Error: can't open file.\n");  
          return 1;  
      }  
      else  
           printf("File opened successfully.\n");  
      for(i=1;i<=noproc;i++)  
      {  
           fscanf(file,"%d",&hs[i].id);//Reading host info from config file  
           fscanf(file,"%s",hs[i].name);  
           fscanf(file,"%d",&hs[i].port);  
      }  
      argc1 = argc;  
      printf("%d %d",argc1,argc);  
      my.id = atoi(argv[1]);  
      me = my.id;  
      strcpy(my.mac,argv[2]);  
      char t[9];  
      strcpy(t,argv[3]);  
      my.portno = atoi(t);  
      listenPort = atoi(t);  
      printf("My ID is : %s My Port : %s and My IP %s\n",argv[1],argv[3],argv[2]);  
      printf("Configuration File\n"); //Printing the configuration file details  
      for(i=1;i<=noproc;i++)  
      {  
           printf("%d %s %d\n",hs[i].id,hs[i].name,hs[i].port);  
      }  
      fclose(file);  
      for(s=0;s<5;s++)  
      {  
           sem_init( &proc[s],0,0);  
      }  
      for(s=0;s<5;s++)  
      {  
           pthread_create( &proc1[s], NULL, &processes, (void *)s);     //Creating processes in site  
      }  
      pthread_create( &tid3, NULL, &recv_request_thread, &msg);     //Creating send thread  
      pthread_create( &tid2, NULL, &recv_reply_thread, &msg);     //Creating recieve thread  
      pthread_create( &tid1, NULL, &process_thread, &msg);     //Creating process thread  
      pthread_join( tid1, NULL );                     //Join all process threads  
      pthread_join( tid2, NULL );                     //Join all recieve reply threads  
      pthread_join( tid3, NULL );                         //Join all recieve request threads  
      for(s=0;s<5;s++)  
      {  
           pthread_join( proc1[s], NULL);               //Join all processes in the site  
      }  
 }  

 
 //     THE SITE CONTROLLER THREAD  
 void * processes(void *msg)  
 {  
      int pid,mycount;  
      pid = (int)msg;  
      for(mycount=1; mycount<=20 ; mycount++)  
      {  
                pthread_mutex_lock(&pqueue);  
                     pinsert(pid);  
            counting++;  
                 pdisplay();  
          pthread_mutex_unlock(&pqueue);  
                sem_wait(&proc[pid]);  
                //entering crictical section  
                printf("Starting CS execution at time : %ld\n",time(NULL));  
                printf("*********SITE %d PROCESS %d ENTERING THE CS*********\n",me,pid);  
          printf("*********INSIDE THE CS*********\n");  
                      sleep(1);  
          printf("*********SITE %d PROCESS %d EXITING THE CS*********\n",me,pid);  
          printf("Exiting CS at time : %ld\n",time(NULL));  
                sem_post(&site);  
                printf("\nProcess %d is in CS for %d times\n",pid,mycount);  
      }  
      printf("*** Total Message count: %d ***\n",counting);  
 }  
 //     THE PROCESSING THREAD  
 void * process_thread(void *msg)  
 {  
      int nodeseq;  
      int pid; //use it for ripping the process to b sent to  
      struct message m;  
      m = *((struct message *)msg);  
      while(1)  
      {  
      printf("Process thread starting to process requests..\n ");  
      sleep(3);  
      while(SIGNAL == 1)  
      {  
      pthread_mutex_lock(&refront);  
      while(1)  
       {  
                //pop data from the request queue  
           if(rfront!=-1)  
           {  
                rdisplay();  
                pthread_mutex_lock(&inCS);  
                     in = in_CS;  
                pthread_mutex_unlock(&inCS);  
                pthread_mutex_lock(&reqCS);  
                     req = req_CS;  
                pthread_mutex_unlock(&reqCS);  
             pid=REQ_QUEUE[rfront].id;  
                nodeseq = REQ_QUEUE[rfront].seq_no;  
                pthread_mutex_lock(&requestq);  
                     m = rdelete();  
                pthread_mutex_unlock(&requestq);  
                if (in == 1)  
                {  
                     printf("PROCESS ALREADY IN CS..So putting in defer queue.!!!\n");  
                     pthread_mutex_lock(&deferq);  
                          dinsert(m);  
                          pthread_mutex_unlock(&deferq);  
                }  
             else if( in == 0)  
             {  
                if (req == 1)  
                {  
                     printf("SITE %d REQUESTING FOR CS AS WELL..\nSo tie break..between %d and %d..!!\n",pid,me,pid);  
                     pthread_mutex_lock(&sequence);  
                          seqno = our_seq_number;  
                     pthread_mutex_unlock(&sequence);  
                     printf("Their seqno: %d My seqno: %d Their ID: %d My ID: %d\n",nodeseq,seqno,pid,me);  
                     if ((nodeseq < seqno) || (nodeseq == seqno && pid < me))  
                     {  
                          m.id = pid;  
                          m.seq_no = 0;  
                          strcpy(m.type, "REPLY");  
                          send_reply(&m); //send reply to that node with my structure (node id and type)  
                     }  
                     else  
                     {  
                          printf("@@@@@ I WIN @@@@@ \nNode %d get the priority...So put %d in the defer queue..!\n",me,pid);  
                          pthread_mutex_lock(&deferq);  
                               dinsert(m);  
                          pthread_mutex_unlock(&deferq);  
                     }  
                 }  
                  else  
                {  
                     m.id = pid;  
                     m.seq_no = 0;  
                     strcpy(m.type, "REPLY");  
                     send_reply(&m); //send reply to that node with my structure (node id and type)  
                }  
               }  
                else  
           {  
                sleep(5);  
           }  
           }  
       }  
       }  
      pthread_mutex_unlock(&refront);  
      pthread_mutex_lock(&signals);  
           SIGNAL = 0;  
      pthread_mutex_unlock(&signals);  
      }  
 }  
 

  
// THE PROCESS QUEUE   
 //PROCESS QUEUE INSERT  
 void pinsert(int temp)  
 {  
      printf("Inside the process queue insert..\n");  
      if(pfront==prear)  
      {  
           pfront = 0;  
           prear = 0;  
      }  
      PROCESS_QUEUE[prear] = temp;  
      prear++;  
 }  
 //PROCESS QUEUE DISPLAY  
 void pdisplay()  
 {  
      int i;  
      if(pfront==-1)  
      printf("Process Queue is Empty\n");  
      for(i=pfront;i<prear;i++)  
      printf("%d \n",PROCESS_QUEUE[i]);  
 }  
 //PROCESS QUEUE DELETE  
 int pdelete()  
 {  
      int tempvar;  
      printf("Inside process queue delete..\n");  
            pdisplay();  
      if(pfront==-1)  
      {  
        printf("CAUTION: Process Queue Underflow !!\n");  
        exit(1);  
      }  
      else if(pfront==prear-1)  
      {  
      tempvar = PROCESS_QUEUE[pfront];  
           pfront = -1;  
           prear = -1;  
      }  
      else  
      {  
      tempvar = PROCESS_QUEUE[pfront];  
      pfront++;  
      }  
      return tempvar;  
 }  