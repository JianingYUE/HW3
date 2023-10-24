#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_LINE_LEN 1024
#define MAX_FIELDS 1024
#define STATS_FORMAT_STR "Number of lines: %d\nLongest field: %d characters\n"
#define USAGE_FORMAT_STR "ERR: expected usage is %s -c <in_cols> [-d delimiter] [-s] col1 ...\n"

int main(int argc, char **argv) {
    int in_cols = 0;
    int out_cols = 0;
    int c_flag = 0;
    char delim[2] = ",";  // making it a string for strtok

    int stats_flag = 0;
    unsigned int linecnt = 0;
    unsigned int lngst_fld = 0;

    int c;
    while ((c = getopt(argc, argv, "scd:")) != -1) {
        switch (c) {
        case 's':
            stats_flag = 1;
            break;
        case 'c':
            in_cols = atoi(optarg);
            c_flag = 1;
            break;
        case 'd':
            if(strlen(optarg) == 1){
                delim[0] = optarg[0];
            } else {
                return EXIT_FAILURE;
            }
            break;
        default:
            fprintf(stderr, USAGE_FORMAT_STR, argv[0]);
            return EXIT_FAILURE;
        }
    }

    if (!c_flag) {
        fprintf(stderr, USAGE_FORMAT_STR, argv[0]);
        return EXIT_FAILURE;
    }

    char *fields[MAX_FIELDS];  
    int target_cols[MAX_FIELDS];

    out_cols = argc - optind;
    for(int i = 0; i < out_cols; i++) {
        target_cols[i] = atoi(argv[optind + i]) - 1;
        if (target_cols[i] >= in_cols || target_cols[i] < 0) {
            fprintf(stderr, "Error: Specified column index is out of range.\n");
            return EXIT_FAILURE;
        }
    }

    char line[MAX_LINE_LEN];
    char *token;
    while(fgets(line, MAX_LINE_LEN, stdin) != NULL) {
        int index = 0;
        token = strtok(line, delim);
        while (token != NULL && index < in_cols) {
            fields[index++] = token;
            token = strtok(NULL, delim);
        }

        for(int i = 0; i < out_cols; i++) {
            printf("%s", fields[target_cols[i]]);
            if(i < out_cols - 1) {
                printf(",");
            }
        }
        printf("\n");
    }

    if(stats_flag) {
        printf(STATS_FORMAT_STR, linecnt, lngst_fld);
    }

    return EXIT_SUCCESS;
}
