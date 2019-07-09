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
#include <stdlib.h>

GRUB_MOD_LICENSE ("GPLv3+");

static grub_err_t
grub_cmd_add (grub_command_t cmd __attribute__ ((unused)), int argc, char **args)
{
  int i, ret, value_int, arg_int;
  const char *value;
  char buf[sizeof ("XXXXXXXX")];
  
  if (argc != 2)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Variable and value required"));
  if (!grub_isdigit(args[1]) || !args[1] >= 0)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Value must be an interger"));
  
  value = grub_env_get (args[0]);
  if (!value)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Variable must be defined and a digit"));
  for (i=0;i<grub_strlen(value);i++)
    if (!grub_isdigit(value[i]))
      return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Variable must be defined and a digit"));
    
  var_value=atoi(value);
  arg_int=atoi(args[1]);
  
  ret=(int)value+(int)args[1];
  grub_snprintf (buf, sizeof (buf), "%d", ret);
  
  grub_env_set (args[0], buf);
}

static grub_err_t
grub_cmd_sub (grub_command_t cmd __attribute__ ((unused)), int argc, char **args)
{
  int i, ret, value_int, arg_int;
  const char *value;
  char buf[sizeof ("XXXXXXXX")];
  
  if (argc != 2)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Variable and value required"));
  if (!grub_isdigit(args[1]) || !args[1] >= 0)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Value must be an interger"));
  
  value = grub_env_get (args[0]);
  if (!value)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Variable must be defined and a digit"));
  for (i=0;i<grub_strlen(value);i++)
    if (!grub_isdigit(value[i]))
      return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Variable must be defined and a digit"));
  
  var_value=atoi(value);
  arg_int=atoi(args[1]);
  
  if (arg_int > var_value)
    return grub_error (GRUB_ERR_BAD_ARGUMENT, N_("Result may not be negitive"));
  
  ret=var_value-arg_int;
  grub_snprintf (buf, sizeof (buf), "%d", ret);
  
  grub_env_set (args[0], buf);
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
