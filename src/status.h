/* Copyright 2012 The Tor Project
   See LICENSE for other credits and copying information
*/

/**
 * \file status.h
 * \brief Headers for status.c.
 **/

void status_init(void);
void status_cleanup(void);
void status_note_connection(const char *addrport);
void status_log_heartbeat(void);
