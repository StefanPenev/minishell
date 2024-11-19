/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_export.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anilchen <anilchen@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 13:14:41 by anilchen          #+#    #+#             */
/*   Updated: 2024/11/19 12:57:52 by anilchen         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "process.h"

// function for sorting linked blist
void	sort_t_process_list(t_env **head)
{
	int		swapped;
	t_env	*ptr1;
	t_env	*last_ptr;
	char	*temp_key;
	char	*temp_value;

	last_ptr = NULL;
	if (*head == NULL)
		return ;
	swapped = 1;
	while (swapped)
	{
		swapped = 0;
		ptr1 = *head;
		while (ptr1->next != last_ptr)
		{
			if (ptr1->key != NULL && ptr1->next->key != NULL
				&& ft_strcmp(ptr1->key, ptr1->next->key) > 0)
			{
				temp_key = ptr1->key;
				temp_value = ptr1->value;
				ptr1->key = ptr1->next->key;
				ptr1->value = ptr1->next->value;
				ptr1->next->key = temp_key;
				ptr1->next->value = temp_value;
				swapped = 1;
			}
			ptr1 = ptr1->next;
		}
		last_ptr = ptr1;
	}
}

// create copy of node to create full copy of list
t_env	*duplicate_t_process_node(t_env *proc)
{
	t_env	*new_proc;

	if (proc == NULL)
		return (NULL);
	new_proc = malloc(sizeof(t_env));
	if (new_proc == NULL)
		return (NULL);
	new_proc->key = ft_strdup(proc->key);
	if (new_proc->key == NULL)
	{
		free(new_proc);
		return (NULL);
	}
	if (proc->value != NULL)
	{
		new_proc->value = ft_strdup(proc->value);
		if (new_proc->value == NULL)
		{
			free(new_proc->key);
			free(new_proc);
			return (NULL);
		}
	}
	else
	{
		new_proc->value = NULL;
	}
	new_proc->next = NULL; // Инициализируем next
	return (new_proc);
}

// create copy of list to sort it
t_env	*duplicate_t_process_list(t_env *list)
{
	t_env	*new_list;
	t_env	*new_node;
	t_env	*last;

	new_list = NULL;
	last = NULL;
	while (list != NULL)
	{
		new_node = duplicate_t_process_node(list);
		if (new_list == NULL)
		{
			new_list = new_node;
		}
		else
		{
			last->next = new_node;
		}
		last = new_node;
		list = list->next;
	}
	return (new_list);
}

// export without any arguments just sort anv_copy in alphabet order and show this list in specific format
int	export_no_args(t_env *env_copy)
{
	t_env	*sorted_list;
	t_env	*current;

	sorted_list = NULL;
	sorted_list = duplicate_t_process_list(env_copy);
	sort_t_process_list(&sorted_list);
	current = sorted_list;
	while (current != NULL)
	{
		if (current->key != NULL && current->value != NULL)
			printf("declare -x %s=\"%s\"\n", current->key, current->value);
		else if (current->key != NULL)
			printf("declare -x %s\n", current->key);
		current = current->next;
	}
	free_env(&sorted_list);
	// minishell_lstclear(&sorted_list);
	return (EXIT_SUCCESS);
}

// ignore quads
char	*remove_quotes(char *str)
{
	size_t	len;
	char	*cleaned;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
	{
		cleaned = ft_strndup(str + 1, len - 2);
		free(str);
		return (cleaned);
	}
	// printf("hello Im remove_quotes and Im working\n");
	return (str);
}

// if we have found variable in the list - replase value, else - create new node
int	export_with_args(char *key, char *value, t_env *env_copy)
{
	t_env	*new_var;
	t_env	*current;
	char	*new_value;

	// char	*key;
	// char	*value;
	// char	**var_to_add;
	int i = 1; // splitted[0] is commandcd ..
	// value = NULL;
	current = env_copy;
	// printf("Debug: Processing argument: %s\n", arg); // Debug message
	// Split the argument by '='
	// var_to_add = ft_split(arg, '=');
	// Debug: Show split parts
	// for (int j = 0; var_to_add[j] != NULL; j++)
	// {
	// 	printf("Debug: Split part[%d]: %s\n", j, var_to_add[j]);
	// }
	// key = ft_strdup(var_to_add[0]);
	// printf("Debug: Key: %s\n", key); // Debug message
	// if (var_to_add[1] != NULL)
	// {
	// 	value = ft_strdup(var_to_add[1]);
	// 	printf("Debug: Value: %s\n", value); // Debug message
	// }
	// else
	// {
	// 	printf("Debug: No value provided, using NULL\n"); // Debug message
	// }
	current = env_copy;
	while (current != NULL)
	{
		// printf("Debug: Comparing key '%s' with existing key '%s'\n", key,
		//	current->key); // Debug message
		if (ft_strcmp(key, current->key) == 0)
		{
			new_value = remove_quotes(value);
			// printf("Debug: Key '%s' found, updating value to '%s'\n", key,
			//		new_value); // Debug message
			my_setenv(key, value, env_copy);
			// free(key);
			// if (value)
			// {
			// 	free(value);
			// }
			// return (EXIT_SUCCESS);
			break ;
		}
		current = current->next;
	}
	// printf("Debug: Key '%s' not found,
	// 	adding new variable with value '%s'\n",
	// //	key, value ? value : "NULL"); // Debug message
	new_value = remove_quotes(value);
	new_var = minishell_lstnew(key, new_value);
	minishell_lstadd_back(&env_copy, new_var);
	// free(key);
	// if (value)
	// {
	// 	free(value);
	// }
	return (EXIT_SUCCESS);
}

int	is_valid(char *arg)
{
	int	i;

	if (!arg || arg[0] == '\0')
	{
		return (0);
	}
	if (!((arg[0] >= 'A' && arg[0] <= 'Z') || (arg[0] >= 'a' && arg[0] <= 'z')
			|| arg[0] == '_'))
	{
		return (0);
	}
	i = 1;
	while (arg[i] != '\0')
	{
		if (!((arg[i] >= 'A' && arg[i] <= 'Z') || (arg[i] >= 'a'
					&& arg[i] <= 'z') || (arg[i] >= '0' && arg[i] <= '9')
				|| arg[i] == '_'))
		{
			return (0);
		}
		i++;
	}
	return (1);
}

// main export
int	minishell_export(char **splitted, t_env *env_copy, t_process *process)
{
	int		i;
	char	**var_to_add;
	char	*key;
	char	*value;

	value = NULL;
	i = 1;
	if (splitted[1] == NULL)
	{
		export_no_args(env_copy);
	}
	else
	{
		while (splitted[i] != NULL)
		{
			if (splitted[i][0] == '=')
			{
				key = ft_strdup("=");
			}
			else
			{
				var_to_add = ft_split(splitted[i], '=');
				// for (int j = 0; var_to_add[j] != NULL; j++)
				// {
				// 	printf("Debug: Split part[%d]: %s\n", j, var_to_add[j]);
				// }
				key = ft_strdup(var_to_add[0]);
				// printf("Debug: Key: %s\n", key); // Debug message
				if (var_to_add[1] != NULL)
				{
					value = ft_strdup(var_to_add[1]);
					// printf("Debug: Value: %s\n", value); // Debug message
				}
			}
			if (is_valid(key))
			{
				cleanup(var_to_add);
				export_with_args(key, value, env_copy);
			}
			else
			{
				write(2, "minishell: export: ", 19);
				write(2, splitted[i], ft_strlen(splitted[i]));
				write(2, ": not a valid identifier\n", 25);
				set_exit_status(process, 1);
				return (EXIT_FAILURE);
			}
			i++;
		}
	}
	set_exit_status(process, 0);
	return (EXIT_SUCCESS);
}

// void	sort_t_process_list(t_process *list)
// {
// 	t_process	*tmp;
// 	int			count;
// 	int			i;
// 	int			j;

// 	if (list == NULL)
// 		return ;
// 	count = 0;
// 	while (list[count] != NULL)
// 		count++;
// 	i = 0;
// 	while (i < count - 1)
// 	{
// 		j = 0;
// 		while (j < count - i - 1)
// 		{
// 			if (ft_strcmp(list[j]->key, list[j + 1]->key) > 0)
// 			{
// 				tmp = list[j];
// 				list[j] = list[j + 1];
// 				list[j + 1] = tmp;
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// }

// void sort_t_process_list(t_process **head)
// {
//     int swapped;
//     t_process *ptr1;
//     t_process *last_ptr = NULL;

//     if (*head == NULL)
//         return ;

//     do
//     {
//         swapped = 0;
//         ptr1 = *head;

//         while (ptr1->next != last_ptr)
//         {
//             if (ft_strcmp(ptr1->key, ptr1->next->key) > 0)
//             {
//                 char *temp_key = ptr1->key;
//                 char *temp_value = ptr1->value;

//                 ptr1->key = ptr1->next->key;
//                 ptr1->value = ptr1->next->value;

//                 ptr1->next->key = temp_key;
//                 ptr1->next->value = temp_value;

//                 swapped = 1;
//             }
//             ptr1 = ptr1->next;
//         }
//         last_ptr = ptr1;
//     } while (swapped);
// }

// t_process	*duplicate_t_process_node(t_process *proc)
// {
// 	t_process	*new_proc;

// 	if (proc == NULL)
// 		return (NULL);
// 	new_proc = malloc(sizeof(t_process));
// 	if (new_proc == NULL)
// 		return (NULL);
// 	new_proc->key = ft_strdup(proc->key);
// 	if (proc->value != NULL)
// 	{
// 		new_proc->value = ft_strdup(proc->value);
// 	}
// 	else
// 	{
// 		new_proc->value = NULL;
// 	}
// 	return (new_proc);
// }

// t_process	**duplicate_t_process_list(t_process **list)
// {
// 	int			i;
// 	int			count;
// 	t_process	*new_list;

// 	i = 0;
// 	if (list == NULL)
// 		return (NULL);
// 	count = 0;
// 	while (list[count] != NULL)
// 	{
// 		count++;
// 	}
// 	new_list = malloc(sizeof(t_process *) * (count + 1));
// 	while (i < count)
// 	{
// 		new_list[i] = duplicate_t_process_line(list[i]);
// 		i++;
// 	}
// 	new_list[count] = NULL;
// 	return (new_list);
// }

// int	export_no_args(t_process *env_copy)
// {
// 	int			i;
// 	t_process	*env_dup;

// 	i = 0;
// 	env_dup = duplicate_t_process_list(env_copy);
// 	sort_t_process_list(env_dup);
// 	while (env_dup[i] != NULL)
// 	{
// 		printf("declare -x %s=\"%s\"\n", env_dup[i]->key, env_dup[i]->value);
// 		i++;
// 	}
//    minishell_lstclear((t_process **)&env_dup);
// 	//free_env_copy(env_dup);
// 	return (EXIT_SUCCESS);
// }

// int	export_with_args(char **splitted, t_procesexport s *env_copy)
// {
// 	t_process	*new_var;
// 	int			i;
// 	char		**var_to_add;
// 	char		*key;
// 	char		*value;

// 	value = NULL;
// 	i = 0;
// 	var_to_add = ft_split(splitted[1], '=');
// 	key = var_to_add[0];
// 	if (var_to_add[1] != NULL)
// 	{
// 		value = var_to_add[1];
// 	}
// 	while (env_copy[i] != NULL)
// 	{
// 		if (ft_strcmp(key, env_copy[i]->key) == 0)
// 		{
// 			value = remove_quotes(value);
// 			my_setenv(key, value, env_copy);
// 			break ;
// 		}
// 		i++;
// 	}
// 	if (env_copy[i] == NULL)
// 	{
// 		new_var = minishell_lstnew(key, value);
// 		// add_to_env_array(env_copy, new_var);
// 		minishell_lstadd_back(env_copy, new_var);
// 	}
// 	return (0);
// }

// int	export_with_args(char **splitted, t_env *env_copy)
// {
// 	t_env	*new_var;
// 	t_env	*current;
// 	char	**var_to_add;
// 	char	*key;
// 	char	*value;
// 	char	*new_value;

// 	int i = 1; // splitted[0] is command
// 	value = NULL;
// 	current = env_copy;
// 	while (splitted[i] != NULL)
// 	{
// 		// ZVAR="123", split by =
// 		var_to_add = ft_split(splitted[i], '=');
// 		// debug part
// 		for (int i = 0; var_to_add[i] != NULL; i++)
// 		{
// 			printf("Debug: %s\n", var_to_add[i]);
// 		}
// 		// end
// 		key = ft_strdup(var_to_add[0]);
// 		if (var_to_add[1] != NULL)
// 		{
// 			value = ft_strdup(var_to_add[1]);
// 		}
// 		cleanup(var_to_add);
// 		current = env_copy;
// 		while (current != NULL)
// 		{
// 			if (ft_strcmp(key, current->key) == 0)
// 			{
// 				new_value = remove_quotes(value);
// 				my_setenv(key, value, env_copy);
// 				// free(key);
// 				// if (value)
// 				// {
// 				// 	free(value);
// 				// }
// 				return (EXIT_SUCCESS);
// 			}
// 			current = current->next;
// 		}
// 		new_value = remove_quotes(value);
// 		new_var = minishell_lstnew(key, new_value);
// 		minishell_lstadd_back(&env_copy, new_var);
// 		// free(key);
// 		// if (value)
// 		// {
// 		// 	free(value);
// 		// }
// 		i++;
// 	}
// 	return (EXIT_SUCCESS);
// }
