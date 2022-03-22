// SPDX-FileCopyrightText: 2022 Michael Jeanson <mjeanson@efficios.com>
//
// SPDX-License-Identifier: MIT

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <lttng/lttng.h>

/*
 * Subscribes to notifications, through the notification channel
 * `notification_channel`, which match the condition of the trigger
 * named `trigger_name`.
 *
 * Returns `true` on success.
 */
static bool subscribe(struct lttng_notification_channel *notification_channel,
                      const char *trigger_name)
{
    const struct lttng_condition *condition = NULL;
    struct lttng_triggers *triggers = NULL;
    unsigned int trigger_count;
    unsigned int i;
    enum lttng_error_code error_code;
    enum lttng_trigger_status trigger_status;
    bool ret = false;

    /* Get all LTTng triggers */
    error_code = lttng_list_triggers(&triggers);
    assert(error_code == LTTNG_OK);

    /* Get the number of triggers */
    trigger_status = lttng_triggers_get_count(triggers, &trigger_count);
    assert(trigger_status == LTTNG_TRIGGER_STATUS_OK);

    /* Find the trigger named `trigger_name` */
    for (i = 0; i < trigger_count; i++) {
        const struct lttng_trigger *trigger;
        const char *this_trigger_name;

        trigger = lttng_triggers_get_at_index(triggers, i);
        trigger_status = lttng_trigger_get_name(trigger, &this_trigger_name);
        assert(trigger_status == LTTNG_TRIGGER_STATUS_OK);

        if (strcmp(this_trigger_name, trigger_name) == 0) {
            /* Trigger found: subscribe with its condition */
            enum lttng_notification_channel_status notification_channel_status;

            notification_channel_status = lttng_notification_channel_subscribe(
                notification_channel,
                lttng_trigger_get_const_condition(trigger));
            assert(notification_channel_status ==
                   LTTNG_NOTIFICATION_CHANNEL_STATUS_OK);
            ret = true;
            break;
        }
    }

    lttng_triggers_destroy(triggers);
    return ret;
}

static bool register_rotation_trigger(const char *session_name)
{
	enum lttng_action_status action_status;
	enum lttng_condition_status condition_status;
	enum lttng_error_code ret;
	struct lttng_action *action_list;
	struct lttng_action *action_notify;
	struct lttng_trigger *trigger;
	struct lttng_condition *condition;

	condition = lttng_condition_session_rotation_completed_create();
	condition_status = lttng_condition_session_rotation_set_session_name(condition, session_name);
	if (condition_status != LTTNG_CONDITION_STATUS_OK) {
		fprintf(stderr, "Failed condition_status\n");
		return false;
	}

	action_list = lttng_action_list_create();
	action_notify = lttng_action_notify_create();
	action_status = lttng_action_list_add_action(action_list, action_notify);
	if (action_status != LTTNG_ACTION_STATUS_OK) {
		fprintf(stderr, "Failed action_status\n");
		return false;
	}

	trigger = lttng_trigger_create(condition, action_list);
	ret = lttng_register_trigger_with_name(trigger, "rotation-completed");
	if (ret == LTTNG_ERR_TRIGGER_EXISTS) {
		fprintf(stderr, "Warning: trigger already exists.\n");
	} else if (ret != LTTNG_OK) {
		fprintf(stderr, "Failed trigger %d\n", ret);
		return false;
	}

	return true;
}

static void handle_chunk(const char *chunk_path, const char *dest_dir)
{
	char command[PATH_MAX];

	sprintf(command, "lttng-archive-chunk '%s' '%s'", chunk_path, dest_dir);

	system(command);
}

static bool handle_rotation_completed(const struct lttng_evaluation *evaluation, const char *dest_dir)
{
    enum lttng_evaluation_status evaluation_status;
    enum lttng_trace_archive_location_status location_status;
    const struct lttng_trace_archive_location *location = NULL;
    const char *chunk_path = NULL;


    evaluation_status = lttng_evaluation_session_rotation_completed_get_location(evaluation, &location);
    if (evaluation_status != LTTNG_EVALUATION_STATUS_OK || !location) {
        return false;
    }

    if (!(lttng_trace_archive_location_get_type(location) == LTTNG_TRACE_ARCHIVE_LOCATION_TYPE_LOCAL)) {
        return false;
    }

    location_status = lttng_trace_archive_location_local_get_absolute_path(location, &chunk_path);
    if (!(location_status == LTTNG_TRACE_ARCHIVE_LOCATION_STATUS_OK && chunk_path)) {
        return false;
    }

    printf("Chunk available at %s\n", chunk_path);
    handle_chunk(chunk_path, dest_dir);

    return true;
}

