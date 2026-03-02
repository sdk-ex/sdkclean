#include "../include/pch.h"
#include "../include/parse_args.h"
#include "../include/commands.h"

int main(int argc, char *argv[]) 
{

    // init structs to zero for basline
    Appconfig config = {0};

    // get status of arg, see ../parse_args.c
    int status = parse_args(argc, argv, &config);
    
    // If parsing failed or help was shown, exit with the status returned
    if (status != EX_OK) 
    {
        return status;
    }
    
    // Check if NO flags were set and NO targets were provided
    if (!config.clean_apt && !config.clean_trash && !config.journal_maint && config.target_count == 0) 
    {
        // If the user asked for nothing, assume they need help
        // We can manually call the help printer here, or just print a hint.
        // For now, let's re-run parse_args with "-h" to show help, 
        // or just print a message.
        fprintf(stderr, "No options selected. Use -h for help.\n");
        return EX_USAGE;
    }
    
    // invokes 'apt clean'
    if (config.clean_apt) {
        if (run_apt(NULL, (void*)&config) != EX_OK) {
            fprintf(stderr, "Error: APT cleanup failed.\n");
            return EX_SOFTWARE;
        }
    }

    // invokes journal maint
    if (config.journal_maint) {
        if (run_journal_maint(NULL, (void*)&config) != EX_OK) {
            fprintf(stderr, "Error: Journal maintenance failed.\n");
            return EX_SOFTWARE;
        }
    }

    // delete contents of trash, 
    // This handles -t (default) OR -t/custom/path
    if (config.clean_trash) {
    
        // Pass the optional path we captured in parse_args (or NULL if they just typed -t)
        if (run_trash(config.trash_path, (void*)&config) != EX_OK) {
            fprintf(stderr, "Error: Trash cleanup failed.\n");
            return EX_SOFTWARE;
        }
    }

    // for extra targets 
    // This handles: ./sysmain /tmp/junk /var/crash
    // TODO: this handling feels clunky ... 
    //  if we dont end up with any other handling, 
    //  the loop should be fixed (?)
    if (config.target_count > 0) 
    {
        printf("Processing %d additional targets...\n", config.target_count);
        
        for (int i = 0; i < config.target_count; i++) 
        {
            char* target = config.targets[i];
            
            // We reuse run_trash because it acts as a generic "recursive delete" wrapper
            if (run_trash(target, NULL) != EX_OK) 
            {
                // For explicit targets, we warn but continue (don't crash the whole script)
                fprintf(stderr, "Warning: Failed to clean target '%s'\n", target);
            }
        }
    }

    printf("System maintenance completed successfully.\n");
    return EX_OK;
}
