 // SEND REPLYS FUNCTION  
 void send_reply(struct message *msg)  
 {  
   struct sockaddr_in their_addr; // Connector's address information  
   struct hostent *h;  
   int sockfds;  
      int pid;  
      struct message m;  
      m = *((struct message *)msg);  
      pid =m.id;  
      pthread_mutex_lock(&types);  
           strcpy(m.type,"REPLY");  
      pthread_mutex_unlock(&types);  
      m.id = me;  
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
         exit(1);  
       }  
      cliconn(stdin, sockfds,my.mac,my.portno,my.id,&m,0);  
      printf("Sending REPLY message from site %d to site %d\n",me,pid);  
      close(sockfds);     //Socket closed after sending the message to the process  
 } 