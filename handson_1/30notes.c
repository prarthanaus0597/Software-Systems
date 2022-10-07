/* NAME : Karishma Chauhan ; 
   ROLL NO. : MT2022056 
   30. Write a program to run a script at a specific time using a Daemon process.
 */

   
//#include <sys/types.h>  //pid_t type needs this
#include <unistd.h>
#include <fcntl.h>    // open , flags
#include <sys/stat.h> //umask
#include <time.h>     // declares the structure tm  ; object time_t 
#include <stdio.h>
#include <string.h>     //strlen
#include <stdlib.h>  //exit

// Argument to be passed as hour minute second (particular time of a day) hour is mandatory

int main(int argc, char *argv[])
{
    struct tm *inputtime, *currenttime;               			  //Deadline in user readable format
    /* Time structure :Structure containing a calendar date and time broken down into its components.Contains nine members of type int (in any order):  */
    
    pid_t child;   //same as int child;
    /*should include the header files `unistd.h' and `sys/types.h' to use these functions. 
    Data Type: pid_t ----> The pid_t data type is a SIGNED INTERGER TYPE which is capable of representing a process ID. In the GNU library, THIS IS AN INT. */

    int  fd=open("FileQ30.txt",O_CREAT|O_RDWR,		0777);     /*
    				O_RDONLY: read only,	int mode 
    				O_WRONLY: write only, 
    				O_RDWR: read and write, 
    				O_CREAT: create file if it doesn’t exist, 
    				O_EXCL: prevent creation if it already exists
    				
    				*/
    time_t currentseconds, secondstorun; 		// Current system time & deadline time in posix time 
    /*time_t : almost always an integral value holding the number of SECONDS (not counting leap seconds) since 00:00, Jan 1 1970 UTC, corresponding to POSIX time; hence used to 			   data type for return value of time function returing the current calendar time as an object time_t type
    */
    
    if (argc <=1 )
        printf("Pass at least hour argument\n");
    else
    {
        time(&currentseconds); 				// Get current time : returns the current time as number of SECONDS (not counting leap seconds) since 00:00, Jan 1 1970 UT
        //return type time_t : if parameter is not a null pointer, the returned value is also stored in the object pointed to by parameter for example here currentTIME is the parameter
        
        
        inputtime = localtime(&currentseconds);		// take seconds from time() as input(pointer to object) and covert number of SECONDS (not counting leap seconds) since 00:00, Jan 1 1970 UT to time we see in laptop
        //return type structure tm
        
         
        //set mytime using user input
        inputtime->tm_hour = atoi(argv[1]);   //atoi : char to int convert
        inputtime->tm_min = argv[2] == NULL ? 0 : atoi(argv[2]);
        inputtime->tm_sec = argv[3] == NULL ? 0 : atoi(argv[3]);

       	secondstorun = mktime(inputtime); 		// Convert tm to time_t
      
        if ((child = fork()) == 0)
        {
     		//inside child
            pid_t sid= setsid();			//set session id  
            
            if(sid<0)
            {  perror("setsid Failed..: ");
            	exit(0);
            }
            printf("Session id=%d\n",sid);		// print session id if no error
            chdir("/");					//change to root 
            umask(0);					//set file mode creation mask to 0 : since the inherited file mode creation mask could be denying certain conditions that daemon process wants to set 
            do
            {
            
                currentseconds=time(NULL);			// To keep updating the current time here for comparison
                // If parameter is not a null pointer, the returned value is also stored in the object pointed to by parameter.else returns the value of the current time as number of SECONDS 
                
            } while ((difftime(secondstorun, currentseconds) > 0)||(difftime(secondstorun, currentseconds) < 0)); // waiting for the time specified to execute script
           
           
           /*can be used to run the script
            
            if(chdir("/home/prarthanaus/handson_list1/excer/submission")<0){ //change the directory to run the script.sh
            printf("Cannot change directory\n");
            exit(0);
            }
               
            execvp(args[0],args);*/
            time_t t;   // not a primitive datatype
	    time(&t);
            //currenttime = localtime(&currentseconds);
           // char buf[20];
          //  buf=currenttime->tm_min;
            write(fd,"Daemon process is running and completed\n",strlen("Daemon process is running and completed"));
            dup2(fd, 1) ;
            printf("\nThis daemon process last ran at (date and time): %s", ctime(&t));

            //after waiting for specified time to exectue file write script 
           // write(fd,"Daemon process is running and completed\n",strlen("Daemon process is running and completed"));
            //write(fd,localtime(&t),sizeof(localtime(&t)));
            exit(0);
        }
        else 
        	exit(0);
        	/* EXITING PARENT : do many things
        	   - 1. if daemon process was started as a simple shell command , terminating parent make shell think command is done.
        	   - 2. child inherits the process group id of the parent but gets new PID so we are sure that the child is no the process group leader. This is prerequisite for call to "setsid" 
        	*/
    }
    return 0;
}

