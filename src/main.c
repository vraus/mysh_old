#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <getopt.h>

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
    int len = strlen(str), j, i = 0;
    for (j = 0; j < len; j++)
    {
        if (str[j] == ' ' || str[j] == '\t')
        {
            str[j] = '\0'; // Remplace les espaces et les tabulations par des terminaisons nulles
        }
        else if (j == 0 || str[j - 1] == '\0')
        {
            args[i] = &str[j]; // Pointe vers le début de chaque argument
            i++;
        }
    }
    args[i] = NULL;
}

/**
 * @brief Permet de savoir quelle option a été saisie par l'utilisateur en modifiant la variable correspondante
 * @param args : tableau contenant les mots de la commande
 */
void hasOption(char **args, int *mask)
{
    *mask = 0x000;
    int opt;

    while ((opt = getopt(1, args, "Ra")) != -1)
    {
        switch (opt)
        {
        case 'a':
            printf("Used parameter a.\n");
            *mask |= (1 << 0);
            break;
        case 'R':
            printf("Used parameter n.\n");
            *mask |= (1 << 1);
            break;

        default:
            printf("Invalid option.\n");
            // handle_bad_usage(args[0]);
        }
    }
}

/**
 * @brief si la commande saisi par l'utilisateur est myls, l'exécute
 * @param mask : permet de savoir si myls a été saisie, et qu'elle option à été saisie
 */
void is_myls(int mask)
{
    char *myls_args[4];
    myls_args[0] = "./option/myls";

    int arg_count = 1;

    if (mask & (1 << 1))
    {
        myls_args[arg_count++] = "-a";
    }

    if (mask & (1 << 2))
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

int main()
{
    char command[COMMAND_LENGTH], *args[COMMAND_LENGTH];
    int wstatus, mask;

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
        // int hasA = 0, hasR = 0, hasMyLs = 0;
        getcommand(command);
        if (strcmp(command, "exit") == 0)
        {
            break; // Quitter le mini-shell
        }
        tokenize(command, args);
        hasOption(args, &mask);

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(1);
        }
        if (pid == 0)
        {
            // Code du fils
            if (strcmp(args[0], "myls") == 0)
            {
                is_myls(mask);
            }
            else if (execvp(args[0], args) == -1)
            {
                perror("execvp");
                exit(1);
            }
        }
        else
        {
            // Code du parent
            if (waitpid(pid, &wstatus, 0) < 0)
            {
                perror("Error waitpid");
                exit(1);
            }
        }
    }

    return 0;
}