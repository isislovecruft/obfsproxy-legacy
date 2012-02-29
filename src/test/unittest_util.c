#include "util.h"
#include "tinytest_macros.h"

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
  T(resolve_address_port),
  END_OF_TESTCASES
};
