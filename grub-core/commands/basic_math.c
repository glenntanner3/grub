/* basic_math.c - basic math functions.  */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2008,2009,2010  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

//what any module needs
#include <grub/dl.h>
#include <grub/mm.h>
#include <grub/misc.h>
#include <grub/extcmd.h>
#include <grub/i18n.h>
//what my module needs
#include <grub/env.h>

GRUB_MOD_LICENSE ("GPLv3+");

struct grub_env_whitelist
{
  grub_size_t len;
  char **list;
};
typedef struct grub_env_whitelist grub_env_whitelist_t;

static int
test_whitelist_membership (const char* name,
                           const grub_env_whitelist_t* whitelist)
{
  grub_size_t i;

  for (i = 0; i < whitelist->len; i++)
    if (grub_strcmp (name, whitelist->list[i]) == 0)
      return 1;  /* found it */

  return 0;  /* not found */
}

// store value
      const char *prefix;
      int len;

      prefix = grub_env_get ("prefix");
      if (! prefix)
        {
          grub_error (GRUB_ERR_FILE_NOT_FOUND, N_("variable `%s' isn't set"), "prefix");
          return 0;
        }

      len = grub_strlen (prefix);
      buf = grub_malloc (len + 1 + sizeof (GRUB_ENVBLK_DEFCFG));
      if (! buf)
        return 0;
      filename = buf;

      grub_strcpy (filename, prefix);
      filename[len] = '/';
      grub_strcpy (filename + len + 1, GRUB_ENVBLK_DEFCFG);

// get value
const char *value;

      value = grub_env_get (args[0]);
      if (value)


static grub_err_t
grub_cmd_add ()
{
  if (argc != 2)
  return grub_error (GRUB_ERR_BAD_ARGUMENT, "Variable and value required");  
}

static grub_err_t
grub_cmd_sub ()
{
  if (argc != 2)
  return grub_error (GRUB_ERR_BAD_ARGUMENT, "Variable and value required");  
}

/* Helper for grub_cmd_load_env.  */
static int
set_var (const char *name, const char *value, void *whitelist)
{
  if (! whitelist)
    {
      grub_env_set (name, value);
      return 0;
    }

  if (test_whitelist_membership (name,
				 (const grub_env_whitelist_t *) whitelist))
    grub_env_set (name, value);

  return 0;
}

static grub_err_t
grub_cmd_load_env (grub_extcmd_context_t ctxt, int argc, char **args)
{
  struct grub_arg_list *state = ctxt->state;
  grub_file_t file;
  grub_envblk_t envblk;
  grub_env_whitelist_t whitelist;

  whitelist.len = argc;
  whitelist.list = args;

  /* state[0] is the -f flag; state[1] is the --skip-sig flag */
  file = open_envblk_file ((state[0].set) ? state[0].arg : 0, state[1].set);
  if (! file)
    return grub_errno;

  envblk = read_envblk_file (file);
  if (! envblk)
    goto fail;

  /* argc > 0 indicates caller provided a whitelist of variables to read. */
  grub_envblk_iterate (envblk, argc > 0 ? &whitelist : 0, set_var);
  grub_envblk_close (envblk);

 fail:
  grub_file_close (file);
  return grub_errno;
}

/* Print all variables in current context.  */
static int
print_var (const char *name, const char *value,
           void *hook_data __attribute__ ((unused)))
{
  grub_printf ("%s=%s\n", name, value);
  return 0;
}

static grub_extcmd_t cmd_add, cmd_sub;

GRUB_MOD_INIT(basic_math)
{
  cmd_add =
    grub_register_extcmd ("add", grub_cmd_add, 0,
			  N_("<variable_name> <value>"),
			  N_("Add value to variable."),
			  options);
  cmd_sub =
    grub_register_extcmd ("sub", grub_cmd_sub, 0,
			  N_("<variable_name> <value>"),
			  N_("Subtract value from variable."),
			  options);
}

GRUB_MOD_FINI(basic_math)
{
  grub_unregister_extcmd (cmd_add);
  grub_unregister_extcmd (cmd_sub);
}
