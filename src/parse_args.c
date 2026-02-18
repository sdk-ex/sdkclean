#include "../include/pch.h"
#include "../include/parse_args.h"
#include <getopt.h>

static struct option long_options[] = {
    {"clean",   no_argument,       0, 'c'},
    {"journal", no_argument,       0, 'j'},
    {"trash",   optional_argument, 0, 't'},
    {"help",    no_argument,       0, 'h'},
    {"ask",     no_argument,       0, 'i'},
    {"dry-run", no_argument,       0, 'n'}, 
    {0, 0, 0, 0}
};

int parse_args(int argc, char* argv[], Appconfig* config) {
    int opt;
    int option_index = 0;

    // We assume config is zeroed by the caller (AppConfig c = {0}).
    
    // "+cjt:h" -> + enforces Strict POSIX (stops at first non-flag)
    // BUT we abandoned that and are now running "cjt::h"
    while ((opt = getopt_long(argc, argv, "+cjt::hni", long_options, &option_index)) != -1) {
        switch (opt) {
            case 'c': config->clean_apt = 1;      
              break;
            case 'j': config->journal_maint = 1;  
              break;
            case 't': 
              config->clean_trash = 1;

              // If 'optarg' is not NULL, the user provided a path (e.g. -t /tmp)
              if (optarg) {
                  config->trash_path = optarg;
              }
                break;
                
              case 'n': config->dryrun = 1; 
                break;
              case 'i': config->ask = 1;    
                break;
                
              case 'h':
                printf("Usage: %s [options] [targets...]\n", argv[0]);
                printf("  -c, --clean    Clean APT cache\n");
                printf("  -j, --journal  Perform journal maintenance\n");
                printf("  -t, --trash    Empty trash (optional path)\n");
                printf("  -h, --help     Show this message\n");
                               
                exit(EX_OK); 
      
            case '?': 
              return EX_USAGE;
            default:  
              return EX_USAGE;
        }
    }

    // Capture the start of the targets (non-flag args)
    if (optind < argc) 
    {
        config->targets = &argv[optind];
        config->target_count = argc - optind;
    }

    return EX_OK;
}