/*
 * Handles the evaluation `evaluation` of a single notification.
 */
static void handle_notification(struct lttng_notification *notification, const char *dest_dir)
{
    const struct lttng_evaluation *evaluation =
                          lttng_notification_get_evaluation(notification);
    const enum lttng_condition_type type =
                          lttng_evaluation_get_type(evaluation);

    switch (type) {
    case LTTNG_CONDITION_TYPE_SESSION_ROTATION_COMPLETED:
        printf("Received session rotation completed notification\n");
	handle_rotation_completed(evaluation, dest_dir);
        break;
    default:
        fprintf(stderr, "Unknown notification type (%d)\n", type);
    }
}

int main(int argc, char *argv[])
{
    int exit_status = EXIT_SUCCESS;
    struct lttng_notification_channel *notification_channel;
    enum lttng_notification_channel_status notification_channel_status;
    const struct lttng_condition *condition;
    const char *trigger_name = "rotation-completed";
    const char *session_name, *dest_dir;
    bool subscribe_res;

    if (argc != 3) {
	    puts("Usage: lttng-notif-client SESSION_NAME DESTINATION_DIR");
	    exit_status = EXIT_FAILURE;
	    goto exit;
    }

    session_name = argv[1];
    dest_dir = argv[2];

    /*
     */
    if (!register_rotation_trigger(session_name)) {
	    fprintf(stderr, "Failed to register rotation completed trigger for session '%s'\n", session_name);
	    exit_status = EXIT_FAILURE;
	    goto exit;
    }

    /*
     * Create a notification channel.
     *
     * A notification channel connects the user application to the LTTng
     * session daemon.
     *
     * You can use this notification channel to listen to various types
     * of notifications.
     */
    notification_channel = lttng_notification_channel_create(
        lttng_session_daemon_notification_endpoint);
    assert(notification_channel);

    /*
     * Subscribe to notifications which match the condition of the
     * trigger named `trigger_name`.
     */
    if (!subscribe(notification_channel, trigger_name)) {
        fprintf(stderr,
                "Error: Failed to subscribe to notifications (trigger `%s`).\n",
                trigger_name);
        exit_status = EXIT_FAILURE;
        goto end;
    }

    /*
     * Notification loop.
     *
     * Put this in a dedicated thread to avoid blocking the main thread.
     */
    while (true) {
        struct lttng_notification *notification;
        enum lttng_notification_channel_status status;
        const struct lttng_evaluation *notification_evaluation;

        /* Receive the next notification */
        status = lttng_notification_channel_get_next_notification(
            notification_channel, &notification);

        switch (status) {
        case LTTNG_NOTIFICATION_CHANNEL_STATUS_OK:
            break;
        case LTTNG_NOTIFICATION_CHANNEL_STATUS_NOTIFICATIONS_DROPPED:
            /*
             * The session daemon can drop notifications if a receiving
             * application doesn't consume the notifications fast
             * enough.
             */
            fprintf(stderr, "Warning: Notifications dropped.\n");
            continue;
        case LTTNG_NOTIFICATION_CHANNEL_STATUS_CLOSED:
            /*
             * The session daemon closed the notification channel.
             *
             * This is typically caused by a session daemon shutting
             * down.
             */
            fprintf(stderr, "Warning: Notification channel closed.\n");
            goto end;
        default:
            /* Unhandled conditions or errors */
            fprintf(stderr, "Error: Unhandled notification channel status.\n");
            exit_status = EXIT_FAILURE;
            goto end;
        }

        /*
         * Handle the notification.
         *
         * A notification provides, amongst other things:
         *
         * * The condition that caused LTTng to send this notification.
         *
         * * The condition evaluation, which provides more specific
         *   information on the evaluation of the condition.
         */
	handle_notification(notification, dest_dir);

        /* Destroy the notification object */
        lttng_notification_destroy(notification);
    }

end:
    lttng_notification_channel_destroy(notification_channel);
exit:
    return exit_status;
}
