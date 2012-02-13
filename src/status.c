/* Copyright 2012 The Tor Project
   See LICENSE for other credits and copying information
*/

/**
 * \file status.c
 * \headerfile status.h
 * \brief Keeps status information and logs heartbeat messages.
 **/

#include "util.h"

#include "container.h"

#include <string.h>
#include <stdio.h>
#include <time.h>

#include <event2/event.h>
#include <event2/listener.h>

/** Count total connections. */
static int connections = 0;

/** Count connections from unique addresses. */
static strmap_t *addresses = NULL;

/** Initialize counting connections from unique addresses. */
static void
connections_init(void)
{
  if (!addresses)
    addresses = strmap_new();
}

/** Clear connection counters. */
static void
connections_clear(int reinit)
{
  connections = 0;
  if (!addresses)
    return;
  strmap_free(addresses, NULL); /* NULL, because no need to free values. */
  addresses = NULL;
  if (reinit)
    addresses = strmap_new();
}

/** Note that we saw a new connection. */
void
status_note_connection(const char *addrport)
{
  char *addr = xstrdup(addrport), *p;

  if ((p = strrchr(addr, ':'))) {
    *p = '\0';
  } else {
    log_debug("Error in address %s: port expected.", addrport);
    free(addr);
    return;
  }

  if (!addresses)
    addresses = strmap_new();
  strmap_set_lc(addresses, addr, (void*)(1));
  free(addr);
  connections++;
}

/** Time when we started. */
static time_t started = 0;

/** Time when we last reset connection and address counters. */
static time_t last_reset_counters = 0;

/** Reset status information this often. */
#define RESET_COUNTERS 86400

/** Initialize status information to print out heartbeat messages. */
void
status_init(void)
{
  time_t now = time(NULL);
  started = now;
  last_reset_counters = now;
  connections_init();
}

/** Free all memory used by printing heartbeat logs. */
void
status_cleanup(void)
{
  started = 0;
  last_reset_counters = 0;
  connections_clear(0);
}

/** Log information about our uptime and the number of connections we saw.
 */
void
status_log_heartbeat(void)
{
  time_t now = time(NULL);
  long secs = now - started;
  long days = secs / 86400;
  int hours = (int)((secs - (days * 86400)) / 3600);
  int last_reset_hours = (int) (now - last_reset_counters) / 3600;
  int minutes = (int)((secs - (days * 86400) - (hours * 3600)) / 60);
  log_info("Heartbeat: obfsproxy's uptime is %ld days, %d hours, and "
           "%d minutes. During the last %d hours we saw %d connections "
           "from %d unique addresses.",
           days, hours, minutes, last_reset_hours,
           connections, strmap_size(addresses));
  if (now - last_reset_counters >= RESET_COUNTERS) {
    log_info("Resetting connection counters.");
    connections_clear(1);
    last_reset_counters += RESET_COUNTERS;
  }
}
