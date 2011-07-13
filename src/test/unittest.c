/* Copyright 2011 Nick Mathewson, George Kadianakis
   See LICENSE for other credits and copying information
*/

#include "tinytest.h"
#include "../crypt.h"

extern struct testcase_t crypt_tests[];
extern struct testcase_t obfs2_tests[];
extern struct testcase_t socks_tests[];

struct testgroup_t groups[] = {
  { "crypt/", crypt_tests },
  { "obfs2/", obfs2_tests },
  { "socks/", socks_tests },
  END_OF_GROUPS
};

int
main(int argc, const char **argv)
{
  initialize_crypto();
  return tinytest_main(argc, argv, groups);
}
