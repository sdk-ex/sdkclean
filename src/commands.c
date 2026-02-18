#include "../include/pch.h"
#include "../include/commands.h"
#include "../include/parse_args.h"

static int execute_cmd(char *const argv[], const Appconfig* config) {
    
    /**
     * The two cases below are meta operations,
     * which is why they require the extra logic, on top
     * of the normal handling seen here.
    ***/
    if (config->dryrun) {
        printf("[DRY RUN] Would execute: ");
        for (int i = 0; argv[i] != NULL; i++) {
            printf("%s ", argv[i]);
        }
        printf("\n");
        return EX_OK;
    }
    
    if (config->ask) {
        printf("About to run: ");
        for (int i = 0; argv[i] != NULL; i++) {
            printf("%s ", argv[i]);
        }
        printf("\nProceed? [y/N]: ");
        
        int c = getchar();
        if (c != '\n') {
            while (getchar() != '\n'); 
        }

        if (c != 'y' && c != 'Y') {
            printf("Skipped.\n");
            return EX_OK; // Treat as success but do nothing
        }
    }

    // logic for fork-exec operations
    pid_t pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return EX_OSERR;
    }

    if (pid == 0) {
        execvp(argv[0], argv);
        fprintf(stderr, "Error: Failed to execute '%s': %s\n", argv[0], strerror(errno));
        exit(EX_UNAVAILABLE);
    } 
    
    int status;
    waitpid(pid, &status, 0);

    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
        return EX_OK;
    }
    
    return EX_SOFTWARE;
}

// Helper: dynamic path builder for safety
// Returns a buffer with the full path to ~/.local/share/Trash/files
// Note: In a larger app, we'd handle memory freeing more carefully, 
// but for a short-lived utility, static buffers are acceptable.
static const char* get_trash_path() {
    static char path[1024];
    const char* home = getenv("HOME");

    if (!home) {
        fprintf(stderr, "Error: HOME environment variable not set.\n");
        return NULL;
    }

    snprintf(path, sizeof(path), "%s/.local/share/Trash/files/*", home);
    return path;
}

// ---------------------------------------------------------
// Command Implementations
// ---------------------------------------------------------

int run_apt(const char* args, void* context) {
    (void)args;    // Unused
    
    const Appconfig* config = (const Appconfig*)context;
    
    printf("[*] Running APT Clean...\n");
    
    // "sudo apt-get clean"
    char *const cmd[] = {"sudo", "apt-get", "clean", NULL};
    return execute_cmd(cmd, config);
}

int run_trash(const char* path, void* context) {
    const Appconfig* config = (const Appconfig*)context;
    
    printf("[*] Emptying Trash...\n");

    // 1. Determine the path
    // If the user provided a specific path via CLI flag, use it.
    // Otherwise, calculate the default Ubuntu trash path.
    const char* target_path = (path != NULL) ? path : get_trash_path();

    if (!target_path) {
        return EX_CONFIG;
    }

    // Safety Check: Don't let rm -rf run on root or home accidentally
    if (strcmp(target_path, "/") == 0 || strcmp(target_path, getenv("HOME")) == 0) {
        fprintf(stderr, "Error: Safety stop! Attempted to delete root or home directory.\n");
        return EX_USAGE;
    }

    printf("    Target: %s\n", target_path);

    // 2. Execute
    // Note: 'rm' with wildcards (*) usually requires shell expansion. 
    // execvp DOES NOT expand wildcards (that's a bash feature).
    // So to use '*', we actually need to invoke sh -c.
    
    char cmd_str[1100];
    snprintf(cmd_str, sizeof(cmd_str), "rm -rf %s", target_path);
    
    char *const cmd[] = {"sh", "-c", cmd_str, NULL};
    return execute_cmd(cmd, config);
}

int run_journal_maint(const char* args, void* context) {
    (void)args;
    const Appconfig* config = (const Appconfig*)context;

    printf("[*] Rotating System Logs...\n");

    // "sudo journalctl --vacuum-time=2d"
    char *const cmd[] = {"sudo", "journalctl", "--vacuum-time=2d", NULL};
    return execute_cmd(cmd, config);
}
