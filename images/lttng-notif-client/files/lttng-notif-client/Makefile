# SPDX-License-Identifier: MIT
#
# Copyright (C) 2013 Jérémie Galarneau <jeremie.galarneau@efficios.com>
# Copyright (C) 2014 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
#
# This Makefile is not using automake so that users may see how to build
# a program with tracepoint provider probes as stand-alone shared objects.
#
# This makefile is purposefully kept simple to support GNU and BSD make.

LIBS = -llttng-ctl
LOCAL_CPPFLAGS += -I.
AM_V_P := :

all: lttng-notif-client

lttng-notif-client.o: lttng-notif-client.c
	@if $(AM_V_P); then set -x; else echo "  CC       $@"; fi; \
		$(CC) $(CPPFLAGS) $(LOCAL_CPPFLAGS) $(AM_CFLAGS) $(AM_CPPFLAGS) \
		$(CFLAGS) -c -o $@ $<

lttng-notif-client: lttng-notif-client.o
	@if $(AM_V_P); then set -x; else echo "  CCLD     $@"; fi; \
		$(CC) $(LDFLAGS) $(AM_CFLAGS) $(AM_LDFLAGS) $(CFLAGS) \
		-o $@ $< $(LIBS)

.PHONY: clean
clean:
	rm -f *.o *.a lttng-notif-client
