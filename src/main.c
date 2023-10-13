#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define COMMAND_LENGTH 1024

int main()
{
    char command[COMMAND_LENGTH];
    char *args[COMMAND_LENGTH];

    for (;;)
    {
        printf("Mini Shell > ");
        fflush(stdout);

        // Lire la commande de l'utilisateur
        if (fgets(command, sizeof(command), stdin) == NULL)
        {
            break;
        }

        // Supprimer le saut de ligne final
        command[strcspn(command, "\n")] = '\0';

        if (strcmp(command, "exit") == 0)
        {
            printf("Au revoir !\n");
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
