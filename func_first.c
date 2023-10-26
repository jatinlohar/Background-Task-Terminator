#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>

long uptime_ticks;

typedef struct node
{
    char info[40];
    unsigned long long val1, val2;
    struct node *left,*right;
}tree_type;


void insert(tree_type **rt,char str[40], unsigned long long value1, unsigned long long value2)
{
    tree_type *p;
    p=(tree_type*)malloc(sizeof(tree_type));

    strcpy(p->info,str);
    p->val1 = value1;
    p->val2 = value2;
    p->right=NULL;
    p->left=NULL;
   
    if(*rt==NULL)
    {
        tree_type *temp = *rt;
        *rt=p;
        free(temp);
        return;
    }
    else
    {
        int temp = strcmp(str,((*rt)->info));
        if(temp<0)
            insert(&((*rt)->left),str, value1, value2);
        else if(temp > 0)
            insert(&((*rt)->right),str, value1, value2);  
        else
        {
        (*rt)->val2 = value2;
        return;
        }    
    }
}

void display(tree_type *rt)
{

    static int c=0;
    if(rt==NULL)
    {
        return;
    }
    else
    {
        display(rt->left);
        if(rt->val1 > 100 && (rt->val2 - rt->val1 > 0) && (uptime_ticks - rt->val2 > 3000))
        {
            FILE *fp;
            fp = fopen("datafile.txt", "a");
            printf("%s - %llu - %llu - %ld\n",rt->info, rt->val1, rt->val2, uptime_ticks);
             
            fprintf(fp, "%s\n", rt->info);
            fclose(fp);   
        }

        display(rt->right);
    }
}

unsigned long long search1(tree_type *rt,char str[40])
{
    if(rt==NULL)
    {
        return 0;
    }

    if(strcmp(str,rt->info)==0)
    {
        return rt->val1;
    }
    else
    {
        if(strcmp(str,rt->info)<0)
            search1(rt->left,str);
        else
            search1(rt->right,str);
    }
}

unsigned long long search2(tree_type * rt, char str[40])
{
    if(rt == NULL)
        return 0;

    if(strcmp(str,rt->info)==0)
    {
        return rt->val2;
    }
    else
    {
        if(strcmp(str,rt->info)<0)
            search1(rt->left,str);
        else
            search1(rt->right,str);
    }
}


tree_type *start_time = NULL;
//tree_type *duration_time = NULL;


int check(char str[], int n)
{
    for(int i=1; i<n; i++)
    {
        if(!isalpha(str[i]))
        return 0;
    }
    return 1;
}


unsigned long long extractStartTime(char* pid)
{
    char path[256];
    snprintf(path, sizeof(path), "/proc/%s/stat", pid);
    FILE* statFile = fopen(path, "r");
    if (statFile == NULL)
    {
    return 0;
    }
   
    char temp[20], creater[20];
    unsigned long long int startTime;
     
    fscanf(statFile, "%s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %llu",temp,creater,temp,temp,temp,
    temp,temp,temp,temp,temp,temp,temp,temp,temp,temp,temp,temp,temp,temp,temp,temp,&startTime);
    int n = strlen(creater);



    if(check(creater, n-1) && !(creater[1] == 'k' && creater[2] == 'e' && creater[3] == 'r'))
    {
        printf("PID: %s, Created by : %s, Start Time: %llu\n", pid, creater, startTime);
    
        insert(&start_time, creater, startTime, startTime);
    }
            
            
    fclose(statFile);
    return startTime;
}



int main()
{
   
    system("clear");

    DIR* directory = opendir("/proc");
    struct dirent* entry;  
    
    if (directory == NULL)
    {
        perror("opendir");
        return 0;
    }


    while ((entry = readdir(directory))) {
        if (entry->d_type == DT_DIR)
        {
            if (atoi(entry->d_name) != 0)
            {
                unsigned long long startTime = extractStartTime(entry->d_name);
            }
        }
    }
       
   
    closedir(directory);
   

    FILE *uptime_file = fopen("/proc/uptime", "r");
    if (uptime_file == NULL) {
        perror("fopen");
        return 1;
    }

    double uptime;
    if (fscanf(uptime_file, "%lf", &uptime) != 1)
    {
        perror("fscanf");
        fclose(uptime_file);
        return 1;
    }

    fclose(uptime_file);
   
    long clock_ticks_per_second = sysconf(_SC_CLK_TCK);
    uptime_ticks = (long)(uptime * clock_ticks_per_second);
   
    printf("System uptime: %ld seconds\n", uptime_ticks);
 
    FILE *fp;
    fp = fopen("datafile.txt", "w");
    fclose(fp);

 
    display(start_time);

    return 0;
    
}