  //     RECIEVE REQUESTS THREAD  
 void * recv_request_thread(void *msg)  
 {  
        struct sockaddr_in their_addr; // Connector's address information  
        struct hostent *h;  
        int sockfds;  
        int pid;  
      int j;  
      int check, procid;  
      struct message m;  
      struct message tm;  
      m = *((struct message *)msg);  
      for(j=0;j<noproc; j++)  
      {  
           count[j];  
      }  
        if (argc1 != 4)      //The command line should have the output file,machine name and  
                                    //my port address as the runtime parameters  
        {                          //Error check the command line  
           fprintf(stderr,"usage: getip address\n");  
           exit(1);  
   }  
      int liveServers = 1;     //Initialising number of live processes counting for itself  
      while(liveServers <= noproc)  
                     //Checks for number of processes that are alive before sending  
                     //the messages. It is similar to the initialization message sent  
                     //to all the proceses  
      {  
           liveServers = 1;  
           int j;  
           for(j=1;j<=noproc;j++)  
           {  
                if ((sockfds = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
                     //Opens a connection to check for the live processes  
                  {  
                         perror("socket");  
                         exit(1);  
                  }  
                  if ((h=gethostbyname(hs[j].name)) == NULL)  
                  {  
                     perror("gethostbyname");  
                     exit(1);  
                  }  
                 their_addr.sin_family = AF_INET;  
                 their_addr.sin_port = htons(hs[j].port);  
                  their_addr.sin_addr = *((struct in_addr *)h->h_addr);  
                    memset(&(their_addr.sin_zero), '\0', 8);  
                 if (connect(sockfds, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)                //Connects to the process  
                {  
                }  
                else  
                {  
                     liveServers++;  
                     //if connection is setup increments liveserver count by one  
                     //everytime it extablishes a connection with a process  
                }  
                close(sockfds); //Connection closed after checking is done  
           }  
      }  
      serverFlag = 1;           //When all processes are alive sets serverFlag to 1.  
      printf("\nALL SERVERS ARE READY!!! \n");     //Processes are ready to listen now.  
 /*REQUESTING ENTRY TO THE CRITICAL SECTION*/  
      while(1)  
      {  
           if(pfront==-1)  
           {  
                check = 0;  
                break;  
           }  
           else  
                check = 1;  
      if(check)  
      {  
      pthread_mutex_lock(&processthd);  
           procid = pdelete();  
           printf("SITE %d REQUESTING FOR CS..\n",me);  
           pthread_mutex_lock(&reqCS);  
                req_CS = 1;  
                req = req_CS;  
           pthread_mutex_unlock(&reqCS);  
      //preparing the structure for sending  
           m.id = me;  
      pthread_mutex_lock(&types);  
           strcpy(m.type,"REQUEST");  
      pthread_mutex_unlock(&types);  
      pthread_mutex_lock(&sequence);  
           our_seq_number = highest_sequence_number+1;  
           m.seq_no = our_seq_number;  
      pthread_mutex_unlock(&sequence);  
      for(i=1; i<=noproc; i++)  
      {  
                if(i == me)               //Checking request not sending to myself  
                {  
                     continue;  
                }  
                if ((h=gethostbyname(hs[i].name)) == NULL)  
             {  
                     perror("gethostbyname");  
                     exit(1);  
             }  
                if ((sockfds = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
                               //Opens socket to send messages  
              {  
                    perror("socket");  
                    exit(1);  
              }  
                their_addr.sin_family = AF_INET;         // Host byte order  
               their_addr.sin_port = htons(hs[i].port); // Short,networbyteorder  
                 their_addr.sin_addr = *((struct in_addr *)h->h_addr);  
                   memset(&(their_addr.sin_zero), '\0', 8); // Zero the rest of the struct  
                  sleep(1);  
                 if (connect(sockfds, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)  
                   {  
                     perror("connect in send thread\n");  
                    exit(1);  
                  }  
                printf("Clock value is updated to %d\n",clockvalue);  
                clockvalue=clockvalue+1;  
                m.clock=clockvalue;  
                requesttime[i]=m.clock;  
                counter = counter + 1;  
                requesttime[i]=m.clock;  
                printf("Site %d sending REQUEST for process %d to site %d with timestamp %d\n",me,procid,i,m.clock);  
                cliconn(stdin, sockfds,my.mac,my.portno,my.id,&m,counter);  
                close(sockfds);  
      }  
      printf("Waiting for reply from other sites...");  
      while(1)  
      {  
           if (replycount == outstanding_reply_count)  
           {  
                pthread_mutex_lock(&replycnt);  
                replycount=0;  
                pthread_mutex_unlock(&replycnt);  
                break;  
           }  
           else  
                sleep(2);  
      }  
 /*ENTERING THE CRITICAL SECTION*/  
      //Is entering inside the CS  
      pthread_mutex_lock(&inCS);  
           in_CS = 1;  
           in = in_CS;  
      pthread_mutex_unlock(&inCS);  
      //Is not requesting for CS again  
      pthread_mutex_lock(&reqCS);  
           req_CS = 0;  
           req = req_CS;  
      pthread_mutex_unlock(&reqCS);  
      sem_post(&proc[procid]);  
      sem_wait(&site);  
      //Entering CS  
      pthread_mutex_lock(&inCS);  
           in_CS = 0;  
           in = in_CS;  
      pthread_mutex_unlock(&inCS);  
 /*RELEASING THE CRITICAL SECTION*/  
      sendcount = 0;  
      // Pop from the defer queue  
           while(drear!=-1)  
      {  
        pthread_mutex_lock(&types);  
                strcpy(m.type,"REPLY"); //copy my node id and the message type  
           pthread_mutex_unlock(&types);  
           m.id = me;  
           pthread_mutex_lock(&deferq);  
                tm = ddelete(); //tm is the buffer in which the values are stored in message  
                pid = tm.id;  
           pthread_mutex_unlock(&deferq);  
           sendcount++;  
           printf("Send Reply Message count: %d\n",sendcount);  
           if ((h=gethostbyname(hs[pid].name)) == NULL)  
        {  
                perror("gethostbyname");  
                exit(1);  
        }  
           if ((sockfds = socket(AF_INET, SOCK_STREAM, 0)) == -1)  
                               //Opens socket to send messages  
         {  
                    perror("socket");  
                     exit(1);  
         }  
            their_addr.sin_family = AF_INET;         // Host byte order  
           their_addr.sin_port = htons(hs[pid].port); // Short,networbyteorder  
            their_addr.sin_addr = *((struct in_addr *)h->h_addr);  
               memset(&(their_addr.sin_zero), '\0', 8); // Zero the rest of the struct  
      sleep(1);  
            if (connect(sockfds, (struct sockaddr *)&their_addr, sizeof(struct sockaddr)) == -1)  
              {  
                     perror("connect in send thread\n");  
                      //exit(1);  
             }  
           cliconn(stdin, sockfds,my.mac,my.portno,my.id,&m,0);  
           close(sockfds);  
      }  
           pthread_mutex_unlock(&processthd);  
      }  
 }  
 }  
 //      RECIEVE REPLYS THREAD  
 void * recv_reply_thread(void *msg)  
 {  
      int sockfdr, new_fd;                  // Listen on sock_fd, new connection on new_fd  
   struct sockaddr_in my_addr;             // My address information  
        struct sockaddr_in their_addr;           // Connector's address information  
        int sin_size;  
      int yes=1;  
      FILE *file;  
      struct message m;  
      m = *((struct message *)msg);  
   if ((sockfdr = socket(AF_INET, SOCK_STREAM, 0)) == -1) //Opening socket connection  
   {  
     perror("socket");                // Checking for any in case if connection failed  
     exit(1);  
   }  
   if (setsockopt(sockfdr, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)  
   {  
     perror("setsockopt");  
     exit(1);  
   }  
   my_addr.sin_family = AF_INET;         // Host byte order  
   my_addr.sin_port = htons(listenPort);        // Short, network byteorder  
   my_addr.sin_addr.s_addr = (INADDR_ANY);      // Automatically fill with myIP  
   memset(&(my_addr.sin_zero), '\0', 8);      // Zero the rest of the struct  
   if (bind(sockfdr, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)  
   {                              // Bind to my address  
     perror("bind");                    // Check for errors  
     exit(1);  
   }  
   if (listen(sockfdr, BACKLOG) == -1)      // Listening from the other processes  
   {  
     perror("listen");               // Checking for errors  
     exit(1);  
   }  
      for(;;)  
      {  
           int numbytes;  
           char buf[MAXDATASIZE];  
           sin_size = sizeof(struct sockaddr_in);  
           if ((new_fd = accept(sockfdr, (struct sockaddr*)&their_addr, (socklen_t*)&sin_size)) == -1)  
           {  
          perror("In server accept");  
          continue;  
           }  
           else  
           {  
                saveconn(new_fd,my.id,counter);  
                close(new_fd);  
           }  
      }  
 } 