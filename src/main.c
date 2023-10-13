#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


#define RESET             "\x1b[0m"
#define BLACK             "\x1b[30m"
#define RED               "\x1b[31m"
#define GREEN             "\x1b[32m"
#define YELLOW            "\x1b[33m"
#define BLUE              "\x1b[34m"
#define MAGENTA           "\x1b[35m"
#define CYAN              "\x1b[36m"
#define WHITE             "\x1b[37m"

#define COMMAND_LENGTH 2048

int getcommand(char* str, int size, FILE* stream){
    int i = 0;
    int ch;

    while (i < size - 1){
        ch = getchar();

        if (ch == EOF || ch == '\n') break;

        str[i++] = (char) ch;
    }

    str[i] = '\0';
    return i;
}

int main()
{
    char command[COMMAND_LENGTH];
    char *args[COMMAND_LENGTH];

    for (;;)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        char* project = strstr(cwd, "/mysh");
        if(project != NULL) printf(BLUE " %s > " RESET, project);
        else printf(GREEN " > ");

        // Lire la commande de l'utilisateur
        // if (fgets(command, sizeof(command), stdin) == NULL)
        // {
        //     break;
        // }

        getcommand(command, sizeof(command), stdin);

        // Supprimer le saut de ligne final
        // command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            break; // Quitter le mini-shell
        }

        // Diviser la commande en tokens
        char *token = strtok(command, " ");
        int i = 0;
        while (token != NULL)
        {
            args[i++] = token;
            token = strtok(NULL, " ");
        }
        args[i] = NULL; // Dernier élément doit être NULL pour execvp

        // Créer un processus enfant
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        { // Code du fils
            // Exécuter la commande
            if (execvp(args[0], args) == -1)
            {
                perror("execvp");
                exit(1);
            }
        }
        else
        { // Code du parent
            // Attendre la fin du processus enfant
            wait(NULL);
        }
    }
    return 0;
}