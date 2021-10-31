
 // THE DEFER QUEUE   
 //DEFER QUEUE INSERT  
 void dinsert(struct message temp)  
 {  
      printf("Inside the defer queue insert..\n");  
      if(dfront==drear)  
   {  
           dfront = 0;  
           drear = 0;  
      }  
   DEFER_QUEUE[drear].id = temp.id;  
   strcpy(REQ_QUEUE[drear].type,temp.type);  
   DEFER_QUEUE[drear].seq_no = temp.seq_no;  
   DEFER_QUEUE[drear].clock = temp.clock;  
      drear++;  
 }

  //DEFER QUEUE DISPLAY  
 void ddisplay()  
 {  
   int i;  
   if(dfront==-1)  
   printf("Defer Queue is Empty..!!\n");  
   for(i=dfront;i<drear;i++)  
   printf("%d %s %d %d\n",DEFER_QUEUE[i].id,DEFER_QUEUE[i].type,DEFER_QUEUE[i].seq_no,DEFER_QUEUE[i].clock);  
 } 


  //DEFER QUEUE DELETE  
 struct message ddelete()  
 {  
   struct message tempvar;  
   printf("Inside the defer queue delete..\n");  
   ddisplay();  
   if(dfront==-1)  
   {  
           printf("CAUTION: Defer queue Underflow !!\n");  
           exit(1);  
   }  
   else if(dfront==drear-1)  
   {  
   tempvar.id = DEFER_QUEUE[dfront].id;  
   strcpy(tempvar.type,DEFER_QUEUE[dfront].type);  
   tempvar.seq_no = DEFER_QUEUE[drear].seq_no;  
   tempvar.clock = DEFER_QUEUE[dfront].clock;  
      dfront = -1;  
      drear = -1;  
   }  
   else  
   {  
   tempvar.id = DEFER_QUEUE[dfront].id;  
   strcpy(tempvar.type, DEFER_QUEUE[dfront].type);  
   tempvar.seq_no = DEFER_QUEUE[drear].seq_no;  
   tempvar.clock = DEFER_QUEUE[dfront].clock;  
      dfront++;  
   }  
   return tempvar;  
 }  