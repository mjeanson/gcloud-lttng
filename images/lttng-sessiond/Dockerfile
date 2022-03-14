# Sidepod lttng-sessiond
FROM lttng:stable-2.13
COPY files/configs/* /configs/
COPY files/wait-session-active.sh /usr/local/bin/
CMD ["/usr/bin/lttng-sessiond"]
