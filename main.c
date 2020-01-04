#include "monty.h"

/**
 * main - a
 * @argc: a
 * @argv: a
 * Return: a
 */
int main(int argc, char **argv)
{
	(void)data;

	if (argc != 2)
	{
		fprintf(stderr, "USAGE: monty file\n");
		exit(EXIT_FAILURE);
	}

	montyFile(argv);
	return (0);
}

/**
 * montyFile - a
 * @argv: a
 * Return: a
 */
void montyFile(char **argv)
{
	const char *filename = argv[1];
	FILE *fp;
	char *buffer = NULL, **tokens = NULL;
	size_t len = 0;
	ssize_t read;
	unsigned int line_number = 1;
	stack_t *stack = NULL;

	fp = fopen(filename, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Error: Can't open file %s\n", filename);
		exit(EXIT_FAILURE);
	}

	while ((read = getline(&buffer, &len, fp)) != -1)
	{
		/* printf("Buffer actual: %s\n", buffer); */
		montyTokens(&buffer, &tokens, read);
		/* printf("Buffer after montyTokens: %s\n", buffer); */
		if (tokens != NULL)
			montyInit(&stack, &tokens, line_number);
		/* printf("tokens %u: **%s**\n", i, tokens[i]); */
		freeTokens(&tokens);
		line_number++;
	}

	fclose(fp);

	if (buffer != NULL)
		free(buffer);
	freeStack(stack);
}

/**
 * montyInit - a
 * @stack: a
 * @tokens: a
 * @line_number: a
 * Return: a
 */
void montyInit(stack_t **stack, char ***tokens, unsigned int line_number)
{
	void (*op_func)(stack_t **stack, unsigned int line_number);

	unsigned int i;

	if ((strcmp((*tokens)[0], "nop") == 0))
		return;

	if (strcmp((*tokens)[0], "push") == 0)
	{
		for (i = 0; (*tokens)[i] != NULL; i++)
		{
			if (i == 1)
			{
				isNumber((*tokens)[i], line_number);
				/* data = atoi((*tokens)[i]); */
				/* printf("DATAAAA *****%d******\n",data); */
			}
		}
		if (i == 1 && (*tokens)[i] == NULL)
		{
			fprintf(stderr, "L%u: usage: push integer\n", line_number);
			exit(EXIT_FAILURE);
		}
		/* printf("value of i: %u\n", i); */
	}
	op_func = getOpcode((*tokens)[0]);

	if (op_func != NULL)
		op_func(stack, line_number);
	else
	{
		fprintf(stderr, "L%u: unknown instruction %s\n", line_number, (*tokens)[0]);
		exit(EXIT_FAILURE);
	}

}

/**
 * isCharDigit - a
 * @c: a
 * Return: a
 */
int isCharDigit(int c)
{
	if ((c >= '0' && c <= '9') || (c == '-'))
		return (1);
	return (0);
}

/**
 * isNumber - a
 * @pushData: a
 * @line_number: a
 * Return: a
 */
void isNumber(char *pushData, unsigned int line_number)
{
	unsigned int i = 0;

	while (pushData[i] != '\0')
	{
		if (i == 0 && pushData[i] == '-')
			;
		else if (isCharDigit(pushData[i]) == 0)
		{
			fprintf(stderr, "L%u: usage: push integer\n", line_number);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	data = atoi(pushData);
}

/**
 * freeTokens - a
 * @tokens: a
 * Return: a
 */
void freeTokens(char ***tokens)
{
	if (*tokens != NULL)
	{
		free(*tokens);
		*tokens = NULL;
	}
}

/**
 * montyTokens - a
 * @buffer: a
 * @tokens: a
 * @read: a
 * Return:
 */
void montyTokens(char **buffer, char ***tokens, ssize_t read)
{
	unsigned int countToken, i;

	/* printf("ENTRO EN MONTYTOKENS\n"); */
	if (read > 0)
	{
		i = 0;
		while ((*buffer)[i] == ' ' || (*buffer)[i] == '\t')
		{
			if ((*buffer)[i + 1] == '\n')
				return;
			i++;
		}
	}

	if (**buffer != '\n')
	{
		/* printf("ENTRO EN BUFFER\n"); */
		replaceNewLine(buffer);
		countToken = lenTokens(read, buffer);
		/* printf("countToken %u\n", countToken); */
		if (countToken > 3)
			countToken = 3;
		processTokens(tokens, buffer, countToken);
	}
	/* printf("SALIO DE MONTYTOKENS\n"); */
}

/**
 * lenTokens - Get the number of words separated by a delimiter
 * @lenReaded: n
 * @buffer: n
 * Return: (unsigned int) of words in the buffer
 */
unsigned int lenTokens(ssize_t lenReaded, char **buffer)
{
	char *tempToken = NULL, *copyBuffer = NULL;
	int i;
	char *delim = " \n\t";

	copyBuffer = malloc(sizeof(char) * lenReaded);
	strcpy(copyBuffer, *buffer);
	tempToken = strtok(copyBuffer, delim);
	for (i = 0; tempToken != NULL; i++)
		tempToken = strtok(NULL, delim);
	i++; /* One more to save NULL */
	free(copyBuffer);
	return (i);
}

/**
 * replaceNewLine - Replace the new line in the buffer
 * by a null character
 * @buffer: n
 * Return: nothing
 */
void replaceNewLine(char **buffer)
{
	int i;

	for (i = 0; (*buffer)[i] != '\0'; i++)
		continue;
	(*buffer)[i - 1] = '\0'; /* Replace '\n' by '\0' */
}

/**
 * processTokens - Get all of the strings separated by a delimiter in
 * an array of strings
 * @tokens: n
 * @buffer: n
 * @countToken: n
 *
 * Return: nothing
 */
void processTokens(char ***tokens, char **buffer, unsigned int countToken)
{
	char *token = NULL;
	unsigned int i;
	char *delim = " \n\t";
	/* char *opcodes[] = {"push", "pall", NULL}; */

	/* printf("countToken en processTokens %u\n", countToken); */
	token = strtok(*buffer, delim);

	if (strcmp(token, "push") == 0)
		countToken = 3;
	else
		countToken = 2;
	*tokens = malloc(sizeof(char *) * countToken);
	for (i = 0; token != NULL && i <= countToken - 2; i++)
	{
		/* printf("VALOR DE I, %d\n", i); */
		(*tokens)[i] = token;
		token = strtok(NULL, delim);
	}
	/* (*tokens)[i] = token; */
	(*tokens)[i] = NULL;
}


/**
 * freeStack - Free the stack
 * @stack: Head of the list
 *
 * Return: nothing
 */
void freeStack(stack_t *stack)
{
	stack_t *temp;

	if (stack == NULL)
		return;

	while (stack != NULL)
	{
		temp = stack;
		stack = temp->next;
		free(temp);
	}
}
