<?php
/**
 * @file
 * comstack.comstack.inc
 */

/**
 * Process and send a message.
 *
 * @param $message
 *   The message entity being used for the notification.
 * @param $options
 *   Array of options to override the plugin's default ones.
 * @param $notifier_name
 *   Optional; The name of the notifier to use. Defaults to "email"
 *   sending method.
 *
 * @return
 *   Boolean value denoting success or failure of the notification.
 */
function comstack_send_message(Message $message, array $options = array(), $notifiers = array('email')) {
  if (!$plugin = message_notify_get_notifier($notifier_name)) {
   throw new MessageNotifyException(format_string('Could not send notification using the "@notifier" notifier.'), array('@notifier' => $notifier_name));
  }

  $plugin['options'] = drupal_array_merge_deep($plugin['options'], $options);

  $class = ctools_plugin_load_class('message_notify', 'notifier', $notifier_name, 'class');
  $notifier = new $class($plugin, $message);
  if ($notifier->access()) {
    return $notifier->send();
  }
}
