/* Copyright 2011 Nick Mathewson, George Kadianakis
   See LICENSE for other credits and copying information
*/

#include "util.h"
#include "protocol.h"
#include "tinytest_macros.h"

static void
test_util_asprintf(void *unused)
{
#define LOREMIPSUM                                              \
  "Lorem ipsum dolor sit amet, consectetur adipisicing elit"
  char *cp=NULL, *cp2=NULL;
  int r;

  /* empty string. */
  r = obfs_asprintf(&cp, "%s", "");
  tt_assert(cp);
  tt_int_op(r, ==, strlen(cp));
  tt_str_op(cp, ==, "");

  /* Short string with some printing in it. */
  r = obfs_asprintf(&cp2, "First=%d, Second=%d", 101, 202);
  tt_assert(cp2);
  tt_int_op(r, ==, strlen(cp2));
  tt_str_op(cp2, ==, "First=101, Second=202");
  tt_assert(cp != cp2);
  free(cp); cp = NULL;
  free(cp2); cp2 = NULL;

  /* Glass-box test: a string exactly 128 characters long. */
  r = obfs_asprintf(&cp, "Lorem1: %sLorem2: %s", LOREMIPSUM, LOREMIPSUM);
  tt_assert(cp);
  tt_int_op(r, ==, 128);
  tt_assert(cp[128] == '\0');
  tt_str_op(cp, ==,
            "Lorem1: "LOREMIPSUM"Lorem2: "LOREMIPSUM);
  free(cp); cp = NULL;

  /* String longer than 128 characters */
  r = obfs_asprintf(&cp, "1: %s 2: %s 3: %s",
                   LOREMIPSUM, LOREMIPSUM, LOREMIPSUM);
  tt_assert(cp);
  tt_int_op(r, ==, strlen(cp));
  tt_str_op(cp, ==, "1: "LOREMIPSUM" 2: "LOREMIPSUM" 3: "LOREMIPSUM);

 end:
  free(cp);
  free(cp2);
}

/* Isis' original test cases for IPv6 address parsing
   These were (and are) broken, so far as I recall, and should
   not be included as is. */
static void
test_util_resolve_address_port(void *unused)
{
  struct addressport_parsing_case {
    int run;
    int should_succeed;
    const char *address;
    int nodns;
    int passive;
    const char *default_port;
  };
  static struct addressport_parsing_case cases[] = {
    /* correct; IPV4 */
    { 1, 1, "50.0.0.1:1337", 0, 0, "666"},
    // correct; no port, so default_port should be used
    { 1, 1, "50.0.0.1", 0, 0, "666"},
    // default_port resolve through evutil_getaddrinfo().servname? 
    { 1, 0, "50.0.0.1:https", 1, 1, "https"},
    // correct; IPV6 
    { 1, 1, "[2001:db8:1002:1003:1004:1005:1006:1007]:1337", 0, 0, "666"},
    // correct; no port, so default_port should be used 
    { 1, 1, "[2001:db8:1002:1003:1004:1005:1006:1007]", 0, 0, "666"},
    // wrong; no port, default_port fail 
    { 1, 0, "[2001:db8:1002:1003:1004:1005:1006:1007]", 0, 0, NULL},
    // wrong; no port, default port as empty string
    { 1, 0, "[2001:db8:1002:1003:1004:1005:1006:1007]", 0, 0, ""},
    // wrong; port must be enutil_getaddrinfo().servname or int
    { 1, 0, "[2001:db8:1002:1003:1004:1005:1006:1007]:fuck", 0, 0, "666"},
    // wrong; no closing bracket 
    { 1, 0, "[2001:db8:1002:1003:1004:1005:1006:1007:1337", 0, 0, "666"},
    // wrong; test even the NULL address
    { 1, 0, 0, 0, 0, 0},
    // Stop running
    { 0, 0, NULL, 0, 0, NULL}
  };

  /* Suppress logs for the duration of this function. */
  //log_set_method(LOG_METHOD_NULL, NULL);

  struct addressport_parsing_case *c;
  for (c = cases; c->run; c++) {
    struct evutil_addrinfo *returnvalue;
    printf("Address: %s\n Nodns: %d\n Passive: %d\n Default port: %s\n", 
	   c->address, c->nodns, c->passive, c->default_port);
    returnvalue = resolve_address_port(c->address, c->nodns, c->passive, 
				       c->default_port);
    printf("Return value: ");
    printf("%p\n", returnvalue);
    if (c->should_succeed)
      tt_assert(returnvalue);
    else 
      tt_assert(!returnvalue);
    if (returnvalue)
      evutil_freeaddrinfo(returnvalue);
  }

  end:
  /* Unsuspend logging */
  log_set_method(LOG_METHOD_STDERR, NULL);
}


#define T(name) \
  { #name, test_util_##name, 0, NULL, NULL }

struct testcase_t util_tests[] = {
  T(asprintf),
  /* Isis' IPv6 test cases: */
  // T(resolve_address_port),
  END_OF_TESTCASES
};

