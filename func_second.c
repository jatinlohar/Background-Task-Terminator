#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include<string.h>
#include<math.h>
#include<signal.h>
#include <sys/wait.h>
#include<time.h>
#include<ctype.h>

unsigned long long check(char* name, FILE *fp)
{
    DIR* directory = opendir("/proc");
    struct dirent* entry;  
    if(directory == NULL)
    {
           perror("opendir");
           return 0;
    }
             


    while ((entry = readdir(directory))) {
        if (entry->d_type == DT_DIR) {
                if (atoi(entry->d_name) != 0)
                {
                    char path[300];
                    snprintf(path, sizeof(path), "/proc/%s/stat", entry->d_name);

                    FILE* statFile = fopen(path, "r");
                    if (statFile == NULL)
                    {
                        return 0;
                    }
                
                    char creater[20];
                    long pid;
                    
                    
                    fscanf(statFile, "%ld %s",&pid,creater);
                    
                    // size_t len = strlen(creater);
                    // // Remove parentheses if they exist
                    // if (creater[0] == '(' && creater[len - 1] == ')') {
                    //     memmove(creater, creater + 1, len - 2);
                    //     creater[len - 2] = '\0';
                    // }

                    // char creater_new[20];
                    // strncpy(creater_new, creater, sizeof(creater_new)-1);
                    // creater_new[sizeof(creater_new)-1] = '\0'; // Ensure null-termination



                    if(strcmp(creater, name) == 0)
                    {   
                        char cmdd[20];
                        strcpy(cmdd, creater);
                        size_t len = strlen(cmdd);
                        // Remove parentheses if they exist
                        if (cmdd[0] == '(' && cmdd[len - 1] == ')') {
                            memmove(cmdd, cmdd + 1, len - 2);
                            cmdd[len - 2] = '\0';
                        }

                        char creater_new[20];
                        strncpy(creater_new, cmdd, sizeof(creater_new)-1);
                        creater_new[sizeof(creater_new)-1] = '\0'; // Ensure null-termination

                        printf("%ld %s\n" , pid, creater_new);
                        fprintf(fp, "%s %ld\n", creater_new, pid);
                        //kill((pid_t)pid, SIGKILL);
                    }

                }
            }
        }
   
    // fclose(fp);
       
   
    closedir(directory);
}

int main ()
{

    FILE* statFile = fopen("datafile.txt", "r");
    char buffer[40];

    FILE *fp;
    fp = fopen("pids.txt", "wa");
    while (fgets(buffer, sizeof(buffer), statFile))
    {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        // Print the line with parentheses
        printf("(%s)\n", buffer);
        check(buffer, fp);

        
        /*
        printf("%s\n",buffer);
        // buffer[0] = '(';

        // int x = strlen(buffer);
        // buffer[x] = ')';
        // buffer[x+1] = '\0';
        check(buffer);
        */
    }
    fclose(fp);
    
    fclose(statFile);

}
