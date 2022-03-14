#!/bin/sh

# Wait until all sessions are enabled
while true; do
	if lttng --mi=xml list | grep -L '<enabled>false</enabled>' >/dev/null; then
		exit 0
	fi
	sleep 1
done
