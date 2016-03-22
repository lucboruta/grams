#ifndef _GRAMS_H
#define _GRAMS_H

/*!
	\brief	Enumerate all n-grams in the input string from n_min to n_max
	
	\param	string	Input string
	\param	delimiters	Delimiters to use to tokenize the string
	\param	n_min	Minimum size of the n-grams (inclusive)
	\param	n_max	Maximum size of the n-grams (inclusive)
*/
size_t grams(wchar_t *string, const wchar_t *delimiters, const size_t n_min, const size_t n_max);

/*!
	\brief	Convert a multibyte string to a wide character string
	
	\param	mbs	Source multibyte string
	\param	wcs	Destination wide char string
*/
ssize_t wcconv(char **mbs, wchar_t **wcs);

/*!
	\brief	Print the usage statement
	
	\param	argv0	Program name
*/
void usage(const char *argv0);

#endif
