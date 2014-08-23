/**
 * setpgrep — run a program in a new process group
 * Copyright © 2014  Mattias Andrée (maandree@member.fsf.org)
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>


int main(int argc, char* argv[])
{
  int switch_print = 0;
  int switch_export = 0;
  int switch_help = 0;
  pid_t group;
  char** args;
  int off;
  
  for (off = 1; off < argc; off++)
    if (!strcmp(argv[off], "--print"))
      switch_print = 1;
    else if (!strcmp(argv[off], "--export"))
      {
	switch_export = ++off;
	if (off == argc)
	  goto help;
      }
    else if (!strcmp(argv[off], "--help"))
      {
	switch_help = 1;
	goto help;
      }
    else if (!strcmp(argv[off], "--"))
      {
	off++;
	break;
      }
    else if (argv[off][0] != '-')
      break;
    else
      goto help;
  
  group = setpgid(0, 0);
  if (group == (pid_t)-1)
    goto fail;
  group = getpgrp();
  
  args = malloc((size_t)(argc - off + 1) * sizeof(char*));
  if (args == NULL)
    goto fail;
  
  memcpy(args, argv + off, (size_t)(argc - off) * sizeof(char*));
  args[argc - off] = NULL;
  
  if (switch_print)
    if (printf("%ji\n", (intmax_t)group) < 0)
      goto fail;
  
  if (switch_export)
    {
      char group_str[3 * sizeof(pid_t) + 1];
      if (sprintf(group_str, "%ji", (intmax_t)group) < 0)
	goto fail;
      if (setenv(argv[switch_export], group_str, 1) < 0)
	goto fail;
    }
  
  execvp(args[0], args);
  
 fail:
  perror(*argv);
  return 1;
  
 help:
  printf("USAGE: %s [options] [--] [command...]\n", *argv);
  printf("\n");
  printf("    --print       Print the process group to stdout.\n");
  printf("    --export VAR  Export the process group to the ENV environment variable.\n");
  printf("\n");
  return !switch_help;
}

