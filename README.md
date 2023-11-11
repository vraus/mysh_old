# MySH - M1 SE

> **Authors**: *Ugo Courche* & *Hadrien Belleville*
>
> **Date of the first commit**: *October 11, 2023*

## Table of Content

1. [How to Use](#how-to-use)

    1. [V-0.1.111023](#v-01111023)

1. [Workspace Organization](#workspace-organization)

## How to Use

> How to read the versions name: V-X.Y.Z
>
> + X: This indicates the major version. It is incremented for significant changes that may introduce backward-incompatible features.
> + Y: This is the minor version. It is typically incremented for smaller, backward-compatible changes or feature additions.
> + Z: This represents the patch version. It is used for bug fixes or small improvements that are backward-compatible. In our case this will always be the date of the first commit of the patch version.

### V-0.1.111023

In order to use the `MySH` at the current state, you only have to run the following command.

```bash
make && ./main
```

You will then be able to access the prompt of the mysh. You don't have a lot of usable commands at this state of the project. Only the most basic UNIX commands you can think of, and the `myls` command that is supposed to work as a normal `ls` command.

## Workspace Organization

### Plugins

#### Todo Tree

> from Gruntfuggly, [zaonce.com](https://www.zaonce.com/cgi-bin/blog.pl)

We use this extension available on the marketplace of VSCode to organize the workspace with TODO tags.

Simple documentation on how to use TODO comment and configure in the workspace can be found [here](https://thomasventurini.com/articles/the-best-way-to-work-with-todos-in-vscode/).

Also interesting if we want to use icons, the list of all supported icons can be found [here](https://microsoft.github.io/vscode-codicons/dist/codicon.html).
