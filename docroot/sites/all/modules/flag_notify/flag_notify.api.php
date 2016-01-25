<?php


/**
 * @file
 * Hooks provided by the Organic groups module.
 */

/**
 * Alter the parameters of the event leading to notifications.
 *
 *  @param bool $break
 *   An option to break the notification process.
 * @param string $event
 *   Type of event issued, either 'new_comment', 'new_content',
 *   'edited_content' or 'new_user'.
 * @param array $groups
 *   Array of og groups to which the content belong to.
 * @param entity $node
 *   The content edited or with new comments.
 * @param entity $comment
 *   The comment sent (if any).
 * @param entity $user
 *   The comment or content author (new comment/content), acting user (edited
 *   content) or the new og user.
 */
function hook_flag_notify_event_alter(&$break, &$event, &$groups, &$node, &$comment, &$user) {

}

/**
 * Alter the parameters of the notification.
 * 
 * Possible [notification-type] / [event-type] matches:
 * - comment - new_comment
 * - content - new_content
 * - content - edited_content
 * - group - new_comment
 * - group - new_content
 * - group - edited_content
 * - group - new_users
 *
 * @param bool $break
 *   An option to break the notification process.
 * @param string $event
 *   Type of event issued, either 'new_comment', 'new_content',
 *   'edited_content' or 'new_user'.
 * @param entity $group
 *   The og groups to which the content and the notified user belong to.
 * @param entity $node
 *   The content edited or with new comments.
 * @param entity $comment
 *   The comment sent (if any).
 * @param entity $user
 *   The comment or content author (new comment/content), acting user (edited
 *   content) or the new og user.
 * @param int $targetuser_uid
 *   The uid of the user to notify.
 * @param string $notification_type
 *   The reason this user is notified, either 'comment', 'content' or 'group'.
 */
function hook_flag_notify_notification_alter(&$break, &$event, &$group, &$node, &$comment, &$user, &$targetuser_uid, &$notification_type) {

}

