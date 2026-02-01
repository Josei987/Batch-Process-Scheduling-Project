#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
   int id;
      int arrival;
      int total_cpu;
      int total_remaining;
      int done;
      int start_time;
      int already_started;
      int end_time;
      int turnaround_time;
} Process;

Process *table = NULL;
int total_processes = 0;

//max of 2 int
int  max_int(int a, int b) {
   if(a>b){
      return a;
   }else{
      return b;
   }//end if than else
}
   
/*************************************/
void print_table(){
   int i;
   
   //prints upper portion of table
   printf("\nID     Arrival Total   Start   End     Turnaround\n");
   printf("---------------------------------------------------\n");
   
   for (i = 0; i < total_processes; i++) {
      if (table[i].done) {
         //print full table included calc fields
         printf("%-7d%-8d%-8d%-8d%-8d%-8d\n" ,
            table[i].id,
            table[i].arrival,
            table[i].total_cpu,
            table[i].start_time,
            table[i].end_time,
            table[i].turnaround_time); //end print
      } else{
         //print only arrival, id, total
         printf("%-7d%-8d%-8d\n", 
            table[i].id,
            table[i].arrival,
            table[i].total_cpu);//end print
      }//end if than else for printing table
   }//end for
}//end void print_table()

/******************************************************/
void enter_parameters() {
   int i;
   if (table != NULL) {
      //if there alr a table, free it
      free(table);
      table = NULL;
      total_processes = 0;
   }//end if
   
   printf("Enter total number of processes: ");
   if (scanf("%d", &total_processes) != 1){
      //invalid input
      while(getchar() != '\n')
      total_processes = 0;
      return;
   }//end if
   
   table = (Process *) malloc(sizeof(Process) * total_processes);
   if (table == NULL) {
      fprintf(stderr, "Memory allocation failed\n");
      exit(EXIT_FAILURE);
   }//enf if
   
   for (i = 0; i <total_processes; i++) {
      //prints prompt for user to enter id, arrival, cycles
      printf("Enter process id: ");
      scanf("%d", &table[i].id);
      printf("Enter arrival cycle for process P[%d]: ", table[i].id);
      scanf("%d", &table[i].arrival);
      printf("Enter total cycles for process P[%d]: ", table[i].id);
      scanf("%d", &table[i].total_cpu);
      
      //init scheduling fields
      table[i].total_remaining = table[i].total_cpu;
      table[i].done = 0;
      table[i].start_time = -1;
      table[i].already_started = 0;
      table[i].end_time = -1;
      table[i].turnaround_time = -1;
   }//end for
   
   //print table
   print_table();
   return;
}//end enter_parameters()

/*****************************************/
void fifo() {
   int i = 0;
   int scheduled = 0;
   int current_time = 0;
   
   //checks for errors
   if (table == NULL || total_processes == 0){
      return;
   }//end if
   
   for (i = 0; i < total_processes; i++) {
      table[i].done = 0;
      table[i].total_remaining = table[i].total_cpu;
      table[i].already_started = 0;
      table[i].start_time = -1;
      table[i].end_time = -1;
      table[i].turnaround_time = -1;
   }//end for
   
   while (scheduled < total_processes) {
      int idx = -1;
      int earliest = INT_MAX;
      for (i = 0; i < total_processes; i++){
         if(!table[i].done) {
            if (table[i].arrival < earliest) {
               earliest = table[i].arrival;
               idx = i;
            }//end if
         }//end if
      }//end for
      
      if (idx == -1) break;
      
      //if cpu idle untile process arrives
      current_time = max_int(current_time, table[idx].arrival);
      table[idx].start_time = current_time;
      table[idx].end_time = table[idx].start_time + table[idx].total_cpu;
      table[idx].turnaround_time = table[idx].end_time - table[idx].arrival;
      table[idx].done = 1;
      table[idx].total_remaining = 0;
      current_time = table[idx].end_time;
      scheduled++;
   }//end while
   
   print_table();
   return;
}//end fifo

