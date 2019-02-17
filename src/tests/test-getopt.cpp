#include <iostream>

#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <getopt.h>

int main(int argc, char *argv[])
{
    int c;
    int digit_optind = 0;
    int persistent_flag = 0;

    while (true) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            //name, has_arg, flagpointer, value to set flag
            {"persistent", no_argument, &persistent_flag, 0},
            {0           ,           0,                0, 0}
        };

        c = getopt_long(argc, argv, "",
                 long_options, &option_index);
        if (c == -1)
            break;

       switch (c) {
        case 0:
            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;

        case 'p':
            printf("option a\n");
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    exit(EXIT_SUCCESS);
}