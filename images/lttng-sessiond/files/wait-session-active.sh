#!/bin/sh

# SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
#
# SPDX-License-Identifier: MIT

# Wait until all sessions are enabled
while true; do
	if lttng --mi=xml list | grep -L '<enabled>false</enabled>' >/dev/null; then
		exit 0
	fi
	sleep 1
done