/***************************************************************************/
void sjf() {
   int i;
   int scheduled = 0;
   int current_time = 0;
   
   if (table == NULL || total_processes == 0) {
      return;
   }//end if
   
   //reset flags
   for (i = 0; i < total_processes; i++) {
      table[i].done = 0;
      table[i].total_remaining = table[i].total_cpu;
      table[i].already_started = 0;
      table[i].start_time = -1;
      table[i].end_time = -1;
      table[i].turnaround_time = -1;
   }//end for
   
   while (scheduled < total_processes) {
      int idx = -1;
      int  smallest = INT_MAX;
      for (i = 0; i < total_processes; i++) {
         if (!table[i].done && table[i].arrival <= current_time) {
            if (table[i].total_cpu < smallest) {
               smallest = table[i].total_cpu;
               idx = i;
            }//end inner if
         }//end if
      }//end for
      
      if (idx == -1) {
         int earliest = INT_MAX;
         for (i = 0; i < total_processes; i++) {
            if (!table[i].done && table[i].arrival < earliest) {
               earliest = table[i].arrival;
            }//end if
         }//end for
      
      current_time = earliest;
      continue;
      }//end if
   
   table[idx].start_time = current_time;
   table[idx].end_time = table[idx].start_time + table[idx].total_cpu;
   table[idx].turnaround_time = table[idx].end_time - table[idx].arrival;
   table[idx].done = 1;
   table[idx].total_remaining = 0;
   current_time = table[idx].end_time;
   scheduled++;
   }//end while
   
   print_table();
   return;
}//end sjf()

/*************************************************************************************/
void srt(){
   int i;
   int done_count = 0;
   int current_time = 0;
   
   if (table == NULL || total_processes == 0) {
      return;
   }//end if
   
   for (i = 0; i < total_processes; i++) {
      table[i].done = 0;
      table[i].total_remaining = table[i].total_cpu;
      table[i].already_started = 0;
      table[i].start_time = -1;
      table[i].end_time = -1;
      table[i].turnaround_time = -1;
   }//end for
   
   while (done_count < total_processes){
      int idx = -1;
      int smallest = INT_MAX;
      for (i = 0; i < total_processes; i++) {
         if (!table[i].done && table[i].arrival <= current_time) {
            if (table[i].total_remaining < smallest) {
               smallest = table[i].total_remaining;
               idx = i;
            }//end inner if
         }//end if
      }//end for
      
      if (idx == -1) {
         int next_arrival = INT_MAX;
         for (i = 0; i < total_processes; i++) {
            if (!table[i].done && table[i].arrival < next_arrival) {
               next_arrival = table[i].arrival;
            }//end inner if
         }//end for
         
         if (next_arrival == INT_MAX) break;
         current_time = max_int(current_time, next_arrival);
         continue;
      }//end if
      
      if(!table[idx].already_started) {
      table[idx].start_time = current_time;
      table[idx].already_started = 1;
      }//end if
      
      table[idx].total_remaining -= 1;
      current_time++;
      
      if (table[idx].total_remaining == 0) {
         table[idx].done = 1;
         table[idx].end_time = current_time;
         table[idx].turnaround_time = table[idx].end_time - table[idx].arrival;
         done_count++;
      }//end if
   }//end while
   
   print_table();
   return;
}//end srt()

/*****************************************************************************************/
void quit() {
   if (table != NULL) {
      free(table);
      table = NULL;
   }//end if
   
   printf("Quitting Program...\n");
   return;
}//end quit()

/************************************************************************/
int main(){
   int choice = 0;
   
   while (1) {
      printf("\nBatch scheduling\n");
      printf("------------------\n");
      printf("1) Enter parameters\n");
      printf("2) Schedule processes with FIFO algorithm\n");
      printf("3) Schedule processes with SJF algorithm\n");
      printf("4) Schedule processes with SRT algorithm\n");
      printf("5) Quit and free memory\n");
      printf("\nEnter selection: ");
      if (scanf("%d", &choice) != 1) {
         while (getchar() != '\n');
         continue;
      }//end if
      
      if (choice == 1) {
         enter_parameters();
      } else if (choice == 2) {
         printf("\n");
         fifo();
      } else if (choice == 3) {
         printf("\n");
         sjf();
      } else if (choice == 4) {
         printf("\n");
         srt();
      } else if (choice == 5) {
         quit();
         break;
      } else {
         continue;
      }//end else if
   }//end while
   
   return 1;
}//end main