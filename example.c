#include "top_proc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//#include "socket_util.h"

// void send_data_to_python(proc_t* procs, unsigned int procs_len) {
//     int client_socket = create_client_socket("127.0.0.1", 8888);

//     // Construct the data string (you should define your own format)
//     char data[1024] = "";
//     for (unsigned int i = 0; i < procs_len; ++i) {
//         char entry[256];
//         snprintf(entry, sizeof(entry), "%s:%f,", procs[i].cmd, procs[i].pcpu);
//         strcat(data, entry);
//     }

//     // Send data to Python
//     send_data(client_socket, data);

//     close(client_socket);
// }



int main() {
    // sample for 500 ms
    struct timespec tm;
    tm.tv_sec  = 0;
    tm.tv_nsec = 500 * 1000 * 1000; /* 500 ms */

    proc_t* procs = NULL;
    unsigned int procs_len = 0;
    sample_processes(&procs, &procs_len, tm);

    // sort by CPU usage and print
    qsort(procs, procs_len, sizeof(procs[0]), proc_comp_pcpu);
    // print
    printf("%-10s %-10s %-10s %-10s\n", "PID", "<CPU>", "RSS", "CMD");
    for (unsigned int i = 0; i < procs_len && i < 20; ++i) {
        if (strlen(procs[i].cmd) == 0) {
            break;
        }
        if(procs[i].pcpu){
            printf("%-10d %-10.2f %-10lu %-10s\n", procs[i].tid, procs[i].pcpu, procs[i].vm_rss, procs[i].cmd);
        }
    }

    printf("\n");
    // sort by resident set size and print
    qsort(procs, procs_len, sizeof(procs[0]), proc_comp_pcpu);
    printf("%-10s %-10s %-10s %-10s\n", "PID", "CPU", "<RSS>", "CMD");
    for (unsigned int i = 0; i < procs_len && i < 20; ++i) {
        if (strlen(procs[i].cmd) == 0) {
            break;
        }
        if(procs[i].pcpu){
            printf("%-10d %-10.2f %-10lu %-10s\n", procs[i].tid, procs[i].pcpu, procs[i].vm_rss, procs[i].cmd);
        }
    }

    // // Initialize a data structure to store aggregated data
    // int MAX_PROCESSES = 1000; // Adjust the size according to your needs
    // proc_t* aggregate_procs = (proc_t*)malloc(MAX_PROCESSES * sizeof(proc_t));

    // for (int i = 0; i < MAX_PROCESSES; i++) {
    //     aggregate_procs[i].tid = -1; // Initialize tid to -1 to indicate unused entry
    //     aggregate_procs[i].pcpu = 0.0;
    //     aggregate_procs[i].vm_rss = 0;
    // }

    // // ...

    // // Sum up CPU usage and RSS for processes with the same name
    // for (unsigned int i = 0; i < procs_len && i < 50; ++i) {
    //     if (strlen(procs[i].cmd) == 0) {
    //         break;
    //     }
    //     if (procs[i].pcpu) {
    //         int j;
    //         for (j = 0; j < MAX_PROCESSES; j++) {
    //             if (aggregate_procs[j].tid == -1 || strcmp(aggregate_procs[j].cmd, procs[i].cmd) == 0) {
    //                 aggregate_procs[j].tid = j; // Assign a unique identifier for the aggregate process
    //                 aggregate_procs[j].pcpu += procs[i].pcpu;
    //                 aggregate_procs[j].vm_rss += procs[i].vm_rss;
    //                 strcpy(aggregate_procs[j].cmd, procs[i].cmd);
    //                 break;
    //             }
    //         }
    //     }
    // }


    FILE *file = fopen("output.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Write the data to the file
    fprintf(file, "%-10s %-10s %-10s %-10s\n", "PID", "CPU", "RSS", "CMD");
    for (unsigned int i = 0; i < procs_len && i < 20; ++i) {
        if (strlen(procs[i].cmd) == 0) {
            break;
        }
        if (procs[i].pcpu) {
            fprintf(file, "%-10d %-10.2f %-10lu %-10s\n", procs[i].tid, procs[i].pcpu, procs[i].vm_rss, procs[i].cmd);
        }

        // if (aggregate_procs[i].tid != -1) {
        //     fprintf(file, "%-10d %-10.2f %-10lu %-10s\n", aggregate_procs[i].tid, aggregate_procs[i].pcpu, aggregate_procs[i].vm_rss, aggregate_procs[i].cmd);
        // }

    }

    fclose(file);

    // send_data_to_python(procs, 20);

    free(procs);
    
}
