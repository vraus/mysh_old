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

#define handle_error(msg, status) \
    do                            \
    {                             \
        perror(msg);              \
        exit(status);             \
    } while (0)

#define handle_error_noexit(msg) \
    do                           \
    {                            \
        perror(msg);             \
    } while (0)

/**
 * @brief `void` Récupère la commande saisie par l'utilisateur
 * @param str `char *`la commande
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
 * @brief `void` Permet de fragmenter la commande saisie par l'utilisateur en plusieur chaine de charactère
 * qui sont récupérer dans un tableau de chaine de caractère
 * @param str `char *` la commande
 * @param args `char *` tableau contenant les mots de la commande
 */
void tokenize_space(char *str, char *args[])
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

void tokenize(char *str, char *commands[], int *command_count)
{
    int len = strlen(str), i = 0, j;
    for (j = 0; j < len; j++)
    {
        if (j + 1 < len && str[j] == '&')
        {
            if (str[j - 1] != '&')
            {
                str[j - 1] = '\0';
            }
            handle_error_noexit("&&: tokenize");
        }
        if (str[j] == ';' || (str[j - 1] == '&' && str[j] != '&'))
        {
            str[j] = '\0';
        }
        else if (j == 0 || str[j - 1] == '\0')
        {
            commands[i] = &str[j]; // Pointe vers le début de chaque argument
            i++;
        }
    }
    *command_count = i;
    printf("c : %d\n", *command_count);
    commands[i] = NULL;
}

/**
 * @brief `void` Permet de savoir quelle option a été saisie par l'utilisateur en modifiant la variable correspondante
 * @param args `char **` tableau contenant les mots de la commande
 * @param mask `int *` permet de savoir quelles options du myls sont utilisées
 */
void hasOption(char **args, int *mask)
{
    int opt;

    printf("getting options\n");

    // BUG: The getopt doesn't work and program never enter this while loop.
    while ((opt = getopt(1, args, "Ra")) != -1)
    {
        printf("opt: %d\n", opt);
        switch (opt)
        {
        case 'a':
            *mask |= (1 << 0);
            break;
        case 'R':
            *mask |= (1 << 1);
            break;

        default:
            printf("Invalid option.\n");
            break;
        }
    }

    printf("final mask is %d", *mask);
}

/**
 * @brief `void` si la commande saisi par l'utilisateur est myls, l'exécute
 * @param mask `int *` permet de savoir quelles options du myls sont utilisées
 */
void is_myls(int *mask)
{
    char *myls_args[4];
    myls_args[0] = "./option/myls";

    int arg_count = 1;

    printf("\nhere\n");

    if (*mask & (1 << 0))
    {
        myls_args[arg_count++] = "-a";
    }

    if (*mask & (1 << 1))
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

/**
 * @brief c'est ici que les execvp sont fait
 * @param mask `int *` permet de savoir quelles options du myls sont utilisées
 * @param args `char **` tableau contenant les mots de la commande
 */
void execute_command(int *mask, char *args[])
{
    if (strcmp(args[0], "myls") == 0)
    {
        hasOption(args, mask);
        is_myls(mask);
    }
    else if (execvp(args[0], args) == -1)
    {
        perror("Erreur d'exécution de la commande");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    char input[COMMAND_LENGTH], *command[COMMAND_LENGTH], *args[20];
    int wstatus, command_count = 0;
    int mask = 0x000;

    for (;;)
    {
        char cwd[1024];
        getcwd(cwd, sizeof(cwd));
        mask = 0x000; // Remise à 0 du mask

        char *project = strstr(cwd, "/mysh");
        if (project != NULL)
            printf(BLUE " %s > " RESET, project);
        else
            printf(GREEN " > ");

        // Lire la commande de l'utilisateur
        // int hasA = 0, hasR = 0, hasMyLs = 0;
        getcommand(input);
        if (strcmp(input, "exit") == 0)
        {
            break; // Quitter le mini-shell
        }

        tokenize(input, command, &command_count);

        pid_t child_pids[command_count];

        for (int i = 0; i < command_count; i++)
        {
            printf("%s\n", command[i]);
            child_pids[i] = fork();
            if (child_pids[i] == -1)
            {
                perror("fork");
                exit(1);
            }
            if (child_pids[i] == 0)
            {
                tokenize_space(command[i], args);
                execute_command(&mask, args);
            }
            if (waitpid(child_pids[i], &wstatus, 0) < 0)
            {
                perror("Error waitpid");
                exit(1);
            }
        }
    }
    return 0;
}