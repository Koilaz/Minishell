#include "../../minishell.h"

int validate_quote_closed(t_segment *segment_list)
{
    while (segment_list != NULL)
    {
        if (segment_list->type == ERROR)
        {
            //write(2, segment_list->type, 52);
            write(2, "minishell: syntax error: unclosed quotes detected\n", 52);
            return (0)
        }
        segment_list = segment_list->next;
    }
    return (1);
}

validate_syntax(t_segment *segment_list);
{
    if (validate_quote_closed(segment_list) == 0)
    {
        //quitter futur verif malloc
    }

    if ()


}


// 1. Tout reverifier pour les sorties avant de lancer ceci. Bien verifier que expand var redonne l'invit de commande avec :
// bash-5.2$ salut ${$sa}
// bash: ${$sa}: bad substitution
// bash-5.2$

// 2. faire les quotes pas fermé (normalement les accoaldes pas fermé auront été vérifier dans le 1)

// 3. faire ces erreurs d'opérateurs pas prit en compte 
// ; pas dans le lexer si jamais

// sujet :
// • Not interpret unclosed quotes or special characters which are not required by the
// subject such as \ (backslash) or ; (semicolon).


// write(2, "minishell: syntax error near unexpected token `;'\n", 50);


// 4.

// Redirections invalides (<< >, | |, > <, etc.)

// Quotes non fermées (", ')

// Parenthèses non fermées ou mal placées

// Séparateurs mal utilisés (;;, |||, etc.)

// Commandes vides entre des pipes (ls || wc)

// Détection de caractères inattendus





// Voici quelques suggestions d'erreurs à intégrer dans ton fichier de gestion des erreurs pour le parsing. Bien sûr, tu pourras les adapter selon tes besoins et la structure de ton lexer/parser, mais voici une base :

// Opérateurs non supportés :

// Pour « || » :

// vbnet
// minishell: syntax error: unsupported operator '||'
// Pour « && » (si tu ne les gères pas) :

// vbnet
// minishell: syntax error: unsupported operator '&&'
// Erreurs de redirection :

// Si une redirection n'a pas d'opérande (exemple : ls > ou cat <), afficher :

// vbnet
// minishell: syntax error: missing filename for redirection
// Si plusieurs redirections se suivent de manière incohérente :

// vbnet
// minishell: syntax error near unexpected token '>'
// Guillemets non fermés :

// Comme tu l'as déjà implémenté :

// vbnet
// minishell: syntax error: unclosed quotes detected
// Commandes vides ou isolées :

// Si, après le parsing, tu obtiens un token vide pour une commande (par exemple si l'utilisateur tape simplement un opérateur sans commande) :

// vbnet
// minishell: syntax error: unexpected token '|' 
// minishell: syntax error: invalid null command
// Parenthèses ou autres caractères spéciaux non supportés :

// Par exemple, pour une parenthèse ouverte sans correspondance :

// vbnet
// minishell: syntax error: mismatched parenthesis


























//-----------------------------------//

// A faire : 

//-----------------------------------//


// voir is_str_digit et ft_str_is_digit avec léo

// Norminer tout et réorganiser fichier parsing

// Début fichier erreurs: suite du projet




// Gerer le code d'erreur de CTRL C

// Gerer lorsque l'utilisateur fait une ligne vide dans le main.c





// LIBFT :

// Organisation LIBFT

// Revoir libft, que des fonctions qui return bien NULL, Verifier secu libft

// Faire nouveau get next line 


// char *get_next_line(int fd, int *status)
// {
//     *status = 0;
//     ...
//     if (malloc_fail)
//         return (*status = -1, NULL);
//     ...
// }
// // appel :
// int status;
// line = get_next_line(fd, &status);

// if (line == NULL && status == -1)
//     // erreur malloc ou read
// else if (line == NULL && status == 0)
//     // EOF


// Vérifier get next line partout


