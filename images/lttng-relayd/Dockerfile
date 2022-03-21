FROM lttng-dev:stable-2.13 AS builder
WORKDIR /lttng/dev/
COPY files/health-check/* ./
RUN gcc -o lttng-relayd-health-check health_check.c -llttng-ctl

FROM lttng:stable-2.13
COPY --from=builder /lttng/dev/lttng-relayd-health-check /usr/local/bin/
CMD ["/usr/bin/lttng-relayd"]
