/*
 * SPDX-License-Identifier: LGPL-2.1-only
 *
 * Copyright (C) 2020 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 */

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include <lttng/tracelog.h>

static volatile int quit = 0;

void int_handler(int dummy) {
	quit = 1;
}

static
void print_err(const char* msg, ...)
	__attribute__((format(printf, 1, 2)));
static
void print_err(const char* msg, ...)
{
	va_list ap;

	va_start(ap, msg);
	lttng_ust_vtracelog(LTTNG_UST_TRACEPOINT_LOGLEVEL_ERR, msg, ap);
	va_end(ap);
}

int main(void)
{
	const char *str = "mystring test";
	int i = 0;
	long l = 0x42;

	signal(SIGINT, int_handler);

	fprintf(stderr, "Demo program starting.\n");

	fprintf(stderr, "Tracing... ");

	while (!quit) {
		print_err("app2: This is a \"%s\" formatted %d error event %lx", str, i, l);
		i++;
		sleep(5);
	}

	fprintf(stderr, " done.\n");
	return 0;
}
