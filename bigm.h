/*-------------------------------------------------------------------------
 *
 * Portions Copyright (c) 2004-2012, PostgreSQL Global Development Group
 *
 * Changelog:
 *   2013/01/09
 *   Support full text search using bigrams.
 *   Author: NTT DATA Corporation
 *
 *-------------------------------------------------------------------------
 */
#ifndef __BIGM_H__
#define __BIGM_H__

#include "access/itup.h"
#include "storage/bufpage.h"
#include "utils/builtins.h"

/* GUC variable */
extern bool	bigm_enable_recheck;
extern int	bigm_gin_key_limit;

/* options */
#define LPADDING		1
#define RPADDING		1

/* operator strategy numbers */
#define LikeStrategyNumber			1

typedef struct
{
	bool	pmatch;		/* partial match is required? */
	int		bytelen;	/* byte length of bi-gram string */
	/*
	 * Bi-gram string; we assume here that the maximum bytes for
	 * a character are four.
	 */
	char	str[8];
} bigm;

#define BIGMSIZE	sizeof(bigm)

int	bigmstrcmp(char *arg1, int len1, char *arg2, int len2);
#define CMPBIGM(a,b) ( bigmstrcmp(((bigm *)a)->str, ((bigm *)a)->bytelen, ((bigm *)b)->str, ((bigm *)b)->bytelen) )

#define CPBIGM(bptr, s, len) do {		\
	Assert(len <= 8);				\
	memcpy(bptr->str, s, len);		\
	bptr->bytelen = len;			\
	bptr->pmatch = false;			\
} while(0);

#define ISESCAPECHAR(x) (*(x) == '\\')	/* Wildcard escape character */
#define ISWILDCARDCHAR(x) (*(x) == '%' || *(x) == '_')	/* Wildcard
														 * meta-character */
typedef struct
{
	int32		vl_len_;		/* varlena header (do not touch directly!) */
	char		data[1];
} BIGM;

#define CALCGTSIZE(len) (VARHDRSZ + len * sizeof(bigm))
#define GETARR(x)		( (bigm *)( (char*)x + VARHDRSZ ) )
#define ARRNELEM(x) ( ( VARSIZE(x) - VARHDRSZ )/sizeof(bigm) )

BIGM	   *generate_bigm(char *str, int slen);
BIGM	   *generate_wildcard_bigm(const char *str, int slen, bool *removeDups);

#endif   /* __BIGM_H__ */
