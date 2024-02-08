/*
** $Id: lobject.c,v 1.55 2000/10/20 16:36:32 roberto Exp $
** Some generic functions over Lua objects
** See Copyright Notice in lua.h
*/

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lua.h"

#include "lmem.h"
#include "lobject.h"
#include "lstate.h"

// const TObject luaO_nilobject = {LUA_TNIL, {NULL}};
TObject luaO_nilobject;

// luaO_nilobject.ttype = 1;
// luaO_nilobject.Value = {NULL};

const char* const luaO_typenames[] = {
  "userdata", "nil", "number", "string", "table", "function"
};

/*
** returns smaller power of 2 larger than `n' (minimum is MINPOWER2)
*/
lint32 luaO_power2(lint32 n)
{
  lint32 p = MINPOWER2;
  while (p <= n)
    p <<= 1;
  return p;
}

int luaO_equalObj(const TObject* t1, const TObject* t2)
{
  if (ttype(t1) != ttype(t2))
    return 0;
  switch (ttype(t1)) {
  case LUA_TNUMBER:
    return (Re(nvalue(t1)) == Re(nvalue(t2))) && (Im(nvalue(t1)) == Im(nvalue(t2)));
  case LUA_TSTRING:
  case LUA_TUSERDATA:
    return tsvalue(t1) == tsvalue(t2);
  case LUA_TTABLE:
    return hvalue(t1) == hvalue(t2);
  case LUA_TFUNCTION:
    return clvalue(t1) == clvalue(t2);
  default:
    LUA_ASSERT(ttype(t1) == LUA_TNIL, "invalid type");
    return 1; /* LUA_TNIL */
  }
}

char* luaO_openspace(lua_State* L, size_t n)
{
  if (n > L->Mbuffsize) {
    luaM_reallocvector(L, L->Mbuffer, n, char);
    L->nblocks += (n - L->Mbuffsize) * sizeof(char);
    L->Mbuffsize = n;
  }
  return L->Mbuffer;
}

// LUA_NUMBER
CComplex lua_str2number(const char* s, char** endptr)
{
  CComplex x;
  char* q;

  // strip off real part;
  x.re = strtod(s, &q);

  // if string _only_ contains a real, return;
  if (q[0] == NULL) {
    *endptr = q;
    return x;
  }

  // remainder of string _may_ contain a complex number;

  if (strcmp(q, "I") == 0) {
    x.im = 1;
    *endptr = (q + 1);
    return x;
  }

  if (strcmp(q, "+I") == 0) {
    x.im = 1;
    *endptr = (q + 2);
    return x;
  }

  if (strcmp(q, "-I") == 0) {
    x.im = -1;
    *endptr = (q + 2);
    return x;
  }

  if (strncmp(q, "I*", 2) == 0) {
    x.im = strtod(q + 2, endptr);
    return x;
  }

  if (strncmp(q, "+I*", 3) == 0) {
    x.im = strtod(q + 3, endptr);
    return x;
  }

  if (strncmp(q, "-I*", 3) == 0) {
    x.im = -strtod(q + 3, endptr);
    return x;
  }

  *endptr = q;
  return x;
}

int luaO_str2d(const char* s, Number* result)
{ // LUA_NUMBER
  char* endptr;
  Number res = lua_str2number(s, &endptr);
  if (endptr == s)
    return 0; // no conversion
  while (isspace((unsigned char)*endptr))
    endptr++;
  if (*endptr != '\0')
    return 0; // invalid trailing characters?
  *result = res;
  return 1;
}

/*
// LUA_NUMBER
int luaO_str2d(const char *v, Number *result)
{
  char s[256];
    int i,k;

  lua_State *L = lua_open(100);
  i=lua_gettop(L);
  sprintf(s,"return %s",s);
  lua_dostring(L,s);
  k=lua_gettop(L);
  if(k!=i)
  {
    *result=lua_tonumber(L,-1);
    lua_close(L);
    return 1;
  }
    lua_close(L);
    return 0;
}
*/

/* maximum length of a string format for `luaO_verror' */
#define MAX_VERROR 280

/* this function needs to handle only '%d' and '%.XXs' formats */
void luaO_verror(lua_State* L, const char* fmt, ...)
{
  va_list argp;
  char buff[MAX_VERROR]; /* to hold formatted message */
  va_start(argp, fmt);
  vsprintf(buff, fmt, argp);
  va_end(argp);
  lua_error(L, buff);
}

void luaO_chunkid(char* out, const char* source, int bufflen)
{
  if (*source == '=') {
    strncpy(out, source + 1, bufflen); /* remove first char */
    out[bufflen - 1] = '\0'; /* ensures null termination */
  } else {
    if (*source == '@') {
      int l;
      source++; /* skip the `@' */
      bufflen -= sizeof("file `...%s'");
      l = strlen(source);
      if (l > bufflen) {
        source += (l - bufflen); /* get last part of file name */
        sprintf(out, "file `...%.99s'", source);
      } else
        sprintf(out, "file `%.99s'", source);
    } else {
      int len = strcspn(source, "\n"); /* stop at first newline */
      bufflen -= sizeof("string \"%.*s...\"");
      if (len > bufflen)
        len = bufflen;
      if (source[len] != '\0') { /* must truncate? */
        strcpy(out, "string \"");
        out += strlen(out);
        strncpy(out, source, len);
        strcpy(out + len, "...\"");
      } else
        sprintf(out, "string \"%.99s\"", source);
    }
  }
}

void lua_number2str(char* s, Number n)
{
  char cc[256];
  sprintf(s, "%s", n.ToString(cc));
}
