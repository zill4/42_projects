/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LS.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcrisp <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/03/25 20:11:45 by jcrisp            #+#    #+#             */
/*   Updated: 2019/03/25 21:11:59 by jcrisp           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ls.h"



int main(int argc, char **argv) 
{ 
    t_dlist *head;
    t_dlist *tmp;
    int args;
 	struct stat buf;
    t_spec *spec;

    args = 1;
    head = malloc(sizeof(t_dlist));
    head->next = NULL;
    head->name = NULL;
    spec = malloc(sizeof(t_spec));
    spec->flags = 0;

    while (args < argc)
    {
        if (argv[args][0] == '-')
            set_flags(argv[args], spec);
        else
        {
            lstat(argv[args], &buf);
            if (S_ISDIR(buf.st_mode))
            {
                if (head->name == NULL)
                {
                    head->name = strdup(argv[args]);
                    memcpy(&(head->buf), &buf, sizeof(buf));
                }
                else
                    append(&head, argv[args], buf);
                next_dir(argv[args], head, spec);
            }
            else
            {
                if (S_ISREG(buf.st_mode))
                {
                    spec->flags |= F_BIT;
                    if (head->name == NULL)
                    {
                        memcpy(&(head->buf), &buf, sizeof(buf));
                        append(&head->sub, argv[args], buf);
                    }
                    else
                        append(&head->sub, argv[args], buf);
                }
                else
                {
                    printf("%s no file or directory", argv[args]);
                    return (-1);
                }
            }
            
        }
        if (spec->flags & ERROR)
        {
            printf("ft_ls: illegal option -- %s\nusage: ls [-ABCFGHLOPRSTUWabcdefghiklmnopqrstuwx1] [file ...]\n", argv[args]);
            return (-1);
        }
        args++;
    }
    if (argc <= 2)
    {
        lstat(".", &buf);
        head->name = strdup(".");
        head->next = NULL;
        memcpy(&(head->buf), &buf, sizeof(buf));
        next_dir(head->name, head, spec);
    }
    if (!(spec->flags & F_BIT))
        loadSubs(head, spec);
    else
    {
        if (spec->flags & T_BIT)
        {
            if (spec->flags & T_BIT && spec->flags & LOWER_R_BIT)
                sort_list(head->sub, s_byTimeR); 
            else
                sort_list(head->sub, s_byTime);
        }
        else
        {
            if (spec->flags & LOWER_R_BIT)
                sort_list(head->sub, s_byNameR);
            else
                sort_list(head->sub, s_byName);
        }
        print_dir(head, spec);
        free(spec);
        ft_strdel(&head->name);
        ft_memdel((void **)&head);
        delList(head);
        return (0);
    }
    print_dir(head, spec);
    tmp = head;
    head = head->next;
    if (spec->flags & T_BIT)
    {
        if (spec->flags & T_BIT && spec->flags & LOWER_R_BIT)
        {
            sort_list(head, s_byTimeR);
            printList(head, spec);

        }
        else
        {
            sort_list(head, s_byTime);
            printListR(head, spec);
        }
    }
    else
    {
        if (spec->flags & LOWER_R_BIT)
        {
            sort_list(head, s_byName);
            printListR(head, spec);

        }
        else
        {
            sort_list(head, s_byNameR);
            printListR(head, spec);
        }
    }
    free(spec);
    delList(tmp);
    return 0; 
} 
