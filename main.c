#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char** split(char* text, const char* delims){
    char **result = malloc(1 * sizeof(char *));
    int size = 1;

    char* the_rest = text;

    char *token = strtok_r(the_rest, delims, &the_rest);
    *result = token;
    while (token)
    {
        size++;
        result = realloc(result, size * sizeof(char *));
        token = strtok_r(the_rest, delims, &the_rest);
        result[size - 1] = token;
    }
    return result;
}

int isEmpty(const char *str) /*The functions returns wether a line is empty or not.*/
{
    char ch;

    do
    {
        ch = *(str++); /*the present char of the string string*/

        if (ch != ' ' && ch != '\t' && ch != '\n' && ch != '\r' && ch != '\0')
            return 0;

    } while (ch != '\0');

    return 1;
}

void remove_newline(char *line)
{
    unsigned new_line = strlen(line) - 1;
    if (line[new_line] == '\n')
    {
        line[new_line] = '\0';
    }
}

void remove_empty_lines(char *file_name)
{
    FILE *f = fopen(file_name, "r");          /* Source file we are reading from */
    FILE *temp_file = fopen("temp.txt", "w"); /* Temporary file we are writing to */

    if (!f || !temp_file)
    {
        printf("Error opening the file!");
        exit(1);
    }

    char buffer[1000];
    while (fgets(buffer, 1000, f) != NULL)
    {
        if (!isEmpty(buffer))
            fputs(buffer, temp_file); /* Writing the line to the temporary file if it's not empty*/
    }

    /* Closing the files */
    fclose(f);
    fclose(temp_file);

    /* Renaming the temp file to source file's name */
    remove(file_name);
    rename("temp.txt", file_name);
}

char **get_text(char *file_name)
{

    remove_empty_lines(file_name);             /* Cleaning the input file from the empty lines */
    FILE *source_file = fopen(file_name, "r"); /* Trying to open the file */
    if (!source_file)
    {
        printf("%s\n", "Error while opening the file!");
        exit(1);
    }

    char **document = NULL;
    int paragraph_index = 0;

    char buffer[1000]; /* Temporary buffer */
    while (fgets(buffer, 1000, source_file))
    {
        document = realloc(document, (paragraph_index + 1) * sizeof(char *)); /* Allocating an array with the number of the existing paragraph's + 1(in the first iteration 1) */
        if (!document)
        {
            printf("Memory can't be allocated!\n");
            exit(2);
        }
        remove_newline(buffer);                                            /* Clearing the buffer(line) from the newline character */
        unsigned buffer_len = strlen(buffer);                              /* The length of the buffer */
        char *paragraph = (char *)malloc((buffer_len + 1) * sizeof(char)); /* Dynamically allocating memory for the line(paragraph) */
        strcpy(paragraph, buffer);
        document[paragraph_index] = paragraph;
        paragraph_index++;
    }

    document = realloc(document, (paragraph_index + 1) * sizeof(char *));
    document[paragraph_index] = NULL; /* The array with a NULL pointer! */

    fclose(source_file); /* Closing the source file */

    /*
    for (int i = 0; i < paragraph_index ; ++i) {
        printf("%d.element: %s\n", i, document[i]);
    }*/

    return document;
}

void text_stat(char **document)
{

    int par_num = 0, sent_num = 0, word_num = 0;

    while (document[par_num] != NULL)
    {
        char **sentences = split(document[par_num], ".!?");
        int j = 0;
        while (sentences[j])
        {
            /* Counting the words*/
            char **words = split(sentences[j], " ");
            int k = 0;
            while (words[k])
            {
                word_num++;
                k++;
            }
            sent_num++;
            j++;
        }
        par_num++;
    }

    /*Freeing up the memory used by the 2D document array*/
    for (int i = 0; i < par_num; i++)
    {
        free(document[i]);
    }
    free(document);

    printf("Number of paragraphs: %d\n", par_num);
    printf("Number of sentences: %d\n", sent_num);
    printf("Number of words: %d\n", word_num);
}

int main()
{
    
    text_stat(get_text("apple.txt"));
    
    
    return 0;
}

