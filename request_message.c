 // THE REQUEST QUEUE  
 //REQUEST QUEUE INSERT  
 void rinsert(struct message temp)  
 {  
      printf("Inside the request queue insert..\n");  
      if(rfront==rrear)  
   {  
           rfront = 0;  
           rrear = 0;  
   }  
      printf("Inserting values in request queue..\n");  
      REQ_QUEUE[rrear].id = temp.id;  
   strcpy(REQ_QUEUE[rrear].type,temp.type);  
   REQ_QUEUE[rrear].seq_no = temp.seq_no;  
   REQ_QUEUE[rrear].clock = temp.clock;  
      rrear++;  
 }  


  //REQUEST QUEUE DISPLAY  
 void rdisplay()  
 {  
   int i;  
   if(rfront==-1)  
   printf("CAUTION: Request Queue is Empty..!!\n");  
   for(i=rfront;i<rrear;i++)  
   printf("%d %s %d %d\n",REQ_QUEUE[i].id,REQ_QUEUE[i].type,REQ_QUEUE[i].seq_no,REQ_QUEUE[i].clock);  
 }


   
 //REQUEST QUEUE DELETE  
 struct message rdelete()  
 {  
   struct message tempvar;  
   printf("Inside request queue delete.. \n");  
   rdisplay();  
   if(rfront==-1)  
   {  
        printf("CAUTION: Request Queue Underflow !!\n");  
           exit(1);  
   }  
   else if(rfront==rrear-1)  
   {  
   tempvar.id = REQ_QUEUE[rfront].id;  
   strcpy(tempvar.type,REQ_QUEUE[rfront].type);  
   tempvar.seq_no = REQ_QUEUE[rfront].seq_no;  
   tempvar.clock = REQ_QUEUE[rfront].clock;  
      rfront = -1;  
      rrear = -1;  
      }  
   else  
   {  
   tempvar.id = REQ_QUEUE[rfront].id;  
   strcpy(tempvar.type,REQ_QUEUE[rfront].type);  
   tempvar.seq_no = REQ_QUEUE[rrear].seq_no;  
   tempvar.clock = REQ_QUEUE[rfront].clock;  
      rfront++;  
   }  
   return tempvar;  
 }  