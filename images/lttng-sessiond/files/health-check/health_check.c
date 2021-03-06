/*
 * Copyright (C) 2012 Christian Babeux <christian.babeux@efficios.com>
 * Copyright (C) 2014 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * SPDX-License-Identifier: GPL-2.0-only
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <lttng/health.h>

static
int check_component(struct lttng_health *lh, const char *component_name,
		int ok_if_not_running)
{
	const struct lttng_health_thread *thread;
	int nr_threads, i, status;

	if (lttng_health_query(lh)) {
		if (ok_if_not_running) {
			return 0;
		}
		fprintf(stderr, "Error querying %s health\n",
			component_name);
		return -1;
	}
	status = lttng_health_state(lh);
	if (!status) {
		return status;
	}

	nr_threads = lttng_health_get_nr_threads(lh);
	if (nr_threads < 0) {
		fprintf(stderr, "Error getting number of threads\n");
		return -1;
	}

	printf("Component \"%s\" is in error.\n", component_name);
	for (i = 0; i < nr_threads; i++) {
		int thread_state;

		thread = lttng_health_get_thread(lh, i);
		if (!thread) {
			fprintf(stderr, "Error getting thread %d\n", i);
			return -1;
		}
		thread_state = lttng_health_thread_state(thread);
		if (!thread_state) {
			continue;
		}
		printf("Thread \"%s\" is not responding in component \"%s\".\n",
			lttng_health_thread_name(thread),
			component_name);

	}
	return status;
}

static
int check_sessiond(void)
{
	struct lttng_health *lh;
	int status;

	lh = lttng_health_create_sessiond();
	if (!lh) {
		perror("lttng_health_create_sessiond");
		return -1;
	}

	status = check_component(lh, "sessiond", 0);

	lttng_health_destroy(lh);

	return status;
}

static
int check_consumerd(enum lttng_health_consumerd hc)
{
	struct lttng_health *lh;
	int status;
	static const char *cnames[NR_LTTNG_HEALTH_CONSUMERD] = {
		"ust-consumerd-32",
		"ust-consumerd-64",
		"kernel-consumerd",
	};

	lh = lttng_health_create_consumerd(hc);
	if (!lh) {
		perror("lttng_health_create_consumerd");
		return -1;
	}

	status = check_component(lh, cnames[hc], 1);

	lttng_health_destroy(lh);

	return status;
}

int main(void)
{
	int status = 0, i;

	status |= check_sessiond();
	for (i = 0; i < NR_LTTNG_HEALTH_CONSUMERD; i++) {
		status |= check_consumerd(i);
	}
	if (!status) {
		exit(EXIT_SUCCESS);
	} else {
		exit(EXIT_FAILURE);
	}
}
