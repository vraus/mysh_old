#include <dirent.h>
#include <errno.h>
#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

int compare(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

const char *get_permissions_string(mode_t mode) {
  static char perms[] = "rwxrwxrwx";
  for (int i = 0; i < 9; i++) {
    if ((mode & (1 << (8 - i))) == 0) {
      perms[i] = '-';
    }
  }
  return perms;
}

void affiche(int totalBlocks, int num_files, char **file_names,
             struct stat st) {
  static const char *noms_mois[] = {"jan", "feb", "mar", "apr", "may", "jun",
                                    "jul", "aug", "sep", "oct", "nov", "dec"};
  printf("total %d\n", totalBlocks);

  for (int i = 0; i < num_files; i++) {
    if (stat(file_names[i], &st) == 0) {
      struct passwd *pw = getpwuid(st.st_uid);
      struct group *gr = getgrgid(st.st_gid);

      char nom_mois[10];  // Pour stocker le nom du mois
      // Utilisation de strftime pour extraire le nom du mois
      strftime(nom_mois, sizeof(nom_mois), "%b", localtime(&st.st_mtime));

      printf("-%s %lu ", get_permissions_string(st.st_mode), st.st_nlink);
      if (pw != NULL && gr != NULL) {
        const char *user_name = pw->pw_name;
        const char *group_name = gr->gr_name;

        printf("%s %s ", user_name, group_name);
      }
      printf("%5ld %s.  %2d %02d:%02d ", st.st_size,
             noms_mois[localtime(&st.st_mtime)->tm_mon],
             localtime(&st.st_mtime)->tm_mday, localtime(&st.st_mtime)->tm_hour,
             localtime(&st.st_mtime)->tm_min);
    }

    printf("%s\n", file_names[i]);
    free(file_names[i]);
  }
}

void files(char *cwd, int *num_files, int *max_files, char **file_names) {
  struct dirent *entry;
  struct stat st;
  int totalBlocks = 0;
  int blockSize = 1024;  // Taille d'un bloc de disque (modif 512)

  DIR *dir = opendir(cwd);

  if (dir == NULL) {
    perror("ERROR OPENDIR");
    exit(1);
  }

  if (stat(cwd, &st) == -1) {
    perror("stat");
    exit(EXIT_FAILURE);
  }

  // Parcours du répertoire courant
  while ((entry = readdir(dir)) != NULL) {
    // Ignorer les entrées commençant par un point
    if (stat(entry->d_name, &st) == 0) {
      int blocks = (st.st_size + blockSize - 1) / blockSize;
      totalBlocks += blocks;
    }
    if (entry->d_name[0] == '.') continue;

    if (*num_files == *max_files) {  // le tableau est plein
      *max_files *= 2;
      file_names = (char **)realloc(file_names, *max_files * sizeof(char *));
      if (file_names == NULL) {
        perror("Erreur d'allocation mémoire");
        exit(1);
      }
    }
    file_names[(*num_files)++] = strdup(
        entry->d_name);  // duplique la chaîne de caractères dans le tableau et
                         // alloue une nouvelle zone de mémoire
  }

  if (closedir(dir) == -1) {
    perror("ERROR CLOSEDIR");
    exit(-1);
  }

  qsort(file_names, *num_files, sizeof(char *),
        compare);  // trier par ordre aphabétique

  affiche(totalBlocks, *num_files, file_names, st);
  free(cwd);
}

int main(void) {
  char **file_names = NULL;
  int num_files = 0;
  int max_files = 10;  // Taille initiale du tableau
  file_names = (char **)malloc(max_files * sizeof(char *));
  char *cwd = (char *)malloc(1024);
  if (getcwd(cwd, 1024) == NULL) {
    perror("ERROR GETCWD");
    exit(1);
  }

  if (file_names == NULL) {
    perror("Erreur d'allocation mémoire");
    return 1;
  }

  files(cwd, &num_files, &max_files, file_names);
}