#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>

#include "myls.h"

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

/**
 * @brief Récupère la commande saisie par l'utilisateur
 * @param str : la commande
 */
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

/**
 * @brief Permet de fragmenter la commande saisie par l'utilisateur en plusieur chaine de charactère
 * qui sont récupérer dans un tableau de chaine de caractère
 * @param str : la commande
 * @param args : tableau contenant les mots de la commande
 */
void tokenize(char *str, char *args[])
{
    char *s;
    int i = 0;
    for (s = str; isspace(s[i]); s++)
        ;
    for (i = 0; s[i]; i++)
    {
        args[i] = s;
        while (!isspace(*s) && *s != ';')
            s++;
        if (*s == ';')
        {
            *s++ = '\0'; // Marque la fin de ligne
            args[i] = s; // Passage à la prochaine commande
        }
        else
        {
            *s++ = '\0';
        }
        while (isspace(*s) || *s == ';')
            s++;
    }
}

/**
 * @brief Permet de savoir quelle option ou commande(myls)
 *  a été saisie par l'utilisateur en modifiant la variable correspondante
 * @param args : tableau contenant les mots de la commande
 * @param hasA : variable lié à l'option -a
 * @param hasR : variable lié à l'option -R
 * @param hasMyLs : variable lié à la commande myls
 */
void hasOption(char **args, int *hasA, int *hasR, int *hasMyLs)
{
    for (int i = 0; args[i] != NULL; i++)
    {
        if (strcmp(args[i], "-a") == 0)
        {
            *hasA = 1;
        }
        else if (strcmp(args[i], "-R") == 0)
        {
            *hasR = 1;
        }
        else if (strcmp(args[i], "myls") == 0)
        {
            *hasMyLs = 1;
        }
    }
}

/**
 * @brief si la commande saisi par l'utilisateur est myls, l'exécute
 * @param hasMyLs: variable lié à la commande myls
 * @param hasA : variable lié à l'option -a
 * @param hasR : variable lié à l'option -R
 */
void is_myls(int hasMyLs, int hasA, int hasR)
{
    if (hasMyLs == 1)
    {
        char *myls_args[4];
        myls_args[0] = "./myls";

        int arg_count = 1;

        if (hasA)
        {
            myls_args[arg_count++] = "-a";
        }

        if (hasR)
        {
            myls_args[arg_count++] = "-R";
        }

        myls_args[arg_count] = NULL;

        if (execvp(myls_args[0], myls_args) == -1)
        {
            perror("execvp");
            exit(1);
        }
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
        int hasA = 0, hasR = 0, hasMyLs = 0;
        getcommand(command);
        if (strcmp(command, "exit") == 0)
        {
            break; // Quitter le mini-shell
        }
        tokenize(command, args);
        hasOption(args, &hasA, &hasR, &hasMyLs);

        // Créer un processus enfant
        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        else if (pid == 0)
        { // Code du fils

            is_myls(hasMyLs, hasA, hasR);

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