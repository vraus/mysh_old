#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#define RESET "\x1b[0m"
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define WHITE "\x1b[37m"

#define COMMAND_LENGTH 2048

void getcommand(char *str)
{
    int i = 0, ch;

    while ((ch = getchar()) != EOF)
    {
        if (ch == '\n')
        {
            break;
        }

        str[i++] = (char)ch;
    }

    if (ferror(stdin))
    {
        perror("Erreur de lecture");
        exit(EXIT_FAILURE);
    }
    str[i] = '\0';
}

void tokenize(char *str, char *args[])
{
    char *s;
    int i = 0;
    for (s = str; isspace(*s); s++)
        ;
    for (i = 0; *s; i++)
    {
        args[i] = s;
        while (!isspace(*s))
            s++;
        *s++ = '\0';
        while (isspace(*s))
            s++;
    }
}

int main()
{
    char command[COMMAND_LENGTH], *args[COMMAND_LENGTH];
    int wstatus;

    for (;;)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));

        char *project = strstr(cwd, "/mysh");
        if (project != NULL)
            printf(BLUE " %s > " RESET, project);
        else
            printf(GREEN " > ");

        // Lire la commande de l'utilisateur
        getcommand(command);
        if (strcmp(command, "exit") == 0)
        {
            break; // Quitter le mini-shell
        }
        tokenize(command, args);

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
            if (waitpid(-1, &wstatus, WEXITSTATUS(wstatus)) < 0)
            {
                perror("Error waitpid");
                exit(1);
            }
        }
    }
    return 0;
}