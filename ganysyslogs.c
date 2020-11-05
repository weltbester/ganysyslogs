/** 
 * Copyright October 2020: Georg Pohl, 70174 Stuttgart
 * File: ganysyslogs.c
 * -------------------
 * This program can be used to identify unknown syslog
 * messages, when you introduce a new router software. It is
 * meant to be used durch den Product Validation (PV) time only.
 * Therefore you feed the program with the involved router
 * hostnames, so that it can browse through the recent logfile
 * and grep out every syslog message of the before defined hosts.
 *
 * Every message is then compared with the 'blacklist'- and
 * a 'whitelist' file.
 *
 * 1. If it is found in 'blacklist', the message is known but
 *    regarded as not-remarkable and the original message is
 *    skipped.
 * 2. If it is found in the 'whitelist' it is already know and
 *    declared as remarkable and therfore under control. So no
 *    further processing needed and therefore also skipped.
 * 3. The message is neither known in the 'blacklist' nor the
 *    'whitelist' means we are dealing with an unknown message
 *    here.
 *    This message is send to the initiator of the script via
 *    e-Mail for further investigation. Depending on the
 *    importance of the syslog message, it then copied either
 *    to the 'black-' or 'whitelist'.
 *
 * 'blacklist': contains log messages which are not transferred
 * to the "MONITORING SYSTEM".
 *
 * 'whitelist': contains log messages which are importand and have
 * to be transferred to the "MONITORING SYSTEM".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SENTINEL 8      // Determines which option can quit the program

/* CONSTANTS */

/* STRUCTS */

/* PROTOTYPES */
char **enterRouters(int *groesse);
void addRouters(char *devices[], int n, int more);
void showRouters(char *arr[], int n);
char **deleteRouters(char *devices[], int *n);

int main(int argc, const char **argv) {
    char **routers = NULL;
    int choice = 0, nHosts = 0, more = 0;
    
    printf("\n\tGANYSYSLOGS: TOOL TO IDENTIFY NEW SYSLOG MESSAGES\n");
    printf("\t-------------------------------------------------\n");

    do {
        putchar('\n');
        printf("\t-1- Enter routers\n");
        printf("\t-2- Add more routers\n");
        printf("\t-3- Delete all routers\n");
        printf("\t-4- Show routers\n");
        printf("\t-5- Create cronjob\n");
        printf("\t-6- Enter syslog signature to 'blacklist'\n");
        printf("\t-7- Enter syslog signature to 'whitelist'\n");
        printf("\t-8- Quit\n\n");

        printf("Your choice: ");
        if ( (scanf("%d", &choice) != 1) ) {
            printf("Fehler bei der Eingabe!\n");
            return 1;
        }
        // putchar('\n');

        // Switch Anweisung
        switch (choice) {
            // Enter routers
            case 1: routers = enterRouters(&nHosts);
                    if (NULL == routers) {
                        return 1;
                    }
            break;
            // Add more routers
            case 2: if (NULL == routers) {
                        printf("Function is only for adding routers\n"
                        "to existing routers. - Use option '1' first.\n");
                        break;
                    } else {
                        printf("How many routers to add? ");
                        if ( (scanf("%d", &more) != 1)) {
                            printf("Input Error!\n");
                            exit(1);
                        }
                        nHosts += more;
                        routers = (char **)realloc(routers, nHosts * sizeof(char *));
                        addRouters(routers, nHosts, more); // Create cronjob
                    }
            break;
            // Delete all entered routers
            case 3: routers = deleteRouters(routers, &nHosts);
                    printf("%d router(s) deleted.\n", nHosts);
            break;
            // Display entered routers
            case 4: showRouters(routers, nHosts);
            break;
            // Create cronjob
            case 5:
            break;
            // Enter syslog signature to 'blacklist'
            case 6:
            break;
            // Enter syslog signature to 'whitelist'
            case 7:
            break;
            case SENTINEL:  printf("Bye!\n");
                            if (NULL != routers) {
                                free(routers);
                                routers = NULL;
                            }
                            
                            /* if ( NULL == routers) {
                                break;
                            } else {
                                free(routers);
                                routers = NULL;
                            }*/
            break;
            default: printf("Gültige Auswahl 1 - %d\n\n", SENTINEL);
            break;
        }
    } while (choice != SENTINEL);
    // Free memory and avoid dangling pointers
    
  return EXIT_SUCCESS;
}
char **enterRouters(int *groesse) {
    char **result;
    printf("How many routers to enter? ");
    if ( (scanf("%d", groesse) != 1)) {
        printf("Input Error!\n");
        return NULL;
    }
    result = (char **)malloc(*groesse * sizeof(char *));
    if ( NULL == result ) {
        fprintf(stderr, "Memory Allocation Failure\n");
        return NULL;
    }
    char hostName[20];
    for (int i=0; i < *groesse; ++i) {
        printf("%d. Hostname: ", i+1);
        if ( (scanf("%s[^\n]", hostName) != 1) ) {
            printf("Input Error!\n");
            return NULL;
        }
        result[i] = (char *)malloc((strlen(hostName)+ 1) * sizeof(char));
        if ( NULL == result[i]) {
            fprintf(stderr, "Memory Allocation Failure\n");
            return NULL;
        }
        strcpy(result[i], hostName);
    }
    return result;
}
void addRouters(char *devices[], int n, int more) {
    char hostName[20];
    for (int i=n-more; i < n; ++i) {
        printf("%d. Hostname: ", i+1);
        if ( (scanf("%s[^\n]", hostName) != 1) ) {
            printf("Input Error!\n");
            exit(1);
        }
        devices[i] = (char *)malloc((strlen(hostName)+ 1) * sizeof(char));
        if ( NULL == devices[i]) {
            fprintf(stderr, "Memory Allocation Failure\n");
            break;
        }
        strcpy(devices[i], hostName);
    }
    return;
}
void showRouters(char *arr[], int n) {
    if (NULL == arr) {
        printf("No routers entered\n");
    } else {
        printf("\nAdded routers: ");
        printf("[");
        for (int i=0; i < n; ++i) {
            if (i == 0) {
                printf("%s", arr[i]);
            } else {
                printf(", %s", arr[i]);
            } 
        }
        printf("]\n\n");
    }    
    return;
}
// TODO: Funktion bringt manchmal SegFaults!
char **deleteRouters(char *devices[], int *n) {
    //int m = *n;
    if ( NULL != devices) {
        for (int i = 0; i < *n; ++i) {
            free(devices[i]);
            //*n -= 1;
            //devices[i] = NULL;
        }
        free(devices);        
        devices = NULL;
    }
    return devices;
}