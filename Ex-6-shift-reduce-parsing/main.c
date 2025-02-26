#include <stdio.h>
#include <string.h>

struct ProductionRule
{
    char left[10];
    char right[10];
};

int main()
{
    char input[50], stack[50] = "$", temp[50], *token1, *token2, *substring;
    int i = 0, j, stack_length, substring_length, stack_top, rule_count = 0;
    struct ProductionRule rules[10];

    // User input for the number of production rules
    printf("\nEnter the number of production rules: ");
    scanf("%d", &rule_count);

    // User input for each production rule in the form 'left->right'
    printf("\nEnter the production rules (in the form 'left->right'): \n");
    for (j = 0; j < rule_count; j++)
    {
        scanf("%s", temp);
        token1 = strtok(temp, "->");
        token2 = strtok(NULL, "->");
        strcpy(rules[j].left, token1);
        strcpy(rules[j].right, token2);
    }

    // User input for the input string
    printf("\nEnter the input string: ");
    scanf("%s", input);
    strcat(input, "$"); // Append '$' to indicate end of input

    printf("\n----------------------------------------------------------\n");
    printf("| %-12s | %-17s | %-20s |\n", "Stack", "Input Buffer", "Parsing Action");
    printf("----------------------------------------------------------\n");

    while (1)
    {
        // If there are more characters in the input string, shift the next token to the stack
        if (i < strlen(input) - 1) // Ignore the final '$' while shifting
        {
            if (strncmp(&input[i], "id", 2) == 0) // Check for "id" as a single token
            {
                strcat(stack, "id");
                i += 2; // Move index by 2 since "id" is a single token
            }
            else
            {
                char ch[2] = {input[i], '\0'};
                strcat(stack, ch);
                i++;
            }

            printf("| %-12s | %-17s | %-20s |\n", stack, &input[i], "Shift");
        }

        // Iterate through the production rules for possible reductions
        int reduced = 0; // Track if a reduction happened
        for (j = 0; j < rule_count; j++)
        {
            // Check if the right-hand side of a production rule is in the stack
            substring = strstr(stack, rules[j].right);
            if (substring != NULL)
            {
                // Replace the matched substring with the left-hand side of the production rule
                stack_length = strlen(stack);
                substring_length = strlen(rules[j].right);
                stack_top = stack_length - substring_length;
                stack[stack_top] = '\0';
                strcat(stack, rules[j].left);

                printf("| %-12s | %-17s | Reduce %-10s->%-5s |\n", stack, &input[i], rules[j].left, rules[j].right);
                reduced = 1; // Mark that a reduction occurred
                j = -1; // Restart checking from the first production rule
            }
        }

        // If no reduction happened and input is fully read, break out
        if (!reduced && i == strlen(input) - 1)
            break;
    }

    // Final check for acceptance
    if (strcmp(stack, "$S") == 0 && i == strlen(input) - 1)
    {
        printf("| %-12s | %-17s | %-20s |\n", stack, "$", "Accept");
        printf("----------------------------------------------------------\n");
        printf("\nAccepted\n");
    }
    else
    {
        printf("\nNot Accepted\n");
    }

    return 0;
}