/*
** EPITECH PROJECT, 2025
** libmy library project
** File description:
** this file countain search in string functions like my_is_in_str and
** my_index_of_str and nbr_occurence_of
*/

#include <stdbool.h>

// bool my_is_in_str(char c, char *str)
// {
//     for (int i = 0; i < my_strlen(str); ++i) {
//         if (str[i] == c)
//             return true;
//     }
//     return false;
// }
//
// int my_index_of_str(char c, char *str)
// {
//     for (int i = 0; i < my_strlen(str); ++i) {
//         if (str[i] == c)
//             return i;
//     }
//     return -1;
// }
//
int my_nbr_occurences_in_str(char c, const char *str)
{
    int nbr_occ = 0;

    for (int i = 0; str[i] != '\0'; ++i)
        if (str[i] == c)
            nbr_occ += 1;
    return nbr_occ;
}
