#include <locale.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#include "grams.h"

size_t grams(wchar_t *string, const wchar_t *delimiters, const size_t n_min, const size_t n_max) {
	size_t n, t;
	ptrdiff_t i, i_max, j, j_max;
	wchar_t *state = NULL;
	wchar_t **tokens = NULL;
	const size_t s_len = string == NULL ? 0 : wcslen(string);
	const size_t t_max = s_len / 2 + 1;
	if (string == NULL || delimiters == NULL)
		return (size_t)-1;
	if (!(1 <= n_min && n_min <= n_max))
		return (size_t)-1;
	if (s_len == 0 || n_min > t_max)
		return 0;
	tokens = (wchar_t**) malloc(t_max * sizeof(wchar_t**));
	tokens[t = 0] = wcstok(string, delimiters, &state);
	while (tokens[t])
		tokens[++t] = wcstok(NULL, delimiters, &state);
	tokens = (wchar_t**) realloc(tokens, t * sizeof(wchar_t**));
	for (n = n_min; n <= n_max; n++) {
		for (i = 0, i_max = t - n + 1; i < i_max; i++) {
			for (j = i, j_max = i + n; j < j_max; ) {
				fputws(tokens[j], stdout);
				fputwc(++j < j_max ? *delimiters : '\n', stdout);
			}
		}
	}
	free(tokens);
	return 0;
}

ssize_t wcconv(char **mbs, wchar_t **wcs){   
	size_t n;
	if (*mbs == NULL)
		return (ssize_t)-1;
	n = mbstowcs(NULL, *mbs, 0);
	if (n == (size_t)-1)
		return (ssize_t)-1;
	*wcs = calloc(n + 1, sizeof(wchar_t));
	if (*wcs == NULL)
		return (ssize_t)-1;
	if (mbstowcs(*wcs, *mbs, n + 1) == (size_t)-1)
		return (ssize_t)-1;
	return (ssize_t)n;
}

void usage(const char *argv0) {
	fprintf(stderr, "usage: %s [-d delim] [[m] n]\n", argv0);
}

int main(int argc, char* argv[]) {
	int opt;
	size_t n_min = 1, n_max = 1;
	wchar_t *delimiters = L" ";
	char *mbline = NULL;
	size_t mbline_size = 0;
	wchar_t *wcline = NULL;
	unsigned long lineno = 1;
	if (setlocale(LC_ALL, "") == NULL) {
		perror("Could not set the locale to LC_ALL");
		exit(EXIT_FAILURE);
	}
	while ((opt = getopt(argc, argv, "d:h")) != -1) {
		switch (opt) {
			case 'd':
				if (wcconv(&optarg, &delimiters) > 0)
					break;
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			default:
				usage(argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	switch (argc - optind) {
		case 0:
			break;
		case 1:
			n_min = n_max = (size_t)atoi(argv[optind]);
			break;
		case 2:
			n_min = (size_t)atoi(argv[optind]);
			n_max = (size_t)atoi(argv[optind+1]);
			if (1 <= n_min && n_min <= n_max)
				break;
		default:
			usage(argv[0]);
			exit(EXIT_FAILURE);
	}
	/* Doesn't handle embedded null bytes */
	while (getline(&mbline, &mbline_size, stdin) != (ssize_t)-1) {
		mbline[strcspn(mbline, "\r\n")] = '\0';
		if (wcconv(&mbline, &wcline) == (ssize_t)-1) {
			perror("Could not convert the input to a wide character string");
			exit(EXIT_FAILURE);
		}
		grams(wcline, delimiters, n_min, n_max);
		lineno++;
	}
	free(mbline);
	free(wcline);
	exit(EXIT_SUCCESS);
}
