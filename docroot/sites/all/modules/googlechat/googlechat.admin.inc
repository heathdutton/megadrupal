<?php

/**
 * @file
 * Administrative functions to configure GoogleChat.
 */

/**
 * Callback for admin/settings/googlechat.
 */
function googlechat_settings_form() {
  drupal_add_js(drupal_get_path('module', 'googlechat') . '/js/googlechat.admin.js');
  $form = array();
  $form['googlechat_chat_status'] = array(
    '#type' => 'radios',
    '#title' => t('Chat'),
    '#options' => array(1 => 'Chat On', 2 => 'Chat Off'),
    '#default_value' => variable_get('googlechat_chat_status', 1),
  );
  $form['googlechat_notification_sound'] = array(
    '#type' => 'select',
    '#title' => t('Notification Sound'),
    '#description' => t('Play a sound notification when new chat messages arrive'),
    '#options' => array(GOOGLECHAT_SOUND_YES => 'Yes', GOOGLECHAT_SOUND_NO => 'No'),
    '#default_value' => variable_get('googlechat_notification_sound', GOOGLECHAT_SOUND_YES),
  );
  $form['googlechat_enable_emoticons'] = array(
    '#type' => 'select',
    '#title' => t('Enable Emoticons'),
    '#description' => t('When an emoticon is sent :-) it will automatically animate.'),
    '#options' => array(1 => 'Yes', 2 => 'No'),
    '#default_value' => variable_get('googlechat_enable_emoticons', 1),
  );
  $themes = _googlechat_load_themes(drupal_get_path('module', 'googlechat') . '/themes', 'css');
  $form['googlechat_theme'] = array(
    '#type' => 'select',
    '#title' => t('Theme'),
    '#description' => t('All themes from inside the <em>themes</em> folder will be displayed here.'),
    '#options' => $themes,
    '#default_value' => variable_get('googlechat_theme', 'base'),
  );
  $intervals = array(30, 60, 120, 180, 300, 600, 900, 1800, 2700, 3600);
  $list_count = array(
    0 => 'unlimited',
    5 => 5,
    10 => 10,
    15 => 15,
    30 => 30,
    50 => 50,
    75 => 75,
    100 =>
    100);
  $period = drupal_map_assoc($intervals, 'format_interval');
  $form['googlechat_seconds_online'] = array(
    '#type' => 'select',
    '#title' => t('User activity'),
    '#default_value' => variable_get('googlechat_seconds_online', 900),
    '#options' => $period,
    '#description' => t('A user is considered online for this long after they have last viewed a page.'
    ));
  $form['googlechat_max_list_count'] = array(
    '#type' => 'select',
    '#title' => t('User list length'),
    '#default_value' => variable_get('googlechat_max_list_count', 10),
    '#options' => drupal_map_assoc($list_count),
    '#description' => t('Maximum number of currently online users to display.'
    ));
  if (module_exists('privatemsg')) {
    $form['googlechat_privatemsg'] = array(
      '#type' => 'fieldset',
      '#title' => t('Private Messages with Googlechat'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['googlechat_privatemsg']['googlechat_privatemsg_when_offline'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private message when offline'),
      '#default_value' => variable_get('googlechat_privatemsg_when_offline', TRUE),
      '#description' => t('Select to send chat messages using the private message module when recipient is offline.'),
    );
    $form['googlechat_privatemsg']['googlechat_privatemsg_plus_chat'] = array(
      '#type' => 'checkbox',
      '#title' => t('Private message plus chat'),
      '#default_value' => variable_get('googlechat_privatemsg_plus_chat', TRUE),
      '#description' => t('Select to enable offline chat messages to also be received normally when recipient comes back online after they have been sent using the private message module. Only effective is above item is selected.'),
    );
  }
  $form['googlechat_buddylist_control_list'] = array(
    '#type' => 'fieldset',
    '#title' => t('GoogleChat Buddy List Control'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['googlechat_buddylist_control_list']['googlechat_rel'] = array(
    '#type' => 'radios',
    '#title' => t('Relationship method'),
    '#default_value' => variable_get('googlechat_rel', 0),
    '#options' => array(
      GOOGLECHAT_REL_ALL => t('All authenticated users'),
    ),
    '#description' => t('This determines the method for creating the chat buddylist.'),
  );
  if (module_exists('flag_friend')) {
    $form['googlechat_buddylist_control_list']['googlechat_rel']['#options'][GOOGLECHAT_REL_FF] = t('Flag Friend module');
  }
  if (module_exists('user_relationships')) {
    $form['googlechat_buddylist_control_list']['googlechat_rel']['#options'][GOOGLECHAT_REL_UR] = t('User Relationship module');
  }
  $form['googlechat_buddylist_control_list']['googlechat_ur_name'] = array(
    '#type' => 'textfield',
    '#title' => t('User Relationships Role Names to integrate with'),
    '#description' => t('The singular form of User Relationships Role Names (e.g. buddy, friend, coworker, spouse) separated by comma.'),
    '#default_value' => variable_get('googlechat_ur_name', NULL),
    '#autocomplete_path' => 'googlechat/ur-autocomplete',
  );
  return system_settings_form($form);
}

/**
 * Function to validate setting form.
 */
function googlechat_settings_form_validate($form, &$form_state) {
  if ($form_state['values']['googlechat_rel'] == GOOGLECHAT_REL_UR) {
    if ($form_state['values']['googlechat_ur_name']) {
      $array = drupal_explode_tags($form_state['values']['googlechat_ur_name']);
      $error = array();
      foreach ($array as $key) {
        if (!db_query("SELECT COUNT(*) FROM {user_relationship_types} WHERE name = :name", array(':name' => $key))->fetchField()) {
          $error[] = $key;
        }
      }
      if (!empty($error)) {
        form_set_error('googlechat_ur_name', t('User Relationship type %type was not found.', array('%type' => drupal_implode_tags($error))));
      }
    }
  }
}
/**
 * Function get list of theme.
 */
function _googlechat_load_themes($outer_dir, $x) {
  $dirs = array_diff(scandir($outer_dir), array('.', '..'));

  $dir_array = array();
  foreach ($dirs as $d) {
    if (is_dir($outer_dir . "/" . $d)) {
      $dir_array[$d] = $d;
    }
  }
  return $dir_array;
}
