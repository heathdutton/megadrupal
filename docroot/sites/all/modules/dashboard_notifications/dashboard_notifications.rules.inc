<?php

/**
 * Implements hook_rules_action_info().
 * 
 * Defines rules to be used in the rules interface.
 * 
 */
function dashboard_notifications_rules_action_info() {
  $actions = array(
   
    'dashboard_notifications_add_notification' => array(
      'label' => t("Add Dashboard Notification"), // Name displayed to admins
      'group' => t('Notification'), // Used for grouping actions in select lists
      'parameter' => array (
        'user' => array(
          'type' => 'user',
          'label' => t('The user for whom the notification has to be shown'),
        ),
        'from_user' => array(
          'type' => 'user',
          'label' => t('The user who created the notification'),
          'optional' => TRUE,
        ),
        'type' => array(
          'type' => 'text',
          'label' => t('The Type of the message.'),
        ),
        'message_key' => array(
          'type' => 'text',
          'label' => t('The unique key of the message used for grouping and also to modify/hide message if exists.'),
        ),
        'message' => array(
          'type' => 'text_formatted',
          'label' => t("The message that's shown to user"),
        ),
        'variables' => array(
          'type' => 'text',
          'label' => t('The Variables of the message.'),
          'optional' => TRUE,
        ),
      ),
    ),
    'dashboard_notifications_hide_notification' => array(
      'label' => t("Hide Dashboard Notification"), // Name displayed to admins
      'group' => t('Notification'), // Used for grouping actions in select lists
      'parameter' => array (
        'user' => array(
          'type' => 'user',
          'label' => t('The user for whom the notification is shown'),
        ),
        'creator' => array(
          'type' => 'user',
          'label' => t('The user who created the notification'),
        ),
        'message_key' => array(
          'type' => 'text',
          'label' => t('The unique key of the message.'),
          'optional' => TRUE,
        ),
      ),
    ),
  );
return $actions;
}

/**
 * Handler for the add notification action.
 * 
 * @param user $user
 * @param text $type
 * @param text $message_key 
 * @param text_formatted $message  
 */
function dashboard_notifications_add_notification($user, $from_user, $type, $message_key, $message, $variables) {
  $variables_item_array = explode("\r\n", $variables);
  $variables_array = array();
  foreach ($variables_item_array as $item) {
  	$variables__key_val_array = explode(",", $item);
  	$variables_array[trim($variables__key_val_array[0])] = trim($variables__key_val_array[1]);
  }
  //array('@percentage_complete' => $profile2_out_percent));
	dashboard_notifications_api_add_notification($user->uid, $type, $message_key, $message['value'], $variables_array, $from_user->uid);
}

/**
 * Handler for the hide notification action.
 * 
 * @param number $user
 * @param number $creator 
 */
function dashboard_notifications_hide_notification($user, $creator, $message_key) {
	dashboard_notifications_api_hide_notification($user->uid, $creator->uid, $message_key);
}
